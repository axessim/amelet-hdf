#include "ah5_c_outreq.h"


// Init outputRequest instance
void AH5_init_ort_instance (AH5_ort_instance_t *ort_instance)
{
    ort_instance->path = NULL;
    ort_instance->opt_attrs.nb_instances = 0;
    ort_instance->opt_attrs.instances = NULL;
    ort_instance->subject = NULL;
    ort_instance->object = NULL;
    ort_instance->subject_name = NULL; /* for purposes of the module */
    ort_instance->output = NULL;
    ort_instance->nb_elements = 0;
    ort_instance->nb_cpes = NULL;
    ort_instance->cpes = NULL;
    ort_instance->ccpes = NULL;
    ort_instance->output_dims[0] = 1;
    ort_instance->output_dims[1] = 3;
    ort_instance->output_dims[2] = 0;
    ort_instance->rank = 0;
    ort_instance->data = NULL;
}


// Init outputRequest group (group of instances)
void AH5_init_ort_group (AH5_ort_group_t *ort_group)
{
    ort_group->path = NULL;
    ort_group->opt_attrs.nb_instances = 0;
    ort_group->opt_attrs.instances = NULL;
    ort_group->nb_instances = 0;
    ort_group->instances = NULL;
}


// Init outputRequest category (all groups/instances)
void AH5_init_outputrequest(AH5_outputrequest_t *outputrequest)
{
    outputrequest->nb_groups = 0;
    outputrequest->groups = NULL;
}




