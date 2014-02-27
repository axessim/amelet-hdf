#ifndef _AMALETDATASET_WRITE_H
#define _AMALETDATASET_WRITE_H

#define HDF5_FAILED(status) ((status)<0)
#define AH5_FAILED(status) ((status)!=AH5_TRUE)
#define RETURN_IF_FAILED(test, ret) if(test){return ret;}
#define AH5_RETURN_IF_FAILED(status, ret) RETURN_IF_FAILED(AH5_FAILED(status), ret)
#define HDF5_RETURN_IF_FAILED(status, ret) RETURN_IF_FAILED(HDF5_FAILED(status), ret)

#include <ah5_config.h>
#include "ah5.h"

#ifdef AH5_WITH_MPI_
#include <mpi.h>
#error "mpi pas bien!"
#endif


#ifdef __cplusplus
extern "C" {
#endif

  typedef enum _AH5_ACCESS_TYPE{
    AH5_serie, AH5_parallel, AH5_undef
  }
  AH5_ACCESS_TYPE;

  // Memory mapping for parallel write.
  typedef struct _AH5_MEMORY_MAPPING_t{
    hsize_t  nb_dims;
    hsize_t* blockdims;
    hsize_t* start;
    hsize_t* stride;
    hsize_t* count;
    hsize_t* block;
  }
  AH5_MEMORY_MAPPING_t;

  typedef struct _AH5_Edataset_t
  {
    hid_t                parent;
    char*                path;
    hid_t                dataset;
    int                  nb_dims;
    hsize_t              extendibledim;
    hsize_t*             dims;
    hid_t                type_class;
    char                 created;
    AH5_ACCESS_TYPE      access;
    AH5_MEMORY_MAPPING_t mapping;
    char*                nature;
    char*                label;
    char*                unit;
  }
  AH5_Edataset_t;

  typedef struct _AH5_Earrayset_t
  {
    hid_t           parent;
    hid_t           loc;
    char*           path;
    AH5_Edataset_t  data;
    hid_t           ds;
    int             nb_dims;
    AH5_Edataset_t* dims;
    hid_t           type_class;
  }
  AH5_Earrayset_t;



  AH5_PUBLIC hid_t AH5_Get_cpx_type();


  /* Write extendible array
     TODO : str, complex */

  AH5_PUBLIC char AH5_create_earray(hid_t loc_id, const char* dset_name,
      const int rank, const hsize_t initialdims[], const hsize_t extendibledims[],
      hid_t mem_type_id, hid_t *dataset);

  AH5_PUBLIC char AH5_create_earray(hid_t loc_id, const char* dset_name,
      const int rank, const hsize_t initialdims[], const hsize_t extendibledims[],
      hid_t mem_type_id, hid_t *dataset);

  AH5_PUBLIC char AH5_extend_earray(hid_t dataset,
      const int rank, hsize_t dims[], hsize_t extensiondims[]);


  AH5_PUBLIC char AH5_write_array_with_properties(hid_t dataset,
      const int rank, const hsize_t dims[], const hsize_t blockdims[],
      const hsize_t start[], const hsize_t stride[],
      const hsize_t count[], const hsize_t block[],
      const void *data, hid_t mem_type_id, hid_t properties);

  AH5_PUBLIC char AH5_write_earray(hid_t dataset,
      const int rank, const hsize_t dims[], const hsize_t blockdims[],
      const hsize_t start[], const hsize_t stride[],
      const hsize_t count[], const hsize_t block[],
      const void *data, hid_t mem_type_id);

  AH5_PUBLIC char AH5_write_pearray(hid_t dataset,
      const int rank, const hsize_t totaldims[], const hsize_t blockdims[],
      const hsize_t start[], const hsize_t stride[],
      const hsize_t count[], const hsize_t block[],
      const void *data, hid_t mem_type_id);




  /* Edataset methods*/



  AH5_PUBLIC void AH5_initialize_Edataset(AH5_Edataset_t* Edataset);

  AH5_PUBLIC char AH5_create_PEorEdataset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      int nb_dims,                  /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      hid_t mem_type_id,            /* data type*/
      AH5_Edataset_t* Edataset,     /* pointer to AH5_Edataset_t*/
      AH5_ACCESS_TYPE access);      /* access type*/


  AH5_PUBLIC char AH5_create_Edataset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      int nb_dims,                  /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      hid_t mem_type_id,            /* data type*/
      AH5_Edataset_t* Edataset);    /* pointer to AH5_Edataset_t*/

  AH5_PUBLIC char AH5_create_int_Edataset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      int nb_dims,                  /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Edataset_t* Edataset);    /* pointer to AH5_Edataset_t*/

  AH5_PUBLIC char AH5_create_flt_Edataset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      int nb_dims,                  /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Edataset_t* Edataset);    /* pointer to AH5_Edataset_t*/

  AH5_PUBLIC char AH5_create_str_Edataset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      size_t strlength,             /* max length of the string*/
      int nb_dims,                  /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Edataset_t* Edataset);    /* pointer to AH5_Edataset_t*/


  AH5_PUBLIC char AH5_create_cpx_Edataset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      int nb_dims,                  /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Edataset_t* Edataset);    /* pointer to AH5_Edataset_t*/



  /* Set the physicalNature, unit and label attrs of the dataset.*/
  /* If NULL is given, the attr is not created.*/
  AH5_PUBLIC char AH5_set_attr_Edataset(
      AH5_Edataset_t* Edataset,     /* pointer to AH5_Edataset_t*/
      const char* nature,           /* physicalNature*/
      const char* unit,             /* unit*/
      const char* label);           /* label*/

  /* The dataset must have ever been created yet. */
  AH5_PUBLIC char AH5_write_attr_Edataset(
      AH5_Edataset_t* Edataset);     /* pointer to AH5_Edataset_t*/

  AH5_PUBLIC char AH5_append_Edataset(
      AH5_Edataset_t* Edataset,     /* pointer to AH5_Edataset_t*/
      hsize_t sizeappend,           /* number of dim to append*/
      void* data);                  /* data to append*/

  AH5_PUBLIC char AH5_free_Edataset(AH5_Edataset_t* Edataset);



  /* Earrayset methods*/

  AH5_PUBLIC void AH5_initialize_Earrayset(AH5_Earrayset_t* Earrayset);

  AH5_PUBLIC char AH5_create_Earrayset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      int nb_dims,                  /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      hid_t mem_type_id,            /* data type*/
      AH5_Earrayset_t* Earrayset);  /* pointer to AH5_Earrayset_t*/

  AH5_PUBLIC char AH5_create_int_Earrayset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      int nb_dims,                  /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Earrayset_t* Earrayset);  /* pointer to AH5_Earrayset_t*/

  AH5_PUBLIC char AH5_create_flt_Earrayset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      int nb_dims,                  /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Earrayset_t* Earrayset);  /* pointer to AH5_Earrayset_t*/

  AH5_PUBLIC char AH5_create_str_Earrayset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      size_t strlength,             /* Max length of the strings*/
      int nb_dims,                  /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Earrayset_t* Earrayset);  /* pointer to AH5_Earrayset_t*/

  AH5_PUBLIC char AH5_create_cpx_Earrayset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      int nb_dims,                  /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Earrayset_t* Earrayset);  /* pointer to AH5_Earrayset_t*/



  AH5_PUBLIC char AH5_set_attr_Earrayset(
      AH5_Earrayset_t* Earrayset,   /* pointer to AH5_Earrayset_t*/
      const char* nature,           /* physicalNature*/
      const char* unit,             /* unit*/
      const char* label);           /* label*/


  AH5_PUBLIC char AH5_set_dim_Earrayset(
      AH5_Earrayset_t* Earrayset,   /* pointer to AH5_Earrayset_t*/
      int idim,                     /* dimension id in C (>=0)*/
      int nb_dims,                  /* Number of dimensions of the dataset*/
      hsize_t dims[],               /* Size of each dimension*/
      void* data,                   /* data (NULL for a void dataset)*/
      hid_t mem_type_id,            /* type*/
      const char* nature,           /* physicalNature*/
      const char* unit,             /* unit*/
      const char* label);           /* label*/

  AH5_PUBLIC char AH5_set_int_dim_Earrayset(
      AH5_Earrayset_t* Earrayset,   /* pointer to AH5_Earrayset_t*/
      int idim,                     /* dimension id in C (>=0)*/
      int nb_dims,                  /* Number of dimensions of the dataset*/
      hsize_t dims[],               /* Size of each dimension*/
      void* data,                   /* data (NULL for a void dataset)*/
      const char* nature,           /* physicalNature*/
      const char* unit,             /* unit*/
      const char* label);           /* label*/

  AH5_PUBLIC char AH5_set_flt_dim_Earrayset(
      AH5_Earrayset_t* Earrayset,   /* pointer to AH5_Earrayset_t*/
      int idim,                     /* dimension id in C (>=0)*/
      int nb_dims,                  /* Number of dimensions of the dataset*/
      hsize_t dims[],               /* Size of each dimension*/
      void* data,                   /* data (NULL for a void dataset)*/
      const char* nature,           /* physicalNature*/
      const char* unit,             /* unit*/
      const char* label);           /* label*/

  AH5_PUBLIC char AH5_set_str_dim_Earrayset(
      AH5_Earrayset_t* Earrayset,   /* pointer to AH5_Earrayset_t*/
      int idim,                     /* dimension id in C (>=0)*/
      int nb_dims,                  /* Number of dimensions of the dataset*/
      hsize_t dims[],               /* Size of each dimension*/
      size_t strlength,             /* Max length of str */
      void* data,                   /* data (NULL for a void dataset)*/
      const char* nature,           /* physicalNature*/
      const char* unit,             /* unit*/
      const char* label);           /* label*/

  AH5_PUBLIC char AH5_set_cpx_dim_Earrayset(
      AH5_Earrayset_t* Earrayset,   /* pointer to AH5_Earrayset_t*/
      int idim,                     /* dimension id in C (>=0)*/
      int nb_dims,                  /* Number of dimensions of the dataset*/
      hsize_t dims[],               /* Size of each dimension*/
      void* data,                   /* data (NULL for a void dataset)*/
      const char* nature,           /* physicalNature*/
      const char* unit,             /* unit*/
      const char* label);           /* label*/

  AH5_PUBLIC char AH5_append_Earrayset(
      AH5_Earrayset_t* Earrayset,   /* pointer to AH5_Earrayset_t*/
      hsize_t sizeappend,           /* number of dim to append*/
      void* data,                   /* data to append*/
      void* dimdata);               /* data to append to extendible dim (can be NULL)*/

  AH5_PUBLIC char AH5_free_Earrayset(AH5_Earrayset_t* Earrayset);



    /* Memory mapping*/
  AH5_PUBLIC void AH5_initialize_memory_mapping(AH5_MEMORY_MAPPING_t* mapping);

  AH5_PUBLIC char AH5_set_memory_mapping(
      AH5_MEMORY_MAPPING_t* mapping,
      int nb_dims,
      hsize_t blockdims[],
      hsize_t start[],
      hsize_t stride[],
      hsize_t count[],
      hsize_t block[]);

  AH5_PUBLIC char AH5_free_memory_mapping(AH5_MEMORY_MAPPING_t* mapping);



