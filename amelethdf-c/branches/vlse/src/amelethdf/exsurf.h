#ifndef EXSURF_H
#define EXSURF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "floatingtype.h"

#define A_TYPE "type"
#define A_NATURE "nature"

#define V_RECIPROCITY "reciprocity"
#define V_HUYGENS "huygens"
#define V_GAUSS "gauss"
#define V_OUTSIDE "outside"
#define V_INSIDE "inside"

    typedef enum exs_type_class_t
    {
        EXS_TYPE_INVALID            = -1,
        EXS_TYPE_RECIPROCITY        = 1,
        EXS_TYPE_HUYGENS            = 2,
        EXS_TYPE_GAUSS              = 3
    } exs_type_class_t;

    typedef enum exs_nature_class_t
    {
        EXS_NATURE_INVALID          = -1,
        EXS_NATURE_OUTSIDE          = 1,
        EXS_NATURE_INSIDE           = 2
    } exs_nature_class_t;

    typedef struct exs_group_t
    {
        char        *name;
        exs_type_class_t type;
        exs_nature_class_t nature;
        hsize_t     nb_instances;
        arrayset_t  *instances;
    } exs_group_t;

    typedef struct exchange_surface_t
    {
        hsize_t     nb_groups;
        exs_group_t *groups;
    } exchange_surface_t;

    void read_exs_group (hid_t file_id, const char *path, exs_group_t *exs_group);
    void read_exchange_surface (hid_t file_id, exchange_surface_t *exchange_surface);

    void print_exs_group (exs_group_t exs_group, int space);
    void print_exchange_surface (exchange_surface_t exchange_surface);

    void free_exs_group (exs_group_t *exs_group);
    void free_exchange_surface (exchange_surface_t *exchange_surface);

#ifdef __cplusplus
}
#endif

#endif // EXSURF_H
