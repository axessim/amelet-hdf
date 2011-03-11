#include "ah5_outreq.h"


// Read outputRequest instance
char AH5_read_ort_instance (hid_t file_id, const char *path, AH5_ort_instance_t *ort_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_SUBJECT, AH5_A_OBJECT, AH5_A_OUTPUT};
    char rdata = TRUE;

    ort_instance->path = strdup(path);
    ort_instance->subject = NULL;
    ort_instance->object = NULL;
    ort_instance->output = NULL;
    ort_instance->opt_attrs.instances = NULL;

    if (AH5_path_valid(file_id, path))
    {
        AH5_read_opt_attrs(file_id, path, &(ort_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
        if (!AH5_read_str_attr(file_id, path, AH5_A_SUBJECT, &(ort_instance->subject)))
        {
            AH5_print_err_attr(AH5_C_OUTPUT_REQUEST, path, AH5_A_SUBJECT);
            rdata = FALSE;
        }
        if (!AH5_read_str_attr(file_id, path, AH5_A_OBJECT, &(ort_instance->object)))
        {
            AH5_print_err_attr(AH5_C_OUTPUT_REQUEST, path, AH5_A_OBJECT);
            rdata = FALSE;
        }
        if (!AH5_read_str_attr(file_id, path, AH5_A_OUTPUT, &(ort_instance->output)))
        {
            AH5_print_err_attr(AH5_C_OUTPUT_REQUEST, path, AH5_A_OUTPUT);
            rdata = FALSE;
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_OUTPUT_REQUEST, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read outputRequest group (group of instances)
char AH5_read_ort_group (hid_t file_id, const char *path, AH5_ort_group_t *ort_group)
{
    char path2[AH5_ABSOLUTE_PATH_LENGTH], rdata = TRUE;
    char mandatory[][AH5_ATTR_LENGTH] = {};
    AH5_children_t children;
    hsize_t i;

    ort_group->path = strdup(path);
    ort_group->instances = NULL;
    ort_group->opt_attrs.instances = NULL;

    if (AH5_path_valid(file_id, path))
    {
        AH5_read_opt_attrs(file_id, path, &(ort_group->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
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
        rdata = FALSE;
    }
    return rdata;
}


// Read outputRequest category (all groups/instances)
char AH5_read_outputrequest(hid_t file_id, AH5_outputrequest_t *outputrequest)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH], rdata = TRUE;
    AH5_children_t children;
    hsize_t i;

    outputrequest->groups = NULL;

    if (H5Lexists(file_id, AH5_C_OUTPUT_REQUEST, H5P_DEFAULT) == TRUE)
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
                    rdata = FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_OUTPUT_REQUEST, AH5_C_OUTPUT_REQUEST);
        rdata = FALSE;
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
    if (ort_instance->path != NULL)
    {
        free(ort_instance->path);
        ort_instance->path = NULL;
    }
    AH5_free_opt_attrs(&(ort_instance->opt_attrs));
    if (ort_instance->subject != NULL)
    {
        free(ort_instance->subject);
        ort_instance->subject = NULL;
    }
    if (ort_instance->object != NULL)
    {
        free(ort_instance->object);
        ort_instance->object = NULL;
    }
    if (ort_instance->output != NULL)
    {
        free(ort_instance->output);
        ort_instance->output = NULL;
    }
}


// Free memory used by structure ort_group (including outputRequest instances)
void AH5_free_ort_group (AH5_ort_group_t *ort_group)
{
    hsize_t i;

    if (ort_group->path != NULL)
    {
        free(ort_group->path);
        ort_group->path = NULL;
    }
    AH5_free_opt_attrs(&(ort_group->opt_attrs));
    if (ort_group->instances != NULL)
    {
        for (i = 0; i < ort_group->nb_instances; i++)
            AH5_free_ort_instance(ort_group->instances + i);
        free(ort_group->instances);
        ort_group->instances = NULL;
        ort_group->nb_instances = 0;
    }
}


// Free memory used by structure outputrequest (including outputRequest groups/instances)
void AH5_free_outputrequest (AH5_outputrequest_t *outputrequest)
{
    hsize_t i;

    if (outputrequest->groups != NULL)
    {
        for (i = 0; i < outputrequest->nb_groups; i++)
        {
            AH5_free_ort_group(outputrequest->groups + i);
        }
        free(outputrequest->groups);
        outputrequest->groups = NULL;
        outputrequest->nb_groups = 0;
    }
}

