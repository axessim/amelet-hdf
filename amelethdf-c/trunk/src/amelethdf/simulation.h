#include <hdf5.h>
#include <hdf5_hl.h>
#include <stdlib.h>
#include <string.h>
#include "stringdataset.h"

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
