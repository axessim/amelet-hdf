#include "mesh.h"


// Read groupGroup (both the un/structured)
void read_groupgroup (hid_t file_id, const char* path, groupgroup_t *groupgroup)
{
    int nb_dims;
    H5T_class_t type_class;
    size_t length;
    char success = FALSE;

    groupgroup->nb_groupgroupnames = 1;  // in case of single value
    groupgroup->path = strdup(path);
    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path, &(groupgroup->nb_groupgroupnames), &type_class, &length) >= 0)
                    if (type_class == H5T_STRING)
                        if(read_string_dataset(file_id, path, groupgroup->nb_groupgroupnames, length, &(groupgroup->groupgroupnames)))
                            success = TRUE;
    if (!success)
    {
        print_err_dset(C_MESH, path);
        groupgroup->nb_groupgroupnames = 0;
        groupgroup->groupgroupnames = NULL;
    }
}


// Read m x n dataset "/x, /y or /z" (32-bit signed float)
void read_smesh_axis (hid_t file_id, const char* path, axis_t *axis)
{
    H5T_class_t type_class;
    int nb_dims;
    size_t length;
    char success = FALSE;

    axis->nb_nodes = 1;
    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path, &(axis->nb_nodes), &type_class, &length) >= 0)
                    if (type_class == H5T_FLOAT && length == 4)
                        if (read_float_dataset(file_id, path, axis->nb_nodes, &(axis->nodes)))
                            success = TRUE;
    if (!success)
    {
        print_err_dset(C_MESH, path);
        axis->nb_nodes = 0;
        axis->nodes = NULL;
    }

}


// Read group in structured mesh (+normals)
void read_smsh_group (hid_t file_id, const char *path, sgroup_t *sgroup)
{
    hsize_t dims[2] = {1, 1};
    int nb_dims;
    H5T_class_t type_class;
    size_t length;
    char normalpath[ABSOLUTE_PATH_NAME_LENGTH];
    char *temp, success = FALSE, success2 = FALSE;

    sgroup->dims[0] = 1;
    sgroup->dims[1] = 1;
    sgroup->path = strdup(path);
    if (!read_str_attr(file_id, path, A_TYPE, &(sgroup->type)))
        print_err_attr(C_MESH, path, A_TYPE);
    if (!read_str_attr(file_id, path, A_ENTITY_TYPE, &(sgroup->entitytype)))
        if (strcmp(sgroup->type, V_NODE) != 0)
            print_err_attr(C_MESH, path, A_ENTITY_TYPE);
    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims == 2)
                if (H5LTget_dataset_info(file_id, path, sgroup->dims, &type_class, &length) >= 0)
                    if (sgroup->dims[1] >= 1 && sgroup->dims[1] <= 6 && type_class == H5T_INTEGER && length == 4)
                        if (read_int_dataset(file_id, path, sgroup->dims[0] * sgroup->dims[1], &(sgroup->elements)))
                            success = TRUE;
    if (success)
    {
        strcpy(normalpath, path);
        temp = strstr(path, "/group/");
        normalpath[temp-path] = '\0';
        temp = get_name_from_path(path);
        strcat(normalpath, G_NORMAL);
        strcat(normalpath, "/");
        strcat(normalpath, temp);

        if (H5Lexists(file_id, normalpath, H5P_DEFAULT) > 0)
            if (H5LTget_dataset_ndims(file_id, normalpath, &nb_dims) >= 0)
                if (nb_dims <= 1)
                    if (H5LTget_dataset_info(file_id, normalpath, dims, &type_class, &length) >= 0)
                        if (dims[0] == sgroup->dims[0] && type_class == H5T_STRING && length == 2)
                            if(read_string_dataset(file_id, normalpath, dims[0], length, &(sgroup->normals)))
                                success2 = TRUE;
        if (!success2)
            sgroup->normals = NULL;
    }
    else
    {
        print_err_dset(C_MESH, path);
        sgroup->dims[0] = 0;
        sgroup->dims[0] = 0;
        sgroup->elements = NULL;
        sgroup->normals = NULL;
    }
}

