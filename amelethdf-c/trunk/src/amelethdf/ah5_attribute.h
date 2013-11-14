#ifndef AH5_ATTRIBUTE_H
#define AH5_ATTRIBUTE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ah5_general.h"

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

    AH5_PUBLIC char AH5_read_int_attr(hid_t file_id, const char *path, char *attr, int *rdata);
    AH5_PUBLIC char AH5_read_flt_attr(hid_t file_id, const char *path, char *attr_name, float *rdata);
    AH5_PUBLIC char AH5_read_cpx_attr(hid_t file_id, const char *path, char *attr_name, AH5_complex_t *rdata);
    AH5_PUBLIC char AH5_read_str_attr(hid_t file_id, const char *path, char *attr_name, char **rdata);
    AH5_PUBLIC char AH5_read_opt_attrs(hid_t file_id, const char *path, AH5_opt_attrs_t *opt_attrs, char mandatory_attrs[][AH5_ATTR_LENGTH], size_t nb_mandatory_attrs);

    AH5_PUBLIC char AH5_write_int_attr(hid_t file_id, const char *path, char *attr_name, const int wdata);
    AH5_PUBLIC char AH5_write_flt_attr(hid_t file_id, const char *path, char *attr_name, const float wdata);
    AH5_PUBLIC char AH5_write_cpx_attr(hid_t file_id, const char *path, char *attr_name, const AH5_complex_t *wdata);
    AH5_PUBLIC char AH5_write_str_attr(hid_t file_id, const char *path, char *attr_name, const char *wdata);
    AH5_PUBLIC char AH5_write_opt_attrs(hid_t file_id, const char *path, AH5_opt_attrs_t *opt_attrs);

    AH5_PUBLIC void AH5_print_int_attr(char *name, int value, int space);
    AH5_PUBLIC void AH5_print_flt_attr(char *name, float value, int space);
    AH5_PUBLIC void AH5_print_cpx_attr(char *name, AH5_complex_t value, int space);
    AH5_PUBLIC void AH5_print_str_attr(char *name, char *value, int space);
    AH5_PUBLIC void AH5_print_opt_attrs(const AH5_opt_attrs_t *opt_attrs, int space);

    AH5_PUBLIC void AH5_free_opt_attrs(AH5_opt_attrs_t *opt_attrs);


#ifdef __cplusplus
}
#endif

#endif // AH5_ATTRIBUTE_H
