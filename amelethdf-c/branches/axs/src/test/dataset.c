// test path tools

#include <string.h>
#include <stdio.h>

#include <ah5.h>
#include "utest.h"

//! Test suite counter.
int tests_run = 0;

hid_t create_type_id(hid_t real_or_double)
{
    hid_t type_id;
    hid_t type_size, two_type_size;
    herr_t status;

    type_size = H5Tget_size(real_or_double);
    two_type_size = type_size * 2;
    type_id = H5Tcreate(H5T_COMPOUND, two_type_size);
    status = H5Tinsert(type_id, "r", 0, real_or_double);
    status = H5Tinsert(type_id, "i", type_size, real_or_double);
    return type_id;
}

//! Test write dataset.
// Data extracted from http://www.hdfgroup.org/ftp/HDF5/examples/examples-by-api/hdf5-examples/1_8/C/H5T/h5ex_t_string.c
char *test_write_complex_dataset()
{
    hid_t file_id, filetype, memtype, space, dset;
    size_t sdim;
    hsize_t dims[1] = {2};
    int ndims, i, j;
    AH5_complex_t cplx[2];

    file_id = AH5_auto_test_file();
    cplx[0].re=10.;
    cplx[0].im=20.;
    cplx[1].re=10.5;
    cplx[1].im=20.5;

    mu_assert("Write complex dataset.",	
              AH5_write_cpx_dataset(file_id,"dataset_name", 2, cplx));
    // Test the written data using hdf5 API.
    int rank;

    int length;
    size_t type_size;
    float * buf;
    hid_t real_id_type;
    herr_t status;
    hsize_t *newdims;

    real_id_type = create_type_id(H5T_NATIVE_FLOAT);
    status = H5LTget_dataset_ndims(file_id, "dataset_name", &rank);
    newdims = (hsize_t *) malloc(rank * sizeof(hsize_t));
    status = H5LTget_dataset_info(file_id,"dataset_name" , newdims, NULL, NULL);
    length = newdims[0];
    for (i = 1; i < rank; i++)
        length = length * newdims[i];
    buf = (float *) malloc(2 * length * sizeof(float));
    status = H5LTread_dataset(file_id, "dataset_name", real_id_type, buf);
    j = 0;
    for (i = 0; i < length; i++)
    {
    	printf("Real parts : %f %f\n", cplx[i].re, buf[j]);
    	printf("Imaginary parts : %f %f\n", cplx[i].im, buf[j+1]);
    	mu_assert_equal("Check the real values.", cplx[i].re, buf[j]);
    	mu_assert_equal("Check the imaginary value.", cplx[i].im, buf[j+1]);
        j = j + 2;
    }
    free(buf);
    free(newdims);


    return MU_FINISHED_WITHOUT_ERRORS;
}
char *test_read_complex_dataset()
{

	int i,rank = 1;
    hsize_t dims[1];
	hid_t dataspace_id, dset_id, dtr_id, dti_id, file_id;
	size_t type_size;
	hid_t type_id;
	herr_t status, status_2;
	float *real_part, *imag_part;
	const char* path = "dataset_name";
	AH5_complex_t cplx[2];
	AH5_complex_t * rdata;

	file_id = AH5_auto_test_file();

	cplx[0].re=10.;
	cplx[0].im=20.;
	cplx[1].re=10.5;
	cplx[1].im=20.5;
    //first write complex array set with hdf5 lib
	real_part = (float *)malloc(2 * sizeof(float));
    imag_part = (float *)malloc(2 * sizeof(float));
    for( i=0;i<2;i++)
    {
        real_part[i] = cplx[i].re;
        imag_part[i] = cplx[i].im;
    }
    type_id = create_type_id(H5T_NATIVE_FLOAT);
    dims[0] = 2;
    dataspace_id = H5Screate_simple(rank, dims, NULL);
    dset_id = H5Dcreate(file_id,path,type_id,dataspace_id,
                H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    type_size = H5Tget_size(H5T_NATIVE_FLOAT);
    dtr_id = H5Tcreate(H5T_COMPOUND,type_size);
    status = H5Tinsert(dtr_id,"r",0, H5T_NATIVE_FLOAT);
    dti_id = H5Tcreate(H5T_COMPOUND,type_size);
    status = H5Tinsert(dti_id,"i",0, H5T_NATIVE_FLOAT);
    status = H5Dwrite(dset_id,dtr_id,H5S_ALL,H5S_ALL,H5P_DEFAULT,real_part);
    status = H5Dwrite(dset_id,dti_id,H5S_ALL,H5S_ALL,H5P_DEFAULT,imag_part);
    status = H5Tclose(dtr_id);
    status = H5Tclose(dti_id);
    free(real_part);
    free(imag_part);
    mu_assert("Read complex dataset.",
        		AH5_read_cpx_dataset(file_id,"dataset_name", 2, &rdata));

    for (i = 0; i < 2; i++)
    {
      	printf("Real parts : %f %f\n", cplx[i].re, rdata[i].re);
       	printf("Imaginary parts : %f %f\n", cplx[i].im, rdata[i].im);
       	mu_assert_equal("Check the real values.", cplx[i].re, rdata[i].re);
       	mu_assert_equal("Check the imaginary value.", cplx[i].im, rdata[i].im);
    }

    return MU_FINISHED_WITHOUT_ERRORS;
}
char *test_write_string_dataset()
{
#define DIM0 4
#define SDIM 8

    hid_t file_id, filetype, memtype, space, dset;
    size_t sdim;
    hsize_t dims[1] = {DIM0};
    int ndims, i, j;
    /*char wdata[DIM0][SDIM] =*/
    char *wdata[] = {"Parting", "is such",
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
  mu_run_test(test_write_complex_dataset);
  mu_run_test(test_read_complex_dataset);
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
