#include "ah5_c_mesh.h"

// Init mesh group
void AH5_init_msh_group(AH5_msh_group_t *msh_group)
{
    msh_group->path = NULL;
    msh_group->nb_msh_instances = 0;
    msh_group->msh_instances = NULL;
    msh_group->nb_mlk_instances = 0;
    msh_group->mlk_instances = NULL;
}

// Init mesh category
void AH5_init_mesh(AH5_mesh_t *mesh)
{
    mesh->nb_groups = 0;
    mesh->groups = NULL;
}



// Read groupGroup (both the un/structured)
char AH5_read_groupgroup(hid_t file_id, const char* path, AH5_groupgroup_t *groupgroup)
{
    int nb_dims;
    H5T_class_t type_class;
    size_t length;
    char rdata = AH5_FALSE;

    groupgroup->nb_groupgroupnames = 1;  /* in case of single value */
    groupgroup->path = strdup(path);
    if (AH5_path_valid(file_id, path))
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path, &(groupgroup->nb_groupgroupnames), &type_class, &length) >= 0)
                    if (type_class == H5T_STRING)
                        if(AH5_read_str_dataset(file_id, path, groupgroup->nb_groupgroupnames, length, &(groupgroup->groupgroupnames)))
                            rdata = AH5_TRUE;
    if (!rdata)
    {
        AH5_print_err_dset(AH5_C_MESH, path);
        groupgroup->path = NULL;
        groupgroup->nb_groupgroupnames = 0;
        groupgroup->groupgroupnames = NULL;
    }
    return rdata;
}


// Read m x n dataset "/x, /y or /z" (32-bit signed float)
char AH5_read_smesh_axis(hid_t file_id, const char* path, AH5_axis_t *axis)
{
    H5T_class_t type_class;
    int nb_dims;
    size_t length;
    char rdata = AH5_FALSE;

    axis->nb_nodes = 1;
    if (AH5_path_valid(file_id, path))
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path, &(axis->nb_nodes), &type_class, &length) >= 0)
                    if (type_class == H5T_FLOAT && length == 4)
                        if (AH5_read_flt_dataset(file_id, path, axis->nb_nodes, &(axis->nodes)))
                            rdata = AH5_TRUE;
    if (!rdata)
    {
        /* print error only in case of missing "x" */
        if (strcmp(AH5_get_name_from_path(path),"x") == 0)
            AH5_print_err_dset(AH5_C_MESH, path);
        axis->nb_nodes = 0;
        axis->nodes = NULL;
    }
    return rdata;
}


// Read group in structured mesh (+normals)
char AH5_read_smsh_group(hid_t file_id, const char *path, AH5_sgroup_t *sgroup)
{
    char *temp, success1 = AH5_FALSE, success2 = AH5_TRUE, success3 = AH5_FALSE, rdata = AH5_TRUE;
    char normalpath[AH5_ABSOLUTE_PATH_LENGTH];
    hsize_t dims[2] = {1, 1};
    H5T_class_t type_class;
    size_t length;
    int nb_dims;

    sgroup->path = strdup(path);
    sgroup->type = NULL;
    sgroup->entitytype = NULL;
    sgroup->normals = NULL;
    sgroup->elements = NULL;

    if (AH5_path_valid(file_id, path))
    {
        sgroup->dims[0] = 1;
        sgroup->dims[1] = 1;
        if (!AH5_read_str_attr(file_id, path, AH5_A_TYPE, &(sgroup->type)))
        {
            AH5_print_err_attr(AH5_C_MESH, path, AH5_A_TYPE);
            success2 = AH5_FALSE;
        }
        if (!AH5_read_str_attr(file_id, path, AH5_A_ENTITY_TYPE, &(sgroup->entitytype)))
            if (strcmp(sgroup->type, AH5_V_NODE) != 0)
            {
                AH5_print_err_attr(AH5_C_MESH, path, AH5_A_ENTITY_TYPE);
                success2 = AH5_FALSE;
            }
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims == 2)
                if (H5LTget_dataset_info(file_id, path, sgroup->dims, &type_class, &length) >= 0)
                    if (sgroup->dims[1] >= 1 && sgroup->dims[1] <= 6 && type_class == H5T_INTEGER && length == 4)
                        if (AH5_read_int_dataset(file_id, path, sgroup->dims[0] * sgroup->dims[1], &(sgroup->elements)))
                            success1 = AH5_TRUE;
        if (!success1)
        {
            AH5_print_err_dset(AH5_C_MESH, path);
            sgroup->dims[0] = 0;  /* in case of invalid dataset only */
            sgroup->dims[1] = 0;
            sgroup->normals = NULL;
            sgroup->elements = NULL;
            rdata = AH5_FALSE;
        }
        else
        {
            if (success2)
                if (strcmp(sgroup->type, AH5_V_ELEMENT) == 0)
                    if (strcmp(sgroup->entitytype, AH5_V_FACE) == 0)
                    {
                        /* path = <mesh_path>/group/<group_name> */
                        strcpy(normalpath, path);
                        temp = strstr(path, "/group/");
                        normalpath[temp-path] = '\0';  /* get <mesh_path> */
                        temp = AH5_get_name_from_path(path);  /* temp = <group_name> */
                        strcat(normalpath, AH5_G_NORMAL);
                        strcat(normalpath, "/");
                        strcat(normalpath, temp);
                        /* normalpath = <mesh_path>/normal/<group_name> */

                        if (AH5_path_valid(file_id, normalpath))
                            if (H5LTget_dataset_ndims(file_id, normalpath, &nb_dims) >= 0)
                                if (nb_dims <= 1)
                                    if (H5LTget_dataset_info(file_id, normalpath, dims, &type_class, &length) >= 0)
                                        if (dims[0] == sgroup->dims[0] && type_class == H5T_STRING && length == 2)
                                            if(AH5_read_str_dataset(file_id, normalpath, dims[0], length, &(sgroup->normals)))
                                                success3 = AH5_TRUE;
                        if (!success3)
                        {
                            AH5_print_err_dset(AH5_C_MESH, normalpath);
                            rdata = AH5_FALSE;
                        }
                    }
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_MESH, path);
        rdata = AH5_FALSE;
    }
    return rdata;
}

