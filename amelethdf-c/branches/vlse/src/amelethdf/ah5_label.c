#include "ah5_label.h"


// Read label dataset
char AH5_read_lbl_dataset(hid_t file_id, const char *path, AH5_lbl_dataset_t *lbl_dataset)
{
    H5T_class_t type_class;
    char rdata = FALSE;
    size_t length;
    int nb_dims;

    lbl_dataset->path = strdup(path);
    lbl_dataset->items = NULL;

    lbl_dataset->nb_items = 1;  // in case of single value
    if (AH5_path_valid(file_id, path))
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path, &(lbl_dataset->nb_items), &type_class, &length) >= 0)
                    if (type_class == H5T_STRING)
                        if(AH5_read_string_dataset(file_id, path, lbl_dataset->nb_items, length, &(lbl_dataset->items)))
                            rdata = TRUE;
    if (!rdata)
    {
        AH5_print_err_dset(AH5_C_LABEL, path);
        lbl_dataset->nb_items = 0;
    }
    return rdata;
}


// Read label category (all datasets)
char AH5_read_label(hid_t file_id, AH5_label_t *label)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH], rdata = TRUE;
    AH5_children_t children;
    hsize_t i;

    label->datasets = NULL;

    if (H5Lexists(file_id, AH5_C_LABEL, H5P_DEFAULT) == TRUE)
    {
        children = AH5_read_children_name(file_id, AH5_C_LABEL);
        label->nb_datasets = children.nb_children;
        if (children.nb_children > 0)
        {
            label->datasets = (AH5_lbl_dataset_t *) malloc(children.nb_children * sizeof(AH5_lbl_dataset_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path, AH5_C_LABEL);
                strcat(path, children.childnames[i]);
                if(!AH5_read_lbl_dataset(file_id, path, label->datasets + i))
                    rdata = FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_LABEL, AH5_C_LABEL);
        rdata = FALSE;
    }
    return rdata;
}




// Print label dataset
void AH5_print_lbl_dataset (const AH5_lbl_dataset_t *lbl_dataset, int space)
{
    hsize_t i;

    printf("%*sName: %s\n", space, "", AH5_get_name_from_path(lbl_dataset->path));
    for (i = 0; i < lbl_dataset->nb_items; i++)
        printf("%*s%s\n", space + 3, "", lbl_dataset->items[i]);
    printf("\n");
}


// Print label category (all datasets)
void AH5_print_label(const AH5_label_t *label)
{
    hsize_t i;

    printf("##################################  Label  ###################################\n\n");
    for (i = 0; i < label->nb_datasets; i++)
        AH5_print_lbl_dataset(&(label->datasets[i]), 0);
    printf("\n");
}




// Free memory used by structure lbl_dataset
void AH5_free_lbl_dataset (AH5_lbl_dataset_t *lbl_dataset)
{
    if (lbl_dataset->path != NULL)
    {
        free(lbl_dataset->path);
        lbl_dataset->path = NULL;
    }

    if (lbl_dataset->items != NULL)
    {
        free(lbl_dataset->items[0]);
        free(lbl_dataset->items);
        lbl_dataset->items = NULL;
        lbl_dataset->nb_items = 0;
    }

}


// Free memory used by structure label (including datasets)
void AH5_free_label (AH5_label_t *label)
{
    hsize_t i;

    if (label->datasets != NULL)
    {
        for (i = 0; i < label->nb_datasets; i++)
            AH5_free_lbl_dataset(label->datasets + i);
        free(label->datasets);
        label->datasets = NULL;
        label->nb_datasets = 0;
    }
}
