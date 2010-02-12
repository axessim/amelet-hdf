#include <hdf5.h>
#include <hdf5_hl.h>

#ifndef AMELETHDF_H
#define AMELETHDF_H

#define ABSOLUTE_PATH_NAME_LENGTH 100
#define ELEMENT_NAME_LENGTH 30

typedef struct
{
    char ** childnames;
    int nbchild;
} children_t;

children_t read_children_name(hid_t loc_id, const char* path);
char * read_string_attribute(hid_t loc_id, const char* path, char* attr);
float read_float_attribute(hid_t loc_id, const char* path, char* attr);
int read_number_of_children(hid_t file_id, const char* path);

#endif
