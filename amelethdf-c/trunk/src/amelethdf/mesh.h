#include "amelethdf.h"

#ifndef MESH_H
#define MESH_H

#define STRUCTURED         0
#define UNSTRUCTURED       1
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

int meshtype(hid_t loc_id, const char * path);
groupgroup_t readGroupGroup(hid_t grpgrp_id, const char* name);

#endif // MESH_H
