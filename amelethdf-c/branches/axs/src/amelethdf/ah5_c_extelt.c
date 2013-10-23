#include "ah5_c_extelt.h"

// for access function
#ifdef _WIN32
# include <io.h>
# include <errno.h>
#else
# include <unistd.h>
#endif

// Read dataset in externalElement and open external files
char AH5_read_eet_dataset (hid_t file_id, const char *path, AH5_eet_dataset_t *eet_dataset)
{
    H5T_class_t type_class;
    char rdata = AH5_FALSE;
    hsize_t dims[2], i;
    size_t length;
    int nb_dims;

    eet_dataset->path = strdup(path);

    if (AH5_path_valid(file_id, path))
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims == 2)
                if (H5LTget_dataset_info(file_id, path, dims, &type_class, &length) >= 0)
                    if (dims[0] > 0 && dims[1] == 3 && type_class == H5T_STRING)
                        if(AH5_read_str_dataset(file_id, path, dims[0] * dims[1], length, &(eet_dataset->eed_items)))
                        {
                            eet_dataset->nb_eed_items = dims[0];
                            rdata = AH5_TRUE;
                            eet_dataset->file_id = (hid_t *) malloc(eet_dataset->nb_eed_items * sizeof(hid_t));
                            for (i = 0; i < eet_dataset->nb_eed_items; i++)
                                eet_dataset->file_id[i] = -1;
                        }
    if (!rdata)
    {
        AH5_print_err_dset(AH5_C_EXTERNAL_ELEMENT, path);
        eet_dataset->nb_eed_items = 0;
        eet_dataset->file_id = NULL;
        eet_dataset->eed_items = NULL;
    }
    else
        if (!AH5_open_external_files(eet_dataset))
            rdata = AH5_FALSE;
    return rdata;
}


// Read externalElement category (all datasets)
char AH5_read_external_element (hid_t file_id, AH5_external_element_t *external_element)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH], rdata = AH5_TRUE;
    AH5_children_t children;
    hsize_t i;

    external_element->datasets = NULL;

    if (AH5_path_valid(file_id, AH5_C_EXTERNAL_ELEMENT))
    {
        children = AH5_read_children_name(file_id, AH5_C_EXTERNAL_ELEMENT);
        external_element->nb_datasets = children.nb_children;
        if (children.nb_children > 0)
        {
            external_element->datasets = (AH5_eet_dataset_t *) malloc(children.nb_children * sizeof(AH5_eet_dataset_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path, AH5_C_EXTERNAL_ELEMENT);
                strcat(path, children.childnames[i]);
                if (!AH5_read_eet_dataset(file_id, path, external_element->datasets + i))
                    rdata = AH5_FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_EXTERNAL_ELEMENT, AH5_C_EXTERNAL_ELEMENT);
        rdata = AH5_FALSE;
    }
    return rdata;
}




// Print dataset in externalElement
void AH5_print_eet_dataset (const AH5_eet_dataset_t *eet_dataset, int space)
{
    hsize_t i;

    printf("%*sInstance: %s\n", space, "", AH5_get_name_from_path(eet_dataset->path));
    for (i = 0; i < eet_dataset->nb_eed_items; i++)
    {
        printf("%*sId %lu:\n", space + 3, "", (long unsigned) i);
        printf("%*s-internal: %s\n", space + 6, "", eet_dataset->eed_items[AH5_EE_INTERNAL_NAME(i)]);
        printf("%*s-external: %s:%s\n", space + 6, "", eet_dataset->eed_items[AH5_EE_EXTERNAL_FILE_NAME(i)], eet_dataset->eed_items[AH5_EE_EXTERNAL_NAME(i)]);
        printf("%*s-file_id:  %i\n\n", space + 6, "",eet_dataset->file_id[i]);
    }
}


// Print externalElement category (all datasets)
void AH5_print_external_element (const AH5_external_element_t *external_element)
{
    hsize_t i;

    printf("#############################  External element  #############################\n\n");
    for (i = 0; i < external_element->nb_datasets; i++)
        AH5_print_eet_dataset(&(external_element->datasets[i]), 0);
    printf("\n");
}




