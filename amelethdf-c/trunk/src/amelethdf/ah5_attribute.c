#include "ah5_attribute.h"


// Read integer attribute <attr_name> given by address <path>
char AH5_read_int_attr(hid_t loc_id, const char *path, char *attr, int *rdata)
{
    char success = AH5_FALSE;

    if (AH5_path_valid(loc_id, path))
        if (H5Aexists_by_name(loc_id, path, attr, H5P_DEFAULT) > 0)
            if (H5LTget_attribute_int(loc_id, path, attr, rdata) >= 0)
                success = AH5_TRUE;
    if(!success)
        *rdata = 0;
    return success;
}


// Read float attribute <attr_name> given by address <path>
char AH5_read_flt_attr(hid_t loc_id, const char *path, char *attr_name, float *rdata)
{
    char success = AH5_FALSE;

    if (AH5_path_valid(loc_id, path))
        if (H5Aexists_by_name(loc_id, path, attr_name, H5P_DEFAULT) > 0)
            if (H5LTget_attribute_float(loc_id, path, attr_name, rdata) >= 0)
                success = AH5_TRUE;
    if(!success)
        *rdata = 0;
    return success;
}


// Read complex attribute <attr_name> given by address <path>
char AH5_read_cpx_attr(hid_t loc_id, const char* path, char* attr_name, AH5_complex_t *rdata)
{
    hid_t attr_id, type_id;
    float buf[2];
    char success = AH5_FALSE;

    if (AH5_path_valid(loc_id, path))
        if (H5Aexists_by_name(loc_id, path, attr_name, H5P_DEFAULT) > 0)
        {
            attr_id = H5Aopen_by_name(loc_id, path, attr_name, H5P_DEFAULT, H5P_DEFAULT);
            type_id = AH5_H5Tcreate_cpx_memtype();
            if (H5Aread(attr_id, type_id, buf) >= 0)
            {
                *rdata = AH5_set_complex(buf[0], buf[1]);
                success = AH5_TRUE;
            }
            H5Tclose(type_id);
            H5Aclose(attr_id);
        }
    if (!success)
        *rdata = AH5_set_complex(0, 0);
    return success;
}