// Read table of type "pointInElement" from /selectorOnMesh (structured) (element: 32-bit unsigned int, vector: 32-bit signed float)
char AH5_read_ssom_pie_table(hid_t file_id, const char *path, AH5_ssom_pie_table_t *ssom_pie_table)
{
    hsize_t nfields, nrecords, i;
    char rdata = AH5_FALSE;
    char **field_names;
    size_t *field_sizes;
    size_t *field_offsets;
    size_t type_size;
    int field_index1[] = {0, 1, 2, 3, 4, 5};
    int field_index2[3];

    ssom_pie_table->path = strdup(path);

    if (AH5_path_valid(file_id, path))
        if (H5TBget_table_info(file_id, path, &nfields, &nrecords) >= 0)
            if ((nfields == 3 || nfields == 6 || nfields == 9) && nrecords > 0)
            {
                field_names = (char **) malloc(nfields * sizeof(char *));
                field_names[0] = (char *) malloc(AH5_TABLE_FIELD_NAME_LENGTH * nfields * sizeof(char));
                for (i = 0; i < nfields; i++)
                    field_names[i] = field_names[0] + i * AH5_TABLE_FIELD_NAME_LENGTH;
                field_sizes = (size_t *) malloc(sizeof(size_t *) * nfields);
                field_offsets = (size_t *) malloc(sizeof(size_t *) * nfields);

                if (H5TBget_field_info(file_id, path, field_names, field_sizes, field_offsets, &type_size) >= 0)
                {
                    if (nfields == 3)
                    {
                        /* imin imax v1 (1D) */
                        if (strcmp(field_names[0], AH5_F_IMIN) == 0
                                && strcmp(field_names[1], AH5_F_IMAX) == 0
                                && strcmp(field_names[2], AH5_F_V1) == 0)
                        {
                            rdata = AH5_TRUE;
                            field_index2[0] = 2;
                        }
                    }
                    else if (nfields == 6)
                    {
                        /* imin jmin imax jmax v1 v2 (2D) */
                        if (strcmp(field_names[0], AH5_F_IMIN) == 0
                                && strcmp(field_names[1], AH5_F_JMIN) == 0
                                && strcmp(field_names[2], AH5_F_IMAX) == 0
                                && strcmp(field_names[3], AH5_F_JMAX) == 0
                                && strcmp(field_names[4], AH5_F_V1) == 0
                                && strcmp(field_names[5], AH5_F_V2) == 0)
                        {
                            rdata = AH5_TRUE;
                            field_index2[0] = 4;
                            field_index2[1] = 5;
                        }
                    }
                    else if (nfields == 9)
                    {
                        /* imin jmin kmin imax jmax kmax v1 v2 v3 (3D) */
                        if (strcmp(field_names[0], AH5_F_IMIN) == 0
                                && strcmp(field_names[1], AH5_F_JMIN) == 0
                                && strcmp(field_names[2], AH5_F_KMIN) == 0
                                && strcmp(field_names[3], AH5_F_IMAX) == 0
                                && strcmp(field_names[4], AH5_F_JMAX) == 0
                                && strcmp(field_names[5], AH5_F_KMAX) == 0
                                && strcmp(field_names[6], AH5_F_V1) == 0
                                && strcmp(field_names[7], AH5_F_V2) == 0
                                && strcmp(field_names[8], AH5_F_V3) == 0)
                        {
                            rdata = AH5_TRUE;
                            field_index2[0] = 6;
                            field_index2[1] = 7;
                            field_index2[2] = 8;
                        }
                    }
                }
                if (rdata)
                {
                    ssom_pie_table->nb_dims = nfields / 3;
                    ssom_pie_table->elements = (unsigned int **) malloc(nrecords * sizeof(unsigned int *));
                    ssom_pie_table->elements[0] = (unsigned int *) malloc(nrecords * 2 * ssom_pie_table->nb_dims * sizeof(unsigned int));
                    ssom_pie_table->vectors = (float **) malloc(nrecords * sizeof(float *));
                    ssom_pie_table->vectors[0] = (float *) malloc(nrecords * ssom_pie_table->nb_dims * sizeof(float));
                    for (i = 1; i < nrecords; i++)
                    {
                        ssom_pie_table->elements[i] = ssom_pie_table->elements[0] + i * 2 * ssom_pie_table->nb_dims;
                        ssom_pie_table->vectors[i] = ssom_pie_table->vectors[0] + i * ssom_pie_table->nb_dims;
                    }

                    if (H5TBread_fields_index(file_id, path, ssom_pie_table->nb_dims*2, field_index1, 0, nrecords, ssom_pie_table->nb_dims*2*sizeof(int), field_offsets, field_sizes, ssom_pie_table->elements[0]) < 0
                            ||
                            H5TBread_fields_index(file_id, path, ssom_pie_table->nb_dims, field_index2, 0, nrecords, ssom_pie_table->nb_dims*sizeof(float), field_offsets, field_sizes, ssom_pie_table->vectors[0]) < 0)
                    {
                        free(ssom_pie_table->elements[0]);
                        free(ssom_pie_table->elements);
                        free(ssom_pie_table->vectors[0]);
                        free(ssom_pie_table->vectors);
                        rdata = AH5_FALSE;
                    }
                    else
                        ssom_pie_table->nb_points = nrecords;
                }
                free(field_names[0]);
                free(field_names);
                free(field_sizes);
                free(field_offsets);
            }
    if (!rdata)
    {
        AH5_print_err_tble(AH5_C_MESH, path);
        ssom_pie_table->nb_dims = 0;
        ssom_pie_table->nb_points = 0;
        ssom_pie_table->elements = NULL;
        ssom_pie_table->vectors = NULL;
    }
    return rdata;
}


