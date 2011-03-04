#ifndef LINK_H
#define LINK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"

#define A_SUBJECT "subject"
#define A_OBJECT "object"

    typedef struct lnk_instance_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        char        *subject;
        char        *object;
    } lnk_instance_t;

    typedef struct lnk_group_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        hsize_t     nb_instances;
        lnk_instance_t *instances;
    } lnk_group_t;

    typedef struct link_t
    {
        hsize_t     nb_groups;
        lnk_group_t *groups;
    } link_t;

    void read_lnk_instance (hid_t file_id, const char *path, lnk_instance_t *lnk_instance);
    void read_lnk_group (hid_t file_id, const char *path, lnk_group_t *lnk_group);
    void read_link (hid_t file_id, link_t *link);

    void print_lnk_instance (lnk_instance_t lnk_instance, int space);
    void print_lnk_group (lnk_group_t lnk_group, int space);
    void print_link (link_t link);

    void free_lnk_instance (lnk_instance_t *lnk_instance);
    void free_lnk_group (lnk_group_t *lnk_group);
    void free_link (link_t *link);

#ifdef __cplusplus
}
#endif

#endif // LINK_H