// Read table of type "pointInElement" from /selectorOnMesh (structured) (element: 32-bit unsigned int, vector: 32-bit signed float)
void read_ssom_pie_table (hid_t file_id, const char *path, ssom_pie_table_t *ssom_pie_table)
{
    hsize_t nfields, nrecords, i;
    char success = FALSE;
    char **field_names;
    size_t *field_sizes;
    size_t *field_offsets;
    size_t type_size;
    int field_index1[] = {0, 1, 2, 3, 4, 5};
    int field_index2[3];

    ssom_pie_table->path = strdup(path);
    if (H5Lexists(file_id, path, H5P_DEFAULT) != FALSE)
        if (H5TBget_table_info(file_id, path, &nfields, &nrecords) >= 0)
            if ((nfields == 3 || nfields == 6 || nfields == 9) && nrecords > 0)
            {
                field_names = (char **) malloc(nfields * sizeof(char *));
                field_names[0] = (char *) malloc(TABLE_FIELD_NAME_LENGTH * nfields * sizeof(char));
                for (i = 0; i < nfields; i++)
                    field_names[i] = field_names[0] + i * TABLE_FIELD_NAME_LENGTH;
                field_sizes = (size_t *) malloc(sizeof(size_t *) * nfields);
                field_offsets = (size_t *) malloc(sizeof(size_t *) * nfields);

                if (H5TBget_field_info(file_id, path, field_names, field_sizes, field_offsets, &type_size) >= 0)
                {
                    if (nfields == 3)
                    {
                        if (strcmp(field_names[0], "imin") == 0 && strcmp(field_names[1], "imax") == 0 && strcmp(field_names[2], "v1") == 0)
                        {
                            success = TRUE;
                            field_index2[0] = 2;
                        }
                    }
                    else if (nfields == 6)
                    {
                        if (strcmp(field_names[0], "imin") == 0 && strcmp(field_names[1], "jmin") == 0 && strcmp(field_names[2], "imax") == 0
                                && strcmp(field_names[3], "jmax") == 0 && strcmp(field_names[4], "v1") == 0 && strcmp(field_names[5], "v2") == 0)
                        {
                            success = TRUE;
                            field_index2[0] = 4;
                            field_index2[1] = 5;
                        }
                    }
                    else if (nfields == 9)
                    {
                        if (strcmp(field_names[0], "imin") == 0 && strcmp(field_names[1], "jmin") == 0 && strcmp(field_names[2], "kmin") == 0
                                && strcmp(field_names[3], "imax") == 0 && strcmp(field_names[4], "jmax") == 0 && strcmp(field_names[5], "kmax") == 0
                                && strcmp(field_names[6], "v1") == 0 && strcmp(field_names[7], "v2") == 0 && strcmp(field_names[8], "v3") == 0)
                        {
                            success = TRUE;
                            field_index2[0] = 6;
                            field_index2[1] = 7;
                            field_index2[2] = 8;
                        }
                    }
                }
                if (success == TRUE)
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

                    if (H5TBread_fields_index(file_id, path, (int) ssom_pie_table->nb_dims * 2, field_index1, 0, nrecords, (hsize_t) ssom_pie_table->nb_dims * 2 * sizeof(int), field_offsets, field_sizes, ssom_pie_table->elements[0])
                            < 0
                            ||
                            H5TBread_fields_index(file_id, path, (int) ssom_pie_table->nb_dims, field_index2, 0, nrecords, (hsize_t) ssom_pie_table->nb_dims * sizeof(float), field_offsets, field_sizes, ssom_pie_table->vectors[0])
                            < 0)
                        printf("***** WARNING(%s): Data from table \"%s\" may be corrupted. *****\n\n", C_MESH, path);
                    ssom_pie_table->nb_points = nrecords;
                }
                free(field_names[0]);
                free(field_names);
                free(field_sizes);
                free(field_offsets);
            }
    if (!success)
    {
        printf("***** ERROR(%s): Cannot read table \"%s\". *****\n\n", C_MESH, path);
        ssom_pie_table->nb_dims = 0;
        ssom_pie_table->nb_points = 0;
        ssom_pie_table->elements = NULL;
        ssom_pie_table->vectors = NULL;
    }
}


