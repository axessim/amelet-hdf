#ifndef AMELETHDF_H
#define AMELETHDF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <hdf5.h>
#include <hdf5_hl.h>
#include <complex.h>

#include "category.h"

#define ABSOLUTE_PATH_NAME_LENGTH 101
#define ELEMENT_NAME_LENGTH 21
#define ATTRIBUTE_LENGTH 31
#define TABLE_FIELD_NAME_LENGTH 31

#define TRUE 1
#define FALSE 0

    typedef struct children_t
    {
        char        **childnames;
        hsize_t     nb_children;
    } children_t;

    typedef struct set_t
    {
        char        **values;
        hsize_t     nb_values;
    } set_t;

    typedef union value_t
    {
        int         i;
        float       f;
        complex float c;
        char        *s;
    } value_t;

    typedef struct attribute_instance_t
    {
        char        *name;
        H5T_class_t type;
        value_t     value;
    } attribute_instance_t;

    typedef struct optional_attributes_t
    {
        hsize_t     nb_attribute_instances;
        attribute_instance_t *attribute_instances;
    } optional_attributes_t;

    set_t add_to_set (set_t aset, char *aelement);
    int index_in_set (set_t aset, char *aelement, hsize_t *index);
    children_t read_children_name (hid_t file_id, const char *path);
    char *get_name_from_path (const char *path);

    char read_string_attribute (hid_t file_id, const char *path, char *attr_name, char **rdata);
    char read_float_attribute (hid_t file_id, const char *path, char* attr_name, float *rdata);
    char read_int_attribute (hid_t file_id, const char *path, char* attr, int *rdata);

    char read_string_dataset (hid_t file_id, const char *path, hsize_t mn, size_t length, char ***rdata);
    char read_int_dataset (hid_t file_id, const char *path, const hsize_t mn, int **rdata);
    char read_float_dataset (hid_t file_id, const char *path, const hsize_t mn, float **rdata);
    char read_complex_dataset (hid_t file_id, const char *path, const hsize_t mn, complex float **rdata);

    char read_optional_attributes (hid_t file_id, const char *path, optional_attributes_t *optional_attributes, char mandatory_attributes[][ATTRIBUTE_LENGTH], size_t nb_mandatory_attributes);
    void print_optional_attributes (optional_attributes_t optional_attributes);
    void free_optional_attributes(optional_attributes_t *optional_attributes);

#ifdef __cplusplus
}
#endif

#endif // AMELETHDF_H

