#include <hdf5.h>
#include <hdf5_hl.h>
#include <stdio.h>
#include <stdlib.h>



#define A_SUBJECT "subject"
#define A_OBJECT "object"
#define C_LINK "/link/"
#define C_OUTPUT_REQUEST "/outputRequest/"

#define DATA_ON_MESH 1

typedef struct
{
	char * name;
	char * subject;
	char * object;
} link_t;

link_t read_link(hid_t loc_id, char * path);
int get_type_link(link_t link);
int isdataonmesh(link_t link);
