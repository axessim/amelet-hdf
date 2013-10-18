#include <stdio.h>

#include "utest.h"
#include <ah5.h>
#include <ah5_edataset.h>


char Verify(hid_t loc_id, const char* path, hsize_t size, int* ref){
  // Read the dataset we have just written

//  printf("Verify %s : %d\n", path, (int)size);

  int* data;
  int i;
  char status = AH5_read_int_dataset(loc_id, path, size, &data);

  if(status!=AH5_TRUE){
    return status;
  }

  
  for(i=0;i<size;i++){
//    printf("%s %d %d\n", path, ref[i], data[i]);

    if(data[i] != ref[i]){
      status = AH5_FALSE;
    }
  }

  free(data);

  return status;
}




static char* test_low_level(hid_t hdf){
  // Create the Earray

  hid_t dataset;

  hsize_t dims[] = {2, 1};

  hsize_t extdims[] = {2, H5S_UNLIMITED };

  hsize_t ones [] = {1, 1};

  hsize_t block[] = {2, 1};

  hsize_t start[] = {0, 0};

  int i;
  int maxdim = 3;
  hsize_t size;
  int* data;
  int iu;
  int iv;

  char status = AH5_create_earray(hdf, // parent
      "ll_array",                          // name
      2,                                   // nb of dimensions
      dims,                                // dimension at creation
      extdims,                             // max dimension
      H5T_NATIVE_INT,
      &dataset);                           // dataset

  mu_assert("Creation of Earray failed.\n", status==AH5_TRUE);


  // Extend dataset

  extdims[0] = 0;
  extdims[1] = 1;

  
  for(i=0;i<maxdim;i++){
    int value[] = {
        2 + i, 2 + i
    };

    if(i!=0){
      status = AH5_extend_earray(dataset, 2, dims, extdims);

      mu_assert("Extension of Earray failed.\n", status==AH5_TRUE);
    }

    mu_assert("Dims of Earray have not been updated.\n", dims[1]==i+1);


    start[1] = i;
    status = AH5_write_earray(dataset, 2, dims,
        block, start, ones, ones, block, value, H5T_NATIVE_INT);

    mu_assert("Write extension of Earray failed.\n", status==AH5_TRUE);

  }

  // Close the dataset

  H5Dclose(dataset);

  // Read the dataset we have just written
  size = dims[0] * dims[1];
  data = malloc(size * sizeof(int));

  for(iu=0;iu<dims[0];iu++){
    for(iv=0;iv<dims[1];iv++){
      i = iu * dims[1] + iv;
      data[i] = iv+2;
    }
  }

  status = Verify(hdf, "ll_array", size, data);

  free(data);

  mu_assert("Error in the written data.\n", status==AH5_TRUE);

  return NULL;

}



static char* test_chararray(hid_t hdf){
  hid_t dataset;

  int rank = 2;
  int dim0 = 2;

  hsize_t dims[] = {dim0, 1};

  hsize_t extdims[] = {2, H5S_UNLIMITED };

  hsize_t ones [] = {1, 1};

  hsize_t block[] = {dim0, 1};

  hsize_t start[] = {0, 0};
  int maxdim = 3;
  int i;

  const char* initval;

  hid_t strtype = H5Tcopy(H5T_C_S1);
  herr_t err = H5Tset_size(strtype, 7);

  char status = AH5_create_earray(hdf, // parent
      "ll_strarray",                          // name
      rank,                                   // nb of dimensions
      dims,                                // dimension at creation
      extdims,                             // max dimension
      strtype,
      &dataset);                           // dataset

  mu_assert("Creation of strEarray failed.\n", status==AH5_TRUE);


  initval = "coucou0""coucou1";
  start[1] = 0;
  status = AH5_write_earray(dataset, rank, dims,
      block, start, ones, ones, block, initval, strtype);

  mu_assert("Write 0 extension of strEarray failed.\n", status==AH5_TRUE);


  // Extend dataset

  extdims[0] = 0;
  extdims[1] = 1;

  for(i=0;i<maxdim;i++){
    char value[20];
    sprintf(value, "coucou%d""coucou%d", 2*(i+1), 2*(i+1)+1);

    status = AH5_extend_earray(dataset, rank, dims, extdims);

    mu_assert("Extension of strEarray failed.\n", status==AH5_TRUE);

//    mu_assert("Dims of strEarray have not been updated.\n", dims[1]==i+1);


    start[1] = i+1;
    status = AH5_write_earray(dataset, rank, dims,
        block, start, ones, ones, block, value, strtype);

    mu_assert("Write extension of strEarray failed.\n", status==AH5_TRUE);

  }

  // Close the dataset

  H5Dclose(dataset);

  return NULL;
}







