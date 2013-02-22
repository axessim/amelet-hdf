#ifndef AH5_C_GLOBENV_H
#define AH5_C_GLOBENV_H

#include "ah5_config.h"
#include "ah5_c_fltype.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum _AH5_gle_class_t
    {
        GE_INVALID              = -1,
        GE_FREQUENCY            = 1,
        GE_TIME                 = 2
    } AH5_gle_class_t;

    typedef struct _AH5_gle_instance_t
    {
        char            *path;
        AH5_gle_class_t type;
        AH5_ft_t        data;
        AH5_opt_attrs_t limit_conditions;
    } AH5_gle_instance_t;

    typedef struct _AH5_global_environment_t
    {
        hsize_t         nb_instances;
        AH5_gle_instance_t *instances;
    } AH5_global_environment_t;

    AH5_PUBLIC void AH5_init_global_environment_instance (AH5_gle_instance_t *gle_instance);
    AH5_PUBLIC void AH5_init_global_environment (AH5_global_environment_t *global_environment);

    AH5_PUBLIC char AH5_read_global_environment_instance (hid_t file_id, const char *path, AH5_gle_instance_t *gle_instance);
    AH5_PUBLIC char AH5_read_global_environment (hid_t file_id, AH5_global_environment_t *global_environment);

    AH5_PUBLIC void AH5_print_global_environment_instance (const AH5_gle_instance_t *gle_instance, int space);
    AH5_PUBLIC void AH5_print_global_environment (const AH5_global_environment_t *global_environment);

    AH5_PUBLIC void AH5_free_global_environment_instance (AH5_gle_instance_t *gle_instance);
    AH5_PUBLIC void AH5_free_global_environment (AH5_global_environment_t *global_environment);

#ifdef __cplusplus
}
#endif

#endif // AH5_C_GLOBENV_H
