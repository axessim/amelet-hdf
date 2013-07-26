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

char* low_level_test(hid_t hdf, int mpirank){
  // Parallel and extendible dataset
  hsize_t initialdims[] = {
      1, 2, 3
  };

  hsize_t extdims[] = {
      H5S_UNLIMITED, 2, 3
  };

  hid_t dataset;
  char status = AH5_create_earray(hdf, "testextnparal",
      3, initialdims, extdims, H5T_NATIVE_INT, &dataset);

  AH5_RETURN_IF_FAILED(status, "Creation of extendible array failed.\n");

  hsize_t blockdims[] = {
      1, 1, 3
  };

  // Offset ineach dimension
  hsize_t start[] = {
      0, mpirank, 0
  };

  // ???
  hsize_t stride[] = {
      1, 1, 1
  };

  // ???
  hsize_t count[] = {
      1, 1, 1
  };

  // ???
  hsize_t block[] = {
      1, 1, 3
  };

  int* idata = malloc(3 * sizeof(int));

  int i;
  for(i=0;i<3;i++)
    idata[i] = mpirank;

  AH5_write_pearray(dataset,
      3, initialdims, blockdims,
      start, stride,
      count, block,
      idata, H5T_NATIVE_INT);


  extdims[0] = 1;
  extdims[1] = 0;
  extdims[2] = 0;

  AH5_extend_earray(dataset, 3, initialdims, extdims);


  start[0] = 1;


  for(i=0;i<3;i++)
    idata[i] = mpirank + 10;

  AH5_write_pearray(dataset,
      3, initialdims, blockdims,
      start, stride,
      count, block,
      idata, H5T_NATIVE_INT);

  free(idata);

  H5Dclose(dataset);


  return NULL;
}


char* test_dataset(hid_t hdf, int mpirank){
  hsize_t i;
  char status;

  AH5_Edataset_t dataset;

  AH5_initialize_Edataset(&dataset);

  hsize_t dims[] = {
      3, 2, H5S_UNLIMITED
  };

  AH5_RETURN_IF_FAILED(AH5_create_int_PEdataset(hdf, "pedataset",
      3, dims, &dataset),
      "Creation of PEdataset failed.\n");

  hsize_t block[]  = {3, 1, 0};
  hsize_t offset[] = {0, mpirank, 0};
  hsize_t ones[]   = {1, 1, 1};

  AH5_RETURN_IF_FAILED(AH5_set_mpi_mapping_PEdataset(&dataset,
      block, offset, ones, ones, block),
      "Memory mapping setting failed.\n");


  status = AH5_TRUE;
  for(i=0;i<3;i++){
    if(dataset.mapping.blockdims[i] != block[i]  ||
        dataset.mapping.block[i]    != block[i]  ||
        dataset.mapping.start[i]    != offset[i] ||
        dataset.mapping.count[i]    != 1         ||
        dataset.mapping.stride[i]   != 1){
      status = AH5_FALSE;
    }
  }
  AH5_RETURN_IF_FAILED(status, "Memory mapping of PEdataset not correctly set.\n");


  hsize_t maxdim = 261;
  int* data = malloc(3 * sizeof(int));
  for(i=0;i<maxdim;i++){
    int j;
    for(j=0;j<3;j++){
      data[j] = 2 * i + mpirank;
    }

    AH5_RETURN_IF_FAILED(AH5_append_Edataset(&dataset,
        1, data),
        "Append to dataset failed.\n");
  }
  free(data);


  AH5_RETURN_IF_FAILED(AH5_free_Edataset(&dataset),
      "Free PEdataset failed.\n");


  // Verif
  data = malloc(maxdim * 6 * sizeof(int));

  dims[2] = maxdim;

  int iu;
  int iv;
  int iw;
  for(iu=0;iu<3;iu++){
    for(iv=0;iv<2;iv++){
      for(iw=0;iw<maxdim;iw++){
        hsize_t rank[] = {
            iu, iv, iw
        };
        hsize_t id = Index(3, dims, rank);

        data[id] = 2 * iw + iv;
      }
    }
  }

  AH5_RETURN_IF_FAILED(Verify(hdf, "pedataset", maxdim * 6, data),
      "Verif PEdataset failed.\n");

  free(data);

  return NULL;
}