// Read structured mesh
void read_smesh(hid_t file_id, const char* path, smesh_t *smesh)
{
    char path2[ABSOLUTE_PATH_NAME_LENGTH], path3[ABSOLUTE_PATH_NAME_LENGTH];
    children_t children;
    char *type, success;
    hsize_t i;

    // X Axis
    strcpy(path2, path);
    strcat(path2, G_CARTESIAN_GRID);
    strcat(path2, G_X);
    read_smesh_axis(file_id, path2, &(smesh->x));

    // Y Axis
    strcpy(path2, path);
    strcat(path2, G_CARTESIAN_GRID);
    strcat(path2, G_Y);
    read_smesh_axis(file_id, path2, &(smesh->y));

    // Z Axis
    strcpy(path2, path);
    strcat(path2, G_CARTESIAN_GRID);
    strcat(path2, G_Z);
    read_smesh_axis(file_id, path2, &(smesh->z));

    // groups
    strcpy(path2, path);
    strcat(path2, G_GROUP);
    children = read_children_name(file_id, path2);
    smesh->nb_groups = children.nb_children;
    smesh->groups = NULL;
    if (children.nb_children > 0)
    {
        smesh->groups = (sgroup_t *) malloc(children.nb_children * sizeof(sgroup_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path3, path2);
            strcat(path3, children.childnames[i]);
            read_smsh_group(file_id, path3, smesh->groups + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }

    // read groupGroup if exists
    strcpy(path2, path);
    strcat(path2, G_GROUPGROUP);
    children = read_children_name(file_id, path2);
    smesh->nb_groupgroups = children.nb_children;
    smesh->groupgroups = NULL;
    if (children.nb_children > 0)
    {
        smesh->groupgroups = (groupgroup_t *) malloc(children.nb_children * sizeof(groupgroup_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path3, path2);
            strcat(path3, children.childnames[i]);
            read_groupgroup(file_id, path3, smesh->groupgroups + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }

    // read selectorOnMesh
    strcpy(path2, path);
    strcat(path2, G_SELECTOR_ON_MESH);
    children = read_children_name(file_id, path2);
    smesh->nb_som_tables = children.nb_children;
    smesh->som_tables = NULL;
    if (children.nb_children > 0)
    {
        smesh->som_tables = (ssom_pie_table_t *) malloc(children.nb_children * sizeof(ssom_pie_table_t));
        for (i = 0; i < children.nb_children; i++)
        {
            success = FALSE;
            strcpy(path3, path2);
            strcat(path3, children.childnames[i]);
            if (read_str_attr(file_id, path3, A_TYPE, &type))
            {
                if (strcmp(type,V_POINT_IN_ELEMENT) == 0)
                {
                    read_ssom_pie_table(file_id, path3, smesh->som_tables + i);
                    success = TRUE;
                }
                free(type);
            }
            if (!success)
            {
                print_err_attr(C_MESH, A_TYPE, path3);
                smesh->som_tables[i].path = NULL;
                smesh->som_tables[i].nb_dims = 0;
                smesh->som_tables[i].nb_points = 0;
                smesh->som_tables[i].elements = NULL;
                smesh->som_tables[i].vectors = NULL;
            }
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}


// Read group in unstructured mesh
void read_umsh_group (hid_t file_id, const char* path, ugroup_t *ugroup)
{
    H5T_class_t type_class;
    char success = FALSE;
    size_t length;
    int nb_dims;

    ugroup->nb_groupelts = 1;
    ugroup->path = strdup(path);
    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path, &(ugroup->nb_groupelts), &type_class, &length) >= 0)
                    if (type_class == H5T_INTEGER && length == 4)
                        if (read_int_dataset(file_id, path, ugroup->nb_groupelts, &(ugroup->groupelts)))
                            success = TRUE;
    if (!success)
    {
        print_err_dset(C_MESH, path);
        ugroup->nb_groupelts = 0;
        ugroup->groupelts = NULL;
    }
}


// Read table of type "pointInElement" from /selectorOnMesh (unstructured) (index: 32-bit signed int, vector: 32-bit signed float)
void read_usom_pie_table (hid_t file_id, const char *path, usom_pie_table_t *usom_pie_table)
{
    hsize_t nfields, nrecords, i;
    char success = FALSE;
    char **field_names;
    size_t *field_sizes;
    size_t *field_offsets;
    size_t type_size;
    int field_index1[] = {0};
    int field_index2[] = {1, 2, 3};

    if (H5Lexists(file_id, path, H5P_DEFAULT) != FALSE)
        if (H5TBget_table_info(file_id, path, &nfields, &nrecords) >= 0)
            if (nfields >= 2 && nfields <=4 && nrecords > 0)
            {
                field_names = (char **) malloc(nfields * sizeof(char *));
                field_names[0] = (char *) malloc(TABLE_FIELD_NAME_LENGTH * nfields * sizeof(char));
                for (i = 0; i < nfields; i++)
                    field_names[i] = field_names[0] + i * TABLE_FIELD_NAME_LENGTH;
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
                        if (H5TBread_fields_index(file_id, path, 1, field_index1, 0, nrecords, sizeof(int), field_offsets, field_sizes, usom_pie_table->indices)
                                < 0
                                ||
                                H5TBread_fields_index(file_id, path, (nfields - 1), field_index2, 0, nrecords, (nfields - 1) * sizeof(float), field_offsets, field_sizes, usom_pie_table->vectors[0])
                                < 0)
                            printf("***** WARNING(%s): Data from table \"%s\" may be corrupted. *****\n\n", C_MESH, path);
                        usom_pie_table->nb_points = nrecords;
                        usom_pie_table->nb_dims = (char) nfields - 1;
                        success = TRUE;
                    }
                free(field_names[0]);
                free(field_names);
                free(field_sizes);
                free(field_offsets);
            }
    if (!success)
    {
        printf("***** ERROR(%s): Cannot read table \"%s\". *****\n\n", C_MESH, path);
        usom_pie_table->nb_points = 0;
        usom_pie_table->nb_dims = 0;
        usom_pie_table->indices = NULL;
        usom_pie_table->vectors = NULL;
    }
}


// Read dataset of type "edge" or "face" from /selectorOnMesh (32-bit signed int)
void read_usom_ef_table (hid_t file_id, const char *path, usom_ef_table_t *usom_ef_table)
{
    int nb_dims;
    H5T_class_t type_class;
    size_t length;
    char success = FALSE;

    usom_ef_table->dims[0] = 1;
    usom_ef_table->dims[1] = 1;
    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims == 2)
                if (H5LTget_dataset_info(file_id, path, usom_ef_table->dims, &type_class, &length) >= 0)
                    if (type_class == H5T_INTEGER && length == 4)
                        if (read_int_dataset(file_id, path, usom_ef_table->dims[0] * usom_ef_table->dims[1], &(usom_ef_table->items)))
                            success = TRUE;
    if (!success)
    {
        print_err_dset(C_MESH, path);
        usom_ef_table->dims[0] = 0;
        usom_ef_table->dims[1] = 0;
        usom_ef_table->items = NULL;
    }
}


// Read selector on mesh (unstructured mesh)
void read_umesh_som_table (hid_t file_id, const char *path, usom_table_t *usom_table)
{
    char *type;

    usom_table->path = strdup(path);
    if (read_str_attr(file_id, path, A_TYPE, &type))
    {
        if (strcmp(type, V_POINT_IN_ELEMENT) == 0)
        {
            usom_table->type = SOM_POINT_IN_ELEMENT;
            read_usom_pie_table(file_id, path, &(usom_table->data.pie));
        }

        else if (strcmp(type, V_EDGE) == 0)
        {
            usom_table->type = SOM_EDGE;
            read_usom_ef_table(file_id, path, &(usom_table->data.ef));
        }
        else if (strcmp(type, V_FACE) == 0)
        {
            usom_table->type = SOM_FACE;
            read_usom_ef_table(file_id, path, &(usom_table->data.ef));
        }
        free(type);
    }
    else
        print_err_attr(C_MESH, path, A_TYPE);
}


// Read unstructured mesh
char read_umesh (hid_t file_id, const char* path, umesh_t *umesh)
{
    char path2[ABSOLUTE_PATH_NAME_LENGTH], path3[ABSOLUTE_PATH_NAME_LENGTH];
    char rdata = TRUE, success = FALSE;
    hsize_t i;
    int nb_dims;
    H5T_class_t type_class;
    size_t length;
    hid_t dset_id;
    children_t children;

    // Read m x 1 dataset "elementNodes" (32-bit signed integer)
    umesh->nb_elementnodes = 1;
    strcpy(path2, path);
    strcat(path2, G_ELEMENT_NODES);
    if (H5Lexists(file_id, path2, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path2, &(umesh->nb_elementnodes), &type_class, &length) >= 0)
                    if (type_class == H5T_INTEGER && length == 4)
                        if (read_int_dataset(file_id, path2, umesh->nb_elementnodes, &(umesh->elementnodes)))
                            success = TRUE;
    if (!success)
    {
        print_err_dset(C_MESH, path2);
        umesh->nb_elementnodes = 0;
        umesh->elementnodes = NULL;
        rdata = FALSE;
    }

    success = FALSE;

    // Read m x 1 dataset "elementTypes" (8-bit signed char)
    umesh->nb_elementtypes = 1;
    strcpy(path2, path);
    strcat(path2, G_ELEMENT_TYPES);
    if (H5Lexists(file_id, path2, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path2, &(umesh->nb_elementtypes), &type_class, &length) >= 0)
                    if (type_class == H5T_INTEGER && length == 1)
                    {
                        umesh->elementtypes = (char *) malloc((umesh->nb_elementtypes * sizeof(char)));
                        dset_id = H5Dopen(file_id, path2, H5P_DEFAULT);
                        if (H5Dread(dset_id, H5T_NATIVE_CHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, umesh->elementtypes) >= 0)
                            success = TRUE;
                        H5Dclose(dset_id);
                        if (!success)
                        {
                            free(umesh->elementtypes);
                            umesh->elementtypes = NULL;
                        }
                    }
    if (!success)
    {
        print_err_dset(C_MESH, path2);
        umesh->nb_elementtypes = 0;
        umesh->elementtypes = NULL;
        rdata = FALSE;
    }

    success = FALSE;
    umesh->nb_nodes[0] = 1;
    umesh->nb_nodes[1] = 1;
    // Read m x n dataset "nodes" (32-bit signed float)
    strcpy(path2, path);
    strcat(path2, G_NODES);
    if (H5Lexists(file_id, path2, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
            if (nb_dims == 2)
                if (H5LTget_dataset_info(file_id, path2, umesh->nb_nodes, &type_class, &length) >= 0)
                    if (umesh->nb_nodes[1] <= 3 && type_class == H5T_FLOAT && length == 4)
                        if (read_float_dataset(file_id, path2, umesh->nb_nodes[0] * umesh->nb_nodes[1], &(umesh->nodes)))
                            success = TRUE;
    if (!success)
    {
        print_err_dset(C_MESH, path2);
        umesh->nb_nodes[0] = 0;
        umesh->nb_nodes[1] = 0;
        umesh->nodes = NULL;
        rdata = FALSE;
    }

    // read groupGroup if exists
    strcpy(path2, path);
    strcat(path2, G_GROUPGROUP);
    children = read_children_name(file_id, path2);
    umesh->nb_groupgroups = children.nb_children;
    umesh->groupgroups = NULL;
    if (children.nb_children > 0)
    {
        umesh->groupgroups = (groupgroup_t *) malloc(children.nb_children * sizeof(groupgroup_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path3, path2);
            strcat(path3, children.childnames[i]);
            read_groupgroup(file_id, path3, umesh->groupgroups + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }

    // read group
    strcpy(path2, path);
    strcat(path2, G_GROUP);
    children = read_children_name(file_id, path2);
    umesh->nb_groups = children.nb_children;
    umesh->groups = NULL;
    if (children.nb_children > 0)
    {
        umesh->groups = (ugroup_t *) malloc(children.nb_children * sizeof(ugroup_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path3, path2);
            strcat(path3, children.childnames[i]);
            read_umsh_group(file_id, path3, umesh->groups + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }

    // read selectorOnMesh
    strcpy(path2, path);
    strcat(path2, G_SELECTOR_ON_MESH);
    children = read_children_name(file_id, path2);
    umesh->nb_som_tables = children.nb_children;
    umesh->som_tables = NULL;
    if (children.nb_children > 0)
    {
        umesh->som_tables = (usom_table_t *) malloc(children.nb_children * sizeof(usom_table_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path3, path2);
            strcat(path3, children.childnames[i]);
            read_umesh_som_table(file_id, path3, umesh->som_tables + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
    return rdata;
}


// Read mesh instance
void read_msh_instance (hid_t file_id, const char *path, msh_instance_t *msh_instance)
{
    char *type;

    if (H5Lexists(file_id, path, H5P_DEFAULT) <= 0)
        printf("***** ERROR(%s): Cannot read mesh instance \"%s\". *****\n\n", C_MESH, path);
    msh_instance->path = strdup(path);
    if (read_str_attr(file_id, path, A_TYPE, &type))
    {
        if (strcmp(type, V_STRUCTURED) == 0)
        {
            msh_instance->type = MSH_STRUCTURED;
            read_smesh(file_id, path, &(msh_instance->data.structured));
        }
        else if (strcmp(type, V_UNSTRUCTURED) == 0)
        {
            msh_instance->type = MSH_UNSTRUCTURED;
            read_umesh(file_id, path, &(msh_instance->data.unstructured));
        }
        else
        {
            msh_instance->type = MSH_INVALID;
            printf("***** ERROR(%s): Unexpected attribute value of \"%s@%s\". *****\n\n", C_MESH, path, A_TYPE);
        }
        free(type);
    }
    else
    {
        msh_instance->type = MSH_INVALID;
        print_err_attr(C_MESH, path, A_TYPE);
    }
}


// Read meshLink instance
void read_mlk_instance (hid_t file_id, const char *path, mlk_instance_t *mlk_instance)
{
    char *type, success = TRUE, dataset_read = FALSE;
    H5T_class_t type_class;
    size_t length;
    int nb_dims;

    mlk_instance->path = strdup(path);
    if (!read_str_attr(file_id, path, A_MESH1, &(mlk_instance->mesh1)))
    {
        print_err_attr(C_MESH, path, A_MESH1);
        success = FALSE;
    }
    if (!read_str_attr(file_id, path, A_MESH2, &(mlk_instance->mesh2)))
    {
        print_err_attr(C_MESH, path, A_MESH2);
        success = FALSE;
    }

    if (!read_str_attr(file_id, path, A_TYPE, &type))
    {
        print_err_attr(C_MESH, path, A_TYPE);
        success = FALSE;
    }

    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims == 2)
                if (H5LTget_dataset_info(file_id, path, mlk_instance->dims, &type_class, &length) >= 0)
                    if (type_class == H5T_INTEGER)
                        if (read_int_dataset(file_id, path, mlk_instance->dims[0] * mlk_instance->dims[1], &(mlk_instance->data)))
                            dataset_read = TRUE;
    if (!dataset_read)
    {
        print_err_dset(C_MESH, path);
        mlk_instance->dims[0] = 0;
        mlk_instance->dims[1] = 0;
        success = FALSE;
    }

    if (success)
    {
        if (strcmp(type, V_NODE))
            mlk_instance->type = MSHLNK_NODE;
        else if (strcmp(type, V_EDGE))
            mlk_instance->type = MSHLNK_EDGE;
        else if (strcmp(type, V_FACE))
            mlk_instance->type = MSHLNK_FACE;
        else if (strcmp(type, V_VOLUME))
            mlk_instance->type = MSHLNK_VOLUME;
        else
            mlk_instance->type = MSHLNK_INVALID;
    }
    else
        mlk_instance->type = MSHLNK_INVALID;
    if (type != NULL)
    {
        free(type);
        type = NULL;
    }
}



// Read mesh group
void read_msh_group (hid_t file_id, const char *path, msh_group_t *msh_group)
{
    char path2[ABSOLUTE_PATH_NAME_LENGTH], path3[ABSOLUTE_PATH_NAME_LENGTH];
    children_t children;
    hsize_t i, j = 0;

    if (H5Lexists(file_id, path, H5P_DEFAULT) <= 0)
        printf("***** ERROR(%s): Cannot read mesh group \"%s\". *****\n\n", C_MESH, path);
    msh_group->path = strdup(path);
    children = read_children_name(file_id, path);
    msh_group->nb_msh_instances = children.nb_children;
    for (i = 0; i < children.nb_children; i++)
        if (strcmp(children.childnames[i], G_MESH_LINK) == 0)
            msh_group->nb_msh_instances--;    // do not count /meshLink
    msh_group->msh_instances = NULL;
    if (children.nb_children > 0)
    {
        msh_group->msh_instances = (msh_instance_t *) malloc(msh_group->nb_msh_instances * sizeof(msh_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            if (strcmp(children.childnames[i], G_MESH_LINK) != 0)
            {
                strcpy(path2, path);
                strcat(path2, children.childnames[i]);
                read_msh_instance(file_id, path2, msh_group->msh_instances + j++);
            }
            free(children.childnames[i]);
        }
        free(children.childnames);
    }

    strcpy(path2, path);
    strcat(path2, G_MESH_LINK);
    children = read_children_name(file_id, path2);
    msh_group->nb_mlk_instances = children.nb_children;
    msh_group->mlk_instances = NULL;
    if (children.nb_children > 0)
    {
        msh_group->mlk_instances = (mlk_instance_t *) malloc(children.nb_children * sizeof(mlk_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path3, path2);
            strcat(path3, children.childnames[i]);
            read_mlk_instance(file_id, path3, msh_group->mlk_instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}


// Read mesh category
void read_mesh (hid_t file_id, mesh_t *mesh)
{
    char path[ABSOLUTE_PATH_NAME_LENGTH];
    children_t children;
    hsize_t i;

    children = read_children_name(file_id, C_MESH);
    mesh->nb_groups = children.nb_children;
    mesh->groups = NULL;
    if (children.nb_children > 0)
    {
        mesh->groups = (msh_group_t *) malloc(children.nb_children * sizeof(msh_group_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_MESH);
            strcat(path, children.childnames[i]);
            read_msh_group(file_id, path, mesh->groups + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}




// Print structured mesh
void print_smesh (smesh_t smesh, int space)
{
    hsize_t i;

    print_str_attr(A_TYPE, V_STRUCTURED, space + 4);
    printf("%*s-groups: %lu\n", space + 2, "", (long unsigned) smesh.nb_groups);
    for (i = 0; i < smesh.nb_groups; i++)
    {
        printf("%*sName: %s\n", space + 5, "", get_name_from_path(smesh.groups[i].path));
        if (smesh.groups[i].type != NULL)
            print_str_attr(A_TYPE, smesh.groups[i].type, space + 8);
        if (smesh.groups[i].entitytype != NULL)
            print_str_attr(A_ENTITY_TYPE, smesh.groups[i].entitytype, space + 8);
        if (smesh.groups[i].normals != NULL)
            printf("%*s-normals: yes\n", space + 8, "");
    }
    printf("%*s-groupgroups: %lu\n", space + 2, "", (unsigned long) smesh.nb_groupgroups);
    for (i = 0; i < smesh.nb_groupgroups; i++)
        printf("%*sName: %s\n", space + 5, "", get_name_from_path(smesh.groupgroups[i].path));

    if (smesh.nb_som_tables > 0)
    {
        printf("%*s-selector on mesh: %lu\n", space + 2, "", (unsigned long) smesh.nb_som_tables);
        for (i = 0; i < smesh.nb_som_tables; i++)
        {
            printf("%*sName: %s\n", space + 5, "", get_name_from_path(smesh.som_tables[i].path));
            print_str_attr(A_TYPE, V_POINT_IN_ELEMENT, space + 9);
            printf("%*s-number of points: %lu\n", space + 7, "", (long unsigned) smesh.som_tables[i].nb_points);
        }
    }

}

void print_umesh_som_table (usom_table_t usom_table, int space)
{
    hsize_t k;
    char dim;

    switch (usom_table.type)
    {
    case SOM_POINT_IN_ELEMENT:
        printf("%*sInstance: %s\n", space + 5, "", get_name_from_path(usom_table.path));
        print_str_attr(A_TYPE, V_POINT_IN_ELEMENT, space + 9);
        for (k = 0; k < usom_table.data.pie.nb_points; k++)
        {
            dim = usom_table.data.pie.nb_dims;
            if (dim == 3)
                if (usom_table.data.pie.vectors[k][2] == -1)
                    dim = 2;
            if (dim == 2)
                if (usom_table.data.pie.vectors[k][1] == -1)
                    dim = 1;

            switch (dim)
            {
            case 1:
                printf("%*sPoint %lu: index=%i, v1=%f\n", space + 7, "", (long unsigned) k, usom_table.data.pie.indices[k], usom_table.data.pie.vectors[k][0]);
                break;
            case 2:
                printf("%*sPoint %lu: index=%i, v1=%f, v2=%f\n", space + 7, "", (long unsigned) k, usom_table.data.pie.indices[k], usom_table.data.pie.vectors[k][0], usom_table.data.pie.vectors[k][1]);
                break;
            case 3:
                printf("%*sPoint %lu: index=%i, v1=%f, v2=%f, v3=%f\n", space + 7, "", (long unsigned) k, usom_table.data.pie.indices[k], usom_table.data.pie.vectors[k][0], usom_table.data.pie.vectors[k][1], usom_table.data.pie.vectors[k][2]);
                break;
            }
        }
        break;
    case SOM_EDGE:
        printf("%*sInstance: %s\n", space + 5, "", get_name_from_path(usom_table.path));
        print_str_attr(A_TYPE, V_EDGE, space + 9);
        for (k = 0; k < usom_table.data.ef.dims[0]; k++)
            printf("%*sId %lu: element=%i, inner_id=%i\n", space + 7, "", (long unsigned) k, usom_table.data.ef.items[2*k], usom_table.data.ef.items[2*k+1]);
        break;
    case SOM_FACE:
        printf("%*sInstance: %s\n", space + 5, "", get_name_from_path(usom_table.path));
        print_str_attr(A_TYPE, V_FACE, space + 9);
        for (k = 0; k < usom_table.data.ef.dims[0]; k++)
            printf("%*sId %lu: element=%i, inner_id=%i\n", space + 7, "", (long unsigned) k, usom_table.data.ef.items[2*k], usom_table.data.ef.items[2*k+1]);
        break;
    default:
        break;
    }
}


// Print unstructured mesh
void print_umesh (umesh_t umesh, int space)
{
    hsize_t i, offset = 0;
    int j, element_type;

    print_str_attr(A_TYPE, V_UNSTRUCTURED, space + 4);
    if (umesh.nb_nodes[0] > 0)
    {
        printf("%*s-nodes: %lu\n", space + 2, "", (unsigned long) umesh.nb_nodes[0]);
        for (i = 0; i < umesh.nb_nodes[0]; i++)
        {
            printf("%*sNode n°%lu: ", space + 5, "", (unsigned long) i);
            for (j = 0; j < (int) umesh.nb_nodes[1]; j++)
                printf("%f ", umesh.nodes[i * umesh.nb_nodes[1] + j]);
            printf("\n");
        }
    }
    if (umesh.nb_elementtypes > 0 && umesh.nb_elementnodes > 0)
    {
        printf("%*s-elements: %lu\n", space + 2, "", (unsigned long) umesh.nb_elementtypes);
        for (i = 0; i < umesh.nb_elementtypes; i++)
        {
            printf("%*sElement %lu: type=%i", space + 5, "", (unsigned long) i, umesh.elementtypes[i]);
            element_type = umesh.elementtypes[i];
            if (element_type == 1)
                for (j = 0; j < 2; j++)
                    printf(", N%i=%i", j, umesh.elementnodes[offset++]);
            else if (element_type == 2)
                for (j = 0; j < 3; j++)
                    printf(", N%i=%i", j, umesh.elementnodes[offset++]);
            else if (element_type == 11)
                for (j = 0; j < 3; j++)
                    printf(", N%i=%i", j, umesh.elementnodes[offset++]);
            else if (element_type == 12)
                for (j = 0; j < 6; j++)
                    printf(", N%i=%i", j, umesh.elementnodes[offset++]);
            else if (element_type == 13)
                for (j = 0; j < 4; j++)
                    printf(", N%i=%i", j, umesh.elementnodes[offset++]);
            else if (element_type == 14)
                for (j = 0; j < 8; j++)
                    printf(", N%i=%i", j, umesh.elementnodes[offset++]);
            else if (element_type == 101)
                for (j = 0; j < 4; j++)
                    printf(", N%i=%i", j, umesh.elementnodes[offset++]);
            else if (element_type == 102)
                for (j = 0; j < 5; j++)
                    printf(", N%i=%i", j, umesh.elementnodes[offset++]);
            else if (element_type == 103)
                for (j = 0; j < 6; j++)
                    printf(", N%i=%i", j, umesh.elementnodes[offset++]);
            else if (element_type == 104)
                for (j = 0; j < 8; j++)
                    printf(", N%i=%i", j, umesh.elementnodes[offset++]);
            printf("\n");
        }
    }
    printf("%*s-groups: %lu\n", space + 2, "", (unsigned long) umesh.nb_groups);
    for (i = 0; i < umesh.nb_groups; i++)
        printf("%*sName: %s\n", space + 5, "", get_name_from_path(umesh.groups[i].path));
    printf("%*s-groupgroups: %lu\n", space + 2, "", (unsigned long) umesh.nb_groupgroups);
    for (i = 0; i < umesh.nb_groupgroups; i++)
        printf("%*sName : %s\n", space + 5, "", get_name_from_path(umesh.groupgroups[i].path));

    if (umesh.nb_som_tables > 0)
    {
        printf("%*s-selector on mesh: %lu\n", space + 2, "", (unsigned long) umesh.nb_som_tables);
        for (i = 0; i < umesh.nb_som_tables; i++)
            print_umesh_som_table(umesh.som_tables[i], space);
    }
}


// Print mesh instance
void print_msh_instance (msh_instance_t msh_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", get_name_from_path(msh_instance.path));
    switch (msh_instance.type)
    {
    case MSH_STRUCTURED:
        print_smesh(msh_instance.data.structured, space);
        break;
    case MSH_UNSTRUCTURED:
        print_umesh(msh_instance.data.unstructured, space);
        break;
    default:
        break;
    }
}


// Print meshLink instance
void print_mlk_instance (mlk_instance_t mlk_instance, int space)
{
    printf("%*sMeshLink instance: %s\n", space, "", get_name_from_path(mlk_instance.path));
    print_str_attr(A_MESH1, mlk_instance.mesh1, space + 3);
    print_str_attr(A_MESH2, mlk_instance.mesh2, space + 3);
}


// Print mesh group
void print_msh_group (msh_group_t msh_group, int space)
{
    hsize_t i;

    printf("%*sGroup: %s\n", space, "", get_name_from_path(msh_group.path));
    for (i = 0; i < msh_group.nb_msh_instances; i++)
        print_msh_instance(msh_group.msh_instances[i], space + 2);
    for (i = 0; i < msh_group.nb_mlk_instances; i++)
        print_mlk_instance(msh_group.mlk_instances[i], space + 2);
    printf("\n");
}


// Print mesh category
void print_mesh (mesh_t mesh)
{
    hsize_t i;

    printf("###################################  Mesh  ###################################\n\n");
    for (i = 0; i < mesh.nb_groups; i++)
        print_msh_group(mesh.groups[i], 0);
    printf("\n");
}




// Free memory used by grouGgroup
void free_groupgroup (groupgroup_t *groupgroup)
{
    if (groupgroup->path != NULL)
    {
        free(groupgroup->path);  // free groupGroup name
        groupgroup->path = NULL;
    }

    if (groupgroup->nb_groupgroupnames > 0)  // if groupGroup is not empty...
    {
        free(*(groupgroup->groupgroupnames));  // free groupGroup member names (strings)
        free(groupgroup->groupgroupnames);  // free groupGroup member names
        groupgroup->groupgroupnames = NULL;
        groupgroup->nb_groupgroupnames = 0;
    }
}


// Free memory used by structured mesh
void free_smesh (smesh_t *smesh)
{
    hsize_t i;

    if (smesh->x.nb_nodes > 0)
    {
        free(smesh->x.nodes);
        smesh->x.nodes = NULL;
        smesh->x.nb_nodes = 0;
    }
    if (smesh->y.nb_nodes > 0)
    {
        free(smesh->y.nodes);
        smesh->y.nodes = NULL;
        smesh->y.nb_nodes = 0;
    }
    if (smesh->z.nb_nodes > 0)
    {
        free(smesh->z.nodes);
        smesh->z.nodes = NULL;
        smesh->z.nb_nodes = 0;
    }

    if (smesh->nb_groups > 0)  // if any groups...
    {
        for (i = 0; i < smesh->nb_groups; i++)    // for each group...
        {
            if (smesh->groups[i].dims[0] > 0)  // if group is not empty...
                free(smesh->groups[i].elements);

            if (smesh->groups[i].normals != NULL)
            {
                free(*(smesh->groups[i].normals));
                free(smesh->groups[i].normals);
            }

            free(smesh->groups[i].path);  // free group name

            if (smesh->groups[i].type != NULL)
                free(smesh->groups[i].type);  // free group type

            if (smesh->groups[i].entitytype != NULL)
                free(smesh->groups[i].entitytype);  // free group entitytype
        }
        free(smesh->groups);  // free space for pointers to groups
        smesh->groups = NULL;
        smesh->nb_groups = 0;
    }

    if (smesh->nb_groupgroups > 0)  // if any groupGroups...
    {
        for (i = 0; i < smesh->nb_groupgroups; i++)    // for each groupGroup...
            free_groupgroup(smesh->groupgroups + i);  // free groupgroup_t structures
        free(smesh->groupgroups);  // free space for pointers to groupGroups
        smesh->groupgroups = NULL;
        smesh->nb_groupgroups = 0;
    }

    if (smesh->nb_som_tables > 0)
    {
        for (i = 0; i < smesh->nb_som_tables; i++)
        {
            if (smesh->som_tables[i].path != NULL)
                free(smesh->som_tables[i].path);
            if (smesh->som_tables[i].nb_points > 0)
            {
                free(*(smesh->som_tables[i].elements));
                free(smesh->som_tables[i].elements);
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
void free_umesh (umesh_t *umesh)
{
    hsize_t i;

    if (umesh->nb_elementnodes > 0)  // if any elementnodes...
    {
        free(umesh->elementnodes);
        umesh->elementnodes = NULL;
        umesh->nb_elementnodes = 0;
    }

    if (umesh->nb_elementtypes > 0)  // if any elementtypes...
    {
        free(umesh->elementtypes);
        umesh->elementtypes = NULL;
        umesh->nb_elementtypes = 0;
    }

    if (umesh->nb_nodes[0] > 0)  // if any nodes...
    {
        free(umesh->nodes);
        umesh->nodes = NULL;
        umesh->nb_nodes[0] = 0;
        umesh->nb_nodes[1] = 0;
    }

    if (umesh->nb_groups > 0)  // if any groups...
    {
        for (i = 0; i < umesh->nb_groups; i++)    // for each group...
        {
            if (umesh->groups[i].path != NULL)
                free(umesh->groups[i].path);  // free group name
            if (umesh->groups[i].nb_groupelts > 0)  // if group is not empty...
                free(umesh->groups[i].groupelts);  // free group values (no need to assign NULL & set nb_groupelts to 0
        }
        free(umesh->groups);  // free space for pointers to groups
        umesh->groups = NULL;
        umesh->nb_groups = 0;
    }

    if (umesh->nb_groupgroups > 0)  // if any groupGroups...
    {
        for (i = 0; i < umesh->nb_groupgroups; i++)    // for each groupGroup...
            free_groupgroup(umesh->groupgroups + i);  // free groupgroup_t structures
        free(umesh->groupgroups);  // free space for pointers to groupGroups
        umesh->groupgroups = NULL;
        umesh->nb_groupgroups = 0;
    }

    if (umesh->nb_som_tables > 0)
    {
        for (i = 0; i < umesh->nb_som_tables; i++)
        {
            if (umesh->som_tables[i].path != NULL)
                free(umesh->som_tables[i].path);
            switch (umesh->som_tables[i].type)
            {
            case SOM_POINT_IN_ELEMENT:
                if (umesh->som_tables[i].data.pie.nb_points > 0)
                {
                    free(umesh->som_tables[i].data.pie.indices);
                    free(*(umesh->som_tables[i].data.pie.vectors));
                    free(umesh->som_tables[i].data.pie.vectors);
                }
                break;
            case SOM_EDGE:
                if (umesh->som_tables[i].data.ef.dims[0] > 0)
                    free(umesh->som_tables[i].data.ef.items);
                break;
            case SOM_FACE:
                if (umesh->som_tables[i].data.ef.dims[0] > 0)
                    free(umesh->som_tables[i].data.ef.items);
                break;
            default:
                break;
            }
        }
        free(umesh->som_tables);
        umesh->nb_som_tables = 0;
    }
}


// Free memory used by mesh instance
void free_msh_instance (msh_instance_t *msh_instance)
{
    if (msh_instance->path != NULL)
    {
        free(msh_instance->path);
        msh_instance->path = NULL;
    }

    switch (msh_instance->type)
    {
    case MSH_STRUCTURED:
        free_smesh(&(msh_instance->data.structured));
        break;
    case MSH_UNSTRUCTURED:
        free_umesh(&(msh_instance->data.unstructured));
        break;
    default:
        break;
    }
}


void free_mlk_instance (mlk_instance_t *mlk_instance)
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
void free_msh_group (msh_group_t *msh_group)
{
    hsize_t i;

    if (msh_group->path != NULL)
    {
        free(msh_group->path);
        msh_group->path = NULL;
    }

    if (msh_group->nb_msh_instances > 0)
    {
        for (i = 0; i < msh_group->nb_msh_instances; i++)
            free_msh_instance(msh_group->msh_instances + i);
        free(msh_group->msh_instances);
        msh_group->msh_instances = NULL;
        msh_group->nb_msh_instances = 0;
    }
    if (msh_group->nb_mlk_instances > 0)
    {
        for (i = 0; i < msh_group->nb_mlk_instances; i++)
            free_mlk_instance(msh_group->mlk_instances + i);
        free(msh_group->mlk_instances);
        msh_group->mlk_instances = NULL;
        msh_group->nb_mlk_instances = 0;
    }
}


// Free memory used by mesh category
void free_mesh (mesh_t *mesh)
{
    hsize_t i;

    if (mesh->nb_groups > 0)
    {
        for (i = 0; i < mesh->nb_groups; i++)
            free_msh_group(mesh->groups + i);
        free(mesh->groups);
        mesh->groups = NULL;
        mesh->nb_groups = 0;
    }
}

