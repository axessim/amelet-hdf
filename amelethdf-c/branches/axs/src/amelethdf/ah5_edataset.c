#include "ah5_edataset.h"

hid_t AH5_Get_cpx_type(){
  hid_t cpxtyp = H5Tcreate(H5T_COMPOUND, H5Tget_size(H5T_NATIVE_FLOAT) * 2);

  H5Tinsert(cpxtyp, "r", 0, H5T_NATIVE_FLOAT);

  H5Tinsert(cpxtyp, "i", H5Tget_size(H5T_NATIVE_FLOAT), H5T_NATIVE_FLOAT);

  return cpxtyp;
}



char AH5_create_earray(hid_t loc_id, const char* dset_name,
    const int rank, const hsize_t initialdims[], const hsize_t extendibledims[],
    hid_t mem_type_id, hid_t* dataset){
  hid_t dataspace;
  hid_t prop;
  herr_t status;


  /* Create the data space with extendible dimensions */
  dataspace = H5Screate_simple (rank, initialdims, extendibledims);


  /* Create properties ... */
  prop = H5Pcreate(H5P_DATASET_CREATE);

  /* ... and set it in order to enable chunking. */
  status = H5Pset_chunk(prop, rank, initialdims);

  if(HDF5_FAILED(status)){
    status = H5Sclose(dataspace);
    return AH5_FALSE;
  }

  /* Create the dataset in the file */

  (*dataset) = H5Dcreate(loc_id, dset_name, mem_type_id, dataspace,
      H5P_DEFAULT, prop, H5P_DEFAULT);

  status = H5Pclose(prop);
  status = H5Sclose(dataspace);


  return AH5_TRUE;
}


char AH5_extend_earray(hid_t dataset,
    const int rank, hsize_t dims[], hsize_t extensiondims[]){
  herr_t status;
  int idim;

  for(idim=0;idim<rank;idim++){
    dims[idim] += extensiondims[idim];
  }

  status = H5Dset_extent(dataset, dims);

  HDF5_RETURN_IF_FAILED(status, AH5_FALSE);

  return AH5_TRUE;
}


char AH5_write_array_with_properties(hid_t dataset,
    const int rank, const hsize_t dims[], const hsize_t blockdims[],
    const hsize_t start[], const hsize_t stride[],
    const hsize_t count[], const hsize_t block[],
    const void *data, hid_t mem_type_id, hid_t properties){
  hid_t dataspace;
  hid_t memspace;
  herr_t status;

  /* Get data space */
  dataspace = H5Dget_space(dataset);


  /* Create the memory space */
  memspace = H5Screate_simple(rank, blockdims, NULL);


  /* Select the part to write */
  status = H5Sselect_hyperslab(dataspace,
      H5S_SELECT_SET,
      start,
      stride,
      count,
      block);

  if(HDF5_FAILED(status)){

    H5Sclose(memspace);

    H5Sclose(dataspace);

    return AH5_FALSE;
  }

  /* Write data */
  status = H5Dwrite(dataset, mem_type_id,
      memspace, dataspace, properties, data);


  if(HDF5_FAILED(status)){

    H5Pclose(properties);

    H5Dclose(dataset);

    H5Sclose(memspace);

    H5Sclose(dataspace);

    return AH5_FALSE;
  }

  H5Pclose(properties);

  H5Sclose(memspace);

  H5Sclose(dataspace);

  return AH5_TRUE;
}



char AH5_write_earray(hid_t dataset,
    const int rank, const hsize_t totaldims[], const hsize_t blockdims[],
    const hsize_t start[], const hsize_t stride[],
    const hsize_t count[], const hsize_t block[],
    const void *data, hid_t mem_type_id){
  return AH5_write_array_with_properties(dataset,
      rank, totaldims, blockdims, start, stride, count, block,
      data, mem_type_id, H5P_DEFAULT);
}


void AH5_initialize_Edataset(AH5_Edataset_t* Edataset){
  Edataset->parent        = 0;
  Edataset->path          = NULL;
  Edataset->dataset       = 0;
  Edataset->nb_dims       = 0;
  Edataset->extendibledim = 0;
  Edataset->dims          = NULL;
  Edataset->created       = AH5_FALSE;
  Edataset->access        = AH5_undef;
  Edataset->type_class    = H5T_NO_CLASS;
  Edataset->nature        = NULL;
  Edataset->label         = NULL;
  Edataset->unit          = NULL;
  AH5_initialize_memory_mapping(&(Edataset->mapping));
}


