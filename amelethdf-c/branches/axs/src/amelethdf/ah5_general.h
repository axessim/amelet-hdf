#ifndef AH5_GENERAL_H
#define AH5_GENERAL_H



#include <stdlib.h>
#include <string.h>
#include <hdf5.h>
#include <hdf5_hl.h>

#include <ah5_config.h>
#include "ah5_category.h"

#ifdef __cplusplus
extern "C" {
#endif

// See http://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
# ifdef AMELETHDF_C_LIBRARY
#  ifdef __GNUC__
#   define AH5_PUBLIC __attribute__ ((dllexport))
#  else
#   define AH5_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#  endif
# else
#  ifdef __GNUC__
#   define AH5_PUBLIC __attribute__ ((dllimport))
#  else
#   define AH5_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#  endif
# endif
#define AH5_LOCAL
#else
# if __GNUC__ >= 4
#  define AH5_PUBLIC __attribute__ ((visibility ("default")))
#  define AH5_LOCAL  __attribute__ ((visibility ("hidden")))
# else
#  define AH5_PUBLIC
#  define AH5_LOCAL
# endif
#endif

// In this block the configur variables are used to define some status variables.
#if AH5_MPI_ENABLE_
# define AH5_WITH_MPI_ 1
# error "with mpi"
#else
# define AH5_WITHOUT_MPI_ 1
#endif

// A specific flag for the complexes.
#if __STDC_VERSION__ >= 199901L
# define AH5_SDT_CCOMPLEX
# define ACCESS _acess
#endif

#ifdef AH5_SDT_CCOMPLEX
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
#endif /*AH5_STD_CCOMPLEX*/

    AH5_PUBLIC AH5_complex_t AH5_set_complex(float real, float imag);

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

    /** 
     * Create a Amelet-HDF file and set entry point if not null.
     * 
     * @param name name of the file to access.
     * @param flags file access flags (see H5Fcreate)
     * @param entry_point the Amelet-HDF entry point if NULL it is ignored.
     * 
     * @return Returns a file identifier if successful; otherwise returns a
     * negative value.
     */
    AH5_PUBLIC hid_t AH5_create(const char *name, unsigned flags, const char *entry_point);

    AH5_PUBLIC hid_t AH5_H5Tcreate_cpx_memtype(void);
    AH5_PUBLIC hid_t AH5_H5Tcreate_cpx_filetype(void);

    AH5_PUBLIC char AH5_version_minimum(const char *required_version, const char *sim_version);
    AH5_PUBLIC char *AH5_trim_zeros(const char *version);
    AH5_PUBLIC char AH5_path_valid(hid_t file_id, const char *path);
    AH5_PUBLIC AH5_set_t AH5_add_to_set(AH5_set_t aset, char *aelement);
    AH5_PUBLIC int AH5_index_in_set(AH5_set_t aset, char *aelement, hsize_t *index);
    AH5_PUBLIC AH5_children_t AH5_read_children_name(hid_t file_id, const char *path);

    AH5_PUBLIC char *AH5_get_name_from_path(const char *path);
    AH5_PUBLIC char *AH5_get_base_from_path(const char *path);
    AH5_PUBLIC char *AH5_join_path(char *base, const char *head);
    AH5_PUBLIC char* AH5_trim_path(char *path);

    AH5_PUBLIC void AH5_print_err_dset(const char *category, const char *path);
    AH5_PUBLIC void AH5_print_err_tble(const char *category, const char *path);
    AH5_PUBLIC void AH5_print_err_attr(const char *category, const char *path, const char *attr_name);
    AH5_PUBLIC void AH5_print_err_path(const char *category, const char *path);
    AH5_PUBLIC void AH5_print_err_inv_attr(const char *category, const char *path, const char *attr_name);
    AH5_PUBLIC void AH5_print_err_func_not_implemented(const char *category, const char *path, const char *func_name);
    AH5_PUBLIC void AH5_print_wrn_attr(const char *category, const char *path, const char *attr_name);


// Some helpers macros of manage error.
#define AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED(category, path) \
  AH5_print_err_func_not_implemented(category, path, (__FUNCTION__))


#ifdef __cplusplus
}
#endif

#endif // AH5_GENERAL_H

