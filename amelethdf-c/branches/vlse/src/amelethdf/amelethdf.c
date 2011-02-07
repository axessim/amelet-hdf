#include "amelethdf.h"


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
    hsize_t i;
    hid_t group_id;
    char *temp;

    children.nb_children = 0;
    children.childnames = NULL;
    if ((H5Lexists(file_id, path, H5P_DEFAULT) != FALSE) || strcmp(path, "/") == 0)
    {
        temp = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        group_id = H5Gopen1(file_id, path);
        H5Gget_info(group_id, &ginfo);

        // Get size of name, add 1 for null terminator
        children.nb_children = ginfo.nlinks;
        if (children.nb_children > 0)
        {
            children.childnames = (char **) malloc(children.nb_children * sizeof(char *));
            for (i = 0; i < children.nb_children; i++)
            {
                H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME, H5_ITER_INC, i, temp, (size_t) ABSOLUTE_PATH_NAME_LENGTH, H5P_DEFAULT);
                children.childnames[i] = (char *) malloc((strlen(temp) + 2) * sizeof(char));
                strcpy(children.childnames[i], "/");
                strcat(children.childnames[i], temp);
            }
        }
        H5Gclose(group_id);
        free(temp);
    }
    return children;
}


// Get last part of a path
char *get_name_from_path (const char *path)
{
    size_t i;
    char *rdata;

    i = strlen(path);
    while (path[i] != '/')
        i--;
    rdata = strdup((char *) path + i + 1);
    return rdata;
}




