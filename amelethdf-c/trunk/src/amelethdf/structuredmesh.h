#include <hdf5.h>

typedef struct
{
        float * nodes;
        int nbnodes;
} axis_t;

typedef struct
{
        int *imin;
        int *jmin;
        int *kmin;
        int *imax;
        int *jmax;
        int *kmax;
        int nbelt;
        char *type;
        char *entityType;
        char *name;
} sgroup_t;

typedef struct
{
        char ** normals;
        int nbelt;
} snorm_t;

axis_t readAxis(hid_t node_id, const char *axisname);
sgroup_t readSGroup(hid_t group_id, const char* name);
snorm_t readNormals(hid_t loc_id);
char* readAttributes(hid_t group_id, const char* attr_name);

