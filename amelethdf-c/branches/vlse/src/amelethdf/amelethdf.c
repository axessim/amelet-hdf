#include "amelethdf.h"


/*
    FALSE = version_minimum("0.5", "0.1");
    FALSE = version_minimum("3.5", "2");
    FALSE = version_minimum("3.1", "2.186");

    TRUE = version_minimum("1.2.3.4", "1.2.3.4");
    TRUE = version_minimum("0.5", "0.6");
    TRUE = version_minimum("0.5", "1.8.6");
    TRUE = version_minimum("0.5.5", "1");
    TRUE = version_minimum("2", "2.5");
    TRUE = version_minimum("2.0.5.0", "2.1");
*/
char version_minimum (const char *required_version, const char *sim_version)
{
    char rdata = FALSE;
    char *a, *b, *atemp, *btemp, *temp;
    int ai = 0, bi = 0;  // storage for version numbers

    // cannot use 2x strtok
    a = trim_zeros(required_version);  // original pointers
    b = trim_zeros(sim_version);  //  to be used in free()
    atemp = a;  // creatin working pointers
    btemp = b;  //  (that will be destroyed)

    while (atemp != NULL && btemp != NULL && bi == ai)
    {
        temp = strstr(atemp, ".");  // pointer to dot in solver_version
        if (temp != NULL)
        {
            *temp = '\0';
            ai = atoi(atemp);
            atemp = temp + 1;  // pointer to the rest of the string
        }
        else
        {
            // no dot is present -> make last check
            ai = atoi(atemp);
            atemp = NULL;
        }

        temp = strstr(btemp, ".");  // pointer to dot in sim_version
        if (temp != NULL)
        {
            *temp = '\0';
            bi = atoi(btemp);
            btemp = temp + 1;  // pointer to the rest of the string
        }
        else
        {
            // no dot is present -> make last check
            bi = atoi(btemp);
            btemp = NULL;
        }

        if (bi > ai)
            rdata = TRUE;
    }
    if (atemp == NULL && bi == ai)
        rdata = TRUE;

    free(b);
    free(a);
    return rdata;
}

char *trim_zeros (const char *version)
{
    int i, number = 0;
    char *rdata;

    rdata = strdup(version);

    while (1)
    {
        for (i = strlen(rdata); i > -1; i--)
            if (rdata[i] == '.')
                break;
        number = atoi(rdata + i + 1);
        if (number == 0 && i > -1)
            rdata[i] = '\0';
        else
            break;
    }
    return rdata;
}


// Add aelement to aset
set_t add_to_set(set_t aset, char *aelement)
{
    hsize_t id = 0;

    if (!index_in_set(aset, aelement, &id))
    {
        aset.values[aset.nb_values] = strdup(aelement);  // malloc!!!
        aset.nb_values++;
    }
    return aset;
}


// Return the index of first occurrence of an element
int index_in_set(set_t aset, char *aelement, hsize_t *index)
{
    int present = FALSE;
    hsize_t i;

    for (i = 0; i < aset.nb_values; i++)
    {
        if (strcmp(aelement, aset.values[i]) == 0)
        {
            *index = i;
            present = TRUE;
            break;
        }
    }
    return present;
}


