#include <complex.h>

#include "amelethdf.h"

#ifndef COMPLEXTYPE_H
#define COMPLEXTYPE_H

hid_t create_type_id(hid_t real_or_double);
hid_t create_real_type_id();
hid_t create_double_type_id();
herr_t write_complex_type(hid_t loc_id);
herr_t create_complex_attribute(hid_t loc_id, const char* path,
        const char* name, complex float value);
complex float read_complex_attribute(hid_t loc_id, const char* path,
        const char* name);
complex float *read_complex_dataset(hid_t loc_id, const char* path);
herr_t write_complexe_1D_dataset(hid_t loc_id, const char* path,
        complex float* values, int nbvalues);

#endif // COMPLEXTYPE_H
