// test path tools

#include <string.h>
#include <stdio.h>

#include <ah5.h>
#include "utest.h"

//! Test suite counter.
int tests_run = 0;

char *test_write_single_flotingtype()
{
    hid_t file_id;

    AH5_singleinteger_t flti;
    AH5_singlereal_t fltr;

    int idata;
    float rdata;

    // Write a simple mesh test.
    file_id = AH5_auto_test_file();

    // single integer
    flti.path = (char*)"/floatingType/flti";
    flti.value = 10;
    flti.opt_attrs.nb_instances = 0;
    mu_assert("Write single integer.",
              AH5_write_ft_singleinteger(file_id, &flti));

    AH5_read_int_attr(file_id, "/floatingType/flti", "value", &idata);
    mu_assert_eq("Test the written int value.", idata, 10);

    // single real
    fltr.path = (char*)"/floatingType/fltr";
    fltr.value = 0.5;
    fltr.opt_attrs.nb_instances = 0;
    mu_assert("Write single real.",
              AH5_write_ft_singlereal(file_id, &fltr));

    AH5_read_flt_attr(file_id, "/floatingType/fltr", "value", &rdata);
    mu_assert_eq("Test the written real value.", rdata, 0.5);

    // Close file.
    AH5_close_test_file(file_id);

    return MU_FINISHED_WITHOUT_ERRORS;
}

char *test_write_ft_vector()
{
    hid_t file_id;
    int i;
    const int nb_values = 10;
    float *fvalues;

    AH5_vector_t vector;

    // Write a simple mesh test.
    file_id = AH5_auto_test_file();
    
    vector.path = "/floatingType/vector";
    vector.opt_attrs.nb_instances = 0;
    vector.type_class = H5T_FLOAT;
    vector.nb_values = nb_values;
    vector.values.f = (float*)malloc(vector.nb_values*sizeof(float));
    for (i = 0; i < vector.nb_values; ++i)
      vector.values.f[i] = 0.3 * i;

    mu_assert("Write vector.",
              AH5_write_ft_vector(file_id, &vector));

    mu_assert("Read float dataset for check.",
              AH5_read_flt_dataset(file_id, "/floatingType/vector", nb_values, &fvalues));
    for (i = 0; i < nb_values; ++i)
      mu_assert_close("Test the written real values.",
                      fvalues[i], 0.3 * i, 1.e-4);

    free(fvalues);
    
    // Close file.
    AH5_close_test_file(file_id);

    return MU_FINISHED_WITHOUT_ERRORS;
}

char *test_write_ft_dataset()
{
    hid_t file_id;
    int i;
    const int nb_values = 10;
    float *fvalues;

    AH5_dataset_t ds;

    // Write a simple mesh test.
    file_id = AH5_auto_test_file();
    
    ds.path = "/floatingType/dataset";
    ds.opt_attrs.nb_instances = 0;
    ds.type_class = H5T_FLOAT;
    ds.nb_dims = 2;
    ds.dims = (hsize_t*)malloc(ds.nb_dims*sizeof(hsize_t));
    ds.dims[0] = 10;
    ds.dims[1] = 2;
    ds.values.f = (float*)malloc(ds.dims[0]*ds.dims[1]*sizeof(float));
    for (i = 0; i < ds.dims[0]*ds.dims[1]; ++i)
      ds.values.f[i] = (float)0.3 * i;

    mu_assert("Write ds.",
              AH5_write_ft_dataset(file_id, &ds));

    AH5_read_flt_dataset(file_id, "/floatingType/dataset", ds.dims[0]*ds.dims[1], &fvalues);
    for (i = 0; i < ds.dims[0]*ds.dims[1]; ++i)
      mu_assert_close("Test the written real values.",
                      fvalues[i], 0.3 * (float)i, 1.e-4);

    free(fvalues);
    
    // Close file.
    AH5_close_test_file(file_id);

    return MU_FINISHED_WITHOUT_ERRORS;
}

