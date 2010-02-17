#include "amelethdf.h"

#ifndef UNSTRUCTUREDMESH_H
#define UNSTRUCTUREDMESH_H

typedef struct
{
        float ** nodes;
        int nbnodes;
} nodes_t;

typedef struct
{
        int * elttypes;
        int nbelttypes;
} elttypes_t;

typedef struct
{
        int * eltnodes;
        int nbeltnodes;
} eltnodes_t;

typedef struct
{
        char shortname[30];
        int index;
        float v1;
        float v2;
        float v3;
} semelt_t;

typedef struct
{
        semelt_t *somelt;
        int nbsomelt;
} semelts_t;

typedef struct
{
        char shortname[30];
        int index;
} semnode_t;

typedef struct
{
        semnode_t *somnodes;
        int nbsomnodes;
} semnodes_t;

typedef struct
{
        int* eltgroup;
        int nbeltgroup;
        char* name;
} ugroup_t;

nodes_t readNodes(hid_t node_id);
elttypes_t readElementTypes(hid_t eltypes_id);
eltnodes_t readElementNodes(hid_t elnodes_id);
semelts_t readSemElt(hid_t file_id, const char *table_name);
semnodes_t readSemNodes(hid_t file_id, const char *table_name);
ugroup_t readUGroup(hid_t group_id, const char* name);

#endif // UNSTRUCTUREDMESH_H
