#include "ah5_dataset.h"


// Read 1D int dataset
char AH5_read_int_dataset(hid_t file_id, const char *path, const hsize_t mn, int **rdata)
{
    char success = AH5_FALSE;
    hid_t dset_id;

    *rdata = (int *) malloc((size_t) mn * sizeof(int));
    dset_id = H5Dopen(file_id, path, H5P_DEFAULT);
    if (H5Dread(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, *rdata) >= 0)
        success = AH5_TRUE;
    H5Dclose(dset_id);
    if (!success)
    {
        free(*rdata);
        *rdata = NULL;
    }
    return success;
}


// Read 1D float dataset
char AH5_read_flt_dataset(hid_t file_id, const char *path, const hsize_t mn, float **rdata)
{
    char success = AH5_FALSE;
    hid_t dset_id;

    *rdata = (float *) malloc((size_t) mn * sizeof(float));
    dset_id = H5Dopen(file_id, path, H5P_DEFAULT);
    if (H5Dread(dset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, *rdata) >= 0)
        success = AH5_TRUE;
    H5Dclose(dset_id);
    if (!success)
    {
        free(*rdata);
        *rdata = NULL;
    }
    return success;
}


// Read 1D complex float dataset
char AH5_read_cpx_dataset(hid_t file_id, const char *path, const hsize_t mn, AH5_complex_t **rdata)
{
    char success = AH5_FALSE;
    hid_t dset_id, type_id;
    hsize_t i;
    float *buf;

    *rdata = (AH5_complex_t *) malloc((size_t) mn * sizeof(AH5_complex_t));
    buf = (float *) malloc((size_t) mn * 2 * sizeof(float));
    type_id = AH5_H5Tcreate_cpx_memtype();

    dset_id = H5Dopen(file_id, path, H5P_DEFAULT);

    if (H5Dread(dset_id, type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf) >= 0)
    {
        success = AH5_TRUE;
        for (i = 0; i < mn; i++)
            rdata[0][i] = AH5_set_complex(buf[2*i], buf[2*i+1]);
    }
    H5Dclose(dset_id);
    H5Tclose(type_id);
    free(buf);
    if (!success)
    {
        free(*rdata);
        *rdata = NULL;
    }
    return success;
}


// Read 1D string dataset
char AH5_read_str_dataset(hid_t file_id, const char *path, const hsize_t mn, size_t length, char ***rdata)
{
    char success = AH5_FALSE;
    hid_t dset_id, memtype;
    hsize_t i;

    length++; // make a space for the null terminator
    dset_id = H5Dopen(file_id, path, H5P_DEFAULT);

    *rdata = (char **) malloc((size_t) mn * sizeof(char*));
    **rdata = (char *) malloc((size_t) mn * length * sizeof(char));
    for (i = 1; i < mn; i++)
      rdata[0][i] = rdata[0][0] + i * length;
    memtype = H5Tcopy(H5T_C_S1);
    if (H5Tset_size(memtype, length) >= 0)
        if (H5Dread(dset_id, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, **rdata) >= 0)
            success = AH5_TRUE;
    H5Tclose(memtype);
    H5Dclose(dset_id);
    if (!success)
    {
        free(**rdata);
        free(*rdata);
        *rdata = NULL;
    }
    return success;
}

// Write 1D char dataset
char AH5_write_char_dataset(hid_t loc_id, const char *dset_name, const hsize_t len, const char *wdata)
{
    hsize_t dims[1] = {len};
    char success = AH5_FALSE;

    // TODO: add check of loc_id using H5Oget_info()
    if(H5LTmake_dataset_char(loc_id, dset_name, 1, dims, wdata) >= 0)
        success = AH5_TRUE;
    return success;
}

// Write 1D int dataset
char AH5_write_int_dataset(hid_t loc_id, const char *dset_name, const hsize_t len, const int *wdata)
{
    char success = AH5_FALSE;
    hsize_t dims[1] = {len};
    H5O_info_t info;

    H5Oget_info(loc_id, &info);
    if (info.type == H5O_TYPE_GROUP)
        if (H5LTmake_dataset_int(loc_id, dset_name, 1, dims, wdata) >= 0)
            success = AH5_TRUE;
    return success;
}


