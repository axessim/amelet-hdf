#ifndef AH5_DATASET_H
#define AH5_DATASET_H

#include "ah5_general.h"

#ifdef __cplusplus
extern "C" {
#endif

    AH5_PUBLIC char AH5_read_int_dataset(hid_t file_id, const char *path, const hsize_t mn, int **rdata);
    AH5_PUBLIC char AH5_read_flt_dataset(hid_t file_id, const char *path, const hsize_t mn, float **rdata);
    AH5_PUBLIC char AH5_read_cpx_dataset(hid_t file_id, const char *path, const hsize_t mn, AH5_complex_t **rdata);
    AH5_PUBLIC char AH5_read_str_dataset(hid_t file_id, const char *path, hsize_t mn, size_t length, char ***rdata);

    AH5_PUBLIC char AH5_write_int_dataset(hid_t loc_id, const char *dset_name, const hsize_t len, const int *wdata);
    AH5_PUBLIC char AH5_write_flt_dataset(hid_t loc_id, const char *dset_name, const hsize_t len, const float *wdata);
    AH5_PUBLIC char AH5_write_cpx_dataset(hid_t loc_id, const char *dset_name, const hsize_t len, const AH5_complex_t *wdata);
    AH5_PUBLIC char AH5_write_str_dataset(hid_t loc_id, const char *dset_name, const hsize_t len, const size_t slen, char **wdata);

#ifdef __cplusplus
}
#endif

#endif // AH5_DATASET_H
