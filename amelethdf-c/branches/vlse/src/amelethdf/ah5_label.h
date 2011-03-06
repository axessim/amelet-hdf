#ifndef AH5_LABEL_H
#define AH5_LABEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ah5_general.h"

    typedef struct AH5_lbl_dataset_t
    {
        char            *path;
        hsize_t         nb_items;
        char            **items;
    } AH5_lbl_dataset_t;

    typedef struct AH5_label_t
    {
        hsize_t         nb_datasets;
        AH5_lbl_dataset_t *datasets;
    } AH5_label_t;

    void AH5_read_lbl_dataset (hid_t file_id, const char *path, AH5_lbl_dataset_t *lbl_dataset);
    void AH5_read_label (hid_t file_id, AH5_label_t *label);

    void AH5_print_lbl_dataset (const AH5_lbl_dataset_t *lbl_dataset, int space);
    void AH5_print_label (const AH5_label_t *label);

    void AH5_free_lbl_dataset (AH5_lbl_dataset_t *lbl_dataset);
    void AH5_free_label (AH5_label_t *label);

#ifdef __cplusplus
}
#endif

#endif // AH5_LABEL_H