char* test_arrayset(hid_t hdf, int mpirank){
  AH5_Earrayset_t arrayset;

  AH5_initialize_Earrayset(&arrayset);

  hsize_t dims[] = {
      H5S_UNLIMITED, 2
  };

  AH5_RETURN_IF_FAILED(AH5_create_PEarrayset(hdf, "peArraySet",
      2, dims, H5T_NATIVE_INT, &arrayset),
      "Creation PEarrayset failed.\n");


  hsize_t block[] = {1, 1};
  hsize_t start[] = {0, mpirank};
  hsize_t ones[]  = {1, 1};
  AH5_RETURN_IF_FAILED(AH5_set_mpi_mapping_PEarrayset(&arrayset,
      block, start, ones, ones, block),
      "Set mapping PEarrayset failed.\n");

  AH5_RETURN_IF_FAILED(AH5_set_int_dim_Earrayset(&arrayset, 0,
      1, dims, NULL,
      "coucou nature", NULL, NULL),
      "Set dim 0 PEarrayset failed.\n");

  dims[0] = 2;
  int datadim[] = {0, 1};
  AH5_RETURN_IF_FAILED(AH5_set_int_dim_Earrayset(&arrayset, 1,
      1, dims, datadim, "mpirank", NULL, NULL),
      "Set dim 1 PEarrayset failed.\n");


  int maxdim = 30;
  int i;
  for(i=0;i<maxdim;i++){
    int data[] = {2* i + mpirank};
    datadim[0] = i;
    AH5_RETURN_IF_FAILED(AH5_append_Earrayset(&arrayset,
        1, data, datadim),
        "Extension PEarrayset failed.\n");
  }

  AH5_RETURN_IF_FAILED(AH5_free_Earrayset(&arrayset),
      "Free PEarrayset failed.\n");

  // Verif

  int* data = malloc(maxdim * 2 * sizeof(int));

  dims[0] = maxdim;
  dims[1] = 2;

  int iu;
  int iv;
  for(iu=0;iu<maxdim;iu++){
    for(iv=0;iv<2;iv++){
      hsize_t rank[] = {
          iu, iv
      };
      hsize_t id = Index(2, dims, rank);

      data[id] = 2 * iu + iv;
    }
  }



  AH5_RETURN_IF_FAILED(Verify(hdf, "peArraySet/data", maxdim * 2, data),
      "Verif PEarrayset failed.\n");

  free(data);

  return NULL;
}


int main(int argc, char** argv){

  MPI_Init(&argc, &argv);

  int mpirank;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

  int nbprocess;
  MPI_Comm_size(MPI_COMM_WORLD, &nbprocess);

  if(nbprocess != 2){
    print(mpirank, "Please relaunch with two process.\n");
    return EXIT_FAILURE;
  }


  // Open properties of the file

  hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);

  MPI_Info info = MPI_INFO_NULL;

  // Create the pro

  herr_t status = H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, info);

  // Create the file
  hid_t hdf = H5Fcreate("ut_paral.h5", H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);

  // close the properties
  H5Pclose(plist_id);


  char* message;

  message = low_level_test(hdf, mpirank);

  if(message!=NULL){
    printf("%d** %s\n", mpirank, message);
    return EXIT_FAILURE;
  }


  message = test_dataset(hdf, mpirank);

  if(message!=NULL){
    printf("%d** %s\n", mpirank, message);
    return EXIT_FAILURE;
  }


  message = test_arrayset(hdf, mpirank);

  if(message!=NULL){
    printf("%d** %s\n", mpirank, message);
    return EXIT_FAILURE;
  }


  H5Fclose(hdf);

  MPI_Finalize();

  print(mpirank, "SUCCESS\n");

  return EXIT_SUCCESS;
}
