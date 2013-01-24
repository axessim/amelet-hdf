#include "ah5_c_exsurf.h"


// Read exchangeSurface group
char AH5_read_exs_group (hid_t file_id, const char *path, AH5_exs_group_t *exs_group)
{
    char path2[AH5_ABSOLUTE_PATH_LENGTH], *temp, rdata = AH5_TRUE;
    AH5_children_t children;
    hsize_t i;

    exs_group->path = strdup(path);
    exs_group->instances = NULL;
    exs_group->type = EXS_TYPE_INVALID;
    exs_group->nature = EXS_NATURE_INVALID;

    if (AH5_path_valid(file_id, path))
    {
        if (AH5_read_str_attr(file_id, path, AH5_A_TYPE, &temp))
        {
            if (strcmp(temp, AH5_V_RECIPROCITY) == 0)
                exs_group->type = EXS_TYPE_RECIPROCITY;
            else if (strcmp(temp, AH5_V_HUYGENS) == 0)
                exs_group->type = EXS_TYPE_HUYGENS;
            else if (strcmp(temp, AH5_V_GAUSS) == 0)
                exs_group->type = EXS_TYPE_GAUSS;
            else
            {
                AH5_print_wrn_attr(AH5_C_EXCHANGE_SURFACE, path, AH5_A_TYPE);
                rdata = AH5_FALSE;
            }
            free(temp);
        }
        else
        {
            exs_group->type = EXS_TYPE_INVALID;
            AH5_print_err_attr(AH5_C_EXCHANGE_SURFACE, path, AH5_A_TYPE);
            rdata = AH5_FALSE;
        }

        if (AH5_read_str_attr(file_id, path, AH5_A_NATURE, &temp))
        {
            if (strcmp(temp, AH5_V_OUTSIDE) == 0)
                exs_group->nature = EXS_NATURE_OUTSIDE;
            else if (strcmp(temp, AH5_V_INSIDE) == 0)
                exs_group->nature = EXS_NATURE_INSIDE;
            else
            {
                AH5_print_wrn_attr(AH5_C_EXCHANGE_SURFACE, path, AH5_A_NATURE);
                rdata = AH5_FALSE;
            }
            free(temp);
        }
        else
        {
            exs_group->nature = EXS_NATURE_INVALID;
            AH5_print_err_attr(AH5_C_EXCHANGE_SURFACE, path, AH5_A_NATURE);
            rdata = AH5_FALSE;
        }

        children = AH5_read_children_name(file_id, path);
        exs_group->nb_instances = children.nb_children;
        if (children.nb_children > 0)
        {
            exs_group->instances = (AH5_arrayset_t *) malloc(children.nb_children * sizeof(AH5_arrayset_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path2, path);
                strcat(path2, children.childnames[i]);
                if (!AH5_path_valid(file_id, path2))
                    rdata = AH5_FALSE;
                else
                    if (!AH5_read_ft_arrayset(file_id, path2, exs_group->instances + i))
                        rdata = AH5_FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_GLOBAL_ENVIRONMENT, path);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read exchangeSurface category
char AH5_read_exchange_surface (hid_t file_id, AH5_exchange_surface_t *exchange_surface)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH], rdata = AH5_TRUE;
    AH5_children_t children;
    hsize_t i;

    exchange_surface->groups = NULL;

    if (AH5_path_valid(file_id, AH5_C_EXCHANGE_SURFACE))
    {
        children = AH5_read_children_name(file_id, AH5_C_EXCHANGE_SURFACE);
        exchange_surface->nb_groups = children.nb_children;
        if (children.nb_children > 0)
        {
            exchange_surface->groups = (AH5_exs_group_t *) malloc(children.nb_children * sizeof(AH5_exs_group_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path, AH5_C_EXCHANGE_SURFACE);
                strcat(path, children.childnames[i]);
                if (!AH5_read_exs_group(file_id, path, exchange_surface->groups + i))
                    rdata = AH5_FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_EXCHANGE_SURFACE, AH5_C_EXCHANGE_SURFACE);
        rdata = AH5_FALSE;
    }
    return rdata;
}




// Print exchangeSurface group
void AH5_print_exs_group (const AH5_exs_group_t *exs_group, int space)
{
    hsize_t i;

    printf("%*sGroup: %s\n", space, "", AH5_get_name_from_path(exs_group->path));
    switch (exs_group->type)
    {
    case EXS_TYPE_RECIPROCITY:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_RECIPROCITY, space + 4);
        break;
    case EXS_TYPE_HUYGENS:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_HUYGENS, space + 4);
        break;
    case EXS_TYPE_GAUSS:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_GAUSS, space + 4);
        break;
    case EXS_TYPE_INVALID:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_INVALID, space + 4);
        break;
    }
    switch (exs_group->nature)
    {
    case EXS_NATURE_OUTSIDE:
        AH5_print_str_attr(AH5_A_NATURE, AH5_V_OUTSIDE, space + 4);
        break;
    case EXS_NATURE_INSIDE:
        AH5_print_str_attr(AH5_A_NATURE, AH5_V_INSIDE, space + 4);
        break;
    case EXS_NATURE_INVALID:
        AH5_print_str_attr(AH5_A_NATURE, AH5_V_INVALID, space + 4);
        break;
    }
    for (i = 0; i < exs_group->nb_instances; i++)
        AH5_print_ft_arrayset(&(exs_group->instances[i]), space + 2);
}


// Print exchangeSurface category
void AH5_print_exchange_surface (const AH5_exchange_surface_t *exchange_surface)
{
    hsize_t i;

    printf("#############################  Exchange surface  #############################\n\n");
    for (i = 0; i < exchange_surface->nb_groups; i++)
        AH5_print_exs_group(&(exchange_surface->groups[i]), 0);
    printf("\n");
}




// Free memory used by exchangeSurface group
void AH5_free_exs_group (AH5_exs_group_t *exs_group)
{
    hsize_t i;

    if (exs_group->path != NULL)
    {
        free(exs_group->path);
        exs_group->path = NULL;
    }
    if (exs_group->instances != NULL)
    {
        for (i = 0; i < exs_group->nb_instances; i++)
            AH5_free_ft_arrayset(exs_group->instances + i);
        free(exs_group->instances);
        exs_group->instances = NULL;
        exs_group->nb_instances = 0;
    }
    exs_group->type = EXS_TYPE_INVALID;
    exs_group->nature = EXS_NATURE_INVALID;
}


// Free memory used by exchangeSurface category
void AH5_free_exchange_surface (AH5_exchange_surface_t *exchange_surface)
{
    hsize_t i;

    if (exchange_surface->groups != NULL)
    {
        for (i = 0; i < exchange_surface->nb_groups; i++)
            AH5_free_exs_group(exchange_surface->groups + i);
        free(exchange_surface->groups);
        exchange_surface->groups = NULL;
        exchange_surface->nb_groups = 0;
    }
}