// Read string attribute <attr_name> given by address <path>
char read_string_attribute (hid_t file_id, const char *path, char *attr_name, char **rdata)
{
    hid_t attr_id, filetype, memtype;
    size_t sdim;
    char success = FALSE;

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


// Read float attribute <attr_name> given by address <path>
char read_float_attribute (hid_t file_id, const char* path, char* attr_name, float *rdata)
{
    char success = FALSE;

    if (H5Aexists_by_name(file_id, path, attr_name, H5P_DEFAULT) >= 0)
        if (H5LTget_attribute_float(file_id, path, attr_name, rdata) >= 0)
            success = TRUE;
    if(!success)
        *rdata = 0;
    return success;
}


// Read int attribute <attr_name> given by address <path>
char read_int_attribute (hid_t file_id, const char* path, char* attr, int *rdata)
{
    char success = FALSE;

    if (H5Aexists_by_name(file_id, path, attr, H5P_DEFAULT) >= 0)
        if (H5LTget_attribute_int(file_id, path, attr, rdata) >= 0)
            success = TRUE;
    if(!success)
        *rdata = 0;
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

    *rdata = (char **) malloc((size_t) (mn * sizeof(char*)));
    **rdata = (char *) malloc((size_t) (mn * length * sizeof(char)));
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


// Read int dataset
char read_int_dataset (hid_t file_id, const char *path, const hsize_t mn, int **rdata)
{
    char success = FALSE;
    hid_t dset_id;

    *rdata = (int *) malloc((size_t) (mn * sizeof(int)));
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

    *rdata = (float *) malloc((size_t) (mn * sizeof(float)));
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

    *rdata = (complex float *) malloc((size_t) (mn * sizeof(complex float)));
    buf = (float *) malloc((size_t) (mn * 2 * sizeof(float)));
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




// Read all optional attributes
char read_optional_attributes (hid_t file_id, const char *path, optional_attributes_t *optional_attributes, char mandatory_attributes[][ATTRIBUTE_LENGTH], size_t nb_mandatory_attributes)
{
    char success = FALSE, is_mandatory;
    H5O_info_t object_info;
    hsize_t i, j, k = 0;
    hid_t attr_id, type_id, memtype;
    float buf[2];
    hsize_t nb_present_mandatory_attributes = 0;
    char temp_name[ATTRIBUTE_LENGTH];

    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
    {
        // Check presence of all mandatory attributes
        for (i = 0; i < (hsize_t) nb_mandatory_attributes; i++)
            if (H5Aexists_by_name(file_id, path, mandatory_attributes[i], H5P_DEFAULT) > 0)
                nb_present_mandatory_attributes++;
        H5Oget_info_by_name(file_id, path, &object_info, H5P_DEFAULT);
        optional_attributes->nb_attribute_instances = object_info.num_attrs - nb_present_mandatory_attributes;
        if (optional_attributes->nb_attribute_instances > 0)
            optional_attributes->attribute_instances = (attribute_instance_t *) malloc (((size_t) optional_attributes->nb_attribute_instances) * sizeof(attribute_instance_t));
        for (i = 0; i < object_info.num_attrs; i++)
        {
            is_mandatory = FALSE;
            attr_id = H5Aopen_by_idx(file_id, path, H5_INDEX_CRT_ORDER, H5_ITER_NATIVE, i, H5P_DEFAULT, H5P_DEFAULT);
            H5Aget_name(attr_id, ATTRIBUTE_LENGTH, temp_name);
            for (j = 0; j < nb_mandatory_attributes; j++)
                if (strcmp(temp_name, mandatory_attributes[j]) == 0)
                    is_mandatory = TRUE;
            if (!is_mandatory)
            {
                optional_attributes->attribute_instances[k].name = strdup(temp_name);
                type_id = H5Aget_type(attr_id);
                optional_attributes->attribute_instances[k].type = H5Tget_class(type_id);
                H5Tclose(type_id);
                switch (optional_attributes->attribute_instances[k].type)
                {
                case H5T_INTEGER:
                    optional_attributes->attribute_instances[k].value.i = 0;
                    if (H5Aread(attr_id, H5T_NATIVE_INT, &(optional_attributes->attribute_instances[k].value.i)) >= 0)
                        success = TRUE;
                    break;
                case H5T_FLOAT:
                    optional_attributes->attribute_instances[k].value.f = 0;
                    if (H5Aread(attr_id, H5T_NATIVE_FLOAT, &(optional_attributes->attribute_instances[k].value.f)) >= 0)
                        success = TRUE;
                    break;
                case H5T_COMPOUND:
                    optional_attributes->attribute_instances[k].value.c = 0 + 0 * _Complex_I;
                    type_id = H5Tcreate(H5T_COMPOUND, H5Tget_size(H5T_NATIVE_FLOAT) * 2);
                    H5Tinsert(type_id, "r", 0, H5T_NATIVE_FLOAT);
                    H5Tinsert(type_id, "i", H5Tget_size(H5T_NATIVE_FLOAT), H5T_NATIVE_FLOAT);
                    if (H5Aread(attr_id, type_id, buf) >= 0)
                    {
                        optional_attributes->attribute_instances[k].value.c = buf[0] + buf[1] * _Complex_I;
                        success = TRUE;
                    }
                    H5Tclose(type_id);
                    break;
                case H5T_STRING:
                    optional_attributes->attribute_instances[k].value.s = NULL;
                    memtype = H5Tcopy(H5T_C_S1);
                    H5Tset_size(memtype, ATTRIBUTE_LENGTH);
                    optional_attributes->attribute_instances[k].value.s = (char *) malloc(ATTRIBUTE_LENGTH * sizeof(char));
                    if (H5Aread(attr_id, memtype, optional_attributes->attribute_instances[k].value.s) >= 0)
                        success = TRUE;
                    H5Tclose(memtype);
                    break;
                default:
                    optional_attributes->attribute_instances[k].type = H5T_NO_CLASS;
                    printf("***** WARNING: Unsupported type of attribute \"%s@%s\". *****\n\n", path, optional_attributes->attribute_instances[k].name);
                    break;
                }
                k++;
            }
            H5Aclose(attr_id);
        }
    }
    if (!success)
    {
        optional_attributes->attribute_instances = NULL;
        optional_attributes->nb_attribute_instances = 0;
    }
    return success;
}


// Print all optional attributes
void print_optional_attributes(optional_attributes_t optional_attributes)
{
    hsize_t i;

    for (i = 0; i < optional_attributes.nb_attribute_instances; i++)
    {
        switch (optional_attributes.attribute_instances[i].type)
        {
        case H5T_INTEGER:
            printf("      -@%s: %i\n", optional_attributes.attribute_instances[i].name, optional_attributes.attribute_instances[i].value.i);
            break;
        case H5T_FLOAT:
            printf("      -@%s: %g\n", optional_attributes.attribute_instances[i].name, optional_attributes.attribute_instances[i].value.f);
            break;
        case H5T_COMPOUND:
            printf("      -@%s: %g%+gi\n", optional_attributes.attribute_instances[i].name, creal(optional_attributes.attribute_instances[i].value.c), cimag(optional_attributes.attribute_instances[i].value.c));
            break;
        case H5T_STRING:
            printf("      -@%s: %s\n", optional_attributes.attribute_instances[i].name, optional_attributes.attribute_instances[i].value.s);
            break;
        default:
            printf("      -@%s: UNSUPPORTED DATA TYPE\n", optional_attributes.attribute_instances[i].name);
            break;
        }
    }
}


// Free memory used by optional attributes
void free_optional_attributes(optional_attributes_t *optional_attributes)
{
    hsize_t i;

    if (optional_attributes->nb_attribute_instances > 0)
    {

        for (i = 0; i < optional_attributes->nb_attribute_instances; i++)
        {
            if (optional_attributes->attribute_instances[i].name != NULL)
            {
                free(optional_attributes->attribute_instances[i].name);
                optional_attributes->attribute_instances[i].name = NULL;
            }
            if (optional_attributes->attribute_instances[i].type == H5T_STRING)
                if (optional_attributes->attribute_instances[i].value.s != NULL)
                {
                    free(optional_attributes->attribute_instances[i].value.s);
                    optional_attributes->attribute_instances[i].value.s = NULL;
                }
        }
        free(optional_attributes->attribute_instances);
        optional_attributes->attribute_instances = NULL;
        optional_attributes->nb_attribute_instances = 0;
    }
}
