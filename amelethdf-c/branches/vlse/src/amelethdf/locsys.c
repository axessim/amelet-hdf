#include "locsys.h"


// Read localizationSystem transformation
void read_lsm_transformation (hid_t file_id, const char *path, lsm_transformation_t *lsm_transformation)
{
    char *type;

    lsm_transformation->path = strdup(path);
    lsm_transformation->type = TRF_INVALID;
    if (read_str_attr(file_id, path, A_TYPE, &type))
    {
        if (strcmp(type, V_SCALE) == 0)
            lsm_transformation->type = TRF_SCALE;
        else if (strcmp(type, V_ROTATION) == 0)
            lsm_transformation->type = TRF_ROTATION;
        else if (strcmp(type, V_TRANSLATION) == 0)
            lsm_transformation->type = TRF_TRANSLATION;
        free(type);
    }
    read_int_attr(file_id, path, A_RANK, &(lsm_transformation->rank));
}


// Read localizationSystem instance
void read_lsm_instance (hid_t file_id, const char *path, lsm_instance_t *lsm_instance)
{
    char mandatory[][ATTR_LENGTH] = {A_DIMENSION};
    char path2[ABSOLUTE_PATH_NAME_LENGTH];
    children_t children;
    hsize_t i;

    lsm_instance->path = strdup(path);
    read_opt_attrs(file_id, path, &(lsm_instance->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
    read_int_attr(file_id, path, A_DIMENSION, &(lsm_instance->dimension));

    children = read_children_name(file_id, path);
    lsm_instance->nb_transformations = children.nb_children;
    lsm_instance->transformations = NULL;
    if (children.nb_children > 0)
    {
        lsm_instance->transformations = (lsm_transformation_t *) malloc(children.nb_children * sizeof(lsm_transformation_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path2, path);
            strcat(path2, children.childnames[i]);
            read_lsm_transformation(file_id, path2, lsm_instance->transformations + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }

}


// Read localizationSystem category
void read_localization_system (hid_t file_id, localization_system_t *localization_system)
{
    char path[ABSOLUTE_PATH_NAME_LENGTH];
    children_t children;
    hsize_t i;

    children = read_children_name(file_id, C_LOCALIZATION_SYSTEM);
    localization_system->nb_instances = children.nb_children;
    localization_system->instances = NULL;
    if (children.nb_children > 0)
    {
        localization_system->instances = (lsm_instance_t *) malloc(children.nb_children * sizeof(lsm_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_LOCALIZATION_SYSTEM);
            strcat(path, children.childnames[i]);
            read_lsm_instance(file_id, path, localization_system->instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}




// Print localizationSystem transformation
void print_lsm_transformation (lsm_transformation_t lsm_transformation, int space)
{
    printf("%*sTransformation: %s\n", space, "", get_name_from_path(lsm_transformation.path));
    print_int_attr(A_RANK, lsm_transformation.rank, space + 3);
    switch (lsm_transformation.type)
    {
    case TRF_SCALE:
        print_str_attr(A_TYPE, V_SCALE, space + 3);
        break;
    case TRF_ROTATION:
        print_str_attr(A_TYPE, V_ROTATION, space + 3);
        break;
    case TRF_TRANSLATION:
        print_str_attr(A_TYPE, V_TRANSLATION, space + 3);
        break;
    default:
        print_str_attr(A_TYPE, V_INVALID, space + 3);
        break;
    }
}


// Print localizationSystem instance
void print_lsm_instance (lsm_instance_t lsm_instance, int space)
{
    hsize_t i;

    printf("%*sInstance: %s\n", space, "", get_name_from_path(lsm_instance.path));
    print_int_attr(A_DIMENSION, lsm_instance.dimension, space + 4);
    print_opt_attrs(lsm_instance.opt_attrs, space + 4);
    for (i = 0; i < lsm_instance.nb_transformations; i++)
        print_lsm_transformation(lsm_instance.transformations[i], space + 2);
}


// Print localizationSystem category
void print_localization_system (localization_system_t localization_system)
{
    hsize_t i;

    printf("############################  Localization system  ###########################\n\n");
    for (i = 0; i < localization_system.nb_instances; i++)
        print_lsm_instance(localization_system.instances[i], 0);
    printf("\n");
}




// Free memory used by localizationSystem transformation
void free_lsm_transformation (lsm_transformation_t *lsm_transformation)
{
    if (lsm_transformation->path != NULL)
    {
        free(lsm_transformation->path);
        lsm_transformation->path = NULL;
    }
}


// Free memory used by localizationSystem instance
void free_lsm_instance (lsm_instance_t *lsm_instance)
{
    hsize_t i;

    if (lsm_instance->path != NULL)
    {
        free(lsm_instance->path);
        lsm_instance->path = NULL;
    }
    free_opt_attrs(&(lsm_instance->opt_attrs));
    if (lsm_instance->nb_transformations > 0)
    {
        for (i = 0; i < lsm_instance->nb_transformations; i++)
            free_lsm_transformation(lsm_instance->transformations + i);
        free(lsm_instance->transformations);
        lsm_instance->transformations = NULL;
        lsm_instance->nb_transformations = 0;
    }
}


// Free memory used by localizationSystem category
void free_localization_system (localization_system_t *localization_system)
{
    hsize_t i;

    if (localization_system->nb_instances > 0)
    {
        for (i = 0; i < localization_system->nb_instances; i++)
            free_lsm_instance(localization_system->instances + i);
        free(localization_system->instances);
        localization_system->instances = NULL;
        localization_system->nb_instances = 0;
    }
}
