#include <stdio.h>

#include <mpi.h>
#include <hdf5.h>
#include <utest.h>
#include <ameletdataset_write.h>

#define print(rank, ...) if((rank)==0)printf(__VA_ARGS__)

hsize_t Index(int nb_dims, const hsize_t dims[], const hsize_t rank[]){
  hsize_t id = rank[0];
  int idim;
  for(idim=1;idim<nb_dims;idim++){
    id *= dims[idim];
    id += rank[idim];
  }
  return id;
}

char Verify(hid_t loc_id, const char* path, const hsize_t size, int* ref){
    // Read the dataset we have just written

  int* data;

  char status = AH5_read_int_dataset(loc_id, path, size, &data);

  if(status!=AH5_TRUE){
    return status;
  }

  int i;
  for(i=0;i<size;i++){
    if(data[i] != ref[i]){
      status = AH5_FALSE;
    }
  }

  free(data);

  return status;
}






char* test_assym_low(hid_t hdf, int mpirank){

  // The process 0 does not write anything.
  // The process 1 writes one column
  // The process 2 writes two column

  hid_t dataset;

  hsize_t dims[] = {
      1, 3
  };
  hsize_t edims[] = {
      H5S_UNLIMITED, 3
  };

  AH5_RETURN_IF_FAILED(AH5_create_earray(hdf, "pearray",
      2, dims, edims, H5T_NATIVE_INT, &dataset),
      "Creation PEarrayset failed.\n");


  hsize_t block[] = {1, mpirank};


  hsize_t start[] = {0, 0};

  if(mpirank!=0){
    start[1] = mpirank - 1;
  }

  hsize_t ones[]  = {1, 1};

  int* data = NULL;
  if(mpirank!=0){
    data = malloc(mpirank * sizeof(int));

    int j;
    for(j=0;j<mpirank;j++){
      data[j] = 0;
    }
  }



  AH5_RETURN_IF_FAILED(AH5_write_pearray(dataset,
        2, dims, block, start, ones, ones, block, data, H5T_NATIVE_INT),
        "Extend array failed.\n");

  if(mpirank!=0){
    free(data);
  }



  int maxdim = 2;
  int i;
  for(i=0;i<maxdim;i++){
    edims[0] = 1;
    edims[1] = 0;
    AH5_RETURN_IF_FAILED(AH5_extend_earray(dataset, 2, dims, edims),
        "Extend array failed.\n");

    data = NULL;
    if(mpirank!=0){
      data = malloc(mpirank * sizeof(int));

      int j;
      for(j=0;j<mpirank;j++){
        data[j] = i+1;
      }
    }

    start[0] = dims[0] - 1;

    AH5_RETURN_IF_FAILED(AH5_write_pearray(dataset,
        2, dims, block, start, ones, ones, block, data, H5T_NATIVE_INT),
        "Extend array failed.\n");

    if(mpirank!=0){
      free(data);
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }

  H5Dclose(dataset);

//  // Verif
//
//  int* data = malloc(maxdim * 2 * sizeof(int));
//
//  dims[0] = maxdim;
//  dims[1] = 2;
//
//  int iu;
//  int iv;
//  for(iu=0;iu<maxdim;iu++){
//    for(iv=0;iv<2;iv++){
//      hsize_t rank[] = {
//          iu, iv
//      };
//      hsize_t id = Index(2, dims, rank);
//
//      data[id] = 2 * iu + iv;
//    }
//  }
//
//
//
//  AH5_RETURN_IF_FAILED(Verify(hdf, "peArraySet/data", maxdim * 2, data),
//      "Verif PEdataset failed.\n");
//
//  free(data);

  return NULL;
}














char* test_assym(hid_t hdf, int mpirank){

  // The process 0 does not write anything.
  // The process 1 writes one column
  // The process 2 writes two column

  AH5_Earrayset_t arrayset;

  AH5_initialize_Earrayset(&arrayset);

  hsize_t dims[] = {
      H5S_UNLIMITED, 3
  };

  AH5_RETURN_IF_FAILED(AH5_create_PEarrayset(hdf, "peArraySet",
      2, dims, H5T_NATIVE_INT, &arrayset),
      "Creation PEarrayset failed.\n");


  hsize_t block[] = {1, mpirank};

  hsize_t start[] = {0, 0};
  if(mpirank != 0){
    start[1] = mpirank - 1;
  }

  hsize_t ones[]  = {1, 1};

  AH5_RETURN_IF_FAILED(AH5_set_mpi_mapping_PEarrayset(&arrayset,
      block, start, ones, ones, block),
      "Set mapping PEarrayset failed.\n");

  AH5_RETURN_IF_FAILED(AH5_set_int_dim_Earrayset(&arrayset,
      0, 1, dims, NULL, "coucou nature", NULL, NULL),
      "Set dim 0 PEarrayset failed.\n");

  dims[0] = 3;
  int datadim[] = {1, 2, 2};
  AH5_RETURN_IF_FAILED(AH5_set_int_dim_Earrayset(&arrayset,
      1, 1, dims, datadim, "mpirank", NULL, NULL),
      "Set dim 1 PEarrayset failed.\n");


  int maxdim = 20;
  int i;
  for(i=0;i<maxdim;i++){

    int* data = NULL;
    if(mpirank!=0){
      data = malloc(mpirank * sizeof(int));

      int j;
      for(j=0;j<mpirank;j++){
        data[j] = 3 * i + j + mpirank - 1;
      }
    }

    datadim[0] = i;
    AH5_RETURN_IF_FAILED(AH5_append_Earrayset(&arrayset,
        1, data, datadim),
        "Extension PEarrayset failed.\n");


    if(mpirank!=0){
      free(data);
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }

  AH5_RETURN_IF_FAILED(AH5_free_Earrayset(&arrayset),
      "Free PEarrayset failed.\n");

  // Verif

  int* refdata = malloc(maxdim * 3 * sizeof(int));

  dims[0] = maxdim;
  dims[1] = 3;

  int iu;
  int iv;
  for(iu=0;iu<maxdim;iu++){
    for(iv=0;iv<3;iv++){
      hsize_t rank[] = { iu, iv };

      hsize_t id = Index(2, dims, rank);

      refdata[id] = 3 * iu + iv;
    }
  }



  AH5_RETURN_IF_FAILED(Verify(hdf, "peArraySet/data", maxdim * 3, refdata),
      "Verif PEdataset failed.\n");

  free(refdata);

  return NULL;
}








int main(int argc, char** argv){

  MPI_Init(&argc, &argv);

  int mpirank;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

  int nbprocess;
  MPI_Comm_size(MPI_COMM_WORLD, &nbprocess);

  if(nbprocess != 3){
    print(mpirank, "Please relaunch with three process.\n");
    return EXIT_FAILURE;
  }


  // Open properties of the file

  hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);

  MPI_Info info = MPI_INFO_NULL;

  // Create the pro

  herr_t status = H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, info);

  // Create the file
  hid_t hdf = H5Fcreate("ut_paral2.h5", H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);

  // close the properties
  H5Pclose(plist_id);



  char* message;



  message = test_assym_low(hdf, mpirank);

  if(message!=NULL){
    printf("%d** %s\n", mpirank, message);
    return EXIT_FAILURE;
  }







  message = test_assym(hdf, mpirank);

  if(message!=NULL){
    printf("%d** %s\n", mpirank, message);
    return EXIT_FAILURE;
  }


  H5Fclose(hdf);

  MPI_Finalize();

  print(mpirank, "SUCCESS\n");

  return EXIT_SUCCESS;
}
