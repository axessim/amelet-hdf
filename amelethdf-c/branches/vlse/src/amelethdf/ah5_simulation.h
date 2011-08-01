#ifndef AH5_SIMULATION_H
#define AH5_SIMULATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ah5_general.h"

    typedef struct _AH5_sim_instance_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        AH5_opt_attrs_t parameter;
        char            *module;
        char            *version;
        hsize_t         nb_inputs;
        char            **inputs;
        hsize_t         nb_outputs;
        char            **outputs;
    } AH5_sim_instance_t;

    typedef struct _AH5_simulation_t
    {
        hsize_t         nb_instances;
        AH5_sim_instance_t *instances;
    } AH5_simulation_t;

    char AH5_read_sim_instance (hid_t file_id, const char *path, AH5_sim_instance_t *sim_instance);
    char AH5_read_simulation (hid_t file_id, AH5_simulation_t *simulation);

    void AH5_print_sim_instance (const AH5_sim_instance_t *sim_instance, int space);
    void AH5_print_simulation (const AH5_simulation_t *simulation);

    void AH5_free_sim_instance (AH5_sim_instance_t *sim_instance);
    void AH5_free_simulation (AH5_simulation_t *simulation);

#ifdef __cplusplus
}
#endif

#endif // AH5_SIMULATION_H
