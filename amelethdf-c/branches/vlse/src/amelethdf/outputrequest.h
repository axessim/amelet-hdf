#ifndef OUTPUTREQUEST_H
#define OUTPUTREQUEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"

#define A_SUBJECT "subject"
#define A_SUBJECT_ID "subject_id"
#define A_OBJECT "object"
#define A_OUTPUT "output"

    typedef struct ort_instance_t
    {
        char        *name;
        opt_attrs_t opt_attrs;
        char        *subject;
        char        *object;
        char        *output;
    } ort_instance_t;

    typedef struct ort_group_t
    {
        char        *name;
        opt_attrs_t opt_attrs;
        hsize_t     nb_instances;
        ort_instance_t *instances;
    } ort_group_t;

    typedef struct outputrequest_t
    {
        hsize_t     nb_groups;
        ort_group_t *groups;
    } outputrequest_t;

    void read_ort_instance (hid_t file_id, const char *path, ort_instance_t *ort_instance);
    void read_ort_group (hid_t file_id, const char *path, ort_group_t *ort_group);
    void read_outputrequest(hid_t file_id, outputrequest_t *outputrequest);

    void print_ort_instance (ort_instance_t ort_instance, int space);
    void print_ort_group (ort_group_t ort_group, int space);
    void print_outputrequest (outputrequest_t outputrequest);

    void free_ort_instance (ort_instance_t *ort_instance);
    void free_ort_group (ort_group_t *ort_group);
    void free_outputrequest (outputrequest_t *outputrequest);

#ifdef __cplusplus
}
#endif

#endif // OUTPUTREQUEST_H
