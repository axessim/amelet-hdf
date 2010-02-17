#include "stringdataset.h"

#ifndef SIMULATION_H
#define SIMULATION_H

#define C_SIMULATION "/simulation"
#define INPUTS "/inputs"
#define OUTPUTS "/outputs"
#define PARAMETERS "/parameters"

typedef struct
{
        char* name;
        char** inputs;
        char** outputs;
        int* nb_input;
        int* nb_output;
} simulation_t;

simulation_t read_simulation(hid_t file_id, const char* sim_path);
int* read_nb_inputs_outputs(hid_t file_id, const char* path);
char ** read_inputs_outputs(hid_t file_id, const char* path);
void printSimulation(simulation_t sim);

#endif // SIMULATION_H
