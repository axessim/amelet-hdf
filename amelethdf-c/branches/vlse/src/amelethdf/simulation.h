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

    typedef struct simulation_instance_t
    {
        char        *name;
        optional_attributes_t optional_attributes;
        char        *module;
        char        *version;
        hsize_t     nb_inputs;
        char        **inputs;
        hsize_t     nb_outputs;
        char        **outputs;
    } simulation_instance_t;

    typedef struct simulation_t
    {
        hsize_t     nb_simulation_instances;
        simulation_instance_t *simulation_instances;
    } simulation_t;

    void read_simulation_instance(hid_t file_id, const char *path, simulation_instance_t *simulation_instance);
    void read_simulation(hid_t file_id, simulation_t *simulation);

    void print_simulation_instance(simulation_instance_t simulation_instance, int space);
    void print_simulation(simulation_t simulation);

    void free_simulation_instance(simulation_instance_t *simulation_instance);
    void free_simulation(simulation_t *simulation);

#ifdef __cplusplus
}
#endif

#endif // SIMULATION_H
