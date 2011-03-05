#ifndef FLOATINGTYPE_H
#define FLOATINGTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"

#define A_FLOATING_TYPE "floatingType"
#define A_LABEL  "label"
#define A_PHYSICAL_NATURE  "physicalNature"
#define A_UNIT  "unit"
#define A_COMMENT  "comment"
#define A_VALUE  "value"
#define A_FIRST "first"
#define A_LAST "last"
#define A_NUMBER_OF_VALUES "numberOfValues"
#define A_STEP "step"
#define A_NUMBER_OF_DECADES "numberOfDecades"
#define A_NUMBER_OF_VALUES_PER_DECADE "numberOfValuesPerDecade"

#define F_TYPE "type"
#define F_A "A"
#define F_B "B"
#define F_F "F"

#define G_DATA "/data"
#define G_FUNCTION "/function"

#define V_SINGLE_INTEGER "singleInteger"
#define V_SINGLE_REAL "singleReal"
#define V_SINGLE_COMPLEX "singleComplex"
#define V_SINGLE_STRING "singleString"
#define V_VECTOR "vector"
#define V_LINEARLISTOFREAL1 "linearListOfReal1"
#define V_LINEARLISTOFREAL2 "linearListOfReal2"
#define V_LOGARITHMLISTOFREAL "logarithmListOfReal"
#define V_PERDECADELISTOFREAL "perDecadeListOfReal"
#define V_LINEARLISTOFINTEGER2 "linearListOfInteger2"
#define V_RATIONAL_FUNCTION "rationalFunction"
#define V_GENERAL_RATIONAL_FUNCTION "generalRationalFunction"
#define V_RATIONAL "rational"
#define V_DATASET "dataSet"
#define V_ARRAYSET "arraySet"


    typedef enum floatingtype_class_t
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
    } floatingtype_class_t;

    typedef struct singleinteger_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        int         value;
    } singleinteger_t;

    typedef struct singlereal_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        float       value;
    } singlereal_t;

    typedef struct singlecomplex_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        complex float value;
    } singlecomplex_t;

    typedef struct singlestring_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        char        *value;
    } singlestring_t;

    typedef union datasetx_t
    {
        int         *i;
        float       *f;
        complex float *c;
        char        **s;
    } datasetx_t;

    typedef struct vector_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        hsize_t     nb_values;
        H5T_class_t type_class;
        datasetx_t  values;
    } vector_t;

    typedef struct linearlistofreal1_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        float       first;
        float       last;
        int         number_of_values;
    } linearlistofreal1_t;

    typedef struct linearlistofreal2_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        float       first;
        float       step;
        int         number_of_values;
    } linearlistofreal2_t;

    typedef struct logarithmlistofreal_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        float       first;
        float       last;
        int         number_of_values;
    } logarithmlistofreal_t;

    typedef struct perdecadelistofreal_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        float       first;
        int         number_of_decades;
        int         number_of_values_per_decade;
    } perdecadelistofreal_t;

    typedef struct linearlistofinteger2_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        int         first;
        int         step;
        int         number_of_values;
    } linearlistofinteger2_t;

    typedef struct rationalfunction_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        hsize_t     nb_types;
        int         *types;
        float       *a;
        float       *b;
        float       *f;
    } rationalfunction_t;

    typedef struct generalrationalfunction_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        int         nb_degrees;
        complex float *numerator;
        complex float *denominator;
    } generalrationalfunction_t;

    typedef union floatingtyper_data_t
    {
        rationalfunction_t rf;
        generalrationalfunction_t grf;
    } floatingtyper_data_t;

    typedef struct floatingtyper_t
    {
        floatingtype_class_t type;
        floatingtyper_data_t data;
    } floatingtyper_t;

    typedef struct rational_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        int         nb_functions;
        floatingtyper_t *functions;
        hsize_t     dims[2];
        char        **data;
    } rational_t;

    typedef struct dataset_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        int         nb_dims;
        hsize_t     *dims;
        H5T_class_t type_class;
        datasetx_t  values;
    } dataset_t;

    typedef struct arrayset_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        dataset_t   data;
        hsize_t     nb_dims;
        vector_t    *dims;
    } arrayset_t;

    typedef union floatingtype_data_t
    {
        singleinteger_t singleinteger;
        singlereal_t singlereal;
        singlecomplex_t singlecomplex;
        singlestring_t singlestring;
        vector_t vector;
        linearlistofreal1_t linearlistofreal1;
        linearlistofreal2_t linearlistofreal2;
        logarithmlistofreal_t logarithmlistofreal;
        perdecadelistofreal_t perdecadelistofreal;
        linearlistofinteger2_t linearlistofinteger2;
        rationalfunction_t rationalfunction;
        generalrationalfunction_t generalrationalfunction;
        rational_t  rational;
        dataset_t   dataset;
        arrayset_t  arrayset;
    } floatingtype_data_t;

    typedef struct floatingtype_t
    {
        floatingtype_class_t type;
        floatingtype_data_t data;
    } floatingtype_t;

    char read_ft_singleinteger (hid_t file_id, const char *path, singleinteger_t *singleinteger);
    char read_ft_singlereal (hid_t file_id, const char *path, singlereal_t *singlereal);
    char read_ft_singlecomplex (hid_t file_id, const char *path, singlecomplex_t *singlecomplex);
    char read_ft_singlestring (hid_t file_id, const char *path, singlestring_t *singlestring);
    char read_ft_vector (hid_t file_id, const char *path, vector_t *vector);
    char read_ft_linearlistofreal1 (hid_t file_id, const char *path, linearlistofreal1_t *linearlistofreal1);
    char read_ft_linearlistofreal2 (hid_t file_id, const char *path, linearlistofreal2_t *linearlistofreal2);
    char read_ft_logarithmlistofreal (hid_t file_id, const char *path, logarithmlistofreal_t *logarithmlistofreal);
    char read_ft_perdecadelistofreal (hid_t file_id, const char *path, perdecadelistofreal_t *perdecadelistofreal);
    char read_ft_linearlistofinteger2 (hid_t file_id, const char *path, linearlistofinteger2_t *linearlistofinteger2);
    char read_ft_rationalfunction (hid_t file_id, const char *path, rationalfunction_t *rationalfunction);
    char read_ft_generalrationalfunction (hid_t file_id, const char *path, generalrationalfunction_t *generalrationalfunction);
    char read_ft_rational (hid_t file_id, const char *path, rational_t *rational);
    char read_ft_dataset (hid_t file_id, const char *path, dataset_t *dataset);
    char read_ft_arrayset (hid_t file_id, const char *path, arrayset_t *arrayset);
    char read_floatingtype (hid_t file_id, const char *path, floatingtype_t *floatingtype);

    void print_ft_singleinteger (const singleinteger_t *singleinteger, int space);
    void print_ft_singlereal (const singlereal_t *singlereal, int space);
    void print_ft_singlecomplex (const singlecomplex_t *singlecomplex, int space);
    void print_ft_singlestring (const singlestring_t *singlestring, int space);
    void print_ft_vector (const vector_t *vector, int space);
    void print_ft_linearlistofreal1 (const linearlistofreal1_t *linearlistofreal1, int space);
    void print_ft_linearlistofreal2 (const linearlistofreal2_t *linearlistofreal2, int space);
    void print_ft_logarithmlistofreal (const logarithmlistofreal_t *logarithmlistofreal, int space);
    void print_ft_perdecadelistofreal (const perdecadelistofreal_t *perdecadelistofreal, int space);
    void print_ft_linearlistofinteger2 (const linearlistofinteger2_t *linearlistofinteger2, int space);
    void print_ft_rationalfunction (const rationalfunction_t *rationalfunction, int space);
    void print_ft_generalrationalfunction (const generalrationalfunction_t *generalrationalfunction, int space);
    void print_ft_rational (const rational_t *rational, int space);
    void print_ft_dataset (const dataset_t *dataset, int space);
    void print_ft_arrayset (const arrayset_t *arrayset, int space);
    void print_floatingtype (const floatingtype_t *floatingtype, int space);

    void free_ft_singleinteger (singleinteger_t *singleinteger);
    void free_ft_singlereal (singlereal_t *singlereal);
    void free_ft_singlecomplex (singlecomplex_t *singlecomplex);
    void free_ft_singlestring (singlestring_t *singlestring);
    void free_ft_vector (vector_t *vector);
    void free_ft_linearlistofreal1 (linearlistofreal1_t *linearlistofreal1);
    void free_ft_linearlistofreal2 (linearlistofreal2_t *linearlistofreal2);
    void free_ft_logarithmlistofreal (logarithmlistofreal_t *logarithmlistofreal);
    void free_ft_perdecadelistofreal (perdecadelistofreal_t *perdecadelistofreal);
    void free_ft_linearlistofinteger2 (linearlistofinteger2_t *linearlistofinteger2);
    void free_ft_rationalfunction (rationalfunction_t *rationalfunction);
    void free_ft_generalrationalfunction (generalrationalfunction_t *generalrationalfunction);
    void free_ft_rational (rational_t *rational);
    void free_ft_dataset (dataset_t *dataset);
    void free_ft_arrayset (arrayset_t *arrayset);
    void free_floatingtype (floatingtype_t *floatingtype);


#ifdef __cplusplus
}
#endif

#endif // FLOATINGTYPE_H
