#ifndef LABEL_H
#define LABEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"

    typedef struct lbl_dataset_t
    {
        char        *name;
        hsize_t     nb_items;
        char        **items;
    } lbl_dataset_t;

    typedef struct label_t
    {
        hsize_t     nb_datasets;
        lbl_dataset_t *datasets;
    } label_t;

    void read_lbl_dataset (hid_t file_id, const char *path, lbl_dataset_t *lbl_dataset);
    void read_label (hid_t file_id, label_t *label);

    void print_lbl_dataset (lbl_dataset_t lbl_dataset, int space);
    void print_label (label_t label);

    void free_lbl_dataset (lbl_dataset_t *lbl_dataset);
    void free_label (label_t *label);

#ifdef __cplusplus
}
#endif

#endif // LABEL_H
