#ifndef AMELETHDF_H
#define AMELETHDF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <hdf5.h>
#include <hdf5_hl.h>
#include "ah5_category.h"

#ifndef _MSC_VER
	#include <complex.h>
	#define AH5_complex_t complex float
#else
    typedef struct _AH5_complex_t
	{
        float			re;
		float			im;
    } AH5_complex_t;

	#define creal(z) ((z).re)
	#define cimag(z) ((z).im)
#endif /*__MSC_VER__*/


    typedef struct _AH5_children_t
    {
        char            **childnames;
        hsize_t         nb_children;
    } AH5_children_t;

    typedef struct _AH5_set_t
    {
        char            **values;
        hsize_t         nb_values;
    } AH5_set_t;

    typedef union _AH5_value_t
    {
        int             i;
        float           f;
        AH5_complex_t   c;
        char            *s;
    } AH5_value_t;

    typedef struct _AH5_attr_instance_t
    {
        char            *name;
        H5T_class_t     type;
        AH5_value_t     value;
    } AH5_attr_instance_t;

    typedef struct _AH5_opt_attrs_t
    {
        hsize_t         nb_instances;
        AH5_attr_instance_t *instances;
    } AH5_opt_attrs_t;

    char AH5_version_minimum (const char *required_version, const char *sim_version);
    char *AH5_trim_zeros (const char *version);
    char AH5_path_valid (hid_t file_id, const char *path);
    AH5_set_t AH5_add_to_set (AH5_set_t aset, char *aelement);
    int AH5_index_in_set (AH5_set_t aset, char *aelement, hsize_t *index);
    AH5_children_t AH5_read_children_name (hid_t file_id, const char *path);

    char *AH5_get_name_from_path (const char *path);
    char *AH5_get_base_from_path (const char *path);

    char AH5_read_int_attr (hid_t file_id, const char *path, char* attr, int *rdata);
    char AH5_read_flt_attr (hid_t file_id, const char *path, char* attr_name, float *rdata);
    char AH5_read_cpx_attr (hid_t file_id, const char* path, char* attr_name, AH5_complex_t *rdata);
    char AH5_read_str_attr (hid_t file_id, const char *path, char *attr_name, char **rdata);

    void AH5_print_int_attr (char *name, int value, int space);
    void AH5_print_flt_attr (char *name, float value, int space);
    void AH5_print_cpx_attr (char *name, AH5_complex_t value, int space);
    void AH5_print_str_attr (char *name, char *value, int space);

    char AH5_read_int_dataset (hid_t file_id, const char *path, const hsize_t mn, int **rdata);
    char AH5_read_float_dataset (hid_t file_id, const char *path, const hsize_t mn, float **rdata);
    char AH5_read_complex_dataset (hid_t file_id, const char *path, const hsize_t mn, AH5_complex_t **rdata);
    char AH5_read_string_dataset (hid_t file_id, const char *path, hsize_t mn, size_t length, char ***rdata);

    char AH5_read_opt_attrs (hid_t file_id, const char *path, AH5_opt_attrs_t *opt_attrs, char mandatory_attrs[][AH5_ATTR_LENGTH], size_t nb_mandatory_attrs);
    void AH5_print_opt_attrs (const AH5_opt_attrs_t *opt_attrs, int space);
    void AH5_free_opt_attrs (AH5_opt_attrs_t *opt_attrs);

    void AH5_print_err_dset (const char *category, const char *path);
    void AH5_print_err_tble (const char *category, const char *path);
    void AH5_print_err_attr (const char *category, const char *path, const char *attr_name);
    void AH5_print_err_path (const char *category, const char *path);
    void AH5_print_err_inv_attr (const char *category, const char *path, const char *attr_name);
    void AH5_print_wrn_attr (const char *category, const char *path, const char *attr_name);

#ifdef __cplusplus
}
#endif

#endif // AMELETHDF_H

