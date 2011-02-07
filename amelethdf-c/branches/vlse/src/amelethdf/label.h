#ifndef LABEL_H
#define LABEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"

    typedef struct label_dataset_t
    {
        char        *name;
        hsize_t     nb_items;
        char        **items;
    } label_dataset_t;

    typedef struct label_t
    {
        hsize_t     nb_label_datasets;
        label_dataset_t *label_datasets;
    } label_t;

    void read_label_dataset(hid_t file_id, const char *path, label_dataset_t *label_dataset);
    void read_label(hid_t file_id, label_t *label);

    void print_label_dataset (label_dataset_t label_dataset);
    void print_label(label_t label);

    void free_label_dataset (label_dataset_t *label_dataset);
    void free_label (label_t *label);

#ifdef __cplusplus
}
#endif

#endif // LABEL_H
