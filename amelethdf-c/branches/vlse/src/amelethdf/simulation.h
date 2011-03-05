#ifndef SIMULATION_H
#define SIMULATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"

#define A_MODULE            "module"
#define A_VERSION           "version"
#define G_INPUTS            "/inputs"
#define G_OUTPUTS           "/outputs"
#define G_PARAMETER         "/parameter"

    typedef struct sim_instance_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        char        *module;
        char        *version;
        hsize_t     nb_inputs;
        char        **inputs;
        hsize_t     nb_outputs;
        char        **outputs;
    } sim_instance_t;

    typedef struct simulation_t
    {
        hsize_t     nb_instances;
        sim_instance_t *instances;
    } simulation_t;

    void read_sim_instance (hid_t file_id, const char *path, sim_instance_t *sim_instance);
    void read_simulation (hid_t file_id, simulation_t *simulation);

    void print_sim_instance (const sim_instance_t *sim_instance, int space);
    void print_simulation (const simulation_t *simulation);

    void free_sim_instance (sim_instance_t *sim_instance);
    void free_simulation (simulation_t *simulation);

#ifdef __cplusplus
}
#endif

#endif // SIMULATION_H