// Close external files and free memory used by dataset in externalElement
void AH5_free_eet_dataset (AH5_eet_dataset_t *eet_dataset)
{
    if (eet_dataset->path != NULL)
    {
        free(eet_dataset->path);
        eet_dataset->path = NULL;
    }
    if (eet_dataset->eed_items != NULL)
    {
        AH5_close_external_files(eet_dataset);
        free(eet_dataset->eed_items[0]);
        free(eet_dataset->eed_items);
        eet_dataset->eed_items = NULL;
        eet_dataset->nb_eed_items = 0;
    }
    if (eet_dataset->file_id != NULL)
    {
        free(eet_dataset->file_id);
        eet_dataset->file_id = NULL;
    }
}


// Free memory used by externalElement (including datasets)
void AH5_free_external_element (AH5_external_element_t *external_element)
{
    hsize_t i;

    if (external_element->datasets != NULL)
    {
        for (i = 0; i < external_element->nb_datasets; i++)
            AH5_free_eet_dataset(external_element->datasets + i);
        free(external_element->datasets);
        external_element->datasets = NULL;
        external_element->nb_datasets = 0;
    }
}




// Open all external files and store the id
char AH5_open_external_files(AH5_eet_dataset_t *eet_dataset)
{
    char success = AH5_TRUE;
    hid_t file_id;
    hid_t *buf_id;
    AH5_set_t buf = {NULL, 0};
    hsize_t i;
    char *name;
    hsize_t id = 0;

    buf_id = (hid_t*) malloc(eet_dataset->nb_eed_items * sizeof(hid_t));  // temporary buffer containing file_id
    buf.values = (char **) malloc(eet_dataset->nb_eed_items * sizeof(char*));  // temporary buffer containing paths
    for (i = 0; i < eet_dataset->nb_eed_items; i++)
    {
        name = eet_dataset->eed_items[AH5_EE_EXTERNAL_FILE_NAME(i)]; // copy of the pointer (shorter expression)
        if (!AH5_index_in_set(buf, name, &id))  // avoid multiple opening
        {
            if (access(name, R_OK) != -1 )
            {
                file_id = H5Fopen(name, H5F_ACC_RDONLY, H5P_DEFAULT);
            }
            else
            {
                file_id = -1;
            }
            buf = AH5_add_to_set(buf, name);  // new memory allocation!!!
            buf_id[buf.nb_values - 1] = file_id;
            eet_dataset->file_id[i] = file_id;  // write new file_id into the ext_elt structure
            if (file_id < 0)
            {
                printf("***** ERROR(%s): Cannot open file \"%s\". *****\n\n", AH5_C_EXTERNAL_ELEMENT, name);
                success = AH5_FALSE;
            }
        }
        else
        {
            eet_dataset->file_id[i] = buf_id[id];  // write existing file_id into the ext_elt structure
        }
    }

    for (i = 0; i < buf.nb_values; i++)
    {
        free(buf.values[i]);
    }
    free(buf.values);
    free(buf_id);
    return success;
}


// Close all external files and set their id to -1
char AH5_close_external_files(AH5_eet_dataset_t *eet_dataset)
{
    char success = AH5_TRUE;
    hsize_t i, j;
    hid_t file_id;

    for (i = 0; i < eet_dataset->nb_eed_items; i++)
    {
        file_id = eet_dataset->file_id[i];
        if (file_id != -1)  // find opened file handles...
        {
            if(H5Fclose(file_id) >= 0)  // ...close file...
            {
                for (j = i; j < eet_dataset->nb_eed_items; j++)  // ...and delete handles of same name in ext_elt
                {
                    if (eet_dataset->file_id[j] == file_id)
                    {
                        eet_dataset->file_id[j] = -1;
                    }
                }
            }
            else
            {
                success = AH5_FALSE;
            }
        }
    }
    return success;
}
