#include "link.h"
// Revision: 7.2.2011


// Read link instance
void read_link_instance (hid_t file_id, const char *path, link_instance_t *link_instance)
{
    char mandatory[][ATTRIBUTE_LENGTH] = {A_SUBJECT, A_OBJECT};

    link_instance->name = get_name_from_path(path);
    read_optional_attributes(file_id, path, &(link_instance->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    if (!read_string_attribute(file_id, path, A_SUBJECT, &(link_instance->subject)))
        printf("***** ERROR(%s): Cannot find mandatory attribute \"%s@%s\". *****\n\n", C_LINK, path, A_SUBJECT);
    if (!read_string_attribute(file_id, path, A_OBJECT, &(link_instance->object)))
        printf("***** ERROR(%s): Cannot find mandatory attribute \"%s@%s\". *****\n\n", C_LINK, path, A_OBJECT);
}


// Read link group (group of instances)
void read_link_group (hid_t file_id, const char *path, link_group_t *link_group)
{
    children_t children;
    hsize_t i;
    char *path2;
    char mandatory[][ATTRIBUTE_LENGTH] = {};

    link_group->name = get_name_from_path(path);
    read_optional_attributes(file_id, path, &(link_group->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    children = read_children_name(file_id, path);
    link_group->nb_link_instances = children.nb_children;
    link_group->link_instances = NULL;
    if (children.nb_children > 0)
    {
        path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        link_group->link_instances = (link_instance_t *) malloc((size_t) children.nb_children * sizeof(link_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path2, path);
            strcat(path2, children.childnames[i]);
            read_link_instance(file_id, path2, link_group->link_instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
        free(path2);
    }
}


// Read link category (all groups/instances)
void read_link (hid_t file_id, link_t *link)
{
    children_t children;
    hsize_t i;
    char *path;

    children = read_children_name(file_id, C_LINK);
    link->nb_link_groups = children.nb_children;
    link->link_groups = NULL;
    if (children.nb_children > 0)
    {
        path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        link->link_groups = (link_group_t *) malloc((size_t) children.nb_children * sizeof(link_group_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_LINK);
            strcat(path, children.childnames[i]);
            read_link_group(file_id, path, link->link_groups + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
        free(path);
    }
}




// Print link instance
void print_link_instance (link_instance_t link_instance)
{
    printf("  -instance name: %s\n", link_instance.name);
    printf("     @subject: %s\n", link_instance.subject);
    printf("     @object:  %s\n", link_instance.object);
    print_optional_attributes(link_instance.optional_attributes);
}


// Print link group (group of instances)
void print_link_group (link_group_t link_group)
{
    hsize_t i;

    printf("Group name: %s\n", link_group.name);
    print_optional_attributes(link_group.optional_attributes);
    for (i = 0; i < link_group.nb_link_instances; i++)
    {
        print_link_instance(link_group.link_instances[i]);
    }
    printf("\n");
}


// Print link category (all groups/instances)
void print_link (link_t link)
{
    hsize_t i;

    printf("\n###################################  Link  ###################################\n\n");
    for (i = 0; i < link.nb_link_groups; i++)
    {
        print_link_group(link.link_groups[i]);
    }
    printf("\n");
}




// Free memory used by structure link_instance
void free_link_instance (link_instance_t *link_instance)
{
    if (link_instance->name != NULL)
    {
        free(link_instance->name);
        link_instance->name = NULL;
    }
    free_optional_attributes(&(link_instance->optional_attributes));
    if (link_instance->subject != NULL)
    {
        free(link_instance->subject);
        link_instance->subject = NULL;
    }
    if (link_instance->object != NULL)
    {
        free(link_instance->object);
        link_instance->object = NULL;
    }
}


// Free memory used by structure link_group (including link instances)
void free_link_group (link_group_t *link_group)
{
    hsize_t i;

    if (link_group->name != NULL)
    {
        free(link_group->name);
        link_group->name = NULL;
    }
    free_optional_attributes(&(link_group->optional_attributes));
    if (link_group->nb_link_instances > 0)
    {
        for (i = 0; i < link_group->nb_link_instances; i++)
            free_link_instance(link_group->link_instances + i);
        free(link_group->link_instances);
        link_group->link_instances = NULL;
        link_group->nb_link_instances = 0;
    }
}


// Free memory used by structure link (including link groups/instances)
void free_link (link_t *link)
{
    hsize_t i;

    if (link->nb_link_groups > 0)
    {
        for (i = 0; i < link->nb_link_groups; i++)
            free_link_group(link->link_groups + i);
        free(link->link_groups);
        link->link_groups = NULL;
        link->nb_link_groups = 0;
    }
}
