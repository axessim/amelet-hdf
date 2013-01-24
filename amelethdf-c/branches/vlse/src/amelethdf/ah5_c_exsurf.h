#ifndef AH5_C_EXSURF_H
#define AH5_C_EXSURF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ah5_c_fltype.h"

    typedef enum _AH5_exs_type_class_t
    {
        EXS_TYPE_INVALID            = -1,
        EXS_TYPE_RECIPROCITY        = 1,
        EXS_TYPE_HUYGENS            = 2,
        EXS_TYPE_GAUSS              = 3
    } AH5_exs_type_class_t;

    typedef enum _AH5_exs_nature_class_t
    {
        EXS_NATURE_INVALID          = -1,
        EXS_NATURE_OUTSIDE          = 1,
        EXS_NATURE_INSIDE           = 2
    } AH5_exs_nature_class_t;

    typedef struct _AH5_exs_group_t
    {
        char            *path;
        AH5_exs_type_class_t type;
        AH5_exs_nature_class_t nature;
        hsize_t         nb_instances;
        AH5_arrayset_t  *instances;
    } AH5_exs_group_t;

    typedef struct _AH5_exchange_surface_t
    {
        hsize_t         nb_groups;
        AH5_exs_group_t *groups;
    } AH5_exchange_surface_t;

    char AH5_read_exs_group (hid_t file_id, const char *path, AH5_exs_group_t *exs_group);
    char AH5_read_exchange_surface (hid_t file_id, AH5_exchange_surface_t *exchange_surface);

    void AH5_print_exs_group (const AH5_exs_group_t *exs_group, int space);
    void AH5_print_exchange_surface (const AH5_exchange_surface_t *exchange_surface);

    void AH5_free_exs_group (AH5_exs_group_t *exs_group);
    void AH5_free_exchange_surface (AH5_exchange_surface_t *exchange_surface);

#ifdef __cplusplus
}
#endif

#endif // AH5_C_EXSURF_H