// Read outputRequest instance
char AH5_read_ort_instance (hid_t file_id, const char *path, AH5_ort_instance_t *ort_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_SUBJECT, AH5_A_OBJECT, AH5_A_OUTPUT};
    AH5_lbl_dataset_t AH5_label_dataset;
    char rdata = AH5_TRUE;
    unsigned int i;

    AH5_init_ort_instance(ort_instance);
    AH5_init_lbl_dataset(&AH5_label_dataset);
    ort_instance->path = strdup(path);

    if (AH5_path_valid(file_id, path))
    {
        AH5_read_opt_attrs(file_id, path, &(ort_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
        if (!AH5_read_str_attr(file_id, path, AH5_A_SUBJECT, &(ort_instance->subject)))
        {
            AH5_print_err_attr(AH5_C_OUTPUT_REQUEST, path, AH5_A_SUBJECT);
            rdata = AH5_FALSE;
        }
        if (!AH5_read_str_attr(file_id, path, AH5_A_OBJECT, &(ort_instance->object)))
        {
            AH5_print_err_attr(AH5_C_OUTPUT_REQUEST, path, AH5_A_OBJECT);
            rdata = AH5_FALSE;
        }
        if (!AH5_read_str_attr(file_id, path, AH5_A_OUTPUT, &(ort_instance->output)))
        {
            AH5_print_err_attr(AH5_C_OUTPUT_REQUEST, path, AH5_A_OUTPUT);
            rdata = AH5_FALSE;
        }

        if (rdata)
            for(i = 0; i < ort_instance->opt_attrs.nb_instances; i++)
            {
                if (strcmp(ort_instance->opt_attrs.instances[i].name, "subject_id") == 0 &&
                        ort_instance->opt_attrs.instances[i].type == H5T_INTEGER &&
                        AH5_read_lbl_dataset(file_id, ort_instance->subject, &AH5_label_dataset))
                {
                    ort_instance->subject_name = strdup(AH5_label_dataset.items[ort_instance->opt_attrs.instances[i].value.i]);
                }
                AH5_free_lbl_dataset(&AH5_label_dataset);
            }
    }
    else
    {
        AH5_print_err_path(AH5_C_OUTPUT_REQUEST, path);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read outputRequest group (group of instances)
char AH5_read_ort_group (hid_t file_id, const char *path, AH5_ort_group_t *ort_group)
{
    char path2[AH5_ABSOLUTE_PATH_LENGTH], rdata = AH5_TRUE;
/*    char mandatory[][AH5_ATTR_LENGTH] = {}; */
    AH5_children_t children;
    hsize_t i;

    AH5_init_ort_group(ort_group);

    ort_group->path = strdup(path);

    if (AH5_path_valid(file_id, path))
    {
        AH5_read_opt_attrs(file_id, path, &(ort_group->opt_attrs), NULL, 0);
        children = AH5_read_children_name(file_id, path);
        ort_group->nb_instances = children.nb_children;
        if (children.nb_children > 0)
        {
            ort_group->instances = (AH5_ort_instance_t *) malloc(children.nb_children * sizeof(AH5_ort_instance_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path2, path);
                strcat(path2, children.childnames[i]);
                AH5_read_ort_instance(file_id, path2, ort_group->instances + i);
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_OUTPUT_REQUEST, path);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read outputRequest category (all groups/instances)
char AH5_read_outputrequest(hid_t file_id, AH5_outputrequest_t *outputrequest)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH], rdata = AH5_TRUE;
    AH5_children_t children;
    hsize_t i;

    AH5_init_outputrequest(outputrequest);

    if (AH5_path_valid(file_id, AH5_C_OUTPUT_REQUEST))    
    {
        children = AH5_read_children_name(file_id, AH5_C_OUTPUT_REQUEST);
        outputrequest->nb_groups = children.nb_children;
        if (children.nb_children > 0)
        {
            outputrequest->groups = (AH5_ort_group_t *) malloc(children.nb_children * sizeof(AH5_ort_group_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path, AH5_C_OUTPUT_REQUEST);
                strcat(path, children.childnames[i]);
                if (!AH5_read_ort_group(file_id, path, outputrequest->groups + i))
                    rdata = AH5_FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_OUTPUT_REQUEST, AH5_C_OUTPUT_REQUEST);
        rdata = AH5_FALSE;
    }
    return rdata;
}



// Print outputRequest instance
void AH5_print_ort_instance (const AH5_ort_instance_t *ort_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", AH5_get_name_from_path(ort_instance->path));
    AH5_print_opt_attrs(&(ort_instance->opt_attrs), space + 3);
    AH5_print_str_attr(AH5_A_SUBJECT, ort_instance->subject, space + 3);
    AH5_print_str_attr(AH5_A_OBJECT, ort_instance->object, space + 3);
    AH5_print_str_attr(AH5_A_OUTPUT, ort_instance->output, space + 3);
}


// Print outputRequest group (group of instances)
void AH5_print_ort_group (const AH5_ort_group_t *ort_group, int space)
{
    hsize_t i;

    printf("%*sGroup: %s\n", space, "", AH5_get_name_from_path(ort_group->path));
    AH5_print_opt_attrs(&(ort_group->opt_attrs), space + 4);
    for (i = 0; i < ort_group->nb_instances; i++)
        AH5_print_ort_instance(&(ort_group->instances[i]), space + 2);
    printf("\n");
}


// Print outputRequest category (all groups/instances)
void AH5_print_outputrequest (const AH5_outputrequest_t *outputrequest)
{
    hsize_t i;

    printf("##############################  Output request  ##############################\n\n");
    for (i = 0; i < outputrequest->nb_groups; i++)
        AH5_print_ort_group(&(outputrequest->groups[i]), 0);
    printf("\n");
}




// Free memory used by structure ort_instance
void AH5_free_ort_instance (AH5_ort_instance_t *ort_instance)
{
    hsize_t i;

    free(ort_instance->path);
    AH5_free_opt_attrs(&(ort_instance->opt_attrs));
    free(ort_instance->subject);
    free(ort_instance->subject_name);
    free(ort_instance->object);
    free(ort_instance->output);
    if (ort_instance->nb_elements > 0)
    {
        for (i = 0; i < ort_instance->nb_elements; i++)
        {
            if (ort_instance->cpes != NULL)
                free(ort_instance->cpes[i]);
            if (ort_instance->ccpes != NULL)
                free(ort_instance->ccpes[i]);
        }
        free(ort_instance->nb_cpes);
        free(ort_instance->cpes);
        free(ort_instance->ccpes);
        free(ort_instance->data);
    }
    AH5_init_ort_instance(ort_instance);
}


// Free memory used by structure ort_group (including outputRequest instances)
void AH5_free_ort_group (AH5_ort_group_t *ort_group)
{
    hsize_t i;

    free(ort_group->path);
    AH5_free_opt_attrs(&(ort_group->opt_attrs));
    if (ort_group->instances != NULL)
    {
        for (i = 0; i < ort_group->nb_instances; i++)
            AH5_free_ort_instance(ort_group->instances + i);
        free(ort_group->instances);
    }
    AH5_init_ort_group(ort_group);
}


// Free memory used by structure outputrequest (including outputRequest groups/instances)
void AH5_free_outputrequest (AH5_outputrequest_t *outputrequest)
{
    hsize_t i;

    if (outputrequest->groups != NULL)
    {
        for (i = 0; i < outputrequest->nb_groups; i++)
            AH5_free_ort_group(outputrequest->groups + i);
        free(outputrequest->groups);
    }
    AH5_init_outputrequest(outputrequest);
}