char AH5_create_PEorEdataset(hid_t loc_id,
    const char* name,
    int nb_dims,
    hsize_t dims[],
    hid_t mem_type_id,
    AH5_Edataset_t* Edataset,
    AH5_ACCESS_TYPE access){
  hsize_t i;

  Edataset->parent        = loc_id;
  Edataset->nb_dims       = nb_dims;
  Edataset->dims          = (hsize_t*)malloc(nb_dims * sizeof(hsize_t));
  Edataset->type_class    = mem_type_id;

  Edataset->access        = access;

  Edataset->path = (char*)malloc((strlen(name)+1)*sizeof(char));
  strcpy(Edataset->path, name);

  for(i=0;i<nb_dims;i++){
    Edataset->dims[i] = dims[i];
    if(dims[i] == H5S_UNLIMITED){
      Edataset->dims[i] = 0;
      Edataset->extendibledim = i;
    }
  }

  return AH5_TRUE;

}


char AH5_create_Edataset(hid_t loc_id,
    const char* name,
    int nb_dims,
    hsize_t dims[],
    hid_t mem_type_id,
    AH5_Edataset_t* Edataset){

  return AH5_create_PEorEdataset(loc_id, name,
      nb_dims, dims, mem_type_id, Edataset, AH5_serie);
}


char AH5_create_int_Edataset(hid_t loc_id,
    const char* name,
    int nb_dims,
    hsize_t dims[],
    AH5_Edataset_t* Edataset){
  return AH5_create_Edataset(loc_id, name,
      nb_dims, dims, H5T_NATIVE_INT,
      Edataset);
}



char AH5_create_flt_Edataset(hid_t loc_id,
    const char* name,
    int nb_dims,
    hsize_t dims[],
    AH5_Edataset_t* Edataset){
  return AH5_create_Edataset(loc_id, name,
      nb_dims, dims, H5T_NATIVE_FLOAT,
      Edataset);
}


char AH5_create_str_Edataset(hid_t loc_id,
    const char* name,
    size_t strlength,
    int nb_dims,
    hsize_t dims[],
    AH5_Edataset_t* Edataset){
  hid_t strtype;

  strtype= H5Tcopy(H5T_C_S1);

  HDF5_RETURN_IF_FAILED(H5Tset_size(strtype, strlength),
      AH5_FALSE);

  return AH5_create_Edataset(loc_id, name,
      nb_dims, dims, strtype,
      Edataset);
}



char AH5_create_cpx_Edataset(hid_t loc_id,
    const char* name,
    int nb_dims,
    hsize_t dims[],
    AH5_Edataset_t* Edataset){

  return AH5_create_Edataset(loc_id, name,
      nb_dims, dims, AH5_Get_cpx_type(),
      Edataset);
}




char AH5_set_attr_Edataset(AH5_Edataset_t* Edataset,
    const char* nature,
    const char* unit,
    const char* label){

  if(nature!=NULL){
    Edataset->nature = (char*)malloc((strlen(nature)+1)*sizeof(char));
    strcpy(Edataset->nature, nature);
  }

  if(unit!=NULL){
    Edataset->unit = (char*)malloc((strlen(unit)+1)*sizeof(char));
    strcpy(Edataset->unit, unit);
  }

  if(label!=NULL){
    Edataset->label = (char*)malloc((strlen(label)+1)*sizeof(char));
    strcpy(Edataset->label, label);
  }

  return AH5_TRUE;
}


char AH5_write_attr_Edataset(AH5_Edataset_t* Edataset){
  char status;

  if(Edataset->created == AH5_FALSE){
    return AH5_FALSE;
  }
  if(Edataset->nature != NULL){
    status = AH5_write_str_attr(Edataset->parent, Edataset->path,
                                "physicalNature", Edataset->nature);
    AH5_RETURN_IF_FAILED(status, status);
  }

  if(Edataset->unit != NULL){
    status = AH5_write_str_attr(Edataset->parent, Edataset->path,
                                "unit", Edataset->unit);
    AH5_RETURN_IF_FAILED(status, status);
  }

  if(Edataset->label != NULL){
    status = AH5_write_str_attr(Edataset->parent, Edataset->path,
                                "label", Edataset->label);
    AH5_RETURN_IF_FAILED(status, status);
  }


  return AH5_TRUE;
}