// Read children names of an object
children_t read_children_name (hid_t file_id, const char* path)
{
    H5G_info_t ginfo;
    children_t children;
    hsize_t i, j = 0;
    ssize_t size;
    hid_t group_id;
    char temp[ELEMENT_NAME_LENGTH], *temp2;

    children.childnames = NULL;
    /*
        - path must exist
        - number of children must be greater than zero
        - element name
            - must be readable
            - must be shorter than ELEMENT_NAME_LENGTH
            - must NOT be same as "_param"
    */
    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0 || strcmp(path, "/") == 0)
    {
        group_id = H5Gopen1(file_id, path);
        H5Gget_info(group_id, &ginfo);
        if (ginfo.nlinks > 0)
        {
            children.childnames = (char **) malloc(ginfo.nlinks * sizeof(char *));
            for (i = 0; i < ginfo.nlinks; i++)
            {
                size = H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME, H5_ITER_INC, i, NULL, 0, H5P_DEFAULT);
                if (size < 0)
                    printf("***** ERROR: Cannot read all children of \"%s\". *****\n\n", path);
                else if (size >= ELEMENT_NAME_LENGTH)
                {
                    temp2 = (char *) malloc ((size + 1) * sizeof(char));
                    H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME, H5_ITER_INC, i, temp2, size + 1, H5P_DEFAULT);
                    printf("***** ERROR: Maximum name length (%i) exceeded in \"%s/%s\". *****\n\n", ELEMENT_NAME_LENGTH - 1, path, temp2);
                    free(temp2);
                }
                else
                {
                    H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME, H5_ITER_INC, i, temp, size + 1, H5P_DEFAULT);
                    if (strcmp(temp, "_param") != 0)  // exclude parameterized attributes
                    {
                        children.childnames[j] = (char *) malloc((size + 2) * sizeof(char));
                        strcpy(children.childnames[j], "/");
                        strcat(children.childnames[j++], temp);
                    }
                }
            }
            if (j == 0)
                free(children.childnames);
        }
        H5Gclose(group_id);
    }
    children.nb_children = j;
    return children;
}


// Get last part of a path; does not allocate new memory
char *get_name_from_path (const char *path)
{
    int i;

    for (i = strlen(path); i > -1; i--)
        if (path[i] == '/')
            break;
    return (char *) path + i + 1;
}

// Get base part of a path; allocates new memory!
char *get_base_from_path (const char *path)
{
    int i;
    char *rdata, *temp;

    temp = strdup(path);
    for (i = strlen(temp); i > 0; i--)
        if (temp[i] == '/')
            break;
    temp[i] = '\0';
    rdata = strdup(temp);  // strndup wasn't available
    free(temp);
    return rdata;
}


// Read integer attribute <attr_name> given by address <path>
char read_int_attr (hid_t file_id, const char* path, char* attr, int *rdata)
{
    char success = FALSE;

    if (H5Lexists(file_id, path, H5P_DEFAULT))
        if (H5Aexists_by_name(file_id, path, attr, H5P_DEFAULT) > 0)
            if (H5LTget_attribute_int(file_id, path, attr, rdata) >= 0)
                success = TRUE;
    if(!success)
        *rdata = 0;
    return success;
}


// Read float attribute <attr_name> given by address <path>
char read_flt_attr (hid_t file_id, const char* path, char* attr_name, float *rdata)
{
    char success = FALSE;

    if (H5Lexists(file_id, path, H5P_DEFAULT))
        if (H5Aexists_by_name(file_id, path, attr_name, H5P_DEFAULT) > 0)
            if (H5LTget_attribute_float(file_id, path, attr_name, rdata) >= 0)
                success = TRUE;
    if(!success)
        *rdata = 0;
    return success;
}


// Read complex attribute <attr_name> given by address <path>
char read_cpx_attr (hid_t file_id, const char* path, char* attr_name, complex float *rdata)
{
    hid_t attr_id, type_id;
    float buf[2];
    char success = FALSE;

    if (H5Lexists(file_id, path, H5P_DEFAULT))
        if (H5Aexists_by_name(file_id, path, attr_name, H5P_DEFAULT) > 0)
        {
            attr_id = H5Aopen_by_name(file_id, path, attr_name, H5P_DEFAULT, H5P_DEFAULT);
            type_id = H5Tcreate(H5T_COMPOUND, H5Tget_size(H5T_NATIVE_FLOAT) * 2);
            H5Tinsert(type_id, "r", 0, H5T_NATIVE_FLOAT);
            H5Tinsert(type_id, "i", H5Tget_size(H5T_NATIVE_FLOAT), H5T_NATIVE_FLOAT);
            if (H5Aread(attr_id, type_id, buf) >= 0)
            {
                *rdata = buf[0] + buf[1] * _Complex_I;
                success = TRUE;
            }
            H5Tclose(type_id);
            H5Aclose(attr_id);
        }
    if (!success)
        *rdata = 0 + 0 * _Complex_I;
    return success;
}


