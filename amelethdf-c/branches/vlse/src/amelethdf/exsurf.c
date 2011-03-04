#include "exsurf.h"


// Read exchangeSurface group
void read_exs_group (hid_t file_id, const char *path, exs_group_t *exs_group)
{
    char path2[ABSOLUTE_PATH_NAME_LENGTH], *temp;
    children_t children;
    hsize_t i;

    exs_group->path = strdup(path);
    if (read_str_attr(file_id, path, A_TYPE, &temp))
    {
        if (strcmp(temp, V_RECIPROCITY) == 0)
            exs_group->type = EXS_TYPE_RECIPROCITY;
        else if (strcmp(temp, V_HUYGENS) == 0)
            exs_group->type = EXS_TYPE_HUYGENS;
        else if (strcmp(temp, V_GAUSS) == 0)
            exs_group->type = EXS_TYPE_GAUSS;
        else
        {
            exs_group->type = EXS_TYPE_INVALID;
            print_wrn_attr(C_EXCHANGE_SURFACE, path, A_TYPE);
        }
        free(temp);
    }
    else
    {
        exs_group->type = EXS_TYPE_INVALID;
        print_err_attr(C_EXCHANGE_SURFACE, path, A_TYPE);
    }

    if (read_str_attr(file_id, path, A_NATURE, &temp))
    {
        if (strcmp(temp, V_OUTSIDE) == 0)
            exs_group->nature = EXS_NATURE_OUTSIDE;
        else if (strcmp(temp, V_INSIDE) == 0)
            exs_group->nature = EXS_NATURE_INSIDE;
        else
        {
            exs_group->nature = EXS_NATURE_INVALID;
            print_wrn_attr(C_EXCHANGE_SURFACE, path, A_NATURE);
        }
        free(temp);
    }
    else
    {
        exs_group->nature = EXS_NATURE_INVALID;
        print_err_attr(C_EXCHANGE_SURFACE, path, A_NATURE);
    }

    children = read_children_name(file_id, path);
    exs_group->nb_instances = children.nb_children;
    exs_group->instances = NULL;
    if (children.nb_children > 0)
    {
        exs_group->instances = (arrayset_t *) malloc(children.nb_children * sizeof(arrayset_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path2, path);
            strcat(path2, children.childnames[i]);
            read_ft_arrayset(file_id, path2, exs_group->instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}


// Read exchangeSurface category
void read_exchange_surface (hid_t file_id, exchange_surface_t *exchange_surface)
{
    char path[ABSOLUTE_PATH_NAME_LENGTH];
    children_t children;
    hsize_t i;

    children = read_children_name(file_id, C_EXCHANGE_SURFACE);
    exchange_surface->nb_groups = children.nb_children;
    exchange_surface->groups = NULL;
    if (children.nb_children > 0)
    {
        exchange_surface->groups = (exs_group_t *) malloc(children.nb_children * sizeof(exs_group_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_EXCHANGE_SURFACE);
            strcat(path, children.childnames[i]);
            read_exs_group(file_id, path, exchange_surface->groups + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}




// Print exchangeSurface group
void print_exs_group (exs_group_t exs_group, int space)
{
    hsize_t i;

    printf("%*sGroup: %s\n", space, "", get_name_from_path(exs_group.path));
    switch (exs_group.type)
    {
    case EXS_TYPE_RECIPROCITY:
        print_str_attr(A_TYPE, V_RECIPROCITY, space + 4);
        break;
    case EXS_TYPE_HUYGENS:
        print_str_attr(A_TYPE, V_HUYGENS, space + 4);
        break;
    case EXS_TYPE_GAUSS:
        print_str_attr(A_TYPE, V_GAUSS, space + 4);
        break;
    case EXS_TYPE_INVALID:
        print_str_attr(A_TYPE, V_INVALID, space + 4);
        break;
    }
    switch (exs_group.nature)
    {
    case EXS_NATURE_OUTSIDE:
        print_str_attr(A_NATURE, V_OUTSIDE, space + 4);
        break;
    case EXS_NATURE_INSIDE:
        print_str_attr(A_NATURE, V_INSIDE, space + 4);
        break;
    case EXS_NATURE_INVALID:
        print_str_attr(A_NATURE, V_INVALID, space + 4);
        break;
    }

    for (i = 0; i < exs_group.nb_instances; i++)
        print_ft_arrayset(exs_group.instances[i], space + 2);
}


// Print exchangeSurface category
void print_exchange_surface (exchange_surface_t exchange_surface)
{
    hsize_t i;

    printf("#############################  Exchange surface  #############################\n\n");
    for (i = 0; i < exchange_surface.nb_groups; i++)
        print_exs_group(exchange_surface.groups[i], 0);
    printf("\n");
}




// Free memory used by exchangeSurface group
void free_exs_group (exs_group_t *exs_group)
{
    hsize_t i;

    if (exs_group->path != NULL)
    {
        free(exs_group->path);
        exs_group->path = NULL;
    }
    if (exs_group->nb_instances > 0)
    {
        for (i = 0; i < exs_group->nb_instances; i++)
            free_ft_arrayset(exs_group->instances + i);
        free(exs_group->instances);
        exs_group->instances = NULL;
        exs_group->nb_instances = 0;
    }
}


// Free memory used by exchangeSurface category
void free_exchange_surface (exchange_surface_t *exchange_surface)
{
    hsize_t i;

    if (exchange_surface->nb_groups > 0)
    {
        for (i = 0; i < exchange_surface->nb_groups; i++)
            free_exs_group(exchange_surface->groups + i);
        free(exchange_surface->groups);
        exchange_surface->groups = NULL;
        exchange_surface->nb_groups = 0;
    }
}

