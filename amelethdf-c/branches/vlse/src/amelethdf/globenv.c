#include "globenv.h"


// Read globalEnvironment instance
void read_global_environment_instance (hid_t file_id, const char *path, gle_instance_t *gle_instance)
{
    char path2[ABSOLUTE_PATH_NAME_LENGTH], path3[ABSOLUTE_PATH_NAME_LENGTH];
    char mandatory[][ATTR_LENGTH] = {};

    gle_instance->path = strdup(path);
    strcpy(path2, path);
    strcat(path2, G_LIMIT_CONDITIONS);
    read_opt_attrs(file_id, path2, &(gle_instance->limit_conditions), mandatory, sizeof(mandatory)/ATTR_LENGTH);

    strcpy(path2, path);
    strcpy(path3, path);
    strcat(path2, G_FREQUENCY);
    strcat(path3, G_TIME);
    gle_instance->type = GE_INVALID;
    if (H5Lexists(file_id, path2, H5P_DEFAULT) > 0 && H5Lexists(file_id, path3, H5P_DEFAULT) <= 0)  // only frequency...
    {
        gle_instance->type = GE_FREQUENCY;
        read_floatingtype(file_id, path2, &(gle_instance->data));
    }
    if (H5Lexists(file_id, path3, H5P_DEFAULT) > 0 && H5Lexists(file_id, path2, H5P_DEFAULT) <= 0)  // only time...
    {
        gle_instance->type = GE_TIME;
        read_floatingtype(file_id, path3, &(gle_instance->data));
    }
}


// Read globalEnvironment category
void read_global_environment (hid_t file_id, global_environment_t *global_environment)
{
    char path[ABSOLUTE_PATH_NAME_LENGTH];
    children_t children;
    hsize_t i;

    children = read_children_name(file_id, C_GLOBAL_ENVIRONMENT);
    global_environment->nb_instances = children.nb_children;
    global_environment->instances = NULL;
    if (children.nb_children > 0)
    {
        global_environment->instances = (gle_instance_t *) malloc(children.nb_children * sizeof(gle_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_GLOBAL_ENVIRONMENT);
            strcat(path, children.childnames[i]);
            read_global_environment_instance(file_id, path, global_environment->instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}




// Print globalEnvironment instance
void print_global_environment_instance (gle_instance_t gle_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", get_name_from_path(gle_instance.path));
    print_floatingtype(gle_instance.data, space + 3);
    print_opt_attrs(gle_instance.limit_conditions, space + 6);
}


// Print globalEnvironment category
void print_global_environment (global_environment_t global_environment)
{
    hsize_t i;

    printf("############################  Global environment  ############################\n\n");
    for (i = 0; i < global_environment.nb_instances; i++)
        print_global_environment_instance(global_environment.instances[i], 0);
    printf("\n");
}




// Free memory used by globalEnvironment instance
void free_global_environment_instance (gle_instance_t *gle_instance)
{
    if (gle_instance->path != NULL)
    {
        free(gle_instance->path);
        gle_instance->path = NULL;
    }
    if (gle_instance->type != GE_INVALID)
        free_floatingtype(&(gle_instance->data));
    free_opt_attrs(&(gle_instance->limit_conditions));
}


// Free memory used by globalEnvironment category
void free_global_environment (global_environment_t *global_environment)
{
    hsize_t i;

    if (global_environment->nb_instances > 0)
    {
        for (i = 0; i < global_environment->nb_instances; i++)
            free_global_environment_instance(global_environment->instances + i);
        free(global_environment->instances);
        global_environment->instances = NULL;
        global_environment->nb_instances = 0;
    }
}

