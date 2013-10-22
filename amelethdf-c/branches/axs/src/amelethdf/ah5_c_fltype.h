#ifndef AH5_C_FLTYPE_H
#define AH5_C_FLTYPE_H

#include "ah5_general.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum _AH5_ft_class_t
    {
        FT_INVALID              = -1,
        FT_SINGLE_INTEGER       = 1,
        FT_SINGLE_REAL          = 2,
        FT_SINGLE_COMPLEX       = 3,
        FT_SINGLE_STRING        = 4,
        FT_VECTOR               = 5,
        FT_LINEARLISTOFREAL1    = 6,
        FT_LINEARLISTOFREAL2    = 7,
        FT_LOGARITHMLISTOFREAL  = 8,
        FT_PERDECADELISTOFREAL  = 9,
        FT_LINEARLISTOFINTEGER2 = 10,
        FT_RATIONAL_FUNCTION    = 11,
        FT_GENERAL_RATIONAL_FUNCTION = 12,
        FT_RATIONAL             = 13,
        FT_DATASET              = 14,
        FT_ARRAYSET             = 15
    } AH5_ft_class_t;

    typedef struct _AH5_singleinteger_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        int             value;
    } AH5_singleinteger_t;

    typedef struct _AH5_singlereal_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        float           value;
    } AH5_singlereal_t;

    typedef struct _AH5_singlecomplex_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        AH5_complex_t   value;
    } AH5_singlecomplex_t;

    typedef struct _AH5_singlestring_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        char            *value;
    } AH5_singlestring_t;

    typedef union _AH5_datasetx_t
    {
        int             *i;
        float           *f;
        AH5_complex_t   *c;
        char            **s;
    } AH5_datasetx_t;

    typedef struct _AH5_vector_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        hsize_t         nb_values;
        H5T_class_t     type_class;
        AH5_datasetx_t  values;
    } AH5_vector_t;

    typedef struct _AH5_linearlistofreal1_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        float           first;
        float           last;
        int             number_of_values;
    } AH5_linearlistofreal1_t;

    typedef struct _AH5_linearlistofreal2_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        float           first;
        float           step;
        int             number_of_values;
    } AH5_linearlistofreal2_t;

    typedef struct _AH5_logarithmlistofreal_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        float           first;
        float           last;
        int             number_of_values;
    } AH5_logarithmlistofreal_t;

    typedef struct _AH5_perdecadelistofreal_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        float           first;
        int             number_of_decades;
        int             number_of_values_per_decade;
    } AH5_perdecadelistofreal_t;

    typedef struct _AH5_linearlistofinteger2_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        int             first;
        int             step;
        int             number_of_values;
    } AH5_linearlistofinteger2_t;

    typedef struct _AH5_rationalfunction_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        hsize_t         nb_types;
        int             *types;
        float           *a;
        float           *b;
        float           *f;
    } AH5_rationalfunction_t;

    typedef struct _AH5_generalrationalfunction_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        int             nb_degrees;
        AH5_complex_t   *numerator;
        AH5_complex_t   *denominator;
    } AH5_generalrationalfunction_t;

    typedef union _AH5_ftr_data_t
    {
        AH5_rationalfunction_t rf;
        AH5_generalrationalfunction_t grf;
    } AH5_ftr_data_t;

    typedef struct _AH5_ftr_t
    {
        AH5_ft_class_t  type;
        AH5_ftr_data_t  data;
    } AH5_ftr_t;

    typedef struct _AH5_rational_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        int             nb_functions;
        AH5_ftr_t       *functions;
        hsize_t         dims[2];
        char            **data;
    } AH5_rational_t;

    typedef struct _AH5_dataset_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        int             nb_dims;
        hsize_t         *dims;
        H5T_class_t     type_class;
        AH5_datasetx_t  values;
    } AH5_dataset_t;

    typedef struct _AH5_arrayset_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        AH5_dataset_t   data;
        hsize_t         nb_dims;
        AH5_vector_t    *dims;
    } AH5_arrayset_t;

    typedef union _AH5_ft_data_t
    {
        AH5_singleinteger_t singleinteger;
        AH5_singlereal_t singlereal;
        AH5_singlecomplex_t singlecomplex;
        AH5_singlestring_t singlestring;
        AH5_vector_t vector;
        AH5_linearlistofreal1_t linearlistofreal1;
        AH5_linearlistofreal2_t linearlistofreal2;
        AH5_logarithmlistofreal_t logarithmlistofreal;
        AH5_perdecadelistofreal_t perdecadelistofreal;
        AH5_linearlistofinteger2_t linearlistofinteger2;
        AH5_rationalfunction_t rationalfunction;
        AH5_generalrationalfunction_t generalrationalfunction;
        AH5_rational_t  rational;
        AH5_dataset_t   dataset;
        AH5_arrayset_t  arrayset;
    } AH5_ft_data_t;

    typedef struct _AH5_ft_t
    {
        AH5_ft_class_t  type;
        AH5_ft_data_t   data;
    } AH5_ft_t;

    AH5_PUBLIC char AH5_read_ft_singleinteger (hid_t file_id, const char *path, AH5_singleinteger_t *singleinteger);
    AH5_PUBLIC char AH5_read_ft_singlereal (hid_t file_id, const char *path, AH5_singlereal_t *singlereal);
    AH5_PUBLIC char AH5_read_ft_singlecomplex (hid_t file_id, const char *path, AH5_singlecomplex_t *singlecomplex);
    AH5_PUBLIC char AH5_read_ft_singlestring (hid_t file_id, const char *path, AH5_singlestring_t *singlestring);
    AH5_PUBLIC char AH5_read_ft_vector (hid_t file_id, const char *path, AH5_vector_t *vector);
    AH5_PUBLIC char AH5_read_ft_linearlistofreal1 (hid_t file_id, const char *path, AH5_linearlistofreal1_t *linearlistofreal1);
    AH5_PUBLIC char AH5_read_ft_linearlistofreal2 (hid_t file_id, const char *path, AH5_linearlistofreal2_t *linearlistofreal2);
    AH5_PUBLIC char AH5_read_ft_logarithmlistofreal (hid_t file_id, const char *path, AH5_logarithmlistofreal_t *logarithmlistofreal);
    AH5_PUBLIC char AH5_read_ft_perdecadelistofreal (hid_t file_id, const char *path, AH5_perdecadelistofreal_t *perdecadelistofreal);
    AH5_PUBLIC char AH5_read_ft_linearlistofinteger2 (hid_t file_id, const char *path, AH5_linearlistofinteger2_t *linearlistofinteger2);
    AH5_PUBLIC char AH5_read_ft_rationalfunction (hid_t file_id, const char *path, AH5_rationalfunction_t *rationalfunction);
    AH5_PUBLIC char AH5_read_ft_generalrationalfunction (hid_t file_id, const char *path, AH5_generalrationalfunction_t *generalrationalfunction);
    AH5_PUBLIC char AH5_read_ft_rational (hid_t file_id, const char *path, AH5_rational_t *rational);
    AH5_PUBLIC char AH5_read_ft_dataset (hid_t file_id, const char *path, AH5_dataset_t *dataset);
    AH5_PUBLIC char AH5_read_ft_arrayset (hid_t file_id, const char *path, AH5_arrayset_t *arrayset);
    AH5_PUBLIC char AH5_read_floatingtype (hid_t file_id, const char *path, AH5_ft_t *floatingtype);

  
    AH5_PUBLIC char AH5_write_ft_singleinteger (hid_t file_id, AH5_singleinteger_t *singleinteger);
    AH5_PUBLIC char AH5_write_ft_singlereal (hid_t file_id, AH5_singlereal_t *singlereal);
    AH5_PUBLIC char AH5_write_ft_singlecomplex (hid_t file_id, AH5_singlecomplex_t *singlecomplex);
    AH5_PUBLIC char AH5_write_ft_singlestring (hid_t file_id, AH5_singlestring_t *singlestring);
    AH5_PUBLIC char AH5_write_ft_vector (hid_t file_id, AH5_vector_t *vector);
    AH5_PUBLIC char AH5_write_ft_linearlistofreal1 (hid_t file_id, AH5_linearlistofreal1_t *linearlistofreal1);
    AH5_PUBLIC char AH5_write_ft_linearlistofreal2 (hid_t file_id, AH5_linearlistofreal2_t *linearlistofreal2);
    AH5_PUBLIC char AH5_write_ft_logarithmlistofreal (hid_t file_id, AH5_logarithmlistofreal_t *logarithmlistofreal);
    AH5_PUBLIC char AH5_write_ft_perdecadelistofreal (hid_t file_id, AH5_perdecadelistofreal_t *perdecadelistofreal);
    AH5_PUBLIC char AH5_write_ft_linearlistofinteger2 (hid_t file_id, AH5_linearlistofinteger2_t *linearlistofinteger2);
    AH5_PUBLIC char AH5_write_ft_rationalfunction (hid_t file_id, AH5_rationalfunction_t *rationalfunction);
    AH5_PUBLIC char AH5_write_ft_generalrationalfunction (hid_t file_id, AH5_generalrationalfunction_t *generalrationalfunction);
    AH5_PUBLIC char AH5_write_ft_rational (hid_t file_id, AH5_rational_t *rational);
    AH5_PUBLIC char AH5_write_ft_dataset (hid_t file_id, AH5_dataset_t *dataset);
    AH5_PUBLIC char AH5_write_ft_arrayset (hid_t file_id, AH5_arrayset_t *arrayset);
    AH5_PUBLIC char AH5_write_floatingtype (hid_t file_id, AH5_ft_t *floatingtype);

    AH5_PUBLIC void AH5_print_ft_singleinteger (const AH5_singleinteger_t *singleinteger, int space);
    AH5_PUBLIC void AH5_print_ft_singlereal (const AH5_singlereal_t *singlereal, int space);
    AH5_PUBLIC void AH5_print_ft_singlecomplex (const AH5_singlecomplex_t *singlecomplex, int space);
    AH5_PUBLIC void AH5_print_ft_singlestring (const AH5_singlestring_t *singlestring, int space);
    AH5_PUBLIC void AH5_print_ft_vector (const AH5_vector_t *vector, int space);
    AH5_PUBLIC void AH5_print_ft_linearlistofreal1 (const AH5_linearlistofreal1_t *linearlistofreal1, int space);
    AH5_PUBLIC void AH5_print_ft_linearlistofreal2 (const AH5_linearlistofreal2_t *linearlistofreal2, int space);
    AH5_PUBLIC void AH5_print_ft_logarithmlistofreal (const AH5_logarithmlistofreal_t *logarithmlistofreal, int space);
    AH5_PUBLIC void AH5_print_ft_perdecadelistofreal (const AH5_perdecadelistofreal_t *perdecadelistofreal, int space);
    AH5_PUBLIC void AH5_print_ft_linearlistofinteger2 (const AH5_linearlistofinteger2_t *linearlistofinteger2, int space);
    AH5_PUBLIC void AH5_print_ft_rationalfunction (const AH5_rationalfunction_t *rationalfunction, int space);
    AH5_PUBLIC void AH5_print_ft_generalrationalfunction (const AH5_generalrationalfunction_t *generalrationalfunction, int space);
    AH5_PUBLIC void AH5_print_ft_rational (const AH5_rational_t *rational, int space);
    AH5_PUBLIC void AH5_print_ft_dataset (const AH5_dataset_t *dataset, int space);
    AH5_PUBLIC void AH5_print_ft_arrayset (const AH5_arrayset_t *arrayset, int space);
    AH5_PUBLIC void AH5_print_floatingtype (const AH5_ft_t *floatingtype, int space);

    AH5_PUBLIC void AH5_free_ft_singleinteger (AH5_singleinteger_t *singleinteger);
    AH5_PUBLIC void AH5_free_ft_singlereal (AH5_singlereal_t *singlereal);
    AH5_PUBLIC void AH5_free_ft_singlecomplex (AH5_singlecomplex_t *singlecomplex);
    AH5_PUBLIC void AH5_free_ft_singlestring (AH5_singlestring_t *singlestring);
    AH5_PUBLIC void AH5_free_ft_vector (AH5_vector_t *vector);
    AH5_PUBLIC void AH5_free_ft_linearlistofreal1 (AH5_linearlistofreal1_t *linearlistofreal1);
    AH5_PUBLIC void AH5_free_ft_linearlistofreal2 (AH5_linearlistofreal2_t *linearlistofreal2);
    AH5_PUBLIC void AH5_free_ft_logarithmlistofreal (AH5_logarithmlistofreal_t *logarithmlistofreal);
    AH5_PUBLIC void AH5_free_ft_perdecadelistofreal (AH5_perdecadelistofreal_t *perdecadelistofreal);
    AH5_PUBLIC void AH5_free_ft_linearlistofinteger2 (AH5_linearlistofinteger2_t *linearlistofinteger2);
    AH5_PUBLIC void AH5_free_ft_rationalfunction (AH5_rationalfunction_t *rationalfunction);
    AH5_PUBLIC void AH5_free_ft_generalrationalfunction (AH5_generalrationalfunction_t *generalrationalfunction);
    AH5_PUBLIC void AH5_free_ft_rational (AH5_rational_t *rational);
    AH5_PUBLIC void AH5_free_ft_dataset (AH5_dataset_t *dataset);
    AH5_PUBLIC void AH5_free_ft_arrayset (AH5_arrayset_t *arrayset);
    AH5_PUBLIC void AH5_free_floatingtype (AH5_ft_t *floatingtype);

#ifdef __cplusplus
}
#endif

#endif // AH5_C_FLTYPE_H
