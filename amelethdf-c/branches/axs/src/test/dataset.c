// test path tools

#include <string.h>
#include <stdio.h>

#include <ah5.h>
#include "utest.h"

//! Test suite counter.
int tests_run = 0;


//! Test write dataset.
// Data extracted from http://www.hdfgroup.org/ftp/HDF5/examples/examples-by-api/hdf5-examples/1_8/C/H5T/h5ex_t_string.c
char *test_write_string_dataset()
{
#define DIM0 4
#define SDIM 8

    hid_t file_id, filetype, memtype, space, dset;
    size_t sdim;
    hsize_t dims[1] = {DIM0};
    int ndims, i, j;
    /*char wdata[DIM0][SDIM] =*/
    char **wdata = (char *[]){"Parting",
                              "is such",
                              "sweet  ",
                              "sorrow."};
    char **rdata;

    // Write a simple mesh test.
    file_id = AH5_auto_test_file();

    mu_assert("Write string dataset.",
              AH5_write_str_dataset(file_id, "dataset_name",
                                    DIM0, SDIM, wdata));
    // Test the written data using hdf5 API.
    dset = H5Dopen(file_id, "/dataset_name", H5P_DEFAULT);
    filetype = H5Dget_type(dset);
    sdim = H5Tget_size(filetype);
    space = H5Dget_space(dset);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    rdata = (char **) malloc(dims[0] * sizeof (char *));
    rdata[0] = (char *) malloc(dims[0] * sdim * sizeof (char));
    for (i=1; i<dims[0]; i++)
      rdata[i] = rdata[0] + i * sdim;
    memtype = H5Tcopy(H5T_C_S1);
    mu_assert("HDF5 error in H5Tset_size.", H5Tset_size(memtype, sdim) >= 0);
    mu_assert("HDF5 error in H5Dread.",
              H5Dread(dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdata[0]) >= 0);
    for (i = 0; i < dims[0]; i++) {
      printf("%s %s\n", wdata[i], rdata[i]);
      /*mu_assert_str_equal("Check the first str dataset values.", wdata[i], rdata[i]);*/
      j = 0;
      while (wdata[i][j] != ' ' && wdata[i][j] != '\0')  {
        mu_assert_equal("Check the first str dataset values.", wdata[i][j], rdata[i][j]);
        ++j;
      }
    }
    // Release resources.
    free(rdata[0]);
    free(rdata);
    mu_assert("HDF5 error in H5Dclose.", H5Dclose(dset) >= 0);
    mu_assert("HDF5 error in H5Sclose.", H5Sclose(space) >= 0);
    mu_assert("HDF5 error in H5Tclose.", H5Tclose(filetype) >= 0);
    mu_assert("HDF5 error in H5Tclose.", H5Tclose(memtype) >= 0);



    // Write a string dataset using strlen.
    mu_assert("Write string dataset using strlen.",
              AH5_write_str_dataset(file_id, "dataset_name_2",
                                    DIM0, strlen(wdata[0]) + 1, wdata));

    // Test the written data using hdf5 API.
    dset = H5Dopen(file_id, "/dataset_name", H5P_DEFAULT);
    filetype = H5Dget_type(dset);
    sdim = H5Tget_size(filetype);
    space = H5Dget_space(dset);
    ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    rdata = (char **) malloc(dims[0] * sizeof (char *));
    rdata[0] = (char *) malloc(dims[0] * sdim * sizeof (char));
    for (i=1; i<dims[0]; i++)
        rdata[i] = rdata[0] + i * sdim;
    memtype = H5Tcopy(H5T_C_S1);
    mu_assert("HDF5 error in H5Tset_size.", H5Tset_size(memtype, sdim) >= 0);
    mu_assert("HDF5 error in H5Dread.",
              H5Dread(dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdata[0]) >= 0);
    for (i = 0; i < dims[0]; i++) {
      /*mu_assert_str_equal("Check the first str dataset values.", wdata[i], rdata[i]);*/
      j = 0;
      while (wdata[i][j] != ' ' && wdata[i][j] != '\0')  {
        mu_assert_equal("Check the first str dataset values.", wdata[i][j], rdata[i][j]);
        ++j;
      }
    }
    // Release resources.
    free(rdata[0]);
    free(rdata);
    mu_assert("HDF5 error in H5Dclose.", H5Dclose(dset) >= 0);
    mu_assert("HDF5 error in H5Sclose.", H5Sclose(space) >= 0);
    mu_assert("HDF5 error in H5Tclose.", H5Tclose(filetype) >= 0);
    mu_assert("HDF5 error in H5Tclose.", H5Tclose(memtype) >= 0);


    // Close file.
    AH5_close_test_file(file_id);

    return MU_FINISHED_WITHOUT_ERRORS;
}


// Run all tests
char *all_tests()
{
  mu_run_test(test_write_string_dataset);

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
