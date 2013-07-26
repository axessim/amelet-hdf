#ifndef AH5_C_MESH_H
#define AH5_C_MESH_H

#include "ah5_general.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _AH5_groupgroup_t
    {
        char            *path;
        hsize_t         nb_groupgroupnames;
        char            **groupgroupnames;
    } AH5_groupgroup_t;

    typedef struct _AH5_ssom_pie_table_t
    {
        char            *path;
        char            nb_dims;
        hsize_t         nb_points;
        unsigned int    **elements;
        float           **vectors;
    } AH5_ssom_pie_table_t;

    typedef struct _AH5_axis_t
    {
        hsize_t         nb_nodes;
        float           *nodes;
    } AH5_axis_t;

    typedef struct _AH5_sgroup_t
    {
        char            *path;
        char            *type;
        char            *entitytype;
        hsize_t         dims[2];
        int             *elements;
        char            **normals;
    } AH5_sgroup_t;

    typedef struct _AH5_smesh_t
    {
        AH5_axis_t      x;
        AH5_axis_t      y;
        AH5_axis_t      z;
        hsize_t         nb_groups;
        AH5_sgroup_t    *groups;
        hsize_t         nb_groupgroups;
        AH5_groupgroup_t *groupgroups;
        hsize_t         nb_som_tables;
        AH5_ssom_pie_table_t *som_tables;
    } AH5_smesh_t;

    typedef struct _AH5_ugroup_t
    {
        char            *path;
        char            *type;
        char            *entitytype;
        hsize_t         nb_groupelts;
        int             *groupelts;
    } AH5_ugroup_t;

    typedef enum _AH5_usom_class_t
    {
        SOM_INVALID             = -1,
        SOM_POINT_IN_ELEMENT    = 1,
        SOM_EDGE                = 2,
        SOM_FACE                = 3
    } AH5_usom_class_t;

    typedef struct _AH5_usom_pie_table_t
    {
        char            nb_dims;
        hsize_t         nb_points;
        int             *indices;
        float           **vectors;
    } AH5_usom_pie_table_t;

    typedef struct _AH5_usom_ef_table_t
    {
        hsize_t         dims[2];
        int             *items;
    } AH5_usom_ef_table_t;

    typedef union _AH5_usomx_t
    {
        AH5_usom_pie_table_t pie;
        AH5_usom_ef_table_t ef;
    } AH5_usomx_t;

    typedef struct _AH5_usom_table_t
    {
        char            *path;
        AH5_usom_class_t type;
        AH5_usomx_t     data;
    } AH5_usom_table_t;

    typedef struct _AH5_umesh_t
    {
        hsize_t         nb_elementnodes;
        int             *elementnodes;
        hsize_t         nb_elementtypes;
        char            *elementtypes;
        hsize_t         nb_nodes[2];
        float           *nodes;
        hsize_t         nb_groups;
        AH5_ugroup_t    *groups;
        hsize_t         nb_groupgroups;
        AH5_groupgroup_t *groupgroups;
        hsize_t         nb_som_tables;
        AH5_usom_table_t *som_tables;
    } AH5_umesh_t;

    typedef enum _AH5_mesh_class_t {
        MSH_INVALID             = -1,
        MSH_STRUCTURED          = 1,
        MSH_UNSTRUCTURED        = 2
    } AH5_mesh_class_t;

    typedef union _AH5_meshx_t
    {
        AH5_smesh_t     structured;
        AH5_umesh_t     unstructured;
    } AH5_meshx_t;

    typedef struct _AH5_msh_instance_t
    {
        char            *path;
        AH5_mesh_class_t type;
        AH5_meshx_t     data;
    } AH5_msh_instance_t;

    typedef enum _AH5_meshlink_class_t {
        MSHLNK_INVALID          = -1,
        MSHLNK_NODE             = 1,
        MSHLNK_EDGE             = 2,
        MSHLNK_FACE             = 3,
        MSHLNK_VOLUME           = 4
    } AH5_meshlink_class_t;

    typedef struct _AH5_mlk_instance_t
    {
        char            *path;
        AH5_meshlink_class_t type;
        char            *mesh1;
        char            *mesh2;
        hsize_t         dims[2];
        int             *data;
    } AH5_mlk_instance_t;

    typedef struct _AH5_msh_group_t
    {
        char            *path;
        hsize_t         nb_msh_instances;
        AH5_msh_instance_t *msh_instances;
        hsize_t         nb_mlk_instances;
        AH5_mlk_instance_t *mlk_instances;
    } AH5_msh_group_t;

    typedef struct _AH5_mesh_t
    {
        hsize_t         nb_groups;
        AH5_msh_group_t *groups;
    } AH5_mesh_t;

    AH5_PUBLIC void AH5_init_msh_group(AH5_msh_group_t *msh_group);
    AH5_PUBLIC void AH5_init_mesh(AH5_mesh_t *mesh);

    AH5_PUBLIC char AH5_read_groupgroup(hid_t file_id, const char* path, AH5_groupgroup_t *groupgroup);
    AH5_PUBLIC char AH5_read_smesh_axis(hid_t file_id, const char* path, AH5_axis_t *axis);
    AH5_PUBLIC char AH5_read_smsh_group(hid_t file_id, const char *path, AH5_sgroup_t *sgroup);
    AH5_PUBLIC char AH5_read_ssom_pie_table(hid_t file_id, const char *path, AH5_ssom_pie_table_t *ssom_pie_table);
    AH5_PUBLIC char AH5_read_smesh(hid_t file_id, const char* path, AH5_smesh_t *smesh);
    AH5_PUBLIC char AH5_read_umsh_group(hid_t file_id, const char* path, AH5_ugroup_t *ugroup);
    AH5_PUBLIC char AH5_read_usom_pie_table(hid_t file_id, const char *path, AH5_usom_pie_table_t *usom_pie_table);
    AH5_PUBLIC char AH5_read_usom_ef_table(hid_t file_id, const char *path, AH5_usom_ef_table_t *usom_ef_table);
    AH5_PUBLIC char AH5_read_umesh_som_table(hid_t file_id, const char *path, AH5_usom_table_t *usom_table);
    AH5_PUBLIC char AH5_read_umesh(hid_t file_id, const char* path, AH5_umesh_t *umesh);
    AH5_PUBLIC char AH5_read_msh_instance(hid_t file_id, const char *path, AH5_msh_instance_t *msh_instance);
    AH5_PUBLIC char AH5_read_mlk_instance(hid_t file_id, const char *path, AH5_mlk_instance_t *mlk_instance);
    AH5_PUBLIC char AH5_read_msh_group(hid_t file_id, const char *path, AH5_msh_group_t *msh_group);
    AH5_PUBLIC char AH5_read_mesh(hid_t file_id, AH5_mesh_t *mesh);

    // General notes about write functions.
    //
    // The data can be relatively named (data.path = the node name not the full path).
    // In this case the 'file_id' must be the direct parent node.
    //
    // The 'file_id' can be the root node but in this case the furst data node's path must by the full node path.
    //
    AH5_PUBLIC char AH5_write_groupgroup(hid_t file_id, const AH5_groupgroup_t *groupgroup, int nb);
    AH5_PUBLIC char AH5_write_smesh_axis(hid_t file_id, const AH5_axis_t *axis);
    AH5_PUBLIC char AH5_write_smsh_group(hid_t file_id, const AH5_sgroup_t *sgroup);
    AH5_PUBLIC char AH5_write_ssom_pie_table(hid_t file_id, const AH5_ssom_pie_table_t *ssom_pie_table);
    AH5_PUBLIC char AH5_write_smesh(hid_t file_id, const AH5_smesh_t *smesh);
    AH5_PUBLIC char AH5_write_umsh_group(hid_t file_id, const AH5_ugroup_t *ugroup, int nb_grp);
    AH5_PUBLIC char AH5_write_usom_pie_table(hid_t file_id, const AH5_usom_pie_table_t *usom_pie_table);
    AH5_PUBLIC char AH5_write_usom_ef_table(hid_t file_id, const AH5_usom_ef_table_t *usom_ef_table);
    AH5_PUBLIC char AH5_write_umesh_som_table(hid_t file_id, const AH5_usom_table_t *usom_table, int nb_som);
    AH5_PUBLIC char AH5_write_umesh(hid_t file_id, const AH5_umesh_t *umesh);
    AH5_PUBLIC char AH5_write_msh_instance(hid_t file_id, const AH5_msh_instance_t *msh_instance);
    AH5_PUBLIC char AH5_write_mlk_instance(hid_t file_id, const AH5_mlk_instance_t *mlk_instance);
    AH5_PUBLIC char AH5_write_msh_group(hid_t file_id, const AH5_msh_group_t *msh_group);
    AH5_PUBLIC char AH5_write_mesh(hid_t file_id, const AH5_mesh_t *mesh);

    AH5_PUBLIC void AH5_print_smesh(const AH5_smesh_t *smesh, int space);
    AH5_PUBLIC void AH5_print_umesh_som_table(const AH5_usom_table_t *usom_table, int space);
    AH5_PUBLIC void AH5_print_umesh(const AH5_umesh_t *umesh, int space);
    AH5_PUBLIC void AH5_print_msh_instance(const AH5_msh_instance_t *msh_instance, int space);
    AH5_PUBLIC void AH5_print_mlk_instance(const AH5_mlk_instance_t *mlk_instance, int space);
    AH5_PUBLIC void AH5_print_msh_group(const AH5_msh_group_t *msh_group, int space);
    AH5_PUBLIC void AH5_print_mesh(const AH5_mesh_t *mesh);

    AH5_PUBLIC void AH5_free_groupgroup(AH5_groupgroup_t *groupgroup);
    AH5_PUBLIC void AH5_free_smesh(AH5_smesh_t *smesh);
    AH5_PUBLIC void AH5_free_umesh(AH5_umesh_t *umesh);
    AH5_PUBLIC void AH5_free_msh_instance(AH5_msh_instance_t *msh_instance);
    AH5_PUBLIC void AH5_free_mlk_instance(AH5_mlk_instance_t *mlk_instance);
    AH5_PUBLIC void AH5_free_msh_group(AH5_msh_group_t *msh_group);
    AH5_PUBLIC void AH5_free_mesh(AH5_mesh_t *mesh);

#ifdef __cplusplus
}
#endif

#endif // AH5_C_MESH_H
