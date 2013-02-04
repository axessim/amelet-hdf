#ifndef AH5_GENERAL_H
#define AH5_GENERAL_H

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

    AH5_complex_t AH5_set_complex(float real, float imag);

#ifdef __cplusplus
}
#endif

#include "ah5_attribute.h"
#include "ah5_dataset.h"

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


    char AH5_version_minimum(const char *required_version, const char *sim_version);
    char *AH5_trim_zeros(const char *version);
    char AH5_path_valid(hid_t file_id, const char *path);
    AH5_set_t AH5_add_to_set(AH5_set_t aset, char *aelement);
    int AH5_index_in_set(AH5_set_t aset, char *aelement, hsize_t *index);
    AH5_children_t AH5_read_children_name(hid_t file_id, const char *path);

    char *AH5_get_name_from_path(const char *path);
    char *AH5_get_base_from_path(const char *path);
    char *AH5_join_path(char *base, const char *head);
    char* AH5_trim_path(char *path);

    void AH5_print_err_dset(const char *category, const char *path);
    void AH5_print_err_tble(const char *category, const char *path);
    void AH5_print_err_attr(const char *category, const char *path, const char *attr_name);
    void AH5_print_err_path(const char *category, const char *path);
    void AH5_print_err_inv_attr(const char *category, const char *path, const char *attr_name);
    void AH5_print_err_func_not_implemented(const char *category, const char *path, const char *func_name);
    void AH5_print_wrn_attr(const char *category, const char *path, const char *attr_name);


// Some helpers macros of manage error.
#define AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED(category, path) \
  AH5_print_err_func_not_implemented(category, path, (__FUNCTION__))


#ifdef __cplusplus
}
#endif

#endif // AH5_GENERAL_H