// Read string attribute <attr_name> given by address <path>
char read_str_attr (hid_t file_id, const char *path, char *attr_name, char **rdata)
{
    hid_t attr_id, filetype, memtype;
    size_t sdim;
    char success = FALSE;

    if (H5Lexists(file_id, path, H5P_DEFAULT))
        if (H5Aexists_by_name(file_id, path, attr_name, H5P_DEFAULT) > 0)
        {
            attr_id = H5Aopen_by_name(file_id, path, attr_name, H5P_DEFAULT, H5P_DEFAULT);
            filetype = H5Aget_type(attr_id);
            sdim = H5Tget_size(filetype);
            sdim++;  // make a space for null terminator
            *rdata = (char *) malloc(sdim * sizeof(char));
            memtype = H5Tcopy(H5T_C_S1);
            H5Tset_size(memtype, sdim);
            if (H5Aread(attr_id, memtype, *rdata) >= 0)
                success = TRUE;
            else
                free(*rdata);
            H5Tclose(memtype);
            H5Tclose(filetype);
            H5Aclose(attr_id);
        }
    if (!success)
        *rdata = NULL;
    return success;
}



// Print integer attribute
void print_int_attr (char *name, int value, int space)
{
    printf("%*s-@%s: %i\n", space, "", name, value);
}


// Print float attribute
void print_flt_attr (char *name, float value, int space)
{
    printf("%*s-@%s: %g\n", space, "", name, value);
}


// Print complex attribute
void print_cpx_attr (char *name, complex float value, int space)
{
    printf("%*s-@%s: %g%+gi\n", space, "", name, creal(value), cimag(value));
}


// Print string attribute
void print_str_attr (char *name, char *value, int space)
{
    printf("%*s-@%s: %s\n", space, "", name, value);
}



// Read int dataset
char read_int_dataset (hid_t file_id, const char *path, const hsize_t mn, int **rdata)
{
    char success = FALSE;
    hid_t dset_id;

    *rdata = (int *) malloc(mn * sizeof(int));
    dset_id = H5Dopen(file_id, path, H5P_DEFAULT);
    if (H5Dread(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, *rdata) >= 0)
        success = TRUE;
    H5Dclose(dset_id);
    if (!success)
    {
        free(*rdata);
        *rdata = NULL;
    }
    return success;
}


// Read float dataset
char read_float_dataset (hid_t file_id, const char *path, const hsize_t mn, float **rdata)
{
    char success = FALSE;
    hid_t dset_id;

    *rdata = (float *) malloc(mn * sizeof(float));
    dset_id = H5Dopen(file_id, path, H5P_DEFAULT);
    if (H5Dread(dset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, *rdata) >= 0)
        success = TRUE;
    H5Dclose(dset_id);
    if (!success)
    {
        free(*rdata);
        *rdata = NULL;
    }
    return success;
}


// Read complex float dataset
char read_complex_dataset (hid_t file_id, const char *path, const hsize_t mn, complex float **rdata)
{
    char success = FALSE;
    hid_t dset_id, type_id;
    hsize_t i;
    float *buf;

    *rdata = (complex float *) malloc(mn * sizeof(complex float));
    buf = (float *) malloc(mn * 2 * sizeof(float));
    type_id = H5Tcreate(H5T_COMPOUND, H5Tget_size(H5T_NATIVE_FLOAT) * 2);
    H5Tinsert(type_id, "r", 0, H5T_NATIVE_FLOAT);
    H5Tinsert(type_id, "i", H5Tget_size(H5T_NATIVE_FLOAT), H5T_NATIVE_FLOAT);
    dset_id = H5Dopen(file_id, path, H5P_DEFAULT);
    if (H5Dread(dset_id, type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, *rdata) >= 0)
    {
        success = TRUE;
        for (i = 0; i < mn; i++)
            rdata[0][i] = buf[2 * i] + buf[2 * i + 1] * _Complex_I;
    }
    H5Dclose(dset_id);
    H5Tclose(type_id);
    free(buf);
    if (!success)
    {
        free(*rdata);
        *rdata = NULL;
    }
    return success;
}