static char* test_Edataset(hid_t hdf){

  // create an empty extendible vector
  AH5_Edataset_t v;

  int i, j, k, p, l;

  int* data;
  char status;

  hsize_t size = 0;

  hsize_t dims[] = {
      3, H5S_UNLIMITED
  };

  AH5_initialize_Edataset(&v);
  status = AH5_create_Edataset(hdf, "e_array",
      2, dims, H5T_NATIVE_INT, &v);

  mu_assert("Creation of Edataset failed.\n", status==AH5_TRUE);

  status = AH5_set_attr_Edataset(&v,
      "mynature", "coucou", NULL);

  // extend it
  
  for(i=0;i<4;i++){

    data = malloc(3 * (i+1) * sizeof(int));

    for(j=0;j<3*(i+1);j++){
      data[j] = i+1;
    }

    status = AH5_append_Edataset(&v, i+1, data);

    mu_assert("Append to Edataset failed (method).\n", status==AH5_TRUE);

    size += i+1;

    mu_assert("Append to Edataset failed (update dims).\n", v.dims[1]==size);

    free(data);
  }

  size *= 3;

  status = AH5_free_Edataset(&v);

  mu_assert("Free of Edataset failed.\n", status==AH5_TRUE);


  // Verify the written data

  data = (int*)malloc(size * sizeof(int));

  l = 0;
  for(i=0;i<4;i++){
    for(j=0;j<i+1;j++){
      for(k=0;k<3;k++){
        p = l + (size / 3) * k;
        data[p] = i+1;
      }
      l++;
    }
  }

  status = Verify(hdf, "e_array", size, data);

  free(data);

  mu_assert("Error in the written data.\n", status==AH5_TRUE);

  return NULL;
}




static char* test_Earrayset(hid_t hdf){

  char status;

  AH5_Earrayset_t a;

  hsize_t i = 0;

  hsize_t dims[] = {
      H5S_UNLIMITED, 3
  };

  // Set dimension
  hsize_t dims0[] = {0};

  hsize_t dims1[] = {3};

  int datadim[] = {1, 2, 3};

  int appdata[] = { 3*i, 3*i+1, 3*i+2 };

  int valdim;

  // Initialize
  AH5_initialize_Earrayset(&a);


  // Create
  status = AH5_create_Earrayset(hdf, "extarrayset",
      2, dims, H5T_NATIVE_INT, &a);

  mu_assert("Creation of Earrayset failed.\n", status==AH5_TRUE);

  

  status = AH5_set_dim_Earrayset(&a, 0,
      1, dims0, NULL, H5T_NATIVE_INT,
      NULL, NULL, NULL);

  mu_assert("Set dim 0 of Earrayset failed.\n", status==AH5_TRUE);

  

  status = AH5_set_dim_Earrayset(&a, 1,
      1, dims1, datadim, H5T_NATIVE_INT,
      "coucou", NULL, NULL);

  mu_assert("Set dim 1 of Earrayset failed.\n", status==AH5_TRUE);


  // Extend
  for(i=0;i<5;i++){
    valdim = 10 * i;

    status = AH5_append_Earrayset(&a, 1, appdata, &valdim);

    mu_assert("Extension of Earrayset failed.\n", status==AH5_TRUE);
  }

  // Free
  status = AH5_free_Earrayset(&a);

  mu_assert("Free of Earrayset failed.\n", status==AH5_TRUE);

  return NULL;
}




int main(int argc, char** argv){

  hid_t hdf = H5Fcreate("ut_ext_array.h5",
      H5F_ACC_TRUNC,
      H5P_DEFAULT,
      H5P_DEFAULT);

  char* message = test_low_level(hdf);
  if(message != NULL){
    H5Fclose(hdf);
    printf("%s", message);
    return EXIT_FAILURE;
  }

  message = test_chararray(hdf);
  if(message != NULL){
    H5Fclose(hdf);
    printf("%s", message);
    return EXIT_FAILURE;
  }

  message = test_Edataset(hdf);
  if(message != NULL){
    H5Fclose(hdf);
    printf("%s", message);
    return EXIT_FAILURE;
  }

  message = test_Earrayset(hdf);
  if(message != NULL){
    H5Fclose(hdf);
    printf("%s", message);
    return EXIT_FAILURE;
  }

  H5Fclose(hdf);
  printf("SUCCESS\n");

  return EXIT_SUCCESS;
}
