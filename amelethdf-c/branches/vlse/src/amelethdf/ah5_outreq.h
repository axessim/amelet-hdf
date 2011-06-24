#ifndef AH5_OUTREQ_H
#define AH5_OUTREQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ah5_general.h"
#include "ah5_label.h"

    typedef struct _AH5_ort_instance_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        char            *subject;
        char            *object;
        char            *subject_name;
        char            *output;
    } AH5_ort_instance_t;

    typedef struct _AH5_ort_group_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        hsize_t         nb_instances;
        AH5_ort_instance_t *instances;
    } AH5_ort_group_t;

    typedef struct _AH5_outputrequest_t
    {
        hsize_t         nb_groups;
        AH5_ort_group_t *groups;
    } AH5_outputrequest_t;

    char AH5_read_ort_instance (hid_t file_id, const char *path, AH5_ort_instance_t *ort_instance);
    char AH5_read_ort_group (hid_t file_id, const char *path, AH5_ort_group_t *ort_group);
    char AH5_read_outputrequest (hid_t file_id, AH5_outputrequest_t *outputrequest);

    void AH5_print_ort_instance (const AH5_ort_instance_t *ort_instance, int space);
    void AH5_print_ort_group (const AH5_ort_group_t *ort_group, int space);
    void AH5_print_outputrequest (const AH5_outputrequest_t *outputrequest);

    void AH5_free_ort_instance (AH5_ort_instance_t *ort_instance);
    void AH5_free_ort_group (AH5_ort_group_t *ort_group);
    void AH5_free_outputrequest (AH5_outputrequest_t *outputrequest);

#ifdef __cplusplus
}
#endif

#endif // AH5_OUTREQ_H
