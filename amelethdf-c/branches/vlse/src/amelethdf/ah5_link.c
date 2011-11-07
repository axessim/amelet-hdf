#include "ah5_link.h"


// Init link instance
void AH5_init_lnk_instance (AH5_lnk_instance_t *lnk_instance)
{
    lnk_instance->path = NULL;
    lnk_instance->opt_attrs.nb_instances = 0;
    lnk_instance->opt_attrs.instances = NULL;
    lnk_instance->subject = NULL;
    lnk_instance->object = NULL;
}

// Init link group (group of instances)
void AH5_init_lnk_group (AH5_lnk_group_t *lnk_group)
{
    lnk_group->path = NULL;
    lnk_group->opt_attrs.nb_instances = 0;
    lnk_group->opt_attrs.instances = NULL;
    lnk_group->nb_instances = 0;
    lnk_group->instances = NULL;
}


// Init link category (all groups/instances)
void AH5_init_link (AH5_link_t *link)
{
    link->nb_groups = 0;
    link->groups = NULL;
}




// Read link instance
char AH5_read_lnk_instance (hid_t file_id, const char *path, AH5_lnk_instance_t *lnk_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_SUBJECT, AH5_A_OBJECT};
    char rdata = TRUE;

    AH5_init_lnk_instance(lnk_instance);
    lnk_instance->path = strdup(path);

    if (AH5_path_valid(file_id, path))
    {
        AH5_read_opt_attrs(file_id, path, &(lnk_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
        if (!AH5_read_str_attr(file_id, path, AH5_A_SUBJECT, &(lnk_instance->subject)))
        {
            AH5_print_err_attr(AH5_C_LINK, path, AH5_A_SUBJECT);
            rdata = FALSE;
        }
        if (!AH5_read_str_attr(file_id, path, AH5_A_OBJECT, &(lnk_instance->object)))
        {
            AH5_print_err_attr(AH5_C_LINK, path, AH5_A_OBJECT);
            rdata = FALSE;
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_LINK, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read link group (group of instances)
char AH5_read_lnk_group (hid_t file_id, const char *path, AH5_lnk_group_t *lnk_group)
{
    char path2[AH5_ABSOLUTE_PATH_LENGTH], rdata = TRUE;
    char mandatory[][AH5_ATTR_LENGTH] = {};
    AH5_children_t children;
    hsize_t i;

    AH5_init_lnk_group(lnk_group);
    lnk_group->path = strdup(path);

    if (AH5_path_valid(file_id, path))
    {
        AH5_read_opt_attrs(file_id, path, &(lnk_group->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
        children = AH5_read_children_name(file_id, path);
        lnk_group->nb_instances = children.nb_children;
        if (children.nb_children > 0)
        {
            lnk_group->instances = (AH5_lnk_instance_t *) malloc(children.nb_children * sizeof(AH5_lnk_instance_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path2, path);
                strcat(path2, children.childnames[i]);
                if (!AH5_read_lnk_instance(file_id, path2, lnk_group->instances + i))
                    rdata = FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_LINK, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read link category (all groups/instances)
char AH5_read_link (hid_t file_id, AH5_link_t *link)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH], rdata = TRUE;
    AH5_children_t children;
    hsize_t i;

    AH5_init_link(link);

    if (H5Lexists(file_id, AH5_C_LINK, H5P_DEFAULT) == TRUE)
    {
        children = AH5_read_children_name(file_id, AH5_C_LINK);
        link->nb_groups = children.nb_children;
        if (children.nb_children > 0)
        {
            link->groups = (AH5_lnk_group_t *) malloc(children.nb_children * sizeof(AH5_lnk_group_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path, AH5_C_LINK);
                strcat(path, children.childnames[i]);
                if (!AH5_read_lnk_group(file_id, path, link->groups + i))
                    rdata = FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_LINK, AH5_C_LINK);
        rdata = FALSE;
    }
    return rdata;
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
    free(lnk_instance->path);
    AH5_free_opt_attrs(&(lnk_instance->opt_attrs));
    free(lnk_instance->subject);
    free(lnk_instance->object);
    AH5_init_lnk_instance(lnk_instance);
}


// Free memory used by structure lnk_group (including link instances)
void AH5_free_lnk_group (AH5_lnk_group_t *lnk_group)
{
    hsize_t i;

    free(lnk_group->path);
    AH5_free_opt_attrs(&(lnk_group->opt_attrs));
    if (lnk_group->instances != NULL)
    {
        for (i = 0; i < lnk_group->nb_instances; i++)
            AH5_free_lnk_instance(lnk_group->instances + i);
        free(lnk_group->instances);
    }
    AH5_init_lnk_group(lnk_group);
}


// Free memory used by structure link (including link groups/instances)
void AH5_free_link (AH5_link_t *link)
{
    hsize_t i;

    if (link->groups != NULL)
    {
        for (i = 0; i < link->nb_groups; i++)
            AH5_free_lnk_group(link->groups + i);
        free(link->groups);
    }
    AH5_init_link(link);
}