//  Toot for write extending dataset with mpi.
#ifdef AH5_WITH_MPI_

  /* Parallel write (Base functions)*/
  AH5_PUBLIC char AH5_write_parray(hid_t loc_id,
      const char* dset_name,
      const int rank,
      const hsize_t totaldims[],
      const hsize_t blockdims[],
      const hsize_t start[],
      const hsize_t stride[],
      const hsize_t count[],
      const hsize_t block[],
      const void *wdata,
      hid_t mem_type_id);

  AH5_PUBLIC char AH5_write_int_parray(hid_t loc_id, const char *dset_name,
      const int rank, const hsize_t totaldims[], const hsize_t blockdims[],
      const hsize_t start[], const hsize_t stride[], const hsize_t count[],
      const hsize_t block[],
      const int *wdata);

  AH5_PUBLIC char AH5_write_flt_parray(hid_t loc_id, const char *dset_name,
      const int rank, const hsize_t totaldims[], const hsize_t blockdims[],
      const hsize_t start[], const hsize_t stride[], const hsize_t count[],
      const hsize_t block[],
      const float *wdata);

  AH5_PUBLIC char AH5_write_str_parray(hid_t loc_id, const char *dset_name,
      const int rank, const hsize_t totaldims[], const hsize_t blockdims[],
      const hsize_t slen,
      const hsize_t start[], const hsize_t stride[], const hsize_t count[],
      const hsize_t block[],
      const char *wdata);


  /* Parallel extendible dataset*/


  /* Create a VOID data set, dims[extendibledim] MUST be null*/
  AH5_PUBLIC char AH5_create_PEdataset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      hsize_t nb_dims,              /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim can be 0*/
      hid_t mem_type_id,            /* data type*/
      AH5_Edataset_t* Edataset);    /* pointer to AH5_Edataset_t*/

  AH5_PUBLIC char AH5_create_int_PEdataset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      hsize_t nb_dims,              /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim can be 0*/
      AH5_Edataset_t* Edataset);    /* pointer to AH5_Edataset_t*/

  AH5_PUBLIC char AH5_create_flt_PEdataset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      hsize_t nb_dims,              /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim can be 0*/
      AH5_Edataset_t* Edataset);    /* pointer to AH5_Edataset_t*/

  AH5_PUBLIC char AH5_create_str_PEdataset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      size_t strlength,             /* Max length of the strings*/
      hsize_t nb_dims,              /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim can be 0*/
      AH5_Edataset_t* Edataset);    /* pointer to AH5_Edataset_t*/

  AH5_PUBLIC char AH5_create_cpx_PEdataset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      hsize_t nb_dims,              /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim can be 0*/
      AH5_Edataset_t* Edataset);    /* pointer to AH5_Edataset_t*/



  /* Set the part of the array to write per each mpi process*/
  /* In the extensible dimension the value should be 1*/
  AH5_PUBLIC char AH5_set_mpi_mapping_PEdataset(
      AH5_Edataset_t* Edataset,     /* pointer to AH5_Edataset_t*/
      hsize_t blockdims[],          /* size of the block to write (depends on the mpi process)*/
      hsize_t start[],              /* offset to write*/
      hsize_t stride[],
      hsize_t count[],              /*    See hdf5 documentation*/
      hsize_t block[]);


  /* Append to extendible parallel dataset*/
  AH5_PUBLIC char AH5_append_PEdataset(
      AH5_Edataset_t* Edataset,     /* pointer to AH5_Edataset_t*/
      hsize_t sizeappend,           /* number of dim to append*/
      hsize_t blockdims[],          /* size of the block to write (depends on the mpi process)*/
      hsize_t start[],              /* offset to write*/
      hsize_t stride[],
      hsize_t count[],              /*    See hdf5 documentation*/
      hsize_t block[],
      void* data);                  /* data to append*/

  /* The array start, stride, count and block are describe in the hfd5 documentation:
   *  see http://www.hdfgroup.org/HDF5/doc/RM/RM_H5S.html#Dataspace-SelectHyperslab
   * If you want to add data such like :
   *
   *                   |
   *        Process 0  |      Process 1
   *                   |
   *     -------------------------------
   *                   |
   *        Process 2  |      Process 3
   *                   |
   * start : offset in each dimension, in the extensible dimension it should
   *         equal the current number of values
   * stride and count : Equal 1 in all dimension
   * block : dimension of the block to write for the current process*/



  /* Parallel and extendible arrayset*/

  AH5_PUBLIC char AH5_create_PEarrayset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      hsize_t nb_dims,              /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      hid_t mem_type_id,            /* data type*/
      AH5_Earrayset_t* Earrayset);  /* pointer to AH5_Earrayset_t*/

  AH5_PUBLIC char AH5_create_int_PEarrayset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      hsize_t nb_dims,              /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Earrayset_t* Earrayset);  /* pointer to AH5_Earrayset_t*/

  AH5_PUBLIC char AH5_create_flt_PEarrayset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      hsize_t nb_dims,              /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Earrayset_t* Earrayset);  /* pointer to AH5_Earrayset_t*/

  AH5_PUBLIC char AH5_create_str_PEarrayset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      size_t strlength,             /* Max length of the strings*/
      hsize_t nb_dims,              /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Earrayset_t* Earrayset);  /* pointer to AH5_Earrayset_t*/

  AH5_PUBLIC char AH5_create_cpx_PEarrayset(
      hid_t loc_id,                 /* Parent*/
      const char* name,             /* name or path*/
      hsize_t nb_dims,              /* number of dimensions*/
      hsize_t dims[],               /* size of dimensions, the extendible dim must be H5S_UNLIMITED*/
      AH5_Earrayset_t* Earrayset);  /* pointer to AH5_Earrayset_t*/



  /* Set the part of the array to write per each mpi process*/
  /* In the extensible dimension the value should be 1*/
  AH5_PUBLIC char AH5_set_mpi_mapping_PEarrayset(
      AH5_Earrayset_t* Earrayset,   /* pointer to AH5_Earrayset_t*/
      hsize_t blockdims[],          /* size of the block to write (depends on the mpi process)*/
      hsize_t start[],              /* offset to write*/
      hsize_t stride[],
      hsize_t count[],              /*    See hdf5 documentation*/
      hsize_t block[]);


  /* Append to extendible parallel arrayset*/
  AH5_PUBLIC char AH5_append_PEarrayset(
      AH5_Earrayset_t* Earrayset,   /* pointer to AH5_Edataset_t*/
      hsize_t sizeappend,           /* number of dim to append*/
      hsize_t blockdims[],          /* size of the block to write (depends on the mpi process)*/
      hsize_t start[],              /* offset to write*/
      hsize_t stride[],
      hsize_t count[],              /*    See hdf5 documentation*/
      hsize_t block[],
      void* data,                   /* data to append*/
      void* dimdata);               /* data to append to extendible dim (can be NULL)*/
#endif // End AH5_WITH_MPI_

#ifdef __cplusplus
}
#endif

#endif


