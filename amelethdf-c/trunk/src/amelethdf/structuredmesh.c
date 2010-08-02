#include <stdlib.h>
#include <string.h>

#include "structuredmesh.h"

axis_t readAxis(hid_t loc_id, const char *axisname)
{
    hid_t space, axis_id;
    int ndims, i;

    hsize_t dims[2];
    axis_t rdata;
    herr_t status;

    /*
     * Get dataspace and allocate memory for read buffer.  HDF5Nodes is a
     * two dimensional dataset so the dynamic allocation must be done
     * in steps.
     */
    axis_id = H5Dopen1(loc_id, axisname);
    space = H5Dget_space(axis_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);

    /*
     * Allocate space for floating point data.
     */
    rdata.nodes = (float *) malloc(dims[0] * sizeof(float *));

    /*
     * Set the rest of the pointers to rows to the correct addresses.
     */
    rdata.nbnodes = dims[0];
    status = H5Dread(axis_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.nodes);
    return rdata;
}

sgroup_t readSGroup(hid_t group_id, const char* name)
{
    hid_t space;
    int ndims;
    unsigned int i;
    hsize_t dims[1];
    sgroup_t ijk;
    unsigned int **rdata;
    herr_t status;

    ijk.name = (char *) malloc(30 * sizeof(char));
    strcpy(ijk.name, name);

    space = H5Dget_space(group_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    /*
     * Allocate array of pointers to rows.
     */
    rdata = (unsigned int **) malloc(dims[0] * sizeof(unsigned int *));

    /*
     * Allocate space for floating point data.
     */
    // read attributes
    ijk.entityType = readAttributes(group_id, "entityType");
    ijk.type = readAttributes(group_id, "type");

    rdata[0] = (unsigned int *) malloc(dims[0] * dims[1] * sizeof(unsigned int));

    ijk.imin = (unsigned int *) malloc(dims[0] * sizeof(unsigned int *));
    ijk.imax = (unsigned int *) malloc(dims[0] * sizeof(unsigned int *));
    ijk.jmin = (unsigned int *) malloc(dims[0] * sizeof(unsigned int *));
    ijk.jmax = (unsigned int *) malloc(dims[0] * sizeof(unsigned int *));
    ijk.kmin = (unsigned int *) malloc(dims[0] * sizeof(unsigned int *));
    ijk.kmax = (unsigned int *) malloc(dims[0] * sizeof(unsigned int *));
    for (i = 1; i < dims[0]; i++)
        rdata[i] = rdata[0] + i * dims[1];
    ijk.nbelt = dims[0];
    status = H5Dread(group_id, H5T_NATIVE_INT32, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata[0]);
    if (strcmp(ijk.type, "node") == 0)
    {
        for (i = 0; i < dims[0]; i++)
        {
            ijk.imin[i] = rdata[i][0];
            ijk.jmin[i] = rdata[i][1];
            ijk.kmin[i] = rdata[i][2];
            ijk.imax[i] = rdata[i][0];
            ijk.jmax[i] = rdata[i][1];
            ijk.kmax[i] = rdata[i][2];
        }
    }
    else
    {
        for (i = 0; i < dims[0]; i++)
        {
            ijk.imin[i] = rdata[i][0];
            ijk.jmin[i] = rdata[i][1];
            ijk.kmin[i] = rdata[i][2];
            ijk.imax[i] = rdata[i][3];
            ijk.jmax[i] = rdata[i][4];
            ijk.kmax[i] = rdata[i][5];
        }
    }
    free(rdata);

    return ijk;
}

char* readAttributes(hid_t loc_id, const char* attr_name)
{
    herr_t status;
    hid_t attr_id, attr_type, space, memtype;
    size_t sdim;
    hsize_t dims[1] =
    { 1 };
    int ndims, i, ret_val;
    char **rdata;

    ret_val = 0;
    attr_id = H5Aopen(loc_id, attr_name, H5P_DEFAULT);

    if (attr_id >= 0)
    {
        attr_type = H5Aget_type(attr_id);
        sdim = H5Tget_size(attr_type);
        sdim++; /* Make room for null terminator */
        space = H5Aget_space(attr_id);
        ndims = H5Sget_simple_extent_dims(space, dims, NULL);
        rdata = (char **) malloc(dims[0] * sizeof(char *));
        rdata[0] = (char *) malloc(dims[0] * sdim * sizeof(char));
        /*
         * Set the rest of the pointers to rows to the correct addresses.
         */
        for (i = 1; i < dims[0]; i++)
            rdata[i] = rdata[0] + i * sdim;
        memtype = H5Tcopy(H5T_C_S1);
        status = H5Tset_size(memtype, sdim);
        status = H5Aread(attr_id, memtype, rdata[0]);
        H5Aclose(attr_id);
        return rdata[0];
    }
    else
        return "Unknown";
}

snorm_t readNormals(hid_t loc_id)
{
    herr_t status;
    hid_t filetype, space, memtype;
    size_t sdim;
    hsize_t dims[1] =  { 1 };
    int ndims, ret_val;
    unsigned int i;
    snorm_t rdata;

    filetype = H5Dget_type(loc_id);
    sdim = H5Tget_size(filetype);
    sdim++;
    space = H5Dget_space(loc_id);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);

    /*
     * Allocate array of pointers to rows.
     */
    rdata.normals = (char **) malloc(dims[0] * sizeof(char *));

    /*
     * Allocate space for floating point data.
     */
    rdata.normals[0] = (char *) malloc(dims[0] * sdim * sizeof(char));
    for (i = 1; i < dims[0]; i++)
        rdata.normals[i] = rdata.normals[0] + i * sdim;
    rdata.nbelt = dims[0];
    memtype = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(memtype, sdim);
    status = H5Dread(loc_id, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            rdata.normals[0]);
    // read attributes
    return rdata;
}

