#include "amelethdf.h"

#ifndef HDFPATH_H
#define HDFPATH_H

typedef struct
{
        int nb;
        char ** chartab;
} stringtab_t;

char* path_basename(char *path);
char* path_dirname(char *path);
int path_exists(hid_t loc_id, char* path);
int path_isabs(char *path);
H5I_type_t gettype(hid_t loc_id, char* path);
herr_t closetype(hid_t obj_id);
int path_isleaf(hid_t loc_id, char* path);
int path_isgroup(hid_t loc_id, char* path);
char * remove_sep(char *path);
char* path_join(char **paths, int nbpath);
char** rsplit(char *path);
char** lsplit(char *path);
char * path_head(char *path);
int path_contain(char *path, char *subpath);
char * path_element(char *path, int index, int back);
stringtab_t path_split(char *path);
int path_like(char * path, char* pattern);

#endif // HDFPATH_H