// Read string dataset
char read_string_dataset(hid_t file_id, const char *path, const hsize_t mn, size_t length, char ***rdata)
{
    char success = FALSE;
    hid_t dset_id, memtype;
    hsize_t i;

    length++; // make a space for the null terminator
    dset_id = H5Dopen(file_id, path, H5P_DEFAULT);

    *rdata = (char **) malloc(mn * sizeof(char*));
    **rdata = (char *) malloc(mn * length * sizeof(char));
    for (i = 1; i < mn; i++)
        rdata[0][i] = rdata[0][0] + i * length;
    memtype = H5Tcopy(H5T_C_S1);
    if (H5Tset_size(memtype, length) >= 0)
        if (H5Dread(dset_id, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, **rdata) >= 0)
            success = TRUE;
    H5Tclose(memtype);
    H5Dclose(dset_id);
    if (!success)
    {
        free(**rdata);
        free(*rdata);
        *rdata = NULL;
    }
    return success;
}



// Read all optional attributes
char read_opt_attrs (hid_t file_id, const char *path, opt_attrs_t *opt_attrs, char mandatory_attrs[][ATTR_LENGTH], size_t nb_mandatory_attrs)
{
    char success = FALSE, is_mandatory;
    H5O_info_t object_info;
    hsize_t i, j, k = 0;
    hid_t attr_id, type_id, memtype;
    float buf[2];
    hsize_t nb_present_mandatory_attrs = 0;
    char temp_name[ATTR_LENGTH];

    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
    {
        // Check presence of all mandatory attributes
        for (i = 0; i < (hsize_t) nb_mandatory_attrs; i++)
            if (H5Aexists_by_name(file_id, path, mandatory_attrs[i], H5P_DEFAULT) > 0)
                nb_present_mandatory_attrs++;
        H5Oget_info_by_name(file_id, path, &object_info, H5P_DEFAULT);
        opt_attrs->nb_instances = object_info.num_attrs - nb_present_mandatory_attrs;
        if (opt_attrs->nb_instances > 0)
            opt_attrs->instances = (attr_instance_t *) malloc (opt_attrs->nb_instances * sizeof(attr_instance_t));
        for (i = 0; i < object_info.num_attrs; i++)
        {
            is_mandatory = FALSE;
            attr_id = H5Aopen_by_idx(file_id, path, H5_INDEX_CRT_ORDER, H5_ITER_NATIVE, i, H5P_DEFAULT, H5P_DEFAULT);
            H5Aget_name(attr_id, ATTR_LENGTH, temp_name);
            for (j = 0; j < nb_mandatory_attrs; j++)
                if (strcmp(temp_name, mandatory_attrs[j]) == 0)
                    is_mandatory = TRUE;
            if (!is_mandatory)
            {
                opt_attrs->instances[k].name = strdup(temp_name);
                type_id = H5Aget_type(attr_id);
                opt_attrs->instances[k].type = H5Tget_class(type_id);
                H5Tclose(type_id);
                switch (opt_attrs->instances[k].type)
                {
                case H5T_INTEGER:
                    opt_attrs->instances[k].value.i = 0;
                    if (H5Aread(attr_id, H5T_NATIVE_INT, &(opt_attrs->instances[k].value.i)) >= 0)
                        success = TRUE;
                    break;
                case H5T_FLOAT:
                    opt_attrs->instances[k].value.f = 0;
                    if (H5Aread(attr_id, H5T_NATIVE_FLOAT, &(opt_attrs->instances[k].value.f)) >= 0)
                        success = TRUE;
                    break;
                case H5T_COMPOUND:
                    opt_attrs->instances[k].value.c = 0 + 0 * _Complex_I;
                    type_id = H5Tcreate(H5T_COMPOUND, H5Tget_size(H5T_NATIVE_FLOAT) * 2);
                    H5Tinsert(type_id, "r", 0, H5T_NATIVE_FLOAT);
                    H5Tinsert(type_id, "i", H5Tget_size(H5T_NATIVE_FLOAT), H5T_NATIVE_FLOAT);
                    if (H5Aread(attr_id, type_id, buf) >= 0)
                    {
                        opt_attrs->instances[k].value.c = buf[0] + buf[1] * _Complex_I;
                        success = TRUE;
                    }
                    H5Tclose(type_id);
                    break;
                case H5T_STRING:
                    opt_attrs->instances[k].value.s = NULL;
                    memtype = H5Tcopy(H5T_C_S1);
                    H5Tset_size(memtype, ATTR_LENGTH);
                    opt_attrs->instances[k].value.s = (char *) malloc(ATTR_LENGTH * sizeof(char));
                    if (H5Aread(attr_id, memtype, opt_attrs->instances[k].value.s) >= 0)
                        success = TRUE;
                    H5Tclose(memtype);
                    break;
                default:
                    opt_attrs->instances[k].type = H5T_NO_CLASS;
                    printf("***** WARNING: Unsupported type of attribute \"%s@%s\". *****\n\n", path, opt_attrs->instances[k].name);
                    break;
                }
                k++;
            }
            H5Aclose(attr_id);
        }
    }
    if (!success)
    {
        opt_attrs->instances = NULL;
        opt_attrs->nb_instances = 0;
    }
    return success;
}


