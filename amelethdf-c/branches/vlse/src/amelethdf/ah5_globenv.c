#include "ah5_globenv.h"


// Read globalEnvironment instance
void AH5_read_global_environment_instance (hid_t file_id, const char *path, AH5_gle_instance_t *gle_instance)
{
    char path2[AH5_ABSOLUTE_PATH_LENGTH], path3[AH5_ABSOLUTE_PATH_LENGTH];
    char mandatory[][AH5_ATTR_LENGTH] = {};

    gle_instance->path = strdup(path);
    strcpy(path2, path);
    strcat(path2, AH5_G_LIMIT_CONDITIONS);
    AH5_read_opt_attrs(file_id, path2, &(gle_instance->limit_conditions), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);

    strcpy(path2, path);
    strcpy(path3, path);
    strcat(path2, AH5_G_FREQUENCY);
    strcat(path3, AH5_G_TIME);
    gle_instance->type = GE_INVALID;
    if (H5Lexists(file_id, path2, H5P_DEFAULT) > 0 && H5Lexists(file_id, path3, H5P_DEFAULT) <= 0)  // only frequency...
    {
        gle_instance->type = GE_FREQUENCY;
        AH5_read_floatingtype(file_id, path2, &(gle_instance->data));
    }
    if (H5Lexists(file_id, path3, H5P_DEFAULT) > 0 && H5Lexists(file_id, path2, H5P_DEFAULT) <= 0)  // only time...
    {
        gle_instance->type = GE_TIME;
        AH5_read_floatingtype(file_id, path3, &(gle_instance->data));
    }
}


// Read globalEnvironment category
void AH5_read_global_environment (hid_t file_id, AH5_global_environment_t *global_environment)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH];
    AH5_children_t children;
    hsize_t i;

    children = AH5_read_children_name(file_id, AH5_C_GLOBAL_ENVIRONMENT);
    global_environment->nb_instances = children.nb_children;
    global_environment->instances = NULL;
    if (children.nb_children > 0)
    {
        global_environment->instances = (AH5_gle_instance_t *) malloc(children.nb_children * sizeof(AH5_gle_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, AH5_C_GLOBAL_ENVIRONMENT);
            strcat(path, children.childnames[i]);
            AH5_read_global_environment_instance(file_id, path, global_environment->instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}




// Print globalEnvironment instance
void AH5_print_global_environment_instance (const AH5_gle_instance_t *gle_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", AH5_get_name_from_path(gle_instance->path));
    AH5_print_floatingtype(&(gle_instance->data), space + 3);
    AH5_print_opt_attrs(&(gle_instance->limit_conditions), space + 6);
}


// Print globalEnvironment category
void AH5_print_global_environment (const AH5_global_environment_t *global_environment)
{
    hsize_t i;

    printf("############################  Global environment  ############################\n\n");
    for (i = 0; i < global_environment->nb_instances; i++)
        AH5_print_global_environment_instance(&(global_environment->instances[i]), 0);
    printf("\n");
}




// Free memory used by globalEnvironment instance
void AH5_free_global_environment_instance (AH5_gle_instance_t *gle_instance)
{
    if (gle_instance->path != NULL)
    {
        free(gle_instance->path);
        gle_instance->path = NULL;
    }
    if (gle_instance->type != GE_INVALID)
        AH5_free_floatingtype(&(gle_instance->data));
    AH5_free_opt_attrs(&(gle_instance->limit_conditions));
}


// Free memory used by globalEnvironment category
void AH5_free_global_environment (AH5_global_environment_t *global_environment)
{
    hsize_t i;

    if (global_environment->nb_instances > 0)
    {
        for (i = 0; i < global_environment->nb_instances; i++)
            AH5_free_global_environment_instance(global_environment->instances + i);
        free(global_environment->instances);
        global_environment->instances = NULL;
        global_environment->nb_instances = 0;
    }
}

