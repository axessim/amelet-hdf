#include "simpletype.h"

#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
        single_t single;
        int* ivalue;
        float* rvalue;
        _Complex float* cvalue;
        char ** svalue;
        int nbvalues;
} vector_t;

vector_t read_vector(hid_t loc_id, const char* path);
char * vector_to_string(vector_t vec);
herr_t write_vector(hid_t loc_id, const char* path, vector_t vec);
vector_t clear_content_vector(vector_t vec);

#endif // VECTOR_H
