#include <stdlib.h>
#include <string.h>

#include "unstructuredmesh.h"

nodes_t readNodes(hid_t nodes_id)
{
    hid_t space;
    int ndims, i;

    hsize_t dims[2];
    nodes_t rdata;
    herr_t status;

    /*
     * Get dataspace and allocate memory for read buffer.  HDF5Nodes is a
     * two dimensional dataset so the dynamic allocation must be done
     * in steps.
     */
    space = H5Dget_space(nodes_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);

    /*
     * Allocate array of pointers to rows.
     */
    rdata.nodes = (float **) malloc(dims[0] * sizeof(float *));

    /*
     * Allocate space for floating point data.
     */
    rdata.nodes[0] = (float *) malloc(dims[0] * dims[1] * sizeof(float));

    /*
     * Set the rest of the pointers to rows to the correct addresses.
     */
    for (i = 1; i < dims[0]; i++)
        rdata.nodes[i] = rdata.nodes[0] + i * dims[1];
    rdata.nbnodes = dims[0];
    status = H5Dread(nodes_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.nodes[0]);
    return rdata;
}

elttypes_t readElementTypes(hid_t eltypes_id)
{
    hid_t space;
    int ndims, i;
    hsize_t dims[1];
    elttypes_t rdata;
    herr_t status;

    space = H5Dget_space(eltypes_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    /*
     * Allocate array of pointers to rows.
     */
    rdata.elttypes = (int *) malloc(dims[0] * sizeof(int *));
    rdata.nbelttypes = dims[0];
    status = H5Dread(eltypes_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.elttypes);
    return rdata;
}

eltnodes_t readElementNodes(hid_t elnodes_id)
{
    hid_t space;
    int ndims, i;
    hsize_t dims[1];
    eltnodes_t rdata;
    herr_t status;

    space = H5Dget_space(elnodes_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    /*
     * Allocate array of pointers to rows.
     */
    rdata.eltnodes = (int *) malloc(dims[0] * sizeof(int *));
    rdata.nbeltnodes = dims[0];
    status = H5Dread(elnodes_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.eltnodes);
    return rdata;
}

semelts_t readSemElt(hid_t file_id, const char *table_name)
{
    herr_t status;
    hsize_t nfields_out;
    hsize_t nrecords_out;
    hid_t table_id;
    semelts_t rdata;

    size_t dst_size = sizeof(semelt_t);
    size_t dst_offset[5] =
    { HOFFSET( semelt_t, shortname ), HOFFSET( semelt_t, index ),
            HOFFSET( semelt_t, v1 ), HOFFSET( semelt_t, v2 ),
            HOFFSET( semelt_t, v3 ) };

    size_t dst_sizes[5] =
    { sizeof(rdata.somelt[0].shortname), sizeof(rdata.somelt[0].index),
            sizeof(rdata.somelt[0].v1), sizeof(rdata.somelt[0].v2),
            sizeof(rdata.somelt[0].v3) };

    table_id = H5TBget_table_info(file_id, table_name, &nfields_out,
            &nrecords_out);

    // Memory allocation for rdata before reading the table
    rdata.somelt = (semelt_t *) malloc((int) nrecords_out * sizeof(semelt_t));
    rdata.nbsomelt = nrecords_out;

    // Read the table
    status = H5TBread_table(file_id, table_name, dst_size, dst_offset,
            dst_sizes, rdata.somelt);
    return rdata;
}

semnodes_t readSemNodes(hid_t file_id, const char *table_name)
{
    herr_t status;
    hsize_t nfields_out;
    hsize_t nrecords_out;
    hid_t table_id;
    semnodes_t rdata;

    size_t dst_size = sizeof(semnode_t);
    size_t dst_offset[2] =
    { HOFFSET( semnode_t, shortname ), HOFFSET( semnode_t, index ) };

    size_t dst_sizes[2] =
    { sizeof(rdata.somnodes[0].shortname), sizeof(rdata.somnodes[0].index) };

    table_id = H5TBget_table_info(file_id, table_name, &nfields_out,
            &nrecords_out);

    // Memory allocation for rdata before reading the table
    rdata.somnodes = (semnode_t *) malloc((int) nrecords_out
            * sizeof(semnode_t));
    rdata.nbsomnodes = nrecords_out;

    // Read the table
    status = H5TBread_table(file_id, table_name, dst_size, dst_offset,
            dst_sizes, rdata.somnodes);
    return rdata;
}

ugroup_t readUGroup(hid_t group_id, const char* name)
{
    hid_t space;
    int ndims, i;
    hsize_t dims[1];
    ugroup_t rdata;
    herr_t status;

    rdata.name = (char *) malloc(30 * sizeof(char));
    strcpy(rdata.name, name);
    space = H5Dget_space(group_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    /*
     * Allocate array of pointers to rows.
     */
    rdata.eltgroup = (int *) malloc(dims[0] * sizeof(int *));
    rdata.nbeltgroup = dims[0];
    status = H5Dread(group_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.eltgroup);
    return rdata;
}