char AH5_append_Edataset(AH5_Edataset_t* Edataset,
    hsize_t sizeappend,
    void* data){

  hsize_t* extendibledims;
  hsize_t* ones;
  hsize_t* offset;
  hsize_t* block;
  hsize_t i;


  extendibledims = (hsize_t*)malloc(Edataset->nb_dims * sizeof(hsize_t));

  if(Edataset->created != AH5_TRUE){

    Edataset->dims[Edataset->extendibledim] += sizeappend;

    for(i=0;i<Edataset->nb_dims;i++){
      extendibledims[i] = Edataset->dims[i];
    }

    extendibledims[Edataset->extendibledim] = H5S_UNLIMITED;

    AH5_RETURN_IF_FAILED(AH5_create_earray(Edataset->parent, Edataset->path,
        Edataset->nb_dims, Edataset->dims, extendibledims,
        Edataset->type_class, &(Edataset->dataset)),
        AH5_FALSE);

    Edataset->created = AH5_TRUE;
  }
  else{

    for(i=0;i<Edataset->nb_dims;i++){
      extendibledims[i] = 0;
    }
    extendibledims[Edataset->extendibledim] = sizeappend;

    AH5_RETURN_IF_FAILED(AH5_extend_earray(Edataset->dataset,
        Edataset->nb_dims, Edataset->dims, extendibledims),
        AH5_FALSE);
  }

  free(extendibledims);

  if(Edataset->access == AH5_serie){
    ones   = (hsize_t*)malloc(Edataset->nb_dims * sizeof(hsize_t));
    offset = (hsize_t*)malloc(Edataset->nb_dims * sizeof(hsize_t));
    block  = (hsize_t*)malloc(Edataset->nb_dims * sizeof(hsize_t));
    for(i=0;i<Edataset->nb_dims;i++){
      ones[i]   = 1;
      offset[i] = 0;
      block[i]  = Edataset->dims[i];
    }
    offset[Edataset->extendibledim] = Edataset->dims[Edataset->extendibledim] -
        sizeappend;
    block[Edataset->extendibledim] = sizeappend;

    AH5_RETURN_IF_FAILED(AH5_write_earray(Edataset->dataset,
        Edataset->nb_dims, Edataset->dims,
        block, offset, ones, ones, block,
        data, Edataset->type_class),
        AH5_FALSE);

    free(ones);
    free(offset);
  }
  else{
//FIXME:
#if AH5_WITH_MPI_
    Edataset->mapping.blockdims[Edataset->extendibledim] = sizeappend;
    Edataset->mapping.block[Edataset->extendibledim] = sizeappend;
    Edataset->mapping.start[Edataset->extendibledim] =
        Edataset->dims[Edataset->extendibledim] - sizeappend;

    AH5_RETURN_IF_FAILED(AH5_write_pearray(Edataset->dataset,
        Edataset->nb_dims, Edataset->dims,
        Edataset->mapping.blockdims, Edataset->mapping.start,
        Edataset->mapping.stride, Edataset->mapping.count,
        Edataset->mapping.block,
        data, Edataset->type_class),
        AH5_FALSE);
#else
      AH5_print_err_dset("Try to use MPI!", "");
      return AH5_FALSE;
#endif
  }

  return AH5_TRUE;
}



char AH5_free_Edataset(AH5_Edataset_t* Edataset){
  char status;

  if(Edataset->dims != NULL){
    free(Edataset->dims);
    Edataset->dims = NULL;
  }

  if(Edataset->path != NULL){
    free(Edataset->path);
        Edataset->path = NULL;
  }

  Edataset->created = AH5_FALSE;
  Edataset->access  = AH5_undef;

  return AH5_free_memory_mapping(&(Edataset->mapping));
}



void AH5_initialize_Earrayset(AH5_Earrayset_t* Earrayset){
  Earrayset->parent     = 0;
  Earrayset->loc        = 0;
  Earrayset->path       = NULL;
  AH5_initialize_Edataset(&(Earrayset->data));
  Earrayset->ds         = 0;
  Earrayset->nb_dims    = 0;
  Earrayset->dims       = NULL;
  Earrayset->type_class = H5T_NO_CLASS;
}



