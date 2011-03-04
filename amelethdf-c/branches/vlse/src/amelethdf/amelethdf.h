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

#define ABSOLUTE_PATH_NAME_LENGTH 101  // length in C (incl. '\0')
#define ELEMENT_NAME_LENGTH 21 // length in C (incl. '\0')
#define ATTR_LENGTH 31
#define TABLE_FIELD_NAME_LENGTH 31

#define V_INVALID "INVALID"

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

    typedef struct attr_instance_t
    {
        char        *name;
        H5T_class_t type;
        value_t     value;
    } attr_instance_t;

    typedef struct opt_attrs_t
    {
        hsize_t     nb_instances;
        attr_instance_t *instances;
    } opt_attrs_t;

    char version_minimum (const char *required_version, const char *sim_version);
    char *trim_zeros (const char *version);
    set_t add_to_set (set_t aset, char *aelement);
    int index_in_set (set_t aset, char *aelement, hsize_t *index);
    children_t read_children_name (hid_t file_id, const char *path);
    char *get_name_from_path (const char *path);
    char *get_base_from_path (const char *path);

    char read_int_attr (hid_t file_id, const char *path, char* attr, int *rdata);
    char read_flt_attr (hid_t file_id, const char *path, char* attr_name, float *rdata);
    char read_cpx_attr (hid_t file_id, const char* path, char* attr_name, complex float *rdata);
    char read_str_attr (hid_t file_id, const char *path, char *attr_name, char **rdata);

    void print_int_attr (char *name, int value, int space);
    void print_flt_attr (char *name, float value, int space);
    void print_cpx_attr (char *name, complex float value, int space);
    void print_str_attr (char *name, char *value, int space);

    char read_int_dataset (hid_t file_id, const char *path, const hsize_t mn, int **rdata);
    char read_float_dataset (hid_t file_id, const char *path, const hsize_t mn, float **rdata);
    char read_complex_dataset (hid_t file_id, const char *path, const hsize_t mn, complex float **rdata);
    char read_string_dataset (hid_t file_id, const char *path, hsize_t mn, size_t length, char ***rdata);

    char read_opt_attrs (hid_t file_id, const char *path, opt_attrs_t *opt_attrs, char mandatory_attrs[][ATTR_LENGTH], size_t nb_mandatory_attrs);
    void print_opt_attrs (opt_attrs_t opt_attrs, int space);
    void free_opt_attrs(opt_attrs_t *opt_attrs);

    void print_err_dset (const char *category, const char *path);
    void print_err_attr (const char *category, const char *path, const char *attr_name);
    void print_wrn_attr (const char *category, const char *path, const char *attr_name);
#ifdef __cplusplus
}
#endif

#endif // AMELETHDF_H

