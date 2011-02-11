#ifndef EXTERNALELEMENT_H
#define EXTERNALELEMENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"
#include <unistd.h>

#define EE_INTERNAL_NAME(id) 3*id
#define EE_EXTERNAL_FILE_NAME(id) 3*id + 1
#define EE_EXTERNAL_NAME(id) 3*id + 2

    typedef struct ee_dataset_t
    {
        char        *name;
        hid_t       *file_id;
        hsize_t     nb_eed_items;
        char        **eed_items;
    } ee_dataset_t;

    typedef struct external_element_t
    {
        hsize_t     nb_ee_datasets;
        ee_dataset_t *ee_datasets;
    } external_element_t;


    void read_external_element_dataset (hid_t file_id, const char *path, ee_dataset_t *ee_dataset);
    void read_external_element (hid_t file_id, external_element_t *external_element);

    void print_external_element_dataset (ee_dataset_t ee_dataset, int space);
    void print_external_element (external_element_t external_element);

    void free_external_element_dataset (ee_dataset_t *ee_dataset);
    void free_external_element (external_element_t *external_element);

    char open_external_files(ee_dataset_t *ee_dataset);
    char close_external_files(ee_dataset_t *ee_dataset);

#ifdef __cplusplus
}
#endif

#endif // EXTERNALELEMENT_H
