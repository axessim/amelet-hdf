#include <hdf5.h>
#include <unstructuredmesh.h>
#include <structuredmesh.h>

#define STRUCTURED            0
#define UNSTRUCTURED          1
#define C_MESH             "/mesh"
#define NODES              "/nodes"
#define ELEMENTS           "/elementTypes"
#define ELEMENT_NODES      "/elementNodes"
#define GROUP              "/group"
#define GROUPGROUP         "/groupGroup"
#define SELECTOR_ON_MESH   "/selectorOnMesh"
#define L_ELEMENTS         "/elements"
#define G_CARTESIAN_GRID   "/cartesianGrid"
#define G_NORMAL           "/normal"
#define D_X                "/x"
#define D_Y                "/y"
#define D_Z                "/z"

typedef struct
{
	char** groupGroupnames;
	int nbeltgroupGroup;
	char* name;
} groupgroup_t;

typedef struct
{
	groupgroup_t *groupgroups;
	int nbgroupGroup;
} groupgroups_t;


typedef struct
{
	ugroup_t* groups;
	int nbgroup;
}ugroups_t;

typedef struct
{
	sgroup_t* groups;
	int nbgroup;
}sgroups_t;

typedef struct
{
	char * name;
	nodes_t nodes;
	elttypes_t elements;
	eltnodes_t element_nodes;
	ugroups_t groups;
	groupgroups_t groupgroups;
	semnodes_t som_nodes;
	semelts_t som_elements;
}unstructured_mesh_t;

typedef struct
{
	char * name;
	axis_t x;
	axis_t y;
	axis_t z;
	sgroups_t groups;
	groupgroups_t groupgroups;
}structured_mesh_t;

int meshtype(hid_t loc_id, const char * path);
groupgroup_t readGroupGroup(hid_t grpgrp_id, const char* name);
unstructured_mesh_t read_unstructured_mesh(hid_t file_id, const char* path);
void print_unstructured_mesh(unstructured_mesh_t umesh);
structured_mesh_t read_structured_mesh(hid_t file_id, const char* path);
void print_structured_mesh(structured_mesh_t smesh);