structured_mesh_t read_structured_mesh(hid_t file_id, const char* path)
{
    structured_mesh_t smesh;
    int i;
    children_t children;
    hid_t group_id, grpGrp_id;

    char *bufpath;
    char *bufpath2;

    bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    // X Axis
    strcpy(bufpath, path);
    strcat(bufpath, G_CARTESIAN_GRID);
    strcat(bufpath, "/x");
    smesh.x = readAxis(file_id, bufpath);
    free(bufpath);

    // Y Axis
    bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(bufpath, path);
    strcat(bufpath, G_CARTESIAN_GRID);
    strcat(bufpath, "/y");
    smesh.y = readAxis(file_id, bufpath);
    free(bufpath);

    // Z Axis
    bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(bufpath, path);
    strcat(bufpath, G_CARTESIAN_GRID);
    strcat(bufpath, "/z");
    smesh.z = readAxis(file_id, bufpath);
    free(bufpath);

    // groups
    bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(bufpath, path);
    strcat(bufpath, GROUP);
    children = read_children_name(file_id, bufpath);
    smesh.groups.nbgroup = children.nbchild;
    smesh.groups.groups = (sgroup_t *) malloc(children.nbchild
            * sizeof(sgroup_t));
    for (i = 0; i < children.nbchild; i++)
    {
        bufpath2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        strcpy(bufpath2, bufpath);
        strcat(bufpath2, "/");
        strcat(bufpath2, children.childnames[i]);
        group_id = H5Dopen1(file_id, bufpath2);
        smesh.groups.groups[i] = readSGroup(group_id, children.childnames[i]);
        H5Dclose(group_id);
        free(bufpath2);
    }
    free(children.childnames);
    free(bufpath);

    // groupGroups
    bufpath = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    strcpy(bufpath, path);
    strcat(bufpath, GROUPGROUP);

    smesh.groupgroups.nbgroupGroup = 0;
    if (H5Lexists(file_id, bufpath, H5P_DEFAULT) != FALSE)
    {
        children = read_children_name(file_id, bufpath);
        smesh.groupgroups.groupgroups = (groupgroup_t *) malloc(
                children.nbchild * sizeof(groupgroup_t));
        smesh.groupgroups.nbgroupGroup = children.nbchild;
        for (i = 0; i < children.nbchild; i++)
        {
            bufpath2
                    = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
            strcpy(bufpath2, bufpath);
            strcat(bufpath2, "/");
            strcat(bufpath2, children.childnames[i]);
            grpGrp_id = H5Dopen1(file_id, bufpath2);
            smesh.groupgroups.groupgroups[i] = readGroupGroup(grpGrp_id,
                    children.childnames[i]);
            H5Dclose(grpGrp_id);
            free(bufpath2);
        }
        free(children.childnames);
    }
    free(bufpath);

    return smesh;
}

void print_structured_mesh(structured_mesh_t smesh)
{
    int i;
    printf("Structured mesh\n");
    printf("Name : %s\n", smesh.name);
    printf("Groups : \n");
    printf("Number of groups : %i\n", smesh.groups.nbgroup);
    for (i = 0; i < smesh.groups.nbgroup; i++)
    {
        printf("Name : %s, type : %s\n", smesh.groups.groups[i].name,
                smesh.groups.groups[i].type);
    }
    printf("GroupGRoups :\n");
    printf("Number of groupgroups : %i\n", smesh.groupgroups.nbgroupGroup);
    for (i = 0; i < smesh.groupgroups.nbgroupGroup; i++)
    {
        printf("Name : %s\n", smesh.groupgroups.groupgroups[i].name);
    }
}
