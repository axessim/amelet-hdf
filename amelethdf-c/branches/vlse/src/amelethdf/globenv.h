#ifndef GLOBENV_H
#define GLOBENV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "floatingtype.h"

#define G_FREQUENCY "/frequency"
#define G_TIME "/time"
#define G_LIMIT_CONDITIONS "/limitConditions"

    typedef enum gle_class_t
    {
        GE_INVALID              = -1,
        GE_FREQUENCY            = 1,
        GE_TIME                 = 2
    } gle_class_t;

    typedef struct gle_instance_t
    {
        char        *name;
        gle_class_t type;
        floatingtype_t data;
        opt_attrs_t limit_conditions;
    } gle_instance_t;

    typedef struct global_environment_t
    {
        hsize_t     nb_instances;
        gle_instance_t *instances;
    } global_environment_t;

    void read_global_environment_instance (hid_t file_id, const char *path, gle_instance_t *gle_instance);
    void read_global_environment (hid_t file_id, global_environment_t *global_environment);

    void print_global_environment_instance (gle_instance_t gle_instance, int space);
    void print_global_environment (global_environment_t global_environment);

    void free_global_environment_instance (gle_instance_t *gle_instance);
    void free_global_environment (global_environment_t *global_environment);

#ifdef __cplusplus
}
#endif

#endif // GLOBENV_H
