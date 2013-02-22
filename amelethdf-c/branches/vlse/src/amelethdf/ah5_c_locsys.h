#ifndef AH5_C_LOCSYS_H
#define AH5_C_LOCSYS_H

#include "ah5_general.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum _AH5_lsm_transf_class_t
    {
        TRF_INVALID             = -1,
        TRF_SCALE               = 1,
        TRF_ROTATION            = 2,
        TRF_TRANSLATION         = 3
    } AH5_lsm_transf_class_t;

    typedef struct _AH5_lsm_transf_t
    {
        char            *path;
        AH5_lsm_transf_class_t type;
        int             rank;
    } AH5_lsm_transf_t;

    typedef struct _AH5_lsm_instance_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        int             dimension;
        hsize_t         nb_transformations;
        AH5_lsm_transf_t *transformations;
    } AH5_lsm_instance_t;

    typedef struct _AH5_localization_system_t
    {
        hsize_t         nb_instances;
        AH5_lsm_instance_t *instances;
    } AH5_localization_system_t;

    AH5_PUBLIC char AH5_read_lsm_transformation (hid_t file_id, const char *path, AH5_lsm_transf_t *lsm_transformation);
    AH5_PUBLIC char AH5_read_lsm_instance (hid_t file_id, const char *path, AH5_lsm_instance_t *lsm_instance);
    AH5_PUBLIC char AH5_read_localization_system (hid_t file_id, AH5_localization_system_t *localization_system);

    AH5_PUBLIC void AH5_print_lsm_transformation (const AH5_lsm_transf_t *lsm_transformation, int space);
    AH5_PUBLIC void AH5_print_lsm_instance (const AH5_lsm_instance_t *lsm_instance, int space);
    AH5_PUBLIC void AH5_print_localization_system (const AH5_localization_system_t *localization_system);

    AH5_PUBLIC void AH5_free_lsm_transformation (AH5_lsm_transf_t *lsm_transformation);
    AH5_PUBLIC void AH5_free_lsm_instance (AH5_lsm_instance_t *lsm_instance);
    AH5_PUBLIC void AH5_free_localization_system (AH5_localization_system_t *localization_system);

#ifdef __cplusplus
}
#endif

#endif // AH5_C_LOCSYS_H