// Read structured mesh
char AH5_read_smesh(hid_t file_id, const char* path, AH5_smesh_t *smesh)
{
    char path2[AH5_ABSOLUTE_PATH_LENGTH], path3[AH5_ABSOLUTE_PATH_LENGTH];
    AH5_children_t children;
    char *type, success, rdata = AH5_TRUE;
    hsize_t i;

    smesh->groups = NULL;
    smesh->groupgroups = NULL;
    smesh->som_tables = NULL;

    if (AH5_path_valid(file_id, path))
    {
        // X Axis
        strcpy(path2, path);
        strcat(path2, AH5_G_CARTESIAN_GRID);
        strcat(path2, AH5_G_X);
        if (!AH5_read_smesh_axis(file_id, path2, &(smesh->x)))
            rdata = AH5_FALSE;

        // Y Axis
        strcpy(path2, path);
        strcat(path2, AH5_G_CARTESIAN_GRID);
        strcat(path2, AH5_G_Y);
        AH5_read_smesh_axis(file_id, path2, &(smesh->y));
        /* problem can be one-dimensional */

        // Z Axis
        strcpy(path2, path);
        strcat(path2, AH5_G_CARTESIAN_GRID);
        strcat(path2, AH5_G_Z);
        AH5_read_smesh_axis(file_id, path2, &(smesh->z));
        /* problem can be two-dimensional */

        // groups
        strcpy(path2, path);
        strcat(path2, AH5_G_GROUP);
        children = AH5_read_children_name(file_id, path2);
        smesh->nb_groups = children.nb_children;
        if (children.nb_children > 0)
        {
            smesh->groups = (AH5_sgroup_t *) malloc(children.nb_children * sizeof(AH5_sgroup_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path3, path2);
                strcat(path3, children.childnames[i]);
                if (!AH5_read_smsh_group(file_id, path3, smesh->groups + i))
                    rdata = AH5_FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }

        // read groupGroup if exists
        strcpy(path2, path);
        strcat(path2, AH5_G_GROUPGROUP);
        children = AH5_read_children_name(file_id, path2);
        smesh->nb_groupgroups = children.nb_children;
        if (children.nb_children > 0)
        {
            smesh->groupgroups = (AH5_groupgroup_t *) malloc(children.nb_children * sizeof(AH5_groupgroup_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path3, path2);
                strcat(path3, children.childnames[i]);
                if (!AH5_read_groupgroup(file_id, path3, smesh->groupgroups + i))
                    rdata = AH5_FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }

        // read selectorOnMesh
        strcpy(path2, path);
        strcat(path2, AH5_G_SELECTOR_ON_MESH);
        children = AH5_read_children_name(file_id, path2);
        smesh->nb_som_tables = children.nb_children;
        if (children.nb_children > 0)
        {
            smesh->som_tables = (AH5_ssom_pie_table_t *) malloc(children.nb_children * sizeof(AH5_ssom_pie_table_t));
            for (i = 0; i < children.nb_children; i++)
            {
                success = AH5_FALSE;
                strcpy(path3, path2);
                strcat(path3, children.childnames[i]);
                if (AH5_read_str_attr(file_id, path3, AH5_A_TYPE, &type))
                {
                    if (strcmp(type,AH5_V_POINT_IN_ELEMENT) == 0)
                        if (AH5_read_ssom_pie_table(file_id, path3, smesh->som_tables + i))
                            success = AH5_TRUE;
                    free(type);
                }
                if (!success)
                {
                    AH5_print_err_attr(AH5_C_MESH, AH5_A_TYPE, path3);
                    smesh->som_tables[i].path = NULL;
                    smesh->som_tables[i].nb_dims = 0;
                    smesh->som_tables[i].nb_points = 0;
                    smesh->som_tables[i].elements = NULL;
                    smesh->som_tables[i].vectors = NULL;
                    rdata = AH5_FALSE;
                }
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_MESH, path);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read group in unstructured mesh
char AH5_read_umsh_group(hid_t file_id, const char* path, AH5_ugroup_t *ugroup)
{
    H5T_class_t type_class;
    char rdata = AH5_FALSE;
    size_t length;
    int nb_dims;

    ugroup->nb_groupelts = 1; /* see H5LTget_dataset_info() below */
    ugroup->path = strdup(path);
    ugroup->type = NULL;
    ugroup->entitytype = NULL;
    ugroup->groupelts = NULL;

    if (AH5_path_valid(file_id, path))
    {
        if(!AH5_read_str_attr(file_id, path, AH5_A_TYPE, &(ugroup->type)))
            AH5_print_err_attr (AH5_C_MESH, path, AH5_A_TYPE);
        else
        {
            if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
                if (nb_dims <= 1)
                    if (H5LTget_dataset_info(file_id, path, &(ugroup->nb_groupelts), &type_class, &length) >= 0)
                        if (type_class == H5T_INTEGER && length == 4)
                            if (AH5_read_int_dataset(file_id, path, ugroup->nb_groupelts, &(ugroup->groupelts)))
                                rdata = AH5_TRUE;
            //XXX Why not point to the constant value (AH5_V_ELEMENT, ...)?
            //      Does not forgot to update free function.
            if(!AH5_read_str_attr(file_id, path, AH5_A_ENTITY_TYPE, &(ugroup->entitytype)))
                if (strcmp(ugroup->type, AH5_V_NODE) != 0)
                {
                    AH5_print_err_attr(AH5_C_MESH, path, AH5_A_ENTITY_TYPE);
                    rdata = AH5_FALSE;
                }
        }
    }
    if (!rdata)
    {
        AH5_print_err_dset(AH5_C_MESH, path);
        ugroup->nb_groupelts = 0;
    }
    return rdata;
}


// Read table of type "pointInElement" from /selectorOnMesh (unstructured) (index: 32-bit signed int, vector: 32-bit signed float)
char AH5_read_usom_pie_table(hid_t file_id, const char *path, AH5_usom_pie_table_t *usom_pie_table)
{
    hsize_t nfields, nrecords, i;
    char rdata = AH5_FALSE;
    char **field_names;
    size_t *field_sizes;
    size_t *field_offsets;
    size_t type_size;
    int field_index1[] = {0};
    int field_index2[] = {1, 2, 3};

    if (AH5_path_valid(file_id, path))
        if (H5TBget_table_info(file_id, path, &nfields, &nrecords) >= 0)
            if (nfields >= 2 && nfields <=4 && nrecords > 0)
            {
                field_names = (char **) malloc(nfields * sizeof(char *));
                field_names[0] = (char *) malloc(AH5_TABLE_FIELD_NAME_LENGTH * nfields * sizeof(char));
                for (i = 0; i < nfields; i++)
                    field_names[i] = field_names[0] + i * AH5_TABLE_FIELD_NAME_LENGTH;
                field_sizes = (size_t *) malloc(sizeof(size_t *) * nfields);
                field_offsets = (size_t *) malloc(sizeof(size_t *) * nfields);

                if (H5TBget_field_info(file_id, path, field_names, field_sizes, field_offsets, &type_size) >= 0)
                    if (strcmp(field_names[0], "index") == 0)
                    {
                        usom_pie_table->indices = (int *) malloc(nrecords * sizeof(int));
                        usom_pie_table->vectors = (float **) malloc(nrecords * sizeof(float *));
                        usom_pie_table->vectors[0] = (float *) malloc(nrecords * (nfields - 1) * sizeof(float));
                        for (i = 1; i < nrecords; i++)
                            usom_pie_table->vectors[i] = usom_pie_table->vectors[0] + i * (nfields - 1);
                        if (H5TBread_fields_index(file_id, path, 1, field_index1, 0, nrecords, sizeof(int), field_offsets, field_sizes, usom_pie_table->indices) < 0
                                ||
                                H5TBread_fields_index(file_id, path, (nfields - 1), field_index2, 0, nrecords, (nfields - 1) * sizeof(float), field_offsets, field_sizes, usom_pie_table->vectors[0]) < 0)
                        {
                            free(usom_pie_table->vectors[0]);
                            free(usom_pie_table->vectors);
                            free(usom_pie_table->indices);
                        }
                        else
                        {
                            usom_pie_table->nb_points = nrecords;
                            usom_pie_table->nb_dims = (char) nfields - 1;
                            rdata = AH5_TRUE;
                        }
                    }
                free(field_names[0]);
                free(field_names);
                free(field_sizes);
                free(field_offsets);
            }
    if (!rdata)
    {
        AH5_print_err_tble(AH5_C_MESH, path);
        usom_pie_table->nb_points = 0;
        usom_pie_table->nb_dims = 0;
        usom_pie_table->indices = NULL;
        usom_pie_table->vectors = NULL;
    }
    return rdata;
}


// Read dataset of type "edge" or "face" from /selectorOnMesh (32-bit signed int)
char AH5_read_usom_ef_table(hid_t file_id, const char *path, AH5_usom_ef_table_t *usom_ef_table)
{
    int nb_dims;
    H5T_class_t type_class;
    size_t length;
    char rdata = AH5_FALSE;

    usom_ef_table->dims[0] = 1;
    usom_ef_table->dims[1] = 1;

    if (AH5_path_valid(file_id, path))
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims == 2)
                if (H5LTget_dataset_info(file_id, path, usom_ef_table->dims, &type_class, &length) >= 0)
                    if (type_class == H5T_INTEGER && length == 4)
                        if (AH5_read_int_dataset(file_id, path, usom_ef_table->dims[0] * usom_ef_table->dims[1], &(usom_ef_table->items)))
                            rdata = AH5_TRUE;
    if (!rdata)
    {
        AH5_print_err_dset(AH5_C_MESH, path);
        usom_ef_table->dims[0] = 0;
        usom_ef_table->dims[1] = 0;
        usom_ef_table->items = NULL;
    }
    return rdata;
}


// Read selector on mesh (unstructured mesh)
char AH5_read_umesh_som_table(hid_t file_id, const char *path, AH5_usom_table_t *usom_table)
{
    char *type, rdata = AH5_TRUE;

    usom_table->path = strdup(path);
    usom_table->type = SOM_INVALID;

    if (AH5_path_valid(file_id, path))
    {
        if (AH5_read_str_attr(file_id, path, AH5_A_TYPE, &type))
        {
            if (strcmp(type, AH5_V_POINT_IN_ELEMENT) == 0)
            {
                usom_table->type = SOM_POINT_IN_ELEMENT;
                if (!AH5_read_usom_pie_table(file_id, path, &(usom_table->data.pie)))
                    rdata = AH5_FALSE;
            }
            else if (strcmp(type, AH5_V_EDGE) == 0)
            {
                usom_table->type = SOM_EDGE;
                if (!AH5_read_usom_ef_table(file_id, path, &(usom_table->data.ef)))
                    rdata = AH5_FALSE;
            }
            else if (strcmp(type, AH5_V_FACE) == 0)
            {
                usom_table->type = SOM_FACE;
                if (!AH5_read_usom_ef_table(file_id, path, &(usom_table->data.ef)))
                    rdata = AH5_FALSE;
            }
            free(type);
        }
        else
        {
            AH5_print_err_attr(AH5_C_MESH, path, AH5_A_TYPE);
            rdata = AH5_FALSE;
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_MESH, path);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read unstructured mesh
char AH5_read_umesh(hid_t file_id, const char* path, AH5_umesh_t *umesh)
{
    char path2[AH5_ABSOLUTE_PATH_LENGTH], path3[AH5_ABSOLUTE_PATH_LENGTH];
    char rdata = AH5_TRUE, success = AH5_FALSE;
    hsize_t i;
    int nb_dims;
    H5T_class_t type_class;
    size_t length;
    hid_t dset_id;
    AH5_children_t children;

    umesh->elementnodes = NULL;
    umesh->elementtypes = NULL;
    umesh->nodes = NULL;
    umesh->groups = NULL;
    umesh->groupgroups = NULL;
    umesh->som_tables = NULL;

    if (AH5_path_valid(file_id, path))
    {
        // Read m x 1 dataset "elementNodes" (32-bit signed integer)
        umesh->nb_elementnodes = 1;
        strcpy(path2, path);
        strcat(path2, AH5_G_ELEMENT_NODES);
        if (AH5_path_valid(file_id, path2))
            if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
                if (nb_dims <= 1)
                    if (H5LTget_dataset_info(file_id, path2, &(umesh->nb_elementnodes), &type_class, &length) >= 0)
                        if (type_class == H5T_INTEGER && length == 4)
                            if (AH5_read_int_dataset(file_id, path2, umesh->nb_elementnodes, &(umesh->elementnodes)))
                                success = AH5_TRUE;
        if (!success)
        {
            AH5_print_err_dset(AH5_C_MESH, path2);
            umesh->nb_elementnodes = 0;
            rdata = AH5_FALSE;
        }

        success = AH5_FALSE;

        // Read m x 1 dataset "elementTypes" (8-bit signed char)
        umesh->nb_elementtypes = 1;
        strcpy(path2, path);
        strcat(path2, AH5_G_ELEMENT_TYPES);
        if (AH5_path_valid(file_id, path2))
            if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
                if (nb_dims <= 1)
                    if (H5LTget_dataset_info(file_id, path2, &(umesh->nb_elementtypes), &type_class, &length) >= 0)
                        if (type_class == H5T_INTEGER && length == 1)
                        {
                            umesh->elementtypes = (char *) malloc((umesh->nb_elementtypes * sizeof(char)));
                            dset_id = H5Dopen(file_id, path2, H5P_DEFAULT);
                            if (H5Dread(dset_id, H5T_NATIVE_CHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, umesh->elementtypes) >= 0)
                                success = AH5_TRUE;
                            H5Dclose(dset_id);
                            if (!success)
                            {
                                free(umesh->elementtypes);
                                umesh->elementtypes = NULL;
                            }
                        }
        if (!success)
        {
            AH5_print_err_dset(AH5_C_MESH, path2);
            umesh->nb_elementtypes = 0;
            rdata = AH5_FALSE;
        }

        success = AH5_FALSE;
        umesh->nb_nodes[0] = 1;
        umesh->nb_nodes[1] = 1;
        // Read m x n dataset "nodes" (32-bit signed float)
        strcpy(path2, path);
        strcat(path2, AH5_G_NODES);
        if (AH5_path_valid(file_id, path2))
            if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
                if (nb_dims == 2)
                    if (H5LTget_dataset_info(file_id, path2, umesh->nb_nodes, &type_class, &length) >= 0)
                        if (umesh->nb_nodes[1] <= 3 && type_class == H5T_FLOAT && length == 4)
                            if (AH5_read_flt_dataset(file_id, path2, umesh->nb_nodes[0] * umesh->nb_nodes[1], &(umesh->nodes)))
                                success = AH5_TRUE;
        if (!success)
        {
            AH5_print_err_dset(AH5_C_MESH, path2);
            umesh->nb_nodes[0] = 0;
            umesh->nb_nodes[1] = 0;
            rdata = AH5_FALSE;
        }

        // read groupGroup if exists
        strcpy(path2, path);
        strcat(path2, AH5_G_GROUPGROUP);
        children = AH5_read_children_name(file_id, path2);
        umesh->nb_groupgroups = children.nb_children;
        if (children.nb_children > 0)
        {
            umesh->groupgroups = (AH5_groupgroup_t *) malloc(children.nb_children * sizeof(AH5_groupgroup_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path3, path2);
                strcat(path3, children.childnames[i]);
                if (!AH5_read_groupgroup(file_id, path3, umesh->groupgroups + i))
                    rdata = AH5_FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }

        // read group
        strcpy(path2, path);
        strcat(path2, AH5_G_GROUP);
        children = AH5_read_children_name(file_id, path2);
        umesh->nb_groups = children.nb_children;
        if (children.nb_children > 0)
        {
            umesh->groups = (AH5_ugroup_t *) malloc(children.nb_children * sizeof(AH5_ugroup_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path3, path2);
                strcat(path3, children.childnames[i]);
                if (!AH5_read_umsh_group(file_id, path3, umesh->groups + i))
                    rdata = AH5_FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }

        // read selectorOnMesh
        strcpy(path2, path);
        strcat(path2, AH5_G_SELECTOR_ON_MESH);
        children = AH5_read_children_name(file_id, path2);
        umesh->nb_som_tables = children.nb_children;
        if (children.nb_children > 0)
        {
            umesh->som_tables = (AH5_usom_table_t *) malloc(children.nb_children * sizeof(AH5_usom_table_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path3, path2);
                strcat(path3, children.childnames[i]);
                if (!AH5_read_umesh_som_table(file_id, path3, umesh->som_tables + i))
                    rdata = AH5_FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_MESH, path);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read mesh instance
char AH5_read_msh_instance(hid_t file_id, const char *path, AH5_msh_instance_t *msh_instance)
{
    char *type, rdata = AH5_TRUE;

    msh_instance->path = strdup(path);
    msh_instance->type = MSH_INVALID;

    if (AH5_path_valid(file_id, path))
    {
        if (AH5_read_str_attr(file_id, path, AH5_A_TYPE, &type))
        {
            if (strcmp(type, AH5_V_STRUCTURED) == 0)
            {
                msh_instance->type = MSH_STRUCTURED;
                if (!AH5_read_smesh(file_id, path, &(msh_instance->data.structured)))
                    rdata = AH5_FALSE;
            }
            else if (strcmp(type, AH5_V_UNSTRUCTURED) == 0)
            {
                msh_instance->type = MSH_UNSTRUCTURED;
                if (!AH5_read_umesh(file_id, path, &(msh_instance->data.unstructured)))
                    rdata = AH5_FALSE;
            }
            else
            {
                printf("***** ERROR(%s): Unexpected attribute value of \"%s@%s\". *****\n\n", AH5_C_MESH, path, AH5_A_TYPE);
                rdata = AH5_FALSE;
            }
            free(type);
        }
        else
        {
            msh_instance->type = MSH_INVALID;
            AH5_print_err_attr(AH5_C_MESH, path, AH5_A_TYPE);
            rdata = AH5_FALSE;
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_MESH, path);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read meshLink instance
char AH5_read_mlk_instance(hid_t file_id, const char *path, AH5_mlk_instance_t *mlk_instance)
{
    char *type, dataset_read = AH5_FALSE, rdata = AH5_TRUE;
    H5T_class_t type_class;
    size_t length;
    int nb_dims;

    mlk_instance->path = strdup(path);
    mlk_instance->mesh1 = NULL;
    mlk_instance->mesh2 = NULL;
    mlk_instance->data = NULL;
    mlk_instance->type = MSHLNK_INVALID;

    if (AH5_path_valid(file_id, path))
    {
        if (!AH5_read_str_attr(file_id, path, AH5_A_MESH1, &(mlk_instance->mesh1)))
        {
            AH5_print_err_attr(AH5_C_MESH, path, AH5_A_MESH1);
            rdata = AH5_FALSE;
        }
        if (!AH5_read_str_attr(file_id, path, AH5_A_MESH2, &(mlk_instance->mesh2)))
        {
            AH5_print_err_attr(AH5_C_MESH, path, AH5_A_MESH2);
            rdata = AH5_FALSE;
        }

        if (!AH5_read_str_attr(file_id, path, AH5_A_TYPE, &type))
        {
            AH5_print_err_attr(AH5_C_MESH, path, AH5_A_TYPE);
            rdata = AH5_FALSE;
        }

        if (AH5_path_valid(file_id, path))
            if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
                if (nb_dims == 2)
                    if (H5LTget_dataset_info(file_id, path, mlk_instance->dims, &type_class, &length) >= 0)
                        if (type_class == H5T_INTEGER)
                            if (AH5_read_int_dataset(file_id, path, mlk_instance->dims[0] * mlk_instance->dims[1], &(mlk_instance->data)))
                                dataset_read = AH5_TRUE;
        if (!dataset_read)
        {
            AH5_print_err_dset(AH5_C_MESH, path);
            mlk_instance->dims[0] = 0;
            mlk_instance->dims[1] = 0;
            rdata = AH5_FALSE;
        }

        if (rdata)
        {
            if (strcmp(type, AH5_V_NODE))
                mlk_instance->type = MSHLNK_NODE;
            else if (strcmp(type, AH5_V_EDGE))
                mlk_instance->type = MSHLNK_EDGE;
            else if (strcmp(type, AH5_V_FACE))
                mlk_instance->type = MSHLNK_FACE;
            else if (strcmp(type, AH5_V_VOLUME))
                mlk_instance->type = MSHLNK_VOLUME;
        }
        else
            mlk_instance->type = MSHLNK_INVALID;
        if (type != NULL)
        {
            free(type);
            type = NULL;
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_MESH, path);
        rdata = AH5_FALSE;
    }
    return rdata;
}



// Read mesh group
char AH5_read_msh_group(hid_t file_id, const char *path, AH5_msh_group_t *msh_group)
{
    char path2[AH5_ABSOLUTE_PATH_LENGTH], rdata = AH5_TRUE;
    char path3[AH5_ABSOLUTE_PATH_LENGTH];
    AH5_children_t children;
    hsize_t i, j = 0;

    msh_group->path = strdup(path);
    msh_group->msh_instances = NULL;
    msh_group->mlk_instances = NULL;

    if (AH5_path_valid(file_id, path))
    {
        children = AH5_read_children_name(file_id, path);
        msh_group->nb_msh_instances = children.nb_children;
        for (i = 0; i < children.nb_children; i++)
            if (strcmp(children.childnames[i], AH5_G_MESH_LINK) == 0)
                msh_group->nb_msh_instances--;    // do not count /meshLink
        if (children.nb_children > 0)
        {
            msh_group->msh_instances = (AH5_msh_instance_t *) malloc(msh_group->nb_msh_instances * sizeof(AH5_msh_instance_t));
            for (i = 0; i < children.nb_children; i++)
            {
                if (strcmp(children.childnames[i], AH5_G_MESH_LINK) != 0)
                {
                    strcpy(path2, path);
                    strcat(path2, children.childnames[i]);
                    if (!AH5_read_msh_instance(file_id, path2, msh_group->msh_instances + j++))
                        rdata = AH5_FALSE;
                }
                free(children.childnames[i]);
            }
            free(children.childnames);
        }

        strcpy(path2, path);
        strcat(path2, AH5_G_MESH_LINK);
        children = AH5_read_children_name(file_id, path2);
        msh_group->nb_mlk_instances = children.nb_children;
        if (children.nb_children > 0)
        {
            msh_group->mlk_instances = (AH5_mlk_instance_t *) malloc(children.nb_children * sizeof(AH5_mlk_instance_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path3, path2);
                strcat(path3, children.childnames[i]);
                if (!AH5_read_mlk_instance(file_id, path3, msh_group->mlk_instances + i))
                    rdata = AH5_FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_MESH, path);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read mesh category
char AH5_read_mesh(hid_t file_id, AH5_mesh_t *mesh)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH], rdata = AH5_TRUE;
    AH5_children_t children;
    hsize_t i;

    mesh->groups = NULL;

    if (AH5_path_valid(file_id, AH5_C_MESH))
    {
        children = AH5_read_children_name(file_id, AH5_C_MESH);
        mesh->nb_groups = children.nb_children;
        if (children.nb_children > 0)
        {
            mesh->groups = (AH5_msh_group_t *) malloc(children.nb_children * sizeof(AH5_msh_group_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path, AH5_C_MESH);
                strcat(path, children.childnames[i]);
                if (!AH5_read_msh_group(file_id, path, mesh->groups + i))
                    rdata = AH5_FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_MESH, AH5_C_MESH);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Write structured mesh
char AH5_write_smesh(hid_t file_id, const AH5_smesh_t *smesh)
{
    char success = AH5_FALSE;

    AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED(AH5_C_MESH, "UNKNOWN PATH");

    return success;
}

// Write groupGroup
char AH5_write_groupgroup(hid_t loc_id, const AH5_groupgroup_t *groupgroup, int nb_ggrp)
{
    char success = AH5_FALSE;
    hid_t grp;
    int i;
    char *ggrp_name;

    // NMT: I prefer build an empty group, because I am not sure that everyone check that the group exist before to open it.
    if (AH5_path_valid(loc_id, AH5_CATEGORY_NAME(AH5_G_GROUPGROUP)))
        grp = H5Gopen(loc_id, AH5_CATEGORY_NAME(AH5_G_GROUPGROUP), H5P_DEFAULT);
    else
        grp = H5Gcreate(loc_id, AH5_CATEGORY_NAME(AH5_G_GROUPGROUP), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    if (nb_ggrp >= 0)
    {
        success = AH5_TRUE;
        for (i = 0; i < nb_ggrp; i++)
        {
            if (groupgroup[i].nb_groupgroupnames > 0)
            {
                if (groupgroup[i].path != NULL)
                {
                    ggrp_name = AH5_get_name_from_path(groupgroup[i].path);
                    success &= AH5_write_str_dataset(grp, ggrp_name, groupgroup[i].nb_groupgroupnames, strlen(groupgroup[i].groupgroupnames[0]) + 1, groupgroup[i].groupgroupnames[0]);
                }
                else
                {
                    success &= AH5_FALSE;
                }
            }
        }
    }
    return success;
}

// Write group in unstructured mesh
char AH5_write_umsh_group(hid_t loc_id, const AH5_ugroup_t *ugroup, int nb_ugroup)
{
    char success = AH5_FALSE;
    int i;
    hid_t grp;
    char *basename;

    // NMT: I prefer build an empty group, because I am not sure that everyone check that the group exist before to open it.
    if (AH5_path_valid(loc_id, AH5_CATEGORY_NAME(AH5_G_GROUP)))
        grp = H5Gopen(loc_id, AH5_CATEGORY_NAME(AH5_G_GROUP), H5P_DEFAULT);
    else
        grp = H5Gcreate(loc_id, AH5_CATEGORY_NAME(AH5_G_GROUP), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    for (i = 0; i < nb_ugroup; i++)
    {
        if (ugroup[i].nb_groupelts > 0)
        {
            basename = AH5_get_name_from_path(ugroup[i].path);
            if (AH5_write_int_dataset(grp, basename, ugroup[i].nb_groupelts, ugroup[i].groupelts))
            {
                if (AH5_write_str_attr(grp, basename, AH5_A_TYPE, ugroup[i].type))
                    success = AH5_TRUE;
                    if (strcmp(ugroup[i].type, AH5_V_NODE))
                      success = AH5_write_str_attr(grp, basename, AH5_A_ENTITY_TYPE, ugroup[i].entitytype);
            }
            if (!success)
              return success;
        }
    }
    return success;
}

// Write table of type "pointInElement" from /selectorOnMesh (unstructured) (index: 32-bit signed int, vector: 32-bit signed float)
char AH5_write_usom_pie_table(hid_t file_id, const AH5_usom_pie_table_t *usom_pie_table)
{
    char success = AH5_FALSE;

    AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED(AH5_C_MESH, "UNKNOWN PATH");

    return success;
}

// Read dataset of type "edge" or "face" from /selectorOnMesh (32-bit signed int)
char AH5_write_usom_ef_table(hid_t file_id, const AH5_usom_ef_table_t *usom_ef_table)
{
    char success = AH5_FALSE;

    AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED(AH5_C_MESH, "UNKNOWN PATH");

    return success;
}

// Read selector on mesh (unstructured mesh)
char AH5_write_umesh_som_table(hid_t file_id, const AH5_usom_table_t *usom_table, int nb_som)
{
    char success = AH5_FALSE;

    AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED(AH5_C_MESH, "UNKNOWN PATH");

    return success;
}

/** Write unstructured mesh */
char AH5_write_umesh(hid_t msh_id, const AH5_umesh_t *umesh)
{
    char success = AH5_FALSE;

    hsize_t dims1[1], dims2[2];

    // Check umesh sanity first
    if (umesh == NULL
        || umesh->nodes == NULL)
        return success;

    if (!AH5_write_str_attr(msh_id, ".", AH5_A_TYPE, AH5_V_UNSTRUCTURED))
        return success;

    // Write m x 1 dataset "elementNodes" (32-bit signed integer)
    if (!AH5_write_int_dataset(msh_id, AH5_CATEGORY_NAME(AH5_G_ELEMENT_NODES), umesh->nb_elementnodes, umesh->elementnodes))
        return success;

    // Write m x 1 dataset "elementTypes" (8-bit signed char)
    if (!AH5_write_char_dataset(msh_id, AH5_CATEGORY_NAME(AH5_G_ELEMENT_TYPES), umesh->nb_elementtypes, umesh->elementtypes))
        return success;

    // Write m x n dataset "nodes" (32-bit signed float)
    if (!AH5_write_flt_array(msh_id, AH5_CATEGORY_NAME(AH5_G_NODES), 2, umesh->nb_nodes, umesh->nodes))
        return success;

    // Write groups
    if (!AH5_write_umsh_group(msh_id, umesh->groups, umesh->nb_groups))
        // handled error
        return success;

    // Write groupGroups
    if (!AH5_write_groupgroup(msh_id, umesh->groupgroups, umesh->nb_groupgroups))
        // handled error
        return success;

    // Write selectorOnMesh
    // FIXME(NMT) not implemented yet...
    //if (!AH5_write_umesh_som_table(msh, AH5_CATEGORY_NAME(AH5_G_SELECTOR_ON_MESH), umesh->som_tables, umesh->nb_som_tables))
    //    return AH5_FAILURE;

    success = AH5_TRUE;
    return success;
}

// Write mesh instance
char AH5_write_msh_instance(hid_t loc_id, const AH5_msh_instance_t *msh_instance)
{
    char success = AH5_FALSE;
    char msh_path[AH5_ABSOLUTE_PATH_LENGTH];
    char *basename;
    hid_t msh_id;

    basename = AH5_get_name_from_path(msh_instance->path);
    msh_id = H5Gcreate(loc_id, basename, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    if (msh_instance->type == MSH_UNSTRUCTURED)
    {
        success = AH5_write_umesh(msh_id, &msh_instance->data.unstructured);
    }
    else if (msh_instance->type == MSH_STRUCTURED)
    {
        success = AH5_write_smesh(msh_id, &msh_instance->data.structured);
    }
    else
    {
        AH5_print_err_inv_attr(AH5_C_MESH, msh_instance->path, AH5_A_TYPE);
        success = AH5_FALSE;
    }

    return success;
}

// Write link between mesh
char AH5_write_mlk_instance(hid_t loc_id, const AH5_mlk_instance_t *mlk_instance)
{
    char success = AH5_FALSE;

    AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED(AH5_C_MESH, "UNKNOWN PATH");

    return success;
}

// Write mesh group
char AH5_write_msh_group(hid_t loc_id, const AH5_msh_group_t *msh_group)
{
    char success = AH5_TRUE;
    int i;
    hid_t msh_group_id;
    char *basename;

    basename = AH5_get_name_from_path(msh_group->path);
    if (AH5_path_valid(loc_id, basename))
        msh_group_id = H5Gopen(loc_id, basename, H5P_DEFAULT);
    else
        msh_group_id = H5Gcreate(loc_id, basename, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    for (i = 0; i < msh_group->nb_msh_instances; i++)
        success &= AH5_write_msh_instance(msh_group_id, msh_group->msh_instances + i);

    for (i = 0; i < msh_group->nb_mlk_instances; i++)
        success &= AH5_write_mlk_instance(msh_group_id, msh_group->mlk_instances + i);

    return success;
}

// Write mesh categories
char AH5_write_mesh(hid_t file_id, const AH5_mesh_t *mesh)
{
  char success = AH5_TRUE;
  int i;
  hid_t msh_category_id;

  if (AH5_path_valid(file_id, AH5_CATEGORY_NAME(AH5_C_MESH)))
      msh_category_id = H5Gopen(file_id, AH5_CATEGORY_NAME(AH5_C_MESH), H5P_DEFAULT);
  else
      msh_category_id = H5Gcreate(file_id, AH5_CATEGORY_NAME(AH5_C_MESH), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  for (i = 0; i < mesh->nb_groups; i++)
      success &= AH5_write_msh_group(msh_category_id, mesh->groups + i);

  return success;
}



// Print structured mesh
void AH5_print_smesh(const AH5_smesh_t *smesh, int space)
{
    hsize_t i;

    AH5_print_str_attr(AH5_A_TYPE, AH5_V_STRUCTURED, space + 4);
    printf("%*s-groups: %lu\n", space + 2, "", (long unsigned) smesh->nb_groups);
    for (i = 0; i < smesh->nb_groups; i++)
    {
        printf("%*sName: %s\n", space + 5, "", AH5_get_name_from_path(smesh->groups[i].path));
        if (smesh->groups[i].type != NULL)
            AH5_print_str_attr(AH5_A_TYPE, smesh->groups[i].type, space + 8);
        if (smesh->groups[i].entitytype != NULL)
            AH5_print_str_attr(AH5_A_ENTITY_TYPE, smesh->groups[i].entitytype, space + 8);
        if (smesh->groups[i].normals != NULL)
            printf("%*s-normals: yes\n", space + 8, "");
    }
    printf("%*s-groupgroups: %lu\n", space + 2, "", (unsigned long) smesh->nb_groupgroups);
    for (i = 0; i < smesh->nb_groupgroups; i++)
        printf("%*sName: %s\n", space + 5, "", AH5_get_name_from_path(smesh->groupgroups[i].path));

    if (smesh->nb_som_tables > 0)
    {
        printf("%*s-selector on mesh: %lu\n", space + 2, "", (unsigned long) smesh->nb_som_tables);
        for (i = 0; i < smesh->nb_som_tables; i++)
        {
            printf("%*sName: %s\n", space + 5, "", AH5_get_name_from_path(smesh->som_tables[i].path));
            AH5_print_str_attr(AH5_A_TYPE, AH5_V_POINT_IN_ELEMENT, space + 9);
            printf("%*s-number of points: %lu\n", space + 7, "", (long unsigned) smesh->som_tables[i].nb_points);
        }
    }
}


// Print selectorOnMesh table in unstructured mesh
void AH5_print_umesh_som_table(const AH5_usom_table_t *usom_table, int space)
{
    hsize_t k;
    char dim;

    switch (usom_table->type)
    {
    case SOM_POINT_IN_ELEMENT:
        printf("%*sInstance: %s\n", space + 5, "", AH5_get_name_from_path(usom_table->path));
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_POINT_IN_ELEMENT, space + 9);
        for (k = 0; k < usom_table->data.pie.nb_points; k++)
        {
            dim = usom_table->data.pie.nb_dims;
            if (dim == 3)
                if (usom_table->data.pie.vectors[k][2] == -1)
                    dim = 2;
            if (dim == 2)
                if (usom_table->data.pie.vectors[k][1] == -1)
                    dim = 1;

            switch (dim)
            {
            case 1:
                printf("%*sPoint %lu: index=%i, v1=%f\n", space + 7, "", (long unsigned) k, usom_table->data.pie.indices[k], usom_table->data.pie.vectors[k][0]);
                break;
            case 2:
                printf("%*sPoint %lu: index=%i, v1=%f, v2=%f\n", space + 7, "", (long unsigned) k, usom_table->data.pie.indices[k], usom_table->data.pie.vectors[k][0], usom_table->data.pie.vectors[k][1]);
                break;
            case 3:
                printf("%*sPoint %lu: index=%i, v1=%f, v2=%f, v3=%f\n", space + 7, "", (long unsigned) k, usom_table->data.pie.indices[k], usom_table->data.pie.vectors[k][0], usom_table->data.pie.vectors[k][1], usom_table->data.pie.vectors[k][2]);
                break;
            }
        }
        break;
    case SOM_EDGE:
        printf("%*sInstance: %s\n", space + 5, "", AH5_get_name_from_path(usom_table->path));
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_EDGE, space + 9);
        for (k = 0; k < usom_table->data.ef.dims[0]; k++)
            printf("%*sId %lu: element=%i, inner_id=%i\n", space + 7, "", (long unsigned) k, usom_table->data.ef.items[2*k], usom_table->data.ef.items[2*k+1]);
        break;
    case SOM_FACE:
        printf("%*sInstance: %s\n", space + 5, "", AH5_get_name_from_path(usom_table->path));
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_FACE, space + 9);
        for (k = 0; k < usom_table->data.ef.dims[0]; k++)
            printf("%*sId %lu: element=%i, inner_id=%i\n", space + 7, "", (long unsigned) k, usom_table->data.ef.items[2*k], usom_table->data.ef.items[2*k+1]);
        break;
    default:
        break;
    }
}


// Print unstructured mesh
void AH5_print_umesh(const AH5_umesh_t *umesh, int space)
{
    hsize_t i, offset = 0;
    int j, element_type;

    AH5_print_str_attr(AH5_A_TYPE, AH5_V_UNSTRUCTURED, space + 4);
    if (umesh->nb_nodes[0] > 0)
    {
        printf("%*s-nodes: %lu\n", space + 2, "", (unsigned long) umesh->nb_nodes[0]);
        for (i = 0; i < umesh->nb_nodes[0]; i++)
        {
            printf("%*sNode n°%lu: ", space + 5, "", (unsigned long) i);
            for (j = 0; j < (int) umesh->nb_nodes[1]; j++)
                printf("%f ", umesh->nodes[i * umesh->nb_nodes[1] + j]);
            printf("\n");
        }
    }
    if (umesh->nb_elementtypes > 0 && umesh->nb_elementnodes > 0)
    {
        printf("%*s-elements: %lu\n", space + 2, "", (unsigned long) umesh->nb_elementtypes);
        for (i = 0; i < umesh->nb_elementtypes; i++)
        {
            printf("%*sElement %lu: type=%i", space + 5, "", (unsigned long) i, umesh->elementtypes[i]);
            element_type = umesh->elementtypes[i];
            if (element_type == 1)
                for (j = 0; j < 2; j++)
                    printf(", N%i=%i", j, umesh->elementnodes[offset++]);
            else if (element_type == 2)
                for (j = 0; j < 3; j++)
                    printf(", N%i=%i", j, umesh->elementnodes[offset++]);
            else if (element_type == 11)
                for (j = 0; j < 3; j++)
                    printf(", N%i=%i", j, umesh->elementnodes[offset++]);
            else if (element_type == 12)
                for (j = 0; j < 6; j++)
                    printf(", N%i=%i", j, umesh->elementnodes[offset++]);
            else if (element_type == 13)
                for (j = 0; j < 4; j++)
                    printf(", N%i=%i", j, umesh->elementnodes[offset++]);
            else if (element_type == 14)
                for (j = 0; j < 8; j++)
                    printf(", N%i=%i", j, umesh->elementnodes[offset++]);
            else if (element_type == 101)
                for (j = 0; j < 4; j++)
                    printf(", N%i=%i", j, umesh->elementnodes[offset++]);
            else if (element_type == 102)
                for (j = 0; j < 5; j++)
                    printf(", N%i=%i", j, umesh->elementnodes[offset++]);
            else if (element_type == 103)
                for (j = 0; j < 6; j++)
                    printf(", N%i=%i", j, umesh->elementnodes[offset++]);
            else if (element_type == 104)
                for (j = 0; j < 8; j++)
                    printf(", N%i=%i", j, umesh->elementnodes[offset++]);
            printf("\n");
        }
    }
    printf("%*s-groups: %lu\n", space + 2, "", (unsigned long) umesh->nb_groups);
    for (i = 0; i < umesh->nb_groups; i++)
        printf("%*sName: %s\n", space + 5, "", AH5_get_name_from_path(umesh->groups[i].path));
    printf("%*s-groupgroups: %lu\n", space + 2, "", (unsigned long) umesh->nb_groupgroups);
    for (i = 0; i < umesh->nb_groupgroups; i++)
        printf("%*sName : %s\n", space + 5, "", AH5_get_name_from_path(umesh->groupgroups[i].path));

    if (umesh->nb_som_tables > 0)
    {
        printf("%*s-selector on mesh: %lu\n", space + 2, "", (unsigned long) umesh->nb_som_tables);
        for (i = 0; i < umesh->nb_som_tables; i++)
            AH5_print_umesh_som_table(&(umesh->som_tables[i]), space);
    }
}


// Print mesh instance
void AH5_print_msh_instance(const AH5_msh_instance_t *msh_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", AH5_get_name_from_path(msh_instance->path));
    switch (msh_instance->type)
    {
    case MSH_STRUCTURED:
        AH5_print_smesh(&(msh_instance->data.structured), space);
        break;
    case MSH_UNSTRUCTURED:
        AH5_print_umesh(&(msh_instance->data.unstructured), space);
        break;
    default:
        break;
    }
}


// Print meshLink instance
void AH5_print_mlk_instance(const AH5_mlk_instance_t *mlk_instance, int space)
{
    printf("%*sMeshLink instance: %s\n", space, "", AH5_get_name_from_path(mlk_instance->path));
    AH5_print_str_attr(AH5_A_MESH1, mlk_instance->mesh1, space + 3);
    AH5_print_str_attr(AH5_A_MESH2, mlk_instance->mesh2, space + 3);
}


// Print mesh group
void AH5_print_msh_group(const AH5_msh_group_t *msh_group, int space)
{
    hsize_t i;

    printf("%*sGroup: %s\n", space, "", AH5_get_name_from_path(msh_group->path));
    for (i = 0; i < msh_group->nb_msh_instances; i++)
        AH5_print_msh_instance(&(msh_group->msh_instances[i]), space + 2);
    for (i = 0; i < msh_group->nb_mlk_instances; i++)
        AH5_print_mlk_instance(&(msh_group->mlk_instances[i]), space + 2);
    printf("\n");
}


// Print mesh category
void AH5_print_mesh(const AH5_mesh_t *mesh)
{
    hsize_t i;

    printf("###################################  Mesh  ###################################\n\n");
    for (i = 0; i < mesh->nb_groups; i++)
        AH5_print_msh_group(&(mesh->groups[i]), 0);
    printf("\n");
}




// Free memory used by grouGgroup
void AH5_free_groupgroup(AH5_groupgroup_t *groupgroup)
{
    if (groupgroup->path != NULL)
    {
        free(groupgroup->path);  // free groupGroup name
        groupgroup->path = NULL;
    }

    if (groupgroup->groupgroupnames != NULL)  // if groupGroup is not empty...
    {
        free(*(groupgroup->groupgroupnames));  // free groupGroup member names (strings)
        free(groupgroup->groupgroupnames);  // free groupGroup member names
        groupgroup->groupgroupnames = NULL;
        groupgroup->nb_groupgroupnames = 0;
    }
}


// Free memory used by structured mesh
void AH5_free_smesh(AH5_smesh_t *smesh)
{
    hsize_t i;

    if (smesh->x.nodes != NULL)
    {
        free(smesh->x.nodes);
        smesh->x.nodes = NULL;
        smesh->x.nb_nodes = 0;
    }
    if (smesh->y.nodes != NULL)
    {
        free(smesh->y.nodes);
        smesh->y.nodes = NULL;
        smesh->y.nb_nodes = 0;
    }
    if (smesh->z.nodes != NULL)
    {
        free(smesh->z.nodes);
        smesh->z.nodes = NULL;
        smesh->z.nb_nodes = 0;
    }

    if (smesh->groups != NULL)  // if any groups...
    {
        for (i = 0; i < smesh->nb_groups; i++)    // for each group...
        {
            if (smesh->groups[i].elements != NULL)  // if group is not empty...
                free(smesh->groups[i].elements);

            if (smesh->groups[i].normals != NULL)
            {
                free(*(smesh->groups[i].normals));
                free(smesh->groups[i].normals);
            }

            if (smesh->groups[i].path != NULL)
                free(smesh->groups[i].path);  // free group name

            //XXX Why release 'type' and 'entitype'. Why not point to the constant value (AH5_V_ELEMENT, ...)?
            if (smesh->groups[i].type != NULL)
                free(smesh->groups[i].type);  // free group type

            if (smesh->groups[i].entitytype != NULL)
                free(smesh->groups[i].entitytype);  // free group entitytype
        }
        free(smesh->groups);  // free space for pointers to groups
        smesh->groups = NULL;
        smesh->nb_groups = 0;
    }

    if (smesh->groupgroups != NULL)  // if any groupGroups...
    {
        for (i = 0; i < smesh->nb_groupgroups; i++)    // for each groupGroup...
            AH5_free_groupgroup(smesh->groupgroups + i);  // free AH5_groupgroup_t structures
        free(smesh->groupgroups);  // free space for pointers to groupGroups
        smesh->groupgroups = NULL;
        smesh->nb_groupgroups = 0;
    }

    if (smesh->som_tables != NULL)
    {
        for (i = 0; i < smesh->nb_som_tables; i++)
        {
            if (smesh->som_tables[i].path != NULL)
                free(smesh->som_tables[i].path);
            if (smesh->som_tables[i].elements != NULL)
            {
                free(*(smesh->som_tables[i].elements));
                free(smesh->som_tables[i].elements);
            }
            if (smesh->som_tables[i].vectors != NULL)
            {
                free(*(smesh->som_tables[i].vectors));
                free(smesh->som_tables[i].vectors);
            }
        }
        free(smesh->som_tables);
        smesh->som_tables = NULL;
        smesh->nb_som_tables = 0;
    }
}


// Free memory used by unstructured mesh
void AH5_free_umesh(AH5_umesh_t *umesh)
{
    hsize_t i;

    if (umesh->elementnodes != NULL)  // if any elementnodes...
    {
        free(umesh->elementnodes);
        umesh->elementnodes = NULL;
        umesh->nb_elementnodes = 0;
    }

    if (umesh->elementtypes != NULL)  // if any elementtypes...
    {
        free(umesh->elementtypes);
        umesh->elementtypes = NULL;
        umesh->nb_elementtypes = 0;
    }

    if (umesh->nodes != NULL)  // if any nodes...
    {
        free(umesh->nodes);
        umesh->nodes = NULL;
        umesh->nb_nodes[0] = 0;
        umesh->nb_nodes[1] = 0;
    }

    if (umesh->groups != NULL)  // if any groups...
    {
        for (i = 0; i < umesh->nb_groups; i++)    // for each group...
        {
            free(umesh->groups[i].path);  // free group name
            free(umesh->groups[i].type);
            free(umesh->groups[i].entitytype);
            free(umesh->groups[i].groupelts);  // free group values (no need to assign NULL & set nb_groupelts to 0
        }
        free(umesh->groups);  // free space for pointers to groups
        umesh->groups = NULL;
        umesh->nb_groups = 0;
    }

    if (umesh->groupgroups != NULL)  // if any groupGroups...
    {
        for (i = 0; i < umesh->nb_groupgroups; i++)    // for each groupGroup...
            AH5_free_groupgroup(umesh->groupgroups + i);  // free AH5_groupgroup_t structures
        free(umesh->groupgroups);  // free space for pointers to groupGroups
        umesh->groupgroups = NULL;
        umesh->nb_groupgroups = 0;
    }

    if (umesh->som_tables != NULL)
    {
        for (i = 0; i < umesh->nb_som_tables; i++)
        {
            if (umesh->som_tables[i].path != NULL)
                free(umesh->som_tables[i].path);

            switch (umesh->som_tables[i].type)
            {
            case SOM_POINT_IN_ELEMENT:
                if (umesh->som_tables[i].data.pie.indices != NULL)
                {
                    free(umesh->som_tables[i].data.pie.indices);
                }
                if (umesh->som_tables[i].data.pie.vectors != NULL)
                {
                    free(*(umesh->som_tables[i].data.pie.vectors));
                    free(umesh->som_tables[i].data.pie.vectors);
                }
                break;
            case SOM_EDGE:
                if (umesh->som_tables[i].data.ef.items != NULL)
                    free(umesh->som_tables[i].data.ef.items);
                break;
            case SOM_FACE:
                if (umesh->som_tables[i].data.ef.items != NULL)
                    free(umesh->som_tables[i].data.ef.items);
                break;
            default:
                break;
            }
            umesh->som_tables[i].type = SOM_INVALID;
        }
        free(umesh->som_tables);
        umesh->som_tables = NULL;
        umesh->nb_som_tables = 0;
    }
}


// Free memory used by mesh instance
void AH5_free_msh_instance(AH5_msh_instance_t *msh_instance)
{
    if (msh_instance->path != NULL)
    {
        free(msh_instance->path);
        msh_instance->path = NULL;
    }

    switch (msh_instance->type)
    {
    case MSH_STRUCTURED:
        AH5_free_smesh(&(msh_instance->data.structured));
        break;
    case MSH_UNSTRUCTURED:
        AH5_free_umesh(&(msh_instance->data.unstructured));
        break;
    default:
        break;
    }
    msh_instance->type = MSH_INVALID;
}


void AH5_free_mlk_instance(AH5_mlk_instance_t *mlk_instance)
{
    if (mlk_instance->path != NULL)
    {
        free(mlk_instance->path);
        mlk_instance->path = NULL;
    }
    mlk_instance->type = MSHLNK_INVALID;
    if (mlk_instance->mesh1 != NULL)
    {
        free(mlk_instance->mesh1);
        mlk_instance->mesh1 = NULL;
    }
    if (mlk_instance->mesh2 != NULL)
    {
        free(mlk_instance->mesh2);
        mlk_instance->mesh2 = NULL;
    }
    if (mlk_instance->data != NULL)
    {
        free(mlk_instance->data);
        mlk_instance->data = NULL;
        mlk_instance->dims[0] = 0;
        mlk_instance->dims[1] = 0;
    }
}


// Free memory used by mesh group
void AH5_free_msh_group(AH5_msh_group_t *msh_group)
{
    hsize_t i;

    free(msh_group->path);

    if (msh_group->msh_instances != NULL)
    {
        for (i = 0; i < msh_group->nb_msh_instances; i++)
            AH5_free_msh_instance(msh_group->msh_instances + i);
        free(msh_group->msh_instances);
    }
    if (msh_group->mlk_instances != NULL)
    {
        for (i = 0; i < msh_group->nb_mlk_instances; i++)
            AH5_free_mlk_instance(msh_group->mlk_instances + i);
        free(msh_group->mlk_instances);
    }
    AH5_init_msh_group(msh_group);
}


// Free memory used by mesh category
void AH5_free_mesh(AH5_mesh_t *mesh)
{
    hsize_t i;

    if (mesh->groups != NULL)
    {
        for (i = 0; i < mesh->nb_groups; i++)
            AH5_free_msh_group(mesh->groups + i);
        free(mesh->groups);
    }
    AH5_init_mesh(mesh);
}

