#include "ah5_locsys.h"


// Read localizationSystem transformation
char AH5_read_lsm_transformation (hid_t file_id, const char *path, AH5_lsm_transf_t *lsm_transformation)
{
    char *type, rdata = TRUE;

    lsm_transformation->path = strdup(path);
    lsm_transformation->type = TRF_INVALID;

    if (AH5_path_valid(file_id, path))
    {
        if (AH5_read_str_attr(file_id, path, AH5_A_TYPE, &type))
        {
            if (strcmp(type, AH5_V_SCALE) == 0)
                lsm_transformation->type = TRF_SCALE;
            else if (strcmp(type, AH5_V_ROTATION) == 0)
                lsm_transformation->type = TRF_ROTATION;
            else if (strcmp(type, AH5_V_TRANSLATION) == 0)
                lsm_transformation->type = TRF_TRANSLATION;
            free(type);
        }
        else
            rdata = FALSE;
        if(!AH5_read_int_attr(file_id, path, AH5_A_RANK, &(lsm_transformation->rank)))
            rdata = FALSE;
    }
    else
    {
        AH5_print_err_path(AH5_C_LOCALIZATION_SYSTEM, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read localizationSystem instance
char AH5_read_lsm_instance (hid_t file_id, const char *path, AH5_lsm_instance_t *lsm_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_DIMENSION}, rdata = TRUE;
    char path2[AH5_ABSOLUTE_PATH_LENGTH];
    AH5_children_t children;
    hsize_t i;

    lsm_instance->path = strdup(path);
    lsm_instance->transformations = NULL;
    lsm_instance->opt_attrs.instances = NULL;

    if (AH5_path_valid(file_id, path))
    {
        AH5_read_opt_attrs(file_id, path, &(lsm_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
        AH5_read_int_attr(file_id, path, AH5_A_DIMENSION, &(lsm_instance->dimension));

        children = AH5_read_children_name(file_id, path);
        lsm_instance->nb_transformations = children.nb_children;
        if (children.nb_children > 0)
        {
            lsm_instance->transformations = (AH5_lsm_transf_t *) malloc(children.nb_children * sizeof(AH5_lsm_transf_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path2, path);
                strcat(path2, children.childnames[i]);
                if (!AH5_read_lsm_transformation(file_id, path2, lsm_instance->transformations + i))
                    rdata = FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_LOCALIZATION_SYSTEM, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read localizationSystem category
char AH5_read_localization_system (hid_t file_id, AH5_localization_system_t *localization_system)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH], rdata = TRUE;
    AH5_children_t children;
    hsize_t i;

    localization_system->instances = NULL;

    if (H5Lexists(file_id, AH5_C_LOCALIZATION_SYSTEM, H5P_DEFAULT) == TRUE)
    {
        children = AH5_read_children_name(file_id, AH5_C_LOCALIZATION_SYSTEM);
        localization_system->nb_instances = children.nb_children;
        if (children.nb_children > 0)
        {
            localization_system->instances = (AH5_lsm_instance_t *) malloc(children.nb_children * sizeof(AH5_lsm_instance_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path, AH5_C_LOCALIZATION_SYSTEM);
                strcat(path, children.childnames[i]);
                if(!AH5_read_lsm_instance(file_id, path, localization_system->instances + i))
                    rdata = FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_LOCALIZATION_SYSTEM, AH5_C_LOCALIZATION_SYSTEM);
        rdata = FALSE;
    }
    return rdata;
}




// Print localizationSystem transformation
void AH5_print_lsm_transformation (const AH5_lsm_transf_t *lsm_transformation, int space)
{
    printf("%*sTransformation: %s\n", space, "", AH5_get_name_from_path(lsm_transformation->path));
    AH5_print_int_attr(AH5_A_RANK, lsm_transformation->rank, space + 3);
    switch (lsm_transformation->type)
    {
    case TRF_SCALE:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_SCALE, space + 3);
        break;
    case TRF_ROTATION:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_ROTATION, space + 3);
        break;
    case TRF_TRANSLATION:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_TRANSLATION, space + 3);
        break;
    default:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_INVALID, space + 3);
        break;
    }
}


// Print localizationSystem instance
void AH5_print_lsm_instance (const AH5_lsm_instance_t *lsm_instance, int space)
{
    hsize_t i;

    printf("%*sInstance: %s\n", space, "", AH5_get_name_from_path(lsm_instance->path));
    AH5_print_int_attr(AH5_A_DIMENSION, lsm_instance->dimension, space + 4);
    AH5_print_opt_attrs(&(lsm_instance->opt_attrs), space + 4);
    for (i = 0; i < lsm_instance->nb_transformations; i++)
        AH5_print_lsm_transformation(&(lsm_instance->transformations[i]), space + 2);
}


// Print localizationSystem category
void AH5_print_localization_system (const AH5_localization_system_t *localization_system)
{
    hsize_t i;

    printf("############################  Localization system  ###########################\n\n");
    for (i = 0; i < localization_system->nb_instances; i++)
        AH5_print_lsm_instance(&(localization_system->instances[i]), 0);
    printf("\n");
}




// Free memory used by localizationSystem transformation
void AH5_free_lsm_transformation (AH5_lsm_transf_t *lsm_transformation)
{
    if (lsm_transformation->path != NULL)
    {
        free(lsm_transformation->path);
        lsm_transformation->path = NULL;
    }
    lsm_transformation->type = TRF_INVALID;
}


// Free memory used by localizationSystem instance
void AH5_free_lsm_instance (AH5_lsm_instance_t *lsm_instance)
{
    hsize_t i;

    if (lsm_instance->path != NULL)
    {
        free(lsm_instance->path);
        lsm_instance->path = NULL;
    }
    AH5_free_opt_attrs(&(lsm_instance->opt_attrs));
    if (lsm_instance->transformations != NULL)
    {
        for (i = 0; i < lsm_instance->nb_transformations; i++)
            AH5_free_lsm_transformation(lsm_instance->transformations + i);
        free(lsm_instance->transformations);
        lsm_instance->transformations = NULL;
        lsm_instance->nb_transformations = 0;
    }
}


// Free memory used by localizationSystem category
void AH5_free_localization_system (AH5_localization_system_t *localization_system)
{
    hsize_t i;

    if (localization_system->instances != NULL)
    {
        for (i = 0; i < localization_system->nb_instances; i++)
            AH5_free_lsm_instance(localization_system->instances + i);
        free(localization_system->instances);
        localization_system->instances = NULL;
        localization_system->nb_instances = 0;
    }
}
