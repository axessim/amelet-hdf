#include "ah5_dataset.h"


// Read 1D int dataset
char AH5_read_int_dataset(hid_t file_id, const char *path, const hsize_t mn, int **rdata)
{
    char success = AH5_FALSE;
    hid_t dset_id;

    *rdata = (int *) malloc(mn * sizeof(int));
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

    *rdata = (float *) malloc(mn * sizeof(float));
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

    *rdata = (AH5_complex_t *) malloc(mn * sizeof(AH5_complex_t));
    buf = (float *) malloc(mn * 2 * sizeof(float));
    type_id = H5Tcreate(H5T_COMPOUND, H5Tget_size(H5T_NATIVE_FLOAT) * 2);
    H5Tinsert(type_id, "r", 0, H5T_NATIVE_FLOAT);
    H5Tinsert(type_id, "i", H5Tget_size(H5T_NATIVE_FLOAT), H5T_NATIVE_FLOAT);
    dset_id = H5Dopen(file_id, path, H5P_DEFAULT);
    if (H5Dread(dset_id, type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, *rdata) >= 0)
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

    *rdata = (char **) malloc(mn * sizeof(char*));
    **rdata = (char *) malloc(mn * length * sizeof(char));
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

