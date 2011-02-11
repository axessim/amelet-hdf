#include "outputrequest.h"
// Revision: 7.2.2011

// Read outputRequest instance
void read_outputrequest_instance (hid_t file_id, const char *path, outputrequest_instance_t *outputrequest_instance)
{
    char mandatory[][ATTRIBUTE_LENGTH] = {A_SUBJECT, A_OBJECT, A_OUTPUT};

    outputrequest_instance->name = get_name_from_path(path);
    read_optional_attributes(file_id, path, &(outputrequest_instance->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    if (!read_str_attribute(file_id, path, A_SUBJECT, &(outputrequest_instance->subject)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_OUTPUT_REQUEST, path, A_SUBJECT);
    if (!read_str_attribute(file_id, path, A_OBJECT, &(outputrequest_instance->object)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_OUTPUT_REQUEST, path, A_OBJECT);
    if (!read_str_attribute(file_id, path, A_OUTPUT, &(outputrequest_instance->output)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_OUTPUT_REQUEST, path, A_OUTPUT);
}


// Read outputRequest group (group of instances)
void read_outputrequest_group (hid_t file_id, const char *path, outputrequest_group_t *outputrequest_group)
{
    children_t children;
    hsize_t i;
    char *path2;
    char mandatory[][ATTRIBUTE_LENGTH] = {};

    outputrequest_group->name = get_name_from_path(path);
    read_optional_attributes(file_id, path, &(outputrequest_group->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    children = read_children_name(file_id, path);
    outputrequest_group->nb_outputrequest_instances = children.nb_children;
    outputrequest_group->outputrequest_instances = NULL;
    if (children.nb_children > 0)
    {
        path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        outputrequest_group->outputrequest_instances = (outputrequest_instance_t *) malloc((size_t) children.nb_children * sizeof(outputrequest_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path2, path);
            strcat(path2, children.childnames[i]);
            read_outputrequest_instance(file_id, path2, outputrequest_group->outputrequest_instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
        free(path2);
    }
}


// Read outputRequest category (all groups/instances)
void read_outputrequest(hid_t file_id, outputrequest_t *outputrequest)
{
    children_t children;
    hsize_t i;
    char *path;

    children = read_children_name(file_id, C_OUTPUT_REQUEST);
    outputrequest->nb_outputrequest_groups = children.nb_children;
    outputrequest->outputrequest_groups = NULL;
    if (children.nb_children > 0)
    {
        path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        outputrequest->outputrequest_groups = (outputrequest_group_t *) malloc((size_t) children.nb_children * sizeof(outputrequest_group_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_OUTPUT_REQUEST);
            strcat(path, children.childnames[i]);
            read_outputrequest_group(file_id, path, outputrequest->outputrequest_groups + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
        free(path);
    }
}



// Print outputRequest instance
void print_outputrequest_instance (outputrequest_instance_t outputrequest_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", outputrequest_instance.name);
    print_optional_attributes(outputrequest_instance.optional_attributes, space + 3);
    print_str_attribute(A_SUBJECT, outputrequest_instance.subject, space + 3);
    print_str_attribute(A_OBJECT, outputrequest_instance.object, space + 3);
    print_str_attribute(A_OUTPUT, outputrequest_instance.output, space + 3);
}


// Print outputRequest group (group of instances)
void print_outputrequest_group (outputrequest_group_t outputrequest_group, int space)
{
    hsize_t i;

    printf("%*sGroup: %s\n", space, "", outputrequest_group.name);
    print_optional_attributes(outputrequest_group.optional_attributes, space + 4);
    for (i = 0; i < outputrequest_group.nb_outputrequest_instances; i++)
        print_outputrequest_instance(outputrequest_group.outputrequest_instances[i], space + 2);
    printf("\n");
}


// Print outputRequest category (all groups/instances)
void print_outputrequest (outputrequest_t outputrequest)
{
    hsize_t i;

    printf("##############################  Output request  ##############################\n\n");
    for (i = 0; i < outputrequest.nb_outputrequest_groups; i++)
        print_outputrequest_group(outputrequest.outputrequest_groups[i], 0);
    printf("\n");
}




// Free memory used by structure outputrequest_instance
void free_outputrequest_instance (outputrequest_instance_t *outputrequest_instance)
{
    if (outputrequest_instance->name != NULL)
    {
        free(outputrequest_instance->name);
        outputrequest_instance->name = NULL;
    }
    free_optional_attributes(&(outputrequest_instance->optional_attributes));
    if (outputrequest_instance->subject != NULL)
    {
        free(outputrequest_instance->subject);
        outputrequest_instance->subject = NULL;
    }
    if (outputrequest_instance->object != NULL)
    {
        free(outputrequest_instance->object);
        outputrequest_instance->object = NULL;
    }
    if (outputrequest_instance->output != NULL)
    {
        free(outputrequest_instance->output);
        outputrequest_instance->output = NULL;
    }
}


// Free memory used by structure outputrequest_group (including outputRequest instances)
void free_outputrequest_group (outputrequest_group_t *outputrequest_group)
{
    hsize_t i;

    if (outputrequest_group->name != NULL)
    {
        free(outputrequest_group->name);
        outputrequest_group->name = NULL;
    }
    free_optional_attributes(&(outputrequest_group->optional_attributes));
    if (outputrequest_group->nb_outputrequest_instances > 0)
    {
        for (i = 0; i < outputrequest_group->nb_outputrequest_instances; i++)
            free_outputrequest_instance(outputrequest_group->outputrequest_instances + i);
        free(outputrequest_group->outputrequest_instances);
        outputrequest_group->outputrequest_instances = NULL;
        outputrequest_group->nb_outputrequest_instances = 0;
    }
}


// Free memory used by structure outputrequest (including outputRequest groups/instances)
void free_outputrequest (outputrequest_t *outputrequest)
{
    hsize_t i;

    if (outputrequest->nb_outputrequest_groups > 0)
    {
        for (i = 0; i < outputrequest->nb_outputrequest_groups; i++)
        {
            free_outputrequest_group(outputrequest->outputrequest_groups + i);
        }
        free(outputrequest->outputrequest_groups);
        outputrequest->outputrequest_groups = NULL;
        outputrequest->nb_outputrequest_groups = 0;
    }
}

