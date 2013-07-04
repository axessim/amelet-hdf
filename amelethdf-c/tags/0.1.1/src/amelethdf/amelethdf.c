#include <string.h>
#include <stdlib.h>
#include "amelethdf.h"

children_t read_children_name(hid_t loc_id, const char* path)
{
    H5G_info_t ginfo;
    children_t child;
    herr_t status;
    hsize_t i;
    ssize_t size;
    hid_t group_id;

    group_id = H5Gopen1(loc_id, path);

    status = H5Gget_info(group_id, &ginfo);

    // Get size of name, add 1 for null terminator
    child.nbchild = (int) ginfo.nlinks;
    child.childnames = (char **) malloc(child.nbchild * sizeof(char *));
    for (i = 0; i < child.nbchild; i++)
    {
        size = 1 + H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME,
                H5_ITER_INC, i, NULL, 0, H5P_DEFAULT);
        child.childnames[i] = (char *) malloc(size);
        size = H5Lget_name_by_idx(group_id, ".", H5_INDEX_NAME, H5_ITER_INC, i,
                child.childnames[i], (size_t) size, H5P_DEFAULT);
    }
    H5Gclose(group_id);
    return child;
}

int read_string_attribute(hid_t loc_id, const char* path, char* attr, char **rdata)
{
    hid_t attr_id, filetype, memtype;
    size_t sdim;
    char success = FALSE;

    if (H5Lexists(loc_id, path, H5P_DEFAULT) == TRUE || strcmp(path, ".") == 0)
        if (H5Aexists_by_name(loc_id, path, attr, H5P_DEFAULT) > 0)
        {
            attr_id = H5Aopen_by_name(loc_id, path, attr, H5P_DEFAULT, H5P_DEFAULT);
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
    return 0;

}

float read_float_attribute(hid_t loc_id, const char* path, char* attr)
{
    herr_t status;
    float rdata;

    status = H5Aexists_by_name(loc_id, path, attr, H5P_DEFAULT);
    if (status < 0)
    {
        printf("%s doesn't exist for %s\n", attr, path);
    }
    else
    {
        H5LTget_attribute_float(loc_id, path, attr, &rdata);
    }
    return rdata;
}

int read_int_attribute(hid_t loc_id, const char* path, char* attr)
{
    herr_t status;
    int idata;

    status = H5Aexists_by_name(loc_id, path, attr, H5P_DEFAULT);
    if (status < 0)
    {
        printf("%s doesn't exist for %s\n", attr, path);
    }
    else
    {
        H5LTget_attribute_int(loc_id, path, attr, &idata);
    }
    return idata;
}

int read_number_of_children(hid_t file_id, const char* path)
{
    int nbchild;
    H5G_info_t ginfo;
    herr_t status;
    status = H5Gget_info_by_name(file_id, path, &ginfo, H5P_DEFAULT);
    // Get size of name, add 1 for null terminator.
    nbchild = (int) ginfo.nlinks;
    return nbchild;
}
