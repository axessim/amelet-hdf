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

    typedef struct outputrequest_instance_t
    {
        char        *name;
        optional_attributes_t optional_attributes;
        char        *subject;
        char        *object;
        char        *output;
    } outputrequest_instance_t;

    typedef struct outputrequest_group_t
    {
        char        *name;
        optional_attributes_t optional_attributes;
        hsize_t     nb_outputrequest_instances;
        outputrequest_instance_t *outputrequest_instances;
    } outputrequest_group_t;

    typedef struct outputrequest_t
    {
        hsize_t     nb_outputrequest_groups;
        outputrequest_group_t *outputrequest_groups;
    } outputrequest_t;

    void read_outputrequest_instance (hid_t file_id, const char *path, outputrequest_instance_t *outputrequest_instance);
    void read_outputrequest_group (hid_t file_id, const char *path, outputrequest_group_t *outputrequest_group);
    void read_outputrequest(hid_t file_id, outputrequest_t *outputrequest);

    void print_outputrequest_instance (outputrequest_instance_t outputrequest_instance, int space);
    void print_outputrequest_group (outputrequest_group_t outputrequest_group, int space);
    void print_outputrequest (outputrequest_t outputrequest);

    void free_outputrequest_instance (outputrequest_instance_t *outputrequest_instance);
    void free_outputrequest_group (outputrequest_group_t *outputrequest_group);
    void free_outputrequest (outputrequest_t *outputrequest);

#ifdef __cplusplus
}
#endif

#endif // OUTPUTREQUEST_H
