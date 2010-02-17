#include <stdlib.h>
#include <string.h>

#include "mesh.h"

int meshtype(hid_t loc_id, const char * path)
{
    herr_t status;
    hid_t attr_id, attr_type, space, memtype, mesh_id;
    size_t sdim;
    hsize_t dims[1] = { 1 };
    int ndims, i, ret_val = -1;
    char **rdata;

    mesh_id = H5Gopen1(loc_id, path);

    ret_val = 0;
    attr_id = H5Aopen(mesh_id, "type", H5P_DEFAULT);

    if (attr_id >= 0)
    {
        attr_type = H5Aget_type(attr_id);
        sdim = H5Tget_size(attr_type);
        sdim++; // Make room for null terminator
        space = H5Aget_space(attr_id);
        ndims = H5Sget_simple_extent_dims(space, dims, NULL);
        rdata = (char **) malloc(dims[0] * sizeof(char *));
        rdata[0] = (char *) malloc(dims[0] * sdim * sizeof(char));

        // Set the rest of the pointers to rows to the correct addresses.
        for (i = 1; i < dims[0]; i++)
            rdata[i] = rdata[0] + i * sdim;
        memtype = H5Tcopy(H5T_C_S1);
        status = H5Tset_size(memtype, sdim);
        status = H5Aread(attr_id, memtype, rdata[0]);
        // Check the value of FORMAT attribute.
        if (strcmp(rdata[0], "unstructured") == 0)
            ret_val = UNSTRUCTURED;
        else if (strcmp(rdata[0], "structured") == 0)
            ret_val = STRUCTURED;
        H5Aclose(attr_id);
    }
    H5Gclose(mesh_id);
    return ret_val;

}

groupgroup_t readGroupGroup(hid_t grpgrp_id, const char* name)
{
    hid_t space, filetype, memtype;
    int ndims, i;
    hsize_t dims[1];
    groupgroup_t rdata;
    herr_t status;
    size_t sdim;

    rdata.name = (char *) malloc(ELEMENT_NAME_LENGTH * sizeof(char));
    strcpy(rdata.name, name);
    filetype = H5Dget_type(grpgrp_id);
    sdim = H5Tget_size(filetype);
    sdim++; // make room for null terminator

    space = H5Dget_space(grpgrp_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);

    // allocate array of pointers to rows
    rdata.groupGroupnames = (char **) malloc(dims[0] * sizeof(char *));
    // allocate space for integer data
    rdata.groupGroupnames[0] = (char *) malloc(dims[0] * sdim * sizeof(char));
    rdata.nbeltgroupGroup = dims[0];
    for (i = 1; i < dims[0]; i++)
        rdata.groupGroupnames[i] = rdata.groupGroupnames[0] + i * sdim;

    // create the memory datatype
    memtype = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(memtype, sdim);

    // read the data
    status = H5Dread(grpgrp_id, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.groupGroupnames[0]);

    return rdata;
}