// Print all optional attributes
void print_opt_attrs(opt_attrs_t opt_attrs, int space)
{
    hsize_t i;

    for (i = 0; i < opt_attrs.nb_instances; i++)
    {
        switch (opt_attrs.instances[i].type)
        {
        case H5T_INTEGER:
            print_int_attr(opt_attrs.instances[i].name, opt_attrs.instances[i].value.i, space);
            break;
        case H5T_FLOAT:
            print_flt_attr(opt_attrs.instances[i].name, opt_attrs.instances[i].value.f, space);
            break;
        case H5T_COMPOUND:
            print_cpx_attr(opt_attrs.instances[i].name, opt_attrs.instances[i].value.c, space);
            break;
        case H5T_STRING:
            print_str_attr(opt_attrs.instances[i].name, opt_attrs.instances[i].value.s, space);
            break;
        default:
            print_str_attr(opt_attrs.instances[i].name, "UNSUPPORTED DATA TYPE", space);
            break;
        }
    }
}


// Free memory used by optional attributes
void free_opt_attrs(opt_attrs_t *opt_attrs)
{
    hsize_t i;

    if (opt_attrs->nb_instances > 0)
    {

        for (i = 0; i < opt_attrs->nb_instances; i++)
        {
            if (opt_attrs->instances[i].name != NULL)
            {
                free(opt_attrs->instances[i].name);
                opt_attrs->instances[i].name = NULL;
            }
            if (opt_attrs->instances[i].type == H5T_STRING)
                if (opt_attrs->instances[i].value.s != NULL)
                {
                    free(opt_attrs->instances[i].value.s);
                    opt_attrs->instances[i].value.s = NULL;
                }
        }
        free(opt_attrs->instances);
        opt_attrs->instances = NULL;
        opt_attrs->nb_instances = 0;
    }
}


void print_err_dset (const char *category, const char *path)
{
    printf("***** ERROR(%s): Cannot read dataset \"%s\". *****\n\n", category, path);
}

void print_err_attr (const char *category, const char *path, const char *attr_name)
{
    printf("***** ERROR(%s): Cannot read mandatory attribute \"%s[@%s]\". *****\n\n", category, path, attr_name);
}

void print_wrn_attr (const char *category, const char *path, const char *attr_name)
{
    printf("***** WARNING(%s): Invalid attribute value in \"%s[@%s]\". *****\n\n", category, path, attr_name);
}

