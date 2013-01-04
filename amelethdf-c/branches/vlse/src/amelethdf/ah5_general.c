#include "ah5_general.h"


/*
    FALSE = AH5_version_minimum("0.5", "0.1");
    FALSE = AH5_version_minimum("3.5", "2");
    FALSE = AH5_version_minimum("3.1", "2.186");

    TRUE = AH5_version_minimum("1.2.3.4", "1.2.3.4");
    TRUE = AH5_version_minimum("0.5", "0.6");
    TRUE = AH5_version_minimum("0.5", "1.8.6");
    TRUE = AH5_version_minimum("0.5.5", "1");
    TRUE = AH5_version_minimum("2", "2.5");
    TRUE = AH5_version_minimum("2.0.5.0", "2.1");
*/

AH5_complex_t AH5_set_complex(float real, float imag)
{
	AH5_complex_t rdata;

#ifndef _MSC_VER
	rdata =  real + imag * _Complex_I;
#else
	rdata.re = real;
	rdata.im = imag;
#endif /*__MSC_VER__*/

	return rdata;
}

char AH5_version_minimum (const char *required_version, const char *sim_version)
{
    char rdata = FALSE;
    char *a, *b, *atemp, *btemp, *temp;
    int ai = 0, bi = 0;  // storage for version numbers

    // cannot use 2x strtok
    a = AH5_trim_zeros(required_version);  // original pointers
    b = AH5_trim_zeros(sim_version);  //  to be used in free()
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

char *AH5_trim_zeros (const char *version)
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

char AH5_path_valid (hid_t file_id, const char *path)
{
    char *temp;
    int i, slashes = 0;

    temp = strdup(path);
    for (i = strlen(path); i > 0; i--)
        if (temp[i] == '/')
        {
            temp[i] = '\0';
            slashes++;  /* count number of slashes excluding the first one */
        }

    if(H5Lexists(file_id, temp, H5P_DEFAULT) != TRUE)
    {
        free(temp);
        return FALSE;
    }

    i = 1;
    while (slashes > 0)
    {
        while (temp[i] != '\0')
            i++;
        temp[i] = '/';
        slashes--;
        if(H5Lexists(file_id, temp, H5P_DEFAULT) != TRUE)
        {
            free(temp);
            return FALSE;
        }
    }
    free(temp);
    return TRUE;
}

// Add aelement to aset
AH5_set_t AH5_add_to_set(AH5_set_t aset, char *aelement)
{
    hsize_t id = 0;

    if (!AH5_index_in_set(aset, aelement, &id))
    {
        aset.values[aset.nb_values] = strdup(aelement);  // malloc!!!
        aset.nb_values++;
    }
    return aset;
}


// Return the index of first occurrence of an element
int AH5_index_in_set(AH5_set_t aset, char *aelement, hsize_t *index)
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
AH5_children_t AH5_read_children_name (hid_t file_id, const char* path)
{
    H5G_info_t ginfo;
    AH5_children_t children;
    hsize_t i, j = 0;
    ssize_t size;
    hid_t group_id;
    char temp[AH5_ELEMENT_NAME_LENGTH], *temp2;

    children.childnames = NULL;
    /*
        - path must exist
        - number of children must be greater than zero
        - element name
            - must be readable
            - must be shorter than AH5_ELEMENT_NAME_LENGTH
            - must NOT be same as "_param"
    */
    if (H5Lexists(file_id, path, H5P_DEFAULT) == TRUE || strcmp(path, "/") == 0)
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
                else if (size >= AH5_ELEMENT_NAME_LENGTH)
                {
                    temp2 = (char *) malloc ((size + 1) * sizeof(char));
                    H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME, H5_ITER_INC, i, temp2, size + 1, H5P_DEFAULT);
                    printf("***** ERROR: Maximum name length (%i) exceeded in \"%s/%s\". *****\n\n", AH5_ELEMENT_NAME_LENGTH - 1, path, temp2);
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
char *AH5_get_name_from_path (const char *path)
{
    int i;

    for (i = strlen(path); i > -1; i--)
        if (path[i] == '/')
            break;
    return (char *) path + i + 1;
}

// Get base part of a path; allocates new memory!
char *AH5_get_base_from_path (const char *path)
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
char AH5_read_int_attr (hid_t file_id, const char* path, char* attr, int *rdata)
{
    char success = FALSE;

    if (H5Lexists(file_id, path, H5P_DEFAULT) == TRUE)
        if (H5Aexists_by_name(file_id, path, attr, H5P_DEFAULT) > 0)
            if (H5LTget_attribute_int(file_id, path, attr, rdata) >= 0)
                success = TRUE;
    if(!success)
        *rdata = 0;
    return success;
}


// Read float attribute <attr_name> given by address <path>
char AH5_read_flt_attr (hid_t file_id, const char* path, char* attr_name, float *rdata)
{
    char success = FALSE;

    if (H5Lexists(file_id, path, H5P_DEFAULT) == TRUE)
        if (H5Aexists_by_name(file_id, path, attr_name, H5P_DEFAULT) > 0)
            if (H5LTget_attribute_float(file_id, path, attr_name, rdata) >= 0)
                success = TRUE;
    if(!success)
        *rdata = 0;
    return success;
}


// Read complex attribute <attr_name> given by address <path>
char AH5_read_cpx_attr (hid_t file_id, const char* path, char* attr_name, AH5_complex_t *rdata)
{
    hid_t attr_id, type_id;
    float buf[2];
    char success = FALSE;

    if (H5Lexists(file_id, path, H5P_DEFAULT) == TRUE)
        if (H5Aexists_by_name(file_id, path, attr_name, H5P_DEFAULT) > 0)
        {
            attr_id = H5Aopen_by_name(file_id, path, attr_name, H5P_DEFAULT, H5P_DEFAULT);
            type_id = H5Tcreate(H5T_COMPOUND, H5Tget_size(H5T_NATIVE_FLOAT) * 2);
            H5Tinsert(type_id, "r", 0, H5T_NATIVE_FLOAT);
            H5Tinsert(type_id, "i", H5Tget_size(H5T_NATIVE_FLOAT), H5T_NATIVE_FLOAT);
            if (H5Aread(attr_id, type_id, buf) >= 0)
            {
                *rdata = AH5_set_complex(buf[0], buf[1]);
                success = TRUE;
            }
            H5Tclose(type_id);
            H5Aclose(attr_id);
        }
    if (!success)
        *rdata = AH5_set_complex(0, 0);
    return success;
}


// Read string attribute <attr_name> given by address <path>
char AH5_read_str_attr (hid_t file_id, const char *path, char *attr_name, char **rdata)
{
    hid_t attr_id, filetype, memtype;
    size_t sdim;
    char success = FALSE;

    if (H5Lexists(file_id, path, H5P_DEFAULT) == TRUE || strcmp(path, ".") == 0)
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
void AH5_print_int_attr (char *name, int value, int space)
{
    printf("%*s-@%s: %i\n", space, "", name, value);
}


// Print float attribute
void AH5_print_flt_attr (char *name, float value, int space)
{
    printf("%*s-@%s: %g\n", space, "", name, value);
}


// Print complex attribute
void AH5_print_cpx_attr (char *name, AH5_complex_t value, int space)
{
    printf("%*s-@%s: %g%+gi\n", space, "", name, creal(value), cimag(value));
}


// Print string attribute
void AH5_print_str_attr (char *name, char *value, int space)
{
    printf("%*s-@%s: %s\n", space, "", name, value);
}



// Read int dataset
char AH5_read_int_dataset (hid_t file_id, const char *path, const hsize_t mn, int **rdata)
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
char AH5_read_float_dataset (hid_t file_id, const char *path, const hsize_t mn, float **rdata)
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
char AH5_read_complex_dataset (hid_t file_id, const char *path, const hsize_t mn, AH5_complex_t **rdata)
{
    char success = FALSE;
    hid_t dset_id, type_id;
    hsize_t i;
    float *buf;

    *rdata = (AH5_complex_t *) malloc(mn * sizeof(AH5_complex_t));
    buf = (float *) malloc(mn * 2 * sizeof(float));
    type_id = H5Tcreate(H5T_COMPOUND, H5Tget_size(H5T_NATIVE_FLOAT) * 2);
    H5Tinsert(type_id, "r", 0, H5T_NATIVE_FLOAT);
    H5Tinsert(type_id, "i", H5Tget_size(H5T_NATIVE_FLOAT), H5T_NATIVE_FLOAT);
    dset_id = H5Dopen(file_id, path, H5P_DEFAULT);
    if (H5Dread(dset_id, type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, *rdata) >= 0)
    {
        success = TRUE;
        for (i = 0; i < mn; i++)
            rdata[0][i] = AH5_set_complex(buf[2*i], buf[2*i+1]);
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
char AH5_read_string_dataset(hid_t file_id, const char *path, const hsize_t mn, size_t length, char ***rdata)
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
char AH5_read_opt_attrs (hid_t file_id, const char *path, AH5_opt_attrs_t *opt_attrs, char mandatory_attrs[][AH5_ATTR_LENGTH], size_t nb_mandatory_attrs)
{
    char success = FALSE, is_mandatory;
    H5O_info_t object_info;
    hsize_t i, j, k = 0;
    hid_t attr_id, type_id, memtype;
    float buf[2];
    hsize_t nb_present_mandatory_attrs = 0;
    char temp_name[AH5_ATTR_LENGTH];

    if (H5Lexists(file_id, path, H5P_DEFAULT) == TRUE)
    {
        // Check presence of all mandatory attributes
        for (i = 0; i < (hsize_t) nb_mandatory_attrs; i++)
            if (H5Aexists_by_name(file_id, path, mandatory_attrs[i], H5P_DEFAULT) > 0)
                nb_present_mandatory_attrs++;
        H5Oget_info_by_name(file_id, path, &object_info, H5P_DEFAULT);
        opt_attrs->nb_instances = object_info.num_attrs - nb_present_mandatory_attrs;
        if (opt_attrs->nb_instances > 0)
            opt_attrs->instances = (AH5_attr_instance_t *) malloc (opt_attrs->nb_instances * sizeof(AH5_attr_instance_t));
        for (i = 0; i < object_info.num_attrs; i++)
        {
            is_mandatory = FALSE;
            attr_id = H5Aopen_by_idx(file_id, path, H5_INDEX_CRT_ORDER, H5_ITER_NATIVE, i, H5P_DEFAULT, H5P_DEFAULT);
            H5Aget_name(attr_id, AH5_ATTR_LENGTH, temp_name);
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
                    opt_attrs->instances[k].value.c = AH5_set_complex(0, 0);
                    type_id = H5Tcreate(H5T_COMPOUND, H5Tget_size(H5T_NATIVE_FLOAT) * 2);
                    H5Tinsert(type_id, "r", 0, H5T_NATIVE_FLOAT);
                    H5Tinsert(type_id, "i", H5Tget_size(H5T_NATIVE_FLOAT), H5T_NATIVE_FLOAT);
                    if (H5Aread(attr_id, type_id, buf) >= 0)
                    {
						opt_attrs->instances[k].value.c = AH5_set_complex(buf[0], buf[1]);
                        success = TRUE;
                    }
                    H5Tclose(type_id);
                    break;
                case H5T_STRING:
                    opt_attrs->instances[k].value.s = NULL;
                    memtype = H5Tcopy(H5T_C_S1);
                    H5Tset_size(memtype, AH5_ATTR_LENGTH);
                    opt_attrs->instances[k].value.s = (char *) malloc(AH5_ATTR_LENGTH * sizeof(char));
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
void AH5_print_opt_attrs(const AH5_opt_attrs_t *opt_attrs, int space)
{
    hsize_t i;

    for (i = 0; i < opt_attrs->nb_instances; i++)
    {
        switch (opt_attrs->instances[i].type)
        {
        case H5T_INTEGER:
            AH5_print_int_attr(opt_attrs->instances[i].name, opt_attrs->instances[i].value.i, space);
            break;
        case H5T_FLOAT:
            AH5_print_flt_attr(opt_attrs->instances[i].name, opt_attrs->instances[i].value.f, space);
            break;
        case H5T_COMPOUND:
            AH5_print_cpx_attr(opt_attrs->instances[i].name, opt_attrs->instances[i].value.c, space);
            break;
        case H5T_STRING:
            AH5_print_str_attr(opt_attrs->instances[i].name, opt_attrs->instances[i].value.s, space);
            break;
        default:
            AH5_print_str_attr(opt_attrs->instances[i].name, "UNSUPPORTED DATA TYPE", space);
            break;
        }
    }
}


// Free memory used by optional attributes
void AH5_free_opt_attrs(AH5_opt_attrs_t *opt_attrs)
{
    hsize_t i;

    if (opt_attrs->instances != NULL)
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

void AH5_print_err_dset (const char *category, const char *path)
{
    printf("\n***** ERROR(%s): Cannot read dataset \"%s\". *****\n\n", category, path);
}

void AH5_print_err_tble (const char *category, const char *path)
{
    printf("\n***** ERROR(%s): Cannot read table \"%s\". *****\n\n", category, path);
}

void AH5_print_err_attr (const char *category, const char *path, const char *attr_name)
{
    printf("\n***** ERROR(%s): Cannot read mandatory attribute \"%s[@%s]\". *****\n\n", category, path, attr_name);
}

void AH5_print_err_path (const char *category, const char *path)
{
    printf("\n***** ERROR(%s): Cannot read path \"%s\". *****\n\n", category, path);
}

void AH5_print_err_inv_attr (const char *category, const char *path, const char *attr_name)
{
    printf("\n***** ERROR(%s): Invalid attribute value in \"%s[@%s]\". *****\n\n", category, path, attr_name);
}

void AH5_print_wrn_attr (const char *category, const char *path, const char *attr_name)
{
    printf("\n***** WARNING(%s): Invalid attribute value in \"%s[@%s]\". *****\n\n", category, path, attr_name);
}

