#ifndef MESH_H
#define MESH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"

#define A_TYPE              "type"
#define A_ENTITY_TYPE       "entityType"
#define A_MESH1             "mesh1"
#define A_MESH2             "mesh2"

#define G_NODES             "/nodes"
#define G_ELEMENT_TYPES     "/elementTypes"
#define G_ELEMENT_NODES     "/elementNodes"
#define G_GROUP             "/group"
#define G_GROUPGROUP        "/groupGroup"
#define G_SELECTOR_ON_MESH  "/selectorOnMesh"
#define G_CARTESIAN_GRID    "/cartesianGrid"
#define G_NORMAL            "/normal"
#define G_X                 "/x"
#define G_Y                 "/y"
#define G_Z                 "/z"
#define G_MESH_LINK         "/meshLink"

#define V_POINT_IN_ELEMENT  "pointInElement"
#define V_NODE              "node"
#define V_EDGE              "edge"
#define V_FACE              "face"
#define V_VOLUME            "volume"
#define V_UNSTRUCTURED      "unstructured"
#define V_STRUCTURED        "structured"


    typedef enum mesh_class_t {
        MSH_INVALID             = -1,
        MSH_STRUCTURED          = 1,
        MSH_UNSTRUCTURED        = 2
    } mesh_class_t;


    typedef struct groupgroup_t
    {
        char*       name;
        hsize_t     nb_groupgroupnames;
        char**      groupgroupnames;
    } groupgroup_t;


    typedef struct ssom_pie_table_t
    {
        char        *name;
        char        nb_dims;
        hsize_t     nb_points;
        unsigned int **elements;
        float       **vectors;
    } ssom_pie_table_t;


    typedef struct axis_t
    {
        hsize_t     nb_nodes;
        float       *nodes;
    } axis_t;


    typedef struct sgroup_t
    {
        char        *name;
        char        *type;
        char        *entitytype;
        char        nb_dims;
        hsize_t     nb_elements;
        unsigned int **elements;
        char        **normals;
    } sgroup_t;


    typedef struct smesh_t
    {
        axis_t      x;
        axis_t      y;
        axis_t      z;
        hsize_t     nb_groups;
        sgroup_t    *groups;
        hsize_t     nb_groupgroups;
        groupgroup_t *groupgroups;
        hsize_t     nb_som_tables;
        ssom_pie_table_t *som_tables;
    } smesh_t;




    typedef struct ugroup_t
    {
        char        *name;
        hsize_t     nb_groupelts;
        int         *groupelts;
    } ugroup_t;


    typedef enum usom_class_t
    {
        SOM_INVALID             = -1,
        SOM_POINT_IN_ELEMENT    = 1,
        SOM_EDGE                = 2,
        SOM_FACE                = 3
    } usom_class_t;


    typedef struct usom_pie_table_t
    {
        char        nb_dims;
        hsize_t     nb_points;
        int         *indices;
        float       **vectors;
    } usom_pie_table_t;


    typedef struct usom_ef_table_t
    {
        hsize_t     nb_items;
        int         **items;
    } usom_ef_table_t;


    typedef union usomx_t
    {
        usom_pie_table_t pie;
        usom_ef_table_t ef;
    } usomx_t;


    typedef struct usom_table_t
    {
        char        *name;
        usom_class_t type;
        usomx_t     data;
    } usom_table_t;


    typedef struct umesh_t
    {
        hsize_t     nb_elementnodes;
        int         *elementnodes;
        hsize_t     nb_elementtypes;
        char        *elementtypes;
        hsize_t     nb_nodes[2];
        float       **nodes;
        hsize_t     nb_groups;
        ugroup_t    *groups;
        hsize_t     nb_groupgroups;
        groupgroup_t *groupgroups;
        hsize_t     nb_som_tables;
        usom_table_t *som_tables;
    } umesh_t;


    typedef union meshx_t
    {
        smesh_t     structured;
        umesh_t     unstructured;
    } meshx_t;


    typedef struct mesh_instance_t
    {
        char        *name;
        mesh_class_t type;
        meshx_t     data;
    } mesh_instance_t;


    typedef enum meshlink_class_t {
        MSHLNK_INVALID          = -1,
        MSHLNK_NODE             = 1,
        MSHLNK_EDGE             = 2,
        MSHLNK_FACE             = 3,
        MSHLNK_VOLUME           = 4
    } meshlink_class_t;


    typedef struct meshlink_instance_t
    {
        char        *name;
        meshlink_class_t type;
        char        *mesh1;
        char        *mesh2;
        hsize_t     dims[2];
        int         *data;
    } meshlink_instance_t;


    typedef struct mesh_group_t
    {
        char        *name;
        hsize_t     nb_mesh_instances;
        mesh_instance_t *mesh_instances;
        hsize_t     nb_meshlink_instances;
        meshlink_instance_t *meshlink_instances;
    } mesh_group_t;


    typedef struct mesh_t
    {
        hsize_t     nb_mesh_groups;
        mesh_group_t *mesh_groups;
    } mesh_t;


    void read_groupgroup (hid_t file_id, const char* path, groupgroup_t *groupgroup);
    void read_smesh_axis (hid_t file_id, const char* path, axis_t *axis);
    void read_smesh_group (hid_t file_id, const char *path, sgroup_t *sgroup);
    void read_ssom_pie_table (hid_t file_id, const char *path, ssom_pie_table_t *ssom_pie_table);
    void read_smesh (hid_t file_id, const char* path, smesh_t *smesh);
    void read_umesh_group (hid_t file_id, const char* path, ugroup_t *ugroup);
    void read_usom_pie_table (hid_t file_id, const char *path, usom_pie_table_t *usom_pie_table);
    void read_usom_ef_table (hid_t file_id, const char *path, usom_ef_table_t *usom_ef_table);
    void read_umesh_som_table (hid_t file_id, const char *path, usom_table_t *usom_table);
    char read_umesh (hid_t file_id, const char* path, umesh_t *umesh);
    void read_mesh_instance (hid_t file_id, const char *path, mesh_instance_t *mesh_instance);
    void read_meshlink_instance (hid_t file_id, const char *path, meshlink_instance_t *meshlink_instance);
    void read_mesh_group (hid_t file_id, const char *path, mesh_group_t *mesh_group);
    void read_mesh (hid_t file_id, mesh_t *mesh);

    void print_smesh (smesh_t smesh);
    void print_umesh_som_table (usom_table_t usom_table);
    void print_umesh (umesh_t umesh);
    void print_mesh_instance (mesh_instance_t mesh_instance);
    void print_meshlink_instance (meshlink_instance_t meshlink_instance);
    void print_mesh_group (mesh_group_t mesh_group);
    void print_mesh (mesh_t mesh);

    void free_groupgroup (groupgroup_t *groupgroup);
    void free_smesh (smesh_t *smesh);
    void free_umesh (umesh_t *umesh);
    void free_mesh_instance (mesh_instance_t *mesh_instance);
    void free_meshlink_instance (meshlink_instance_t *meshlink_instance);
    void free_mesh_group (mesh_group_t *mesh_group);
    void free_mesh (mesh_t *mesh);


#ifdef __cplusplus
}
#endif

#endif // MESH_H
