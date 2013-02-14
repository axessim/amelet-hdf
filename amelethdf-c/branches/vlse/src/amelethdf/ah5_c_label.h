#ifndef AH5_C_LABEL_H
#define AH5_C_LABEL_H

#include "ah5_config.h"
#include "ah5_general.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _AH5_lbl_dataset_t
    {
        char            *path;
        hsize_t         nb_items;
        char            **items;
    } AH5_lbl_dataset_t;

    typedef struct _AH5_label_t
    {
        hsize_t         nb_datasets;
        AH5_lbl_dataset_t *datasets;
    } AH5_label_t;

    AH5_EXPORT void AH5_init_lbl_dataset(AH5_lbl_dataset_t *lbl_dataset);
    AH5_EXPORT void AH5_init_label(AH5_label_t *label);

    AH5_EXPORT char AH5_read_lbl_dataset (hid_t file_id, const char *path, AH5_lbl_dataset_t *lbl_dataset);
    AH5_EXPORT char AH5_read_label (hid_t file_id, AH5_label_t *label);

    AH5_EXPORT void AH5_print_lbl_dataset (const AH5_lbl_dataset_t *lbl_dataset, int space);
    AH5_EXPORT void AH5_print_label (const AH5_label_t *label);

    AH5_EXPORT void AH5_free_lbl_dataset (AH5_lbl_dataset_t *lbl_dataset);
    AH5_EXPORT void AH5_free_label (AH5_label_t *label);

#ifdef __cplusplus
}
#endif

#endif // AH5_C_LABEL_H
