#include <hdf5.h>
#include <hdf5_hl.h>
#include <string.h>
#include "amelethdf.h"

char ** read_string_dataset2(hid_t file_id, const char * path, int l, int nb);
char *** read_string_dataset1(hid_t file_id, const char * path, int l, int m, int n);
int *get_dataset_lmn(hid_t file_id, char* path);
children_t read_string_vector(hid_t file_id, char* path);
int str_index(char ** arr, char *aelement, int nbel);
void write_nd_dataset(hid_t file_id, char* path, int values_len,
					  int rank, hsize_t* dims, char* values[]);
