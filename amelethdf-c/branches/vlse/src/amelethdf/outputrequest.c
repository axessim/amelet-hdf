#include "outputrequest.h"


// Read outputRequest instance
void read_ort_instance (hid_t file_id, const char *path, ort_instance_t *ort_instance)
{
    char mandatory[][ATTR_LENGTH] = {A_SUBJECT, A_OBJECT, A_OUTPUT};

    ort_instance->path = strdup(path);
    read_opt_attrs(file_id, path, &(ort_instance->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
    if (!read_str_attr(file_id, path, A_SUBJECT, &(ort_instance->subject)))
        print_err_attr(C_OUTPUT_REQUEST, path, A_SUBJECT);
    if (!read_str_attr(file_id, path, A_OBJECT, &(ort_instance->object)))
        print_err_attr(C_OUTPUT_REQUEST, path, A_OBJECT);
    if (!read_str_attr(file_id, path, A_OUTPUT, &(ort_instance->output)))
        print_err_attr(C_OUTPUT_REQUEST, path, A_OUTPUT);
}


// Read outputRequest group (group of instances)
void read_ort_group (hid_t file_id, const char *path, ort_group_t *ort_group)
{
    children_t children;
    hsize_t i;
    char path2[ABSOLUTE_PATH_NAME_LENGTH];
    char mandatory[][ATTR_LENGTH] = {};

    ort_group->path = strdup(path);
    read_opt_attrs(file_id, path, &(ort_group->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
    children = read_children_name(file_id, path);
    ort_group->nb_instances = children.nb_children;
    ort_group->instances = NULL;
    if (children.nb_children > 0)
    {
        ort_group->instances = (ort_instance_t *) malloc(children.nb_children * sizeof(ort_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path2, path);
            strcat(path2, children.childnames[i]);
            read_ort_instance(file_id, path2, ort_group->instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}


// Read outputRequest category (all groups/instances)
void read_outputrequest(hid_t file_id, outputrequest_t *outputrequest)
{
    children_t children;
    hsize_t i;
    char path[ABSOLUTE_PATH_NAME_LENGTH];

    children = read_children_name(file_id, C_OUTPUT_REQUEST);
    outputrequest->nb_groups = children.nb_children;
    outputrequest->groups = NULL;
    if (children.nb_children > 0)
    {
        outputrequest->groups = (ort_group_t *) malloc(children.nb_children * sizeof(ort_group_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_OUTPUT_REQUEST);
            strcat(path, children.childnames[i]);
            read_ort_group(file_id, path, outputrequest->groups + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}



// Print outputRequest instance
void print_ort_instance (const ort_instance_t *ort_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", get_name_from_path(ort_instance->path));
    print_opt_attrs(&(ort_instance->opt_attrs), space + 3);
    print_str_attr(A_SUBJECT, ort_instance->subject, space + 3);
    print_str_attr(A_OBJECT, ort_instance->object, space + 3);
    print_str_attr(A_OUTPUT, ort_instance->output, space + 3);
}


// Print outputRequest group (group of instances)
void print_ort_group (const ort_group_t *ort_group, int space)
{
    hsize_t i;

    printf("%*sGroup: %s\n", space, "", get_name_from_path(ort_group->path));
    print_opt_attrs(&(ort_group->opt_attrs), space + 4);
    for (i = 0; i < ort_group->nb_instances; i++)
        print_ort_instance(&(ort_group->instances[i]), space + 2);
    printf("\n");
}


// Print outputRequest category (all groups/instances)
void print_outputrequest (const outputrequest_t *outputrequest)
{
    hsize_t i;

    printf("##############################  Output request  ##############################\n\n");
    for (i = 0; i < outputrequest->nb_groups; i++)
        print_ort_group(&(outputrequest->groups[i]), 0);
    printf("\n");
}




// Free memory used by structure ort_instance
void free_ort_instance (ort_instance_t *ort_instance)
{
    if (ort_instance->path != NULL)
    {
        free(ort_instance->path);
        ort_instance->path = NULL;
    }
    free_opt_attrs(&(ort_instance->opt_attrs));
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
void free_ort_group (ort_group_t *ort_group)
{
    hsize_t i;

    if (ort_group->path != NULL)
    {
        free(ort_group->path);
        ort_group->path = NULL;
    }
    free_opt_attrs(&(ort_group->opt_attrs));
    if (ort_group->nb_instances > 0)
    {
        for (i = 0; i < ort_group->nb_instances; i++)
            free_ort_instance(ort_group->instances + i);
        free(ort_group->instances);
        ort_group->instances = NULL;
        ort_group->nb_instances = 0;
    }
}


// Free memory used by structure outputrequest (including outputRequest groups/instances)
void free_outputrequest (outputrequest_t *outputrequest)
{
    hsize_t i;

    if (outputrequest->nb_groups > 0)
    {
        for (i = 0; i < outputrequest->nb_groups; i++)
        {
            free_ort_group(outputrequest->groups + i);
        }
        free(outputrequest->groups);
        outputrequest->groups = NULL;
        outputrequest->nb_groups = 0;
    }
}