// Write 1D float dataset
char AH5_write_flt_dataset(hid_t loc_id, const char *dset_name, const hsize_t len, const float *wdata)
{
    char success = AH5_FALSE;
    hsize_t dims[1] = {len};
    H5O_info_t info;

    H5Oget_info(loc_id, &info);
    if (info.type == H5O_TYPE_GROUP)
        if (H5LTmake_dataset_float(loc_id, dset_name, 1, dims, wdata) >= 0)
            success = AH5_TRUE;
    return success;
}


// Write 1D complex float dataset
char AH5_write_cpx_dataset(hid_t loc_id, const char *dset_name, const hsize_t len, const AH5_complex_t *wdata)
{
    hid_t cpx_filetype, cpx_memtype, space, dset;
    char success = AH5_FALSE;
    hsize_t dims[1] = {len};
    H5O_info_t info;

    H5Oget_info(loc_id, &info);
    if (info.type == H5O_TYPE_GROUP)
    {
        cpx_filetype = AH5_H5Tcreate_cpx_filetype();
        cpx_memtype = AH5_H5Tcreate_cpx_memtype();
        space = H5Screate_simple (1, dims, NULL);

        if ((dset = H5Dcreate(loc_id, dset_name, cpx_filetype, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT)) >= 0)
            if (H5Dwrite(dset, cpx_memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata) >= 0)
                success = AH5_TRUE;

        H5Dclose(dset);
        H5Sclose(space);
        H5Tclose(cpx_memtype);
        H5Tclose(cpx_filetype);
    }

    return success;
}


// Write 1D string dataset
// wdata[len][slen]; param slen: string length with null char.
// TODO Check HDF5 return code.
char AH5_write_str_dataset(hid_t loc_id, const char *dset_name, const hsize_t len, const size_t slen, char **wdata)
{
    char success = AH5_FALSE;
    hid_t filetype, memtype, space, dset;
    hsize_t dims[1] = {len}, k;
    char *buf = NULL;
    
    buf = (char *) malloc((size_t) len*(slen-1)+1 * sizeof(char));
    buf[0] = '\0';
    for (k = 0; k < len; k++)  {
      printf("***************** AH5_write_str_dataset s[%d]=%s\n", k, wdata[k]);
      strcat(buf + (k*(slen-1)), wdata[k]);
    }

    filetype = H5Tcopy(AH5_NATIVE_STRING);
    H5Tset_size(filetype, slen);
    memtype = H5Tcopy(H5T_C_S1);
    H5Tset_size(memtype, slen-1);
    space = H5Screate_simple(1, dims, NULL);

    if ((dset = H5Dcreate(loc_id, dset_name, filetype, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT)) >= 0)
        if (H5Dwrite(dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf) >= 0)
            success = AH5_TRUE;

    H5Dclose(dset);
    H5Sclose(space);
    H5Tclose(memtype);
    H5Tclose(filetype);
    free(buf);

    return success;
}


// Write nD int dataset
char AH5_write_int_array(hid_t loc_id, const char *dset_name, const int rank, const hsize_t dims[], const int *wdata)
{
    char success = AH5_FALSE;
    hid_t dataset_id, dataspace_id;

    dataspace_id = H5Screate_simple(rank, dims, NULL);
    dataset_id = H5Dcreate(loc_id, dset_name, AH5_NATIVE_INT, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata) >= 0)
        success = AH5_TRUE;
    H5Dclose(dataset_id);
    H5Sclose(dataspace_id);
    return success;
}


// Write nD float dataset
char AH5_write_flt_array(hid_t loc_id, const char *dset_name, const int rank, const hsize_t dims[], const float *wdata)
{
    char success = AH5_FALSE;
    hid_t dataset_id, dataspace_id;

    dataspace_id = H5Screate_simple(rank, dims, NULL);
    dataset_id = H5Dcreate(loc_id, dset_name, AH5_NATIVE_FLOAT, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (H5Dwrite(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata) >= 0)
        success = AH5_TRUE;
    H5Dclose(dataset_id);
    H5Sclose(dataspace_id);
    return success;
}


// Write nD complex float dataset
char AH5_write_cpx_array(hid_t loc_id, const char *dset_name, const int rank, const hsize_t dims[], const AH5_complex_t *wdata)
{
    char success = AH5_FALSE;

    // TO BE IMPLEMENTED...
    AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED("Array", dset_name);
    
    return success;
}


// Write 1D string dataset
char AH5_write_str_array(hid_t loc_id, const char *dset_name, const int rank, const hsize_t dims[], char *wdata)
{
    char success = AH5_FALSE;

    // TO BE IMPLEMENTED...
    AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED("Array", dset_name);
    
    return success;
}