// Read string attribute <attr_name> given by address <path>
char AH5_read_str_attr(hid_t loc_id, const char *path, char *attr_name, char **rdata)
{
    hid_t attr_id, filetype, memtype;
    size_t sdim;
    char success = AH5_FALSE;

    if (AH5_path_valid(loc_id, path) || strcmp(path, ".") == 0)
        if (H5Aexists_by_name(loc_id, path, attr_name, H5P_DEFAULT) > 0)
        {
            attr_id = H5Aopen_by_name(loc_id, path, attr_name, H5P_DEFAULT, H5P_DEFAULT);
            filetype = H5Aget_type(attr_id);
            sdim = H5Tget_size(filetype);
            sdim++;  // make a space for null terminator
            *rdata = (char *) malloc(sdim * sizeof(char));
            memtype = H5Tcopy(H5T_C_S1);
            H5Tset_size(memtype, sdim);
            if (H5Aread(attr_id, memtype, *rdata) >= 0)
                success = AH5_TRUE;
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


// Read all optional attributes
char AH5_read_opt_attrs(hid_t loc_id, const char *path, AH5_opt_attrs_t *opt_attrs, char mandatory_attrs[][AH5_ATTR_LENGTH], size_t nb_mandatory_attrs)
{
    char success = AH5_FALSE, is_mandatory;
    H5O_info_t object_info;
    hsize_t i, j, k = 0;
    hid_t attr_id, type_id, memtype;
    float buf[2];
    hsize_t nb_present_mandatory_attrs = 0;
    char temp_name[AH5_ATTR_LENGTH];

    if (AH5_path_valid(loc_id, path))
    {
        // Check presence of all mandatory attributes
        for (i = 0; i < (hsize_t) nb_mandatory_attrs; i++)
            if (H5Aexists_by_name(loc_id, path, mandatory_attrs[i], H5P_DEFAULT) > 0)
                nb_present_mandatory_attrs++;
        H5Oget_info_by_name(loc_id, path, &object_info, H5P_DEFAULT);
        opt_attrs->nb_instances = object_info.num_attrs - nb_present_mandatory_attrs;
        if (opt_attrs->nb_instances > 0)
            opt_attrs->instances = (AH5_attr_instance_t *) malloc (opt_attrs->nb_instances * sizeof(AH5_attr_instance_t));
        for (i = 0; i < object_info.num_attrs; i++)
        {
            is_mandatory = AH5_FALSE;
            attr_id = H5Aopen_by_idx(loc_id, path, H5_INDEX_CRT_ORDER, H5_ITER_NATIVE, i, H5P_DEFAULT, H5P_DEFAULT);
            H5Aget_name(attr_id, AH5_ATTR_LENGTH, temp_name);
            for (j = 0; j < nb_mandatory_attrs; j++)
                if (strcmp(temp_name, mandatory_attrs[j]) == 0)
                    is_mandatory = AH5_TRUE;
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
                        success = AH5_TRUE;
                    break;
                case H5T_FLOAT:
                    opt_attrs->instances[k].value.f = 0;
                    if (H5Aread(attr_id, H5T_NATIVE_FLOAT, &(opt_attrs->instances[k].value.f)) >= 0)
                        success = AH5_TRUE;
                    break;
                case H5T_COMPOUND:
                    opt_attrs->instances[k].value.c = AH5_set_complex(0, 0);
                    type_id = AH5_H5Tcreate_cpx_memtype();
                    if (H5Aread(attr_id, type_id, buf) >= 0)
                    {
                        opt_attrs->instances[k].value.c = AH5_set_complex(buf[0], buf[1]);
                        success = AH5_TRUE;
                    }
                    H5Tclose(type_id);
                    break;
                case H5T_STRING:
                    opt_attrs->instances[k].value.s = NULL;
                    memtype = H5Tcopy(H5T_C_S1);
                    H5Tset_size(memtype, AH5_ATTR_LENGTH);
                    opt_attrs->instances[k].value.s = (char *) malloc(AH5_ATTR_LENGTH * sizeof(char));
                    if (H5Aread(attr_id, memtype, opt_attrs->instances[k].value.s) >= 0)
                        success = AH5_TRUE;
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



// Write int attribute <attr_name> given by address <path>
char AH5_write_int_attr(hid_t loc_id, const char *path, char* attr_name, const int wdata)
{
    char success = AH5_FALSE;

    if (H5LTset_attribute_int(loc_id, path, attr_name, &wdata, 1) >= 0)
        success = AH5_TRUE;

    return success;
}


// Write float attribute <attr_name> given by address <path>
char AH5_write_flt_attr(hid_t loc_id, const char *path, char* attr_name, const float wdata)
{
    char success = AH5_FALSE;

    if (H5LTset_attribute_float(loc_id, path, attr_name, &wdata, 1) >= 0)
        success = AH5_TRUE;

    return success;
}


// Write complex attribute <attr_name> given by address <path>
char AH5_write_cpx_attr(hid_t loc_id, const char* path, char* attr_name, const AH5_complex_t *wdata)
{
    char success = AH5_FALSE;
    hid_t attr_id, cpx_filetype, cpx_memtype, object_id, space;
    hsize_t dims[1] = {1};

    cpx_memtype = AH5_H5Tcreate_cpx_memtype();
    cpx_filetype = AH5_H5Tcreate_cpx_filetype();
    object_id = H5Oopen(loc_id, path, H5P_DEFAULT);
    space = H5Screate_simple (1, dims, NULL);

    attr_id = H5Acreate(object_id, attr_name, cpx_filetype, space, H5P_DEFAULT, H5P_DEFAULT);
    if (H5Awrite(attr_id, cpx_memtype, wdata) >= 0)
        success = AH5_TRUE;

    H5Aclose(attr_id);
    H5Sclose(space);
    H5Oclose(object_id);
    H5Tclose(cpx_filetype);
    H5Tclose(cpx_memtype);

    return success;
}


// Write string attribute <attr_name> given by address <path>
char AH5_write_str_attr(hid_t loc_id, const char *path, char *attr_name, const char *wdata)
{
    char success = AH5_FALSE;

    if (H5LTset_attribute_string(loc_id, path, attr_name, wdata) >= 0);
    success = AH5_TRUE;

    return success;
}


// Write all optional attributes
char AH5_write_opt_attrs(hid_t file_id, const char *path, AH5_opt_attrs_t *opt_attrs)
{
    char success = AH5_TRUE;
    hsize_t i;

    for (i = 0; i < opt_attrs->nb_instances && success; i++)
    {
        switch (opt_attrs->instances[i].type)
        {
        case H5T_INTEGER:
            if (!AH5_write_int_attr(file_id, path, opt_attrs->instances[i].name, opt_attrs->instances[i].value.i))
                success = AH5_FALSE;
            break;
        case H5T_FLOAT:
            if (!AH5_write_flt_attr(file_id, path, opt_attrs->instances[i].name, opt_attrs->instances[i].value.f))
                success = AH5_FALSE;
            break;
        case H5T_COMPOUND:
            if (!AH5_write_cpx_attr(file_id, path, opt_attrs->instances[i].name, &(opt_attrs->instances[i].value.c)))
                success = AH5_FALSE;
            break;
        case H5T_STRING:
            if (!AH5_write_str_attr(file_id, path, opt_attrs->instances[i].name, opt_attrs->instances[i].value.s))
                success = AH5_FALSE;
            break;
        default:
            success = AH5_FALSE;
            break;
        }
    }
    return success;
}



// Print integer attribute
void AH5_print_int_attr(char *name, int value, int space)
{
    printf("%*s-@%s: %i\n", space, "", name, value);
}


// Print float attribute
void AH5_print_flt_attr(char *name, float value, int space)
{
    printf("%*s-@%s: %g\n", space, "", name, value);
}


// Print complex attribute
void AH5_print_cpx_attr(char *name, AH5_complex_t value, int space)
{
    printf("%*s-@%s: %g%+gi\n", space, "", name, creal(value), cimag(value));
}


// Print string attribute
void AH5_print_str_attr(char *name, char *value, int space)
{
    printf("%*s-@%s: %s\n", space, "", name, value);
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



