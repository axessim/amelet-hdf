#include <hdf5.h>
#include <string.h>
#include "simpletype.h"
#include "complextype.h"





vector_t read_vector(hid_t loc_id, const char* path);
char * vector_to_string(vector_t vec);
herr_t write_vector(hid_t loc_id, const char* path, vector_t vec);
vector_t clear_content_vector(vector_t vec);
