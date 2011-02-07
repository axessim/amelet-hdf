#ifndef LINK_H
#define LINK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"

#define A_SUBJECT "subject"
#define A_OBJECT "object"

    typedef struct
    {
        char        *name;
        optional_attributes_t optional_attributes;
        char        *subject;
        char        *object;
    } link_instance_t;

    typedef struct
    {
        char        *name;
        optional_attributes_t optional_attributes;
        hsize_t     nb_link_instances;
        link_instance_t *link_instances;
    } link_group_t;

    typedef struct
    {
        hsize_t     nb_link_groups;
        link_group_t *link_groups;
    } link_t;

    void read_link_instance (hid_t file_id, const char *path, link_instance_t *link_instance);
    void read_link_group (hid_t file_id, const char *path, link_group_t *link_group);
    void read_link (hid_t file_id, link_t *link);

    void print_link_instance (link_instance_t link_instance);
    void print_link_group (link_group_t link_group);
    void print_link (link_t link);

    void free_link_instance(link_instance_t *link_instance);
    void free_link_group(link_group_t *link_group);
    void free_link (link_t *link);

#ifdef __cplusplus
}
#endif

#endif // LINK_H