char AH5_create_Earrayset(hid_t loc_id,
    const char* name,
    int nb_dims,
    hsize_t dims[],
    hid_t mem_type_id,
    AH5_Earrayset_t* Earrayset){
  hsize_t i;

  Earrayset->parent     = loc_id;

  Earrayset->path = (char*)malloc((strlen(name) + 1) * sizeof(char));
  strcpy(Earrayset->path, name);

  Earrayset->nb_dims    = nb_dims;

  Earrayset->type_class = mem_type_id;

  Earrayset->loc = H5Gcreate(Earrayset->parent,
      name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  AH5_RETURN_IF_FAILED(AH5_write_str_attr(Earrayset->parent, name,
      "floatingType", "arraySet"), AH5_FALSE);

  Earrayset->ds = H5Gcreate(Earrayset->loc,
      "ds", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  Earrayset->dims = (AH5_Edataset_t*)malloc(nb_dims * sizeof(AH5_Edataset_t));

  for(i=0;i<nb_dims;i++){
    AH5_initialize_Edataset(Earrayset->dims + i);
  }

  /* Create data */
  AH5_initialize_Edataset(&(Earrayset->data));

  AH5_RETURN_IF_FAILED(AH5_create_Edataset(Earrayset->loc, "data",
      nb_dims, dims, mem_type_id, &(Earrayset->data)),
      AH5_FALSE);

  return AH5_TRUE;
}

char AH5_create_int_Earrayset(hid_t loc_id,
    const char* name,
    int nb_dims,
    hsize_t dims[],
    AH5_Earrayset_t* Earrayset){
  return AH5_create_Earrayset(loc_id, name,
      nb_dims, dims, H5T_NATIVE_INT,
      Earrayset);
}

char AH5_create_flt_Earrayset(hid_t loc_id,
    const char* name,
    int nb_dims,
    hsize_t dims[],
    AH5_Earrayset_t* Earrayset){
  return AH5_create_Earrayset(loc_id, name,
      nb_dims, dims, H5T_NATIVE_FLOAT,
      Earrayset);
}


char AH5_create_str_Earrayset(hid_t loc_id,
    const char* name,
    size_t strlength,
    int nb_dims,
    hsize_t dims[],
    AH5_Earrayset_t* Earrayset){
  hid_t strtype;
  strtype= H5Tcopy(H5T_C_S1);

  HDF5_RETURN_IF_FAILED(H5Tset_size(strtype, strlength),
      AH5_FALSE);

  return AH5_create_Earrayset(loc_id, name,
      nb_dims, dims, strtype,
      Earrayset);
}

char AH5_create_cpx_Earrayset(hid_t loc_id,
    const char* name,
    int nb_dims,
    hsize_t dims[],
    AH5_Earrayset_t* Earrayset){
  return AH5_create_Earrayset(loc_id, name,
      nb_dims, dims, AH5_Get_cpx_type(),
      Earrayset);
}


char AH5_set_attr_Earrayset(AH5_Earrayset_t* Earrayset,
    const char* nature,
    const char* unit,
    const char* label){
  return AH5_set_attr_Edataset(&(Earrayset->data),
      nature, unit, label);
}



char AH5_set_dim_Earrayset(AH5_Earrayset_t* Earrayset,
    int idim,
    int nb_dims,
    hsize_t dims[],
    void* data,
    hid_t mem_type_id,
    const char* nature,
    const char* unit,
    const char* label){
  hsize_t extdim = 0;
  hsize_t i;
  hsize_t sizeappend;

  /* build name of dimension */

  char name[50];
  sprintf(name, "dim%d", Earrayset->nb_dims - idim);

  for(i=0;i<nb_dims;i++){
    if(dims[i] == H5S_UNLIMITED){
      extdim = i;
    }
  }
  sizeappend = dims[extdim];
  dims[extdim] = H5S_UNLIMITED;


  AH5_RETURN_IF_FAILED(AH5_create_Edataset(Earrayset->ds, name,
      nb_dims, dims, mem_type_id, &(Earrayset->dims[idim])), AH5_FALSE);

  if(data != NULL){
    AH5_RETURN_IF_FAILED(AH5_append_Edataset(&(Earrayset->dims[idim]),
        sizeappend, data),
        AH5_FALSE);
  }

  return AH5_set_attr_Edataset(&(Earrayset->dims[idim]),
      nature, unit, label);
}


char AH5_set_int_dim_Earrayset(AH5_Earrayset_t* Earrayset,
    int idim,
    int nb_dims,
    hsize_t dims[],
    void* data,
    const char* nature,
    const char* unit,
    const char* label){
  return AH5_set_dim_Earrayset(Earrayset, idim,
      nb_dims, dims, data, H5T_NATIVE_INT,
      nature, unit, label);
}


char AH5_set_flt_dim_Earrayset(AH5_Earrayset_t* Earrayset,
    int idim,
    int nb_dims,
    hsize_t dims[],
    void* data,
    const char* nature,
    const char* unit,
    const char* label){
  return AH5_set_dim_Earrayset(Earrayset, idim,
      nb_dims, dims, data, H5T_NATIVE_FLOAT,
      nature, unit, label);
}


char AH5_set_cpx_dim_Earrayset(AH5_Earrayset_t* Earrayset,
    int idim,
    int nb_dims,
    hsize_t dims[],
    void* data,
    const char* nature,
    const char* unit,
    const char* label){
  return AH5_set_dim_Earrayset(Earrayset, idim,
      nb_dims, dims, data, AH5_Get_cpx_type(),
      nature, unit, label);
}


char AH5_set_str_dim_Earrayset(AH5_Earrayset_t* Earrayset,
    int idim,
    int nb_dims,
    hsize_t dims[],
    size_t strlength,
    void* data,
    const char* nature,
    const char* unit,
    const char* label){
  hid_t strtype;

  strtype = H5Tcopy(H5T_C_S1);

  HDF5_RETURN_IF_FAILED(H5Tset_size(strtype, strlength),
      AH5_FALSE);


  return AH5_set_dim_Earrayset(Earrayset, idim,
      nb_dims, dims, data, strtype,
      nature, unit, label);
}



char AH5_append_Earrayset(AH5_Earrayset_t* Earrayset,
    hsize_t sizeappend,
    void* data,
    void* dimdata){


  AH5_RETURN_IF_FAILED(AH5_append_Edataset(&(Earrayset->data),
      sizeappend, data), AH5_FALSE);

  if(dimdata != NULL){
    AH5_RETURN_IF_FAILED(AH5_append_Edataset(
        Earrayset->dims + Earrayset->data.extendibledim,
        sizeappend, dimdata), AH5_FALSE);
  }

  return AH5_TRUE;
}


char AH5_free_Earrayset(AH5_Earrayset_t* Earrayset){
  int idim;

  AH5_RETURN_IF_FAILED(AH5_free_Edataset(&(Earrayset->data)), AH5_FALSE);

  for(idim=0;idim<Earrayset->nb_dims;idim++){
    AH5_RETURN_IF_FAILED(AH5_free_Edataset(Earrayset->dims + idim), AH5_FALSE);
  }
  free(Earrayset->dims);
  Earrayset->dims = NULL;

  HDF5_RETURN_IF_FAILED(H5Gclose(Earrayset->ds), AH5_FALSE);

  HDF5_RETURN_IF_FAILED(H5Gclose(Earrayset->loc), AH5_FALSE);

  Earrayset->loc        = 0;
  Earrayset->parent     = 0;
  //if(Earrayset->path!=NULL){
  //  free(Earrayset->path);
  //}
  Earrayset->path       = NULL;
  Earrayset->nb_dims    = 0;
  Earrayset->type_class = H5T_NO_CLASS;

  return AH5_TRUE;
}


void AH5_initialize_memory_mapping(AH5_MEMORY_MAPPING_t* mapping){
  mapping->nb_dims   = 0;
  mapping->blockdims = NULL;
  mapping->start     = NULL;
  mapping->stride    = NULL;
  mapping->count     = NULL;
  mapping->block     = NULL;
}


char AH5_set_memory_mapping(
    AH5_MEMORY_MAPPING_t* mapping,
    int nb_dims,
    hsize_t blockdims[],
    hsize_t start[],
    hsize_t stride[],
    hsize_t count[],
    hsize_t block[]){
  hsize_t i;

  mapping->nb_dims = nb_dims;

  mapping->blockdims = (hsize_t*)malloc(nb_dims * sizeof(hsize_t));
  mapping->start     = (hsize_t*)malloc(nb_dims * sizeof(hsize_t));
  mapping->stride    = (hsize_t*)malloc(nb_dims * sizeof(hsize_t));
  mapping->count     = (hsize_t*)malloc(nb_dims * sizeof(hsize_t));
  mapping->block     = (hsize_t*)malloc(nb_dims * sizeof(hsize_t));

  for(i=0;i<nb_dims;i++){
    mapping->blockdims[i] = blockdims[i];
    mapping->start[i]     = start[i];
    mapping->stride[i]    = stride[i];
    mapping->count[i]     = count[i];
    mapping->block[i]     = block[i];
  }

  return AH5_TRUE;
}

char AH5_free_memory_mapping(AH5_MEMORY_MAPPING_t* mapping){
  if(mapping->blockdims != NULL){
    free(mapping->blockdims);
  }
  mapping->blockdims = NULL;

  if(mapping->start != NULL){
    free(mapping->start);
  }
  mapping->start     = NULL;

  if(mapping->stride != NULL){
    free(mapping->stride);
  }
  mapping->stride    = NULL;

  if(mapping->count != NULL){
    free(mapping->count);
  }
  mapping->count     = NULL;

  if(mapping->block != NULL){
    free(mapping->block);
  }
  mapping->block     = NULL;

  mapping->nb_dims   = 0;
  return AH5_TRUE;
}



#ifdef AH5_WITH_MPI_
char AH5_write_parray(hid_t loc_id, const char *dset_name,
    const int rank, const hsize_t totaldims[], const hsize_t blockdims[],
    const hsize_t start[], const hsize_t stride[], const hsize_t count[],
    const hsize_t block[],
    const void *wdata,  hid_t mem_type_id){
  hid_t dataset_id;
  hid_t dataspace_id;
  hid_t memspace_id;
  hid_t plist_id;
  herr_t status;

  /* Create a dataSet */

  /* Create the data space */
  dataspace_id = H5Screate_simple(rank, totaldims, NULL);

  /* Create the memory space */
  memspace_id = H5Screate_simple(rank, blockdims, NULL);


  /* Creta data Set */
  plist_id = H5Pcreate(H5P_DATASET_CREATE);

  H5Pset_chunk(plist_id, rank, blockdims);

  dataset_id = H5Dcreate(loc_id, dset_name,
      mem_type_id, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Pclose(plist_id);
  H5Sclose(dataspace_id);

  /* Get data space */

  dataspace_id = H5Dget_space(dataset_id);

  /* Select the part to write */
  status = H5Sselect_hyperslab(dataspace_id,
      H5S_SELECT_SET,
      start,
      stride,
      count,
      block);

  if(HDF5_FAILED(status)){

    H5Pclose(plist_id);

    H5Dclose(dataset_id);

    H5Sclose(memspace_id);

    H5Sclose(dataspace_id);

    return AH5_FALSE;
  }

  /* Propertiess for collective write */
  plist_id = H5Pcreate(H5P_DATASET_XFER);
  H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);


  /* Write data */
  status = H5Dwrite(dataset_id, mem_type_id,
      memspace_id, dataspace_id, plist_id, wdata);


  if(HDF5_FAILED(status)){

    H5Pclose(plist_id);

    H5Dclose(dataset_id);

    H5Sclose(memspace_id);

    H5Sclose(dataspace_id);

    return AH5_FALSE;
  }

  H5Pclose(plist_id);

  H5Dclose(dataset_id);

  H5Sclose(memspace_id);

  H5Sclose(dataspace_id);

  return AH5_TRUE;
}

char AH5_write_int_parray(hid_t loc_id,
    const char *dset_name,
    const int rank,
    const hsize_t totaldims[],
    const hsize_t blockdims[],
    const hsize_t start[],
    const hsize_t stride[],
    const hsize_t count[],
    const hsize_t block[],
    const int *wdata){
  return AH5_write_parray(loc_id, dset_name,
      rank, totaldims, blockdims, start, stride, count, block,
      (void*)wdata, H5T_NATIVE_INT);
}

char AH5_write_flt_parray(hid_t loc_id,
    const char *dset_name,
    const int rank,
    const hsize_t totaldims[],
    const hsize_t blockdims[],
    const hsize_t start[],
    const hsize_t stride[],
    const hsize_t count[],
    const hsize_t block[],
    const float *wdata){
  return AH5_write_parray(loc_id, dset_name,
      rank, totaldims, blockdims, start, stride, count, block,
      (void*)wdata, H5T_NATIVE_FLOAT);
}


char AH5_write_str_parray(hid_t loc_id,
    const char *dset_name,
    const int rank,
    const hsize_t totaldims[],
    const hsize_t blockdims[],
    const hsize_t slen,
    const hsize_t start[],
    const hsize_t stride[],
    const hsize_t count[],
    const hsize_t block[],
    const char *wdata){
  hid_t atype = H5Tcopy(H5T_C_S1);
  H5Tset_size(atype, slen);

  return AH5_write_parray(loc_id, dset_name,
      rank, totaldims, blockdims, start, stride, count, block,
      (void*)wdata, atype);
}


char AH5_create_PEdataset(hid_t loc_id,
    const char* name,
    hsize_t nb_dims,
    hsize_t dims[],
    hid_t mem_type_id,
    AH5_Edataset_t* Edataset){

  return AH5_create_PEorEdataset(loc_id, name,
      nb_dims, dims, mem_type_id, Edataset, AH5_parallel);
}


char AH5_create_int_PEdataset(hid_t loc_id,
    const char* name,
    hsize_t nb_dims,
    hsize_t dims[],
    AH5_Edataset_t* Edataset){
  return AH5_create_PEdataset(loc_id, name,
      nb_dims, dims, H5T_NATIVE_INT, Edataset);
}



char AH5_create_flt_PEdataset(hid_t loc_id,
    const char* name,
    hsize_t nb_dims,
    hsize_t dims[],
    AH5_Edataset_t* Edataset){
  return AH5_create_PEdataset(loc_id, name,
      nb_dims, dims, H5T_NATIVE_FLOAT, Edataset);
}



char AH5_create_str_PEdataset(hid_t loc_id,
    const char* name,
    size_t strlength,
    hsize_t nb_dims,
    hsize_t dims[],
    AH5_Edataset_t* Edataset){
  hid_t strtype;

  strtype = H5Tcopy(H5T_C_S1);

  HDF5_RETURN_IF_FAILED(H5Tset_size(strtype, strlength),
      AH5_FALSE);

  return AH5_create_PEdataset(loc_id, name,
      nb_dims, dims, strtype, Edataset);
}


char AH5_create_cpx_PEdataset(hid_t loc_id,
    const char* name,
    hsize_t nb_dims,
    hsize_t dims[],
    AH5_Edataset_t* Edataset){
  return AH5_create_PEdataset(loc_id, name,
      nb_dims, dims, AH5_Get_cpx_type(), Edataset);
}





char AH5_set_mpi_mapping_PEdataset(
    AH5_Edataset_t* Edataset,
    hsize_t blockdims[],
    hsize_t start[],
    hsize_t stride[],
    hsize_t count[],
    hsize_t block[]){
  if(Edataset->nb_dims == 0){
    return AH5_FALSE;
  }

  AH5_RETURN_IF_FAILED(AH5_set_memory_mapping(&(Edataset->mapping),
      Edataset->nb_dims, blockdims, start, stride, count, block),
      AH5_FALSE);

  Edataset->mapping.blockdims[Edataset->extendibledim] = 0;
  Edataset->mapping.block[Edataset->extendibledim] = 0;
  Edataset->mapping.start[Edataset->extendibledim] = 0;

  return AH5_TRUE;
}



char AH5_append_PEdataset(AH5_Edataset_t* Edataset,
    hsize_t sizeappend, hsize_t blockdims[],
    hsize_t start[], hsize_t stride[],
    hsize_t count[], hsize_t block[],
    void* data){
  char status;
  hsize_t* extensiondims;
  hsize_t i;

  if(Edataset->created == AH5_FALSE){

    Edataset->dims[Edataset->extendibledim] += sizeappend;

    extensiondims = malloc(Edataset->nb_dims * sizeof(hsize_t));

    for(i=0;i<Edataset->nb_dims;i++){
      extensiondims[i] = Edataset->dims[i];
    }
    extensiondims[Edataset->extendibledim] = H5S_UNLIMITED;

    status = AH5_create_earray(Edataset->parent, Edataset->path,
        Edataset->nb_dims, Edataset->dims, extensiondims, Edataset->type_class,
        &(Edataset->dataset));

    AH5_RETURN_IF_FAILED(status, status);

    free(extensiondims);
    Edataset->created = AH5_TRUE;
  }
  else{
    extensiondims = malloc(Edataset->nb_dims * sizeof(hsize_t));

    for(i=0;i<Edataset->nb_dims;i++){
      extensiondims[i] = 0;
    }
    extensiondims[Edataset->extendibledim] = sizeappend;

    status = AH5_extend_earray(Edataset->dataset,
        Edataset->nb_dims, Edataset->dims, extensiondims);

    AH5_RETURN_IF_FAILED(status, status);

    free(extensiondims);
  }

  AH5_RETURN_IF_FAILED(AH5_write_pearray(Edataset->dataset,
      Edataset->nb_dims, Edataset->dims, blockdims,
      start, stride, count, block,
      data, Edataset->type_class), status);

  return AH5_TRUE;
}



char AH5_set_mpi_mapping_PEarrayset(AH5_Earrayset_t* Earrayset,
    hsize_t blockdims[],
    hsize_t start[],
    hsize_t stride[],
    hsize_t count[],
    hsize_t block[]){
  return AH5_set_mpi_mapping_PEdataset(&(Earrayset->data),
      blockdims, start, stride, count, block);
}



char AH5_append_PEarrayset(AH5_Earrayset_t* Earrayset,
    hsize_t sizeappend,
    hsize_t blockdims[],
    hsize_t start[],
    hsize_t stride[],
    hsize_t count[],
    hsize_t block[],
    void* data,
    void* dimdata){

  char status;

  status = AH5_append_PEdataset(&(Earrayset->data),
      sizeappend,
      blockdims, start,
      stride, count,
      block,
      data);

  AH5_RETURN_IF_FAILED(status, status);

  if(dimdata != NULL){
    status = AH5_append_Edataset(
        Earrayset->dims + Earrayset->data.extendibledim,
        sizeappend, dimdata);
  }

  return status;

}

char AH5_create_PEarrayset(hid_t loc_id,
    const char* name,
    hsize_t nb_dims,
    hsize_t dims[],
    hid_t mem_type_id,
    AH5_Earrayset_t* Earrayset){
  hsize_t i;

  Earrayset->parent     = loc_id;

  Earrayset->path = malloc(strlen(name));
  strcpy(Earrayset->path, name);

  Earrayset->nb_dims    = nb_dims;

  Earrayset->type_class = mem_type_id;

  Earrayset->loc = H5Gcreate(Earrayset->parent,
      name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  AH5_RETURN_IF_FAILED(AH5_write_str_attr(Earrayset->parent, name,
      "floatingType", "arraySet"), AH5_FALSE);

  Earrayset->ds = H5Gcreate(Earrayset->loc,
      "ds", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  Earrayset->dims = malloc(nb_dims * sizeof(AH5_Edataset_t));

  for(i=0;i<nb_dims;i++){
    AH5_initialize_Edataset(Earrayset->dims + i);
  }

  /* Create data */
  AH5_initialize_Edataset(&(Earrayset->data));

  AH5_RETURN_IF_FAILED(AH5_create_PEdataset(Earrayset->loc, "data",
      nb_dims, dims, mem_type_id, &(Earrayset->data)),
      AH5_FALSE);

  return AH5_TRUE;
}

char AH5_create_int_PEarrayset(hid_t loc_id,
    const char* name,
    hsize_t nb_dims,
    hsize_t dims[],
    AH5_Earrayset_t* Earrayset){
  return AH5_create_Earrayset(loc_id, name,
      nb_dims, dims, H5T_NATIVE_INT,
      Earrayset);
}

char AH5_create_flt_PEarrayset(hid_t loc_id,
    const char* name,
    hsize_t nb_dims,
    hsize_t dims[],
    AH5_Earrayset_t* Earrayset){
  return AH5_create_Earrayset(loc_id, name,
      nb_dims, dims, H5T_NATIVE_FLOAT,
      Earrayset);
}

char AH5_create_str_PEarrayset(hid_t loc_id,
    const char* name,
    size_t strlength,
    hsize_t nb_dims,
    hsize_t dims[],
    AH5_Earrayset_t* Earrayset){
  hid_t strtype;

  strtype = H5Tcopy(H5T_C_S1);

  HDF5_RETURN_IF_FAILED(H5Tset_size(strtype, strlength),
      AH5_FALSE);

  return AH5_create_Earrayset(loc_id, name,
      nb_dims, dims, strtype,
      Earrayset);
}

char AH5_create_cpx_PEarrayset(hid_t loc_id,
    const char* name,
    hsize_t nb_dims,
    hsize_t dims[],
    AH5_Earrayset_t* Earrayset){
  return AH5_create_Earrayset(loc_id, name,
      nb_dims, dims, AH5_Get_cpx_type(),
      Earrayset);
}
#endif // End AH5_WITH_MPI_
