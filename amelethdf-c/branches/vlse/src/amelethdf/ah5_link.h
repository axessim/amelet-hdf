#ifndef AH5_LINK_H
#define AH5_LINK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ah5_general.h"

    typedef struct AH5_lnk_instance_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        char            *subject;
        char            *object;
    } AH5_lnk_instance_t;

    typedef struct AH5_lnk_group_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        hsize_t         nb_instances;
        AH5_lnk_instance_t *instances;
    } AH5_lnk_group_t;

    typedef struct AH5_link_t
    {
        hsize_t         nb_groups;
        AH5_lnk_group_t *groups;
    } AH5_link_t;

    char AH5_read_lnk_instance (hid_t file_id, const char *path, AH5_lnk_instance_t *lnk_instance);
    char AH5_read_lnk_group (hid_t file_id, const char *path, AH5_lnk_group_t *lnk_group);
    char AH5_read_link (hid_t file_id, AH5_link_t *link);

    void AH5_print_lnk_instance (const AH5_lnk_instance_t *lnk_instance, int space);
    void AH5_print_lnk_group (const AH5_lnk_group_t *lnk_group, int space);
    void AH5_print_link (const AH5_link_t *link);

    void AH5_free_lnk_instance (AH5_lnk_instance_t *lnk_instance);
    void AH5_free_lnk_group (AH5_lnk_group_t *lnk_group);
    void AH5_free_link (AH5_link_t *link);

#ifdef __cplusplus
}
#endif

#endif // AH5_LINK_H
