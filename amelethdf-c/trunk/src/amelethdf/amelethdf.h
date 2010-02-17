#include <hdf5.h>
#include <hdf5_hl.h>

#ifndef AMELETHDF_H
#define AMELETHDF_H

#define ABSOLUTE_PATH_NAME_LENGTH 100
#define ELEMENT_NAME_LENGTH 30

#define TRUE 1
#define FALSE 0

typedef struct
{
    char ** childnames;
    int nbchild;
} children_t;


/*
 * Returns the number of children of a group at path
 */
int read_number_of_children(hid_t file_id, const char* path);

/*
 * Returns children of a group at path
 */
children_t read_children_name(hid_t loc_id, const char* path);

/*
 * Reads a string attribute at path
 */
char * read_string_attribute(hid_t loc_id, const char* path, char* attr);

/*
 * Reads a float attribute at path
 */
float read_float_attribute(hid_t loc_id, const char* path, char* attr);

#endif // AMELETHDF_H