char *test_write_ft_arrayset()
{
    hid_t file_id;
    int i;
    int nb_values = 1;
    float *fvalues;
    char **svalues;

    AH5_arrayset_t array, dataonmesh;
    char meshpath[1][34] = {"/mesh/gmesh/umesh/group/groupname"};
    
    // Write a simple mesh test.
    file_id = AH5_auto_test_file();

    /* simple array set*/
    array.path = "/floatingType/array";
    array.opt_attrs.nb_instances = 0;
    array.nb_dims = 2;
    array.dims = (AH5_vector_t*)malloc(array.nb_dims*sizeof(AH5_vector_t));
    
    array.dims[0].nb_values = 10;
    array.dims[0].opt_attrs.nb_instances = 0;
    array.dims[0].type_class = H5T_FLOAT;
    array.dims[0].values.f = (float*)malloc(array.dims[0].nb_values*sizeof(float));
    for (i = 0; i < array.dims[0].nb_values; ++i)
      array.dims[0].values.f[i] = 0.3 * i;
    
    array.dims[1].nb_values = 2;
    array.dims[1].opt_attrs.nb_instances = 0;
    array.dims[1].type_class = H5T_INTEGER;
    array.dims[1].values.i = (int*)malloc(array.dims[1].nb_values*sizeof(int));
    for (i = 0; i < array.dims[1].nb_values; ++i)
      array.dims[1].values.i[i] = i * 3;
    
    array.data.type_class = H5T_FLOAT;
    array.data.opt_attrs.nb_instances = 0;
    array.data.nb_dims = array.nb_dims;
    array.data.dims = (hsize_t*)malloc(array.data.nb_dims*sizeof(hsize_t));
    for (i = 0; i < array.data.nb_dims; ++i) {
      array.data.dims[i] = array.dims[i].nb_values;
      nb_values *= array.dims[i].nb_values;
    }
    array.data.values.f = (float*)malloc(nb_values*sizeof(float));
    for (i = 0; i < nb_values; ++i)
      array.data.values.f[i] = 0.3 * i;
    
    mu_assert("Write array set.",
              AH5_write_ft_arrayset(file_id, &array));

    AH5_read_flt_dataset(file_id, "/floatingType/array/data", nb_values, &fvalues);
    for (i = 0; i < nb_values; ++i)
      mu_assert_close("Test the written real values.",
                      fvalues[i], 0.3 * i, 1.e-4);

    free(fvalues);

    /* a data on mesh */
    dataonmesh.path = "/floatingType/dataonmesh";
    dataonmesh.opt_attrs.nb_instances = 0;
    dataonmesh.nb_dims = 2;
    dataonmesh.dims = (AH5_vector_t*)malloc(dataonmesh.nb_dims*sizeof(AH5_vector_t));
    
    dataonmesh.dims[0].nb_values = 1;
    dataonmesh.dims[0].opt_attrs.nb_instances = 0;
    dataonmesh.dims[0].type_class = H5T_STRING;
    dataonmesh.dims[0].values.s = meshpath;
    
    dataonmesh.dims[1].nb_values = 2;
    dataonmesh.dims[1].opt_attrs.nb_instances = 0;
    dataonmesh.dims[1].type_class = H5T_INTEGER;
    dataonmesh.dims[1].values.i = (int*)malloc(dataonmesh.dims[1].nb_values*sizeof(int));
    for (i = 0; i < dataonmesh.dims[1].nb_values; ++i)
      dataonmesh.dims[1].values.i[i] = i * 3;
    
    dataonmesh.data.type_class = H5T_FLOAT;
    dataonmesh.data.opt_attrs.nb_instances = 0;
    dataonmesh.data.nb_dims = dataonmesh.nb_dims;
    dataonmesh.data.dims = (hsize_t*)malloc(dataonmesh.data.nb_dims*sizeof(hsize_t));
    dataonmesh.data.dims[0] = 10;
    dataonmesh.data.dims[1] = dataonmesh.dims[1].nb_values;
    nb_values = 1;
    for (i = 0; i < dataonmesh.data.nb_dims; ++i) {
      nb_values *= dataonmesh.data.dims[i];
    }
    dataonmesh.data.values.f = (float*)malloc(nb_values*sizeof(float));
    for (i = 0; i < nb_values; ++i)
      dataonmesh.data.values.f[i] = 0.3 * i;
    
    mu_assert("Write dataonmesh.",
              AH5_write_ft_arrayset(file_id, &dataonmesh));

    AH5_read_flt_dataset(file_id, "/floatingType/dataonmesh/data", nb_values, &fvalues);
    for (i = 0; i < nb_values; ++i)
      mu_assert_close("Test the written real values.",
                      fvalues[i], 0.3 * i, 1.e-4);
    
    AH5_read_str_dataset(file_id, "/floatingType/dataonmesh/ds/dim1", 1, strlen(meshpath[0]), &svalues);
    mu_assert_str_equal("Test the written string dims.",
        *svalues, meshpath[0]);
    
    free(fvalues);
    
    // Close file.
    AH5_close_test_file(file_id);

    return MU_FINISHED_WITHOUT_ERRORS;
}


// Run all tests
char *all_tests()
{
    mu_run_test(test_write_single_flotingtype);
    mu_run_test(test_write_ft_vector);
    mu_run_test(test_write_ft_dataset);
    mu_run_test(test_write_ft_arrayset);
    
    return MU_FINISHED_WITHOUT_ERRORS;
}

// Main function, run tests and print results.
int main(int argc, char **argv)
{
    char *result;
    tests_run = 0;
    result = all_tests();

    if (result != 0)
    {
        printf("%s\n", result);
    }
    else
    {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
