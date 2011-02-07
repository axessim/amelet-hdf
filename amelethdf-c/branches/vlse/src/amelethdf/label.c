#include "label.h"
// Revision: 7.2.2011


// Read label dataset
void read_label_dataset(hid_t file_id, const char *path, label_dataset_t *label_dataset)
{
    H5T_class_t type_class;
    int nb_dims;
    size_t length;
    char success = FALSE;

    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path, &(label_dataset->nb_items), &type_class, &length) >= 0)
                    if (label_dataset->nb_items > 0 && type_class == H5T_STRING)
                        if(read_string_dataset(file_id, path, label_dataset->nb_items, length, &(label_dataset->items)))
                            success = TRUE;
    if (success)
    {
        label_dataset->name = get_name_from_path(path);
    }
    else
    {
        printf("***** ERROR(%s): Cannot read dataset \"%s\". *****\n\n", C_LABEL, path);
        label_dataset->name = NULL;
        label_dataset->nb_items = 0;
        label_dataset->items = NULL;
    }
}


// Read label category (all datasets)
void read_label(hid_t file_id, label_t *label)
{
    children_t children;
    hsize_t i;
    char *path;

    children = read_children_name(file_id, C_LABEL);
    label->nb_label_datasets = children.nb_children;
    label->label_datasets = NULL;
    if (children.nb_children > 0)
    {
        path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        label->label_datasets = (label_dataset_t *) malloc((size_t) children.nb_children * sizeof(label_dataset_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_LABEL);
            strcat(path, children.childnames[i]);
            read_label_dataset(file_id, path, label->label_datasets + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
        free(path);
    }
}




// Print label dataset
void print_label_dataset (label_dataset_t label_dataset)
{
    hsize_t i;

    printf("Name: %s\n", label_dataset.name);
    for (i = 0; i < label_dataset.nb_items; i++)
        printf("  %s\n", label_dataset.items[i]);
}


// Print label category (all datasets)
void print_label(label_t label)
{
    hsize_t i;

    printf("\n##################################  Label  ###################################\n\n");
    for (i = 0; i < label.nb_label_datasets; i++)
    {
        print_label_dataset(label.label_datasets[i]);
    }
    printf("\n");
}




// Free memory used by structure label_dataset
void free_label_dataset (label_dataset_t *label_dataset)
{
    if (label_dataset->name != NULL)
    {
        free(label_dataset->name);
        label_dataset->name = NULL;
    }

    if (label_dataset->nb_items > 0)
    {
        free(label_dataset->items[0]);
        free(label_dataset->items);
        label_dataset->items = NULL;
        label_dataset->nb_items = 0;
    }

}


// Free memory used by structure label (including label_datasets)
void free_label (label_t *label)
{
    hsize_t i;

    if (label->nb_label_datasets > 0)
    {
        for (i = 0; i < label->nb_label_datasets; i++)
            free_label_dataset(label->label_datasets + i);
        free(label->label_datasets);
        label->label_datasets = NULL;
        label->nb_label_datasets = 0;
    }
}
