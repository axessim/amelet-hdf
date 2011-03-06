#include "ah5_link.h"


// Read link instance
void AH5_read_lnk_instance (hid_t file_id, const char *path, AH5_lnk_instance_t *lnk_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_SUBJECT, AH5_A_OBJECT};

    lnk_instance->path = strdup(path);
    AH5_read_opt_attrs(file_id, path, &(lnk_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    if (!AH5_read_str_attr(file_id, path, AH5_A_SUBJECT, &(lnk_instance->subject)))
        AH5_print_err_attr(AH5_C_LINK, path, AH5_A_SUBJECT);
    if (!AH5_read_str_attr(file_id, path, AH5_A_OBJECT, &(lnk_instance->object)))
        AH5_print_err_attr(AH5_C_LINK, path, AH5_A_OBJECT);
}


// Read link group (group of instances)
void AH5_read_lnk_group (hid_t file_id, const char *path, AH5_lnk_group_t *lnk_group)
{
    char path2[AH5_ABSOLUTE_PATH_LENGTH];
    char mandatory[][AH5_ATTR_LENGTH] = {};
    AH5_children_t children;
    hsize_t i;

    lnk_group->path = strdup(path);
    AH5_read_opt_attrs(file_id, path, &(lnk_group->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    children = AH5_read_children_name(file_id, path);
    lnk_group->nb_instances = children.nb_children;
    lnk_group->instances = NULL;
    if (children.nb_children > 0)
    {
        lnk_group->instances = (AH5_lnk_instance_t *) malloc(children.nb_children * sizeof(AH5_lnk_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path2, path);
            strcat(path2, children.childnames[i]);
            AH5_read_lnk_instance(file_id, path2, lnk_group->instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}


// Read link category (all groups/instances)
void AH5_read_link (hid_t file_id, AH5_link_t *link)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH];
    AH5_children_t children;
    hsize_t i;

    children = AH5_read_children_name(file_id, AH5_C_LINK);
    link->nb_groups = children.nb_children;
    link->groups = NULL;
    if (children.nb_children > 0)
    {
        link->groups = (AH5_lnk_group_t *) malloc(children.nb_children * sizeof(AH5_lnk_group_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, AH5_C_LINK);
            strcat(path, children.childnames[i]);
            AH5_read_lnk_group(file_id, path, link->groups + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}




// Print link instance
void AH5_print_lnk_instance (const AH5_lnk_instance_t *lnk_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", AH5_get_name_from_path(lnk_instance->path));
    AH5_print_str_attr(AH5_A_SUBJECT, lnk_instance->subject, space + 3);
    AH5_print_str_attr(AH5_A_OBJECT, lnk_instance->object, space + 3);
    AH5_print_opt_attrs(&(lnk_instance->opt_attrs), space + 3);
}


// Print link group (group of instances)
void AH5_print_lnk_group (const AH5_lnk_group_t *lnk_group, int space)
{
    hsize_t i;

    printf("%*sGroup: %s\n", space, "", AH5_get_name_from_path(lnk_group->path));
    AH5_print_opt_attrs(&(lnk_group->opt_attrs), space + 4);
    for (i = 0; i < lnk_group->nb_instances; i++)
        AH5_print_lnk_instance(&(lnk_group->instances[i]), space + 2);
    printf("\n");
}


// Print link category (all groups/instances)
void AH5_print_link (const AH5_link_t *link)
{
    hsize_t i;

    printf("###################################  Link  ###################################\n\n");
    for (i = 0; i < link->nb_groups; i++)
        AH5_print_lnk_group(&(link->groups[i]), 0);
    printf("\n");
}




// Free memory used by structure lnk_instance
void AH5_free_lnk_instance (AH5_lnk_instance_t *lnk_instance)
{
    if (lnk_instance->path != NULL)
    {
        free(lnk_instance->path);
        lnk_instance->path = NULL;
    }
    AH5_free_opt_attrs(&(lnk_instance->opt_attrs));
    if (lnk_instance->subject != NULL)
    {
        free(lnk_instance->subject);
        lnk_instance->subject = NULL;
    }
    if (lnk_instance->object != NULL)
    {
        free(lnk_instance->object);
        lnk_instance->object = NULL;
    }
}


// Free memory used by structure lnk_group (including link instances)
void AH5_free_lnk_group (AH5_lnk_group_t *lnk_group)
{
    hsize_t i;

    if (lnk_group->path != NULL)
    {
        free(lnk_group->path);
        lnk_group->path = NULL;
    }
    AH5_free_opt_attrs(&(lnk_group->opt_attrs));
    if (lnk_group->nb_instances > 0)
    {
        for (i = 0; i < lnk_group->nb_instances; i++)
            AH5_free_lnk_instance(lnk_group->instances + i);
        free(lnk_group->instances);
        lnk_group->instances = NULL;
        lnk_group->nb_instances = 0;
    }
}


// Free memory used by structure link (including link groups/instances)
void AH5_free_link (AH5_link_t *link)
{
    hsize_t i;

    if (link->nb_groups > 0)
    {
        for (i = 0; i < link->nb_groups; i++)
            AH5_free_lnk_group(link->groups + i);
        free(link->groups);
        link->groups = NULL;
        link->nb_groups = 0;
    }
}
