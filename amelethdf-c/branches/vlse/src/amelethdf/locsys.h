#ifndef LOCSYS_H
#define LOCSYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"

#define A_TYPE "type"
#define A_RANK "rank"
#define A_DIMENSION "dimension"

#define V_SCALE "scale"
#define V_ROTATION "rotation"
#define V_TRANSLATION "translation"

    typedef enum lsm_transf_class_t
    {
        TRF_INVALID             = -1,
        TRF_SCALE               = 1,
        TRF_ROTATION            = 2,
        TRF_TRANSLATION         = 3
    } lsm_transf_class_t;

    typedef struct lsm_transformation_t
    {
        char        *path;
        lsm_transf_class_t type;
        int         rank;
    } lsm_transformation_t;

    typedef struct lsm_instance_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        int         dimension;
        hsize_t     nb_transformations;
        lsm_transformation_t *transformations;
    } lsm_instance_t;

    typedef struct localization_system_t
    {
        hsize_t     nb_instances;
        lsm_instance_t *instances;
    } localization_system_t;

    void read_lsm_transformation (hid_t file_id, const char *path, lsm_transformation_t *lsm_transformation);
    void read_lsm_instance (hid_t file_id, const char *path, lsm_instance_t *lsm_instance);
    void read_localization_system (hid_t file_id, localization_system_t *localization_system);

    void print_lsm_transformation (lsm_transformation_t lsm_transformation, int space);
    void print_lsm_instance (lsm_instance_t lsm_instance, int space);
    void print_localization_system (localization_system_t localization_system);

    void free_lsm_transformation (lsm_transformation_t *lsm_transformation);
    void free_lsm_instance (lsm_instance_t *lsm_instance);
    void free_localization_system (localization_system_t *localization_system);

#ifdef __cplusplus
}
#endif

#endif // LOCSYS_H
