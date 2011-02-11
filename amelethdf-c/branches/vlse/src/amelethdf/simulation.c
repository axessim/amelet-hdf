#include "simulation.h"
// Revision: 7.2.2011

// Read simulation instance
void read_simulation_instance(hid_t file_id, const char *path, simulation_instance_t *simulation_instance)
{
    H5T_class_t type_class;
    size_t length;
    char success = FALSE;
    char *path2;
    int nb_dims;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_MODULE, A_VERSION};

    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    simulation_instance->name = get_name_from_path(path);
    read_optional_attributes(file_id, path2, &(simulation_instance->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    if (!read_str_attribute(file_id, path, A_MODULE, &(simulation_instance->module)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_SIMULATION, path, A_MODULE);
    if (!read_str_attribute(file_id, path, A_VERSION, &(simulation_instance->version)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_SIMULATION, path, A_VERSION);
    strcpy(path2, path);
    strcat(path2, G_PARAMETER);

    // inputs
    simulation_instance->nb_inputs = 1;  // in case of single value
    strcpy(path2, path);
    strcat(path2, G_INPUTS);
    if (H5Lexists(file_id, path2, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path2, &(simulation_instance->nb_inputs), &type_class, &length) >= 0)
                    if (type_class == H5T_STRING)
                        if(read_string_dataset(file_id, path2, simulation_instance->nb_inputs, length, &(simulation_instance->inputs)))
                            success = TRUE;
    if (!success)
    {
        printf("***** ERROR(%s): Cannot read dataset \"%s\". *****\n\n", C_SIMULATION, path2);
        simulation_instance->nb_inputs = 0;
        simulation_instance->inputs = NULL;
    }

    // outputs
    simulation_instance->nb_outputs = 1;  // in case of single value
    success = FALSE;
    strcpy(path2, path);
    strcat(path2, G_OUTPUTS);
    if (H5Lexists(file_id, path2, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path2, &(simulation_instance->nb_outputs), &type_class, &length) >= 0)
                    if (type_class == H5T_STRING)
                        if(read_string_dataset(file_id, path2, simulation_instance->nb_outputs, length, &(simulation_instance->outputs)))
                            success = TRUE;
    if (!success)
    {
        printf("***** ERROR(%s): Cannot read dataset \"%s\". *****\n\n", C_SIMULATION, path2);
        simulation_instance->nb_outputs = 0;
        simulation_instance->outputs = NULL;
    }
    free(path2);
}


// Read simulation category (all instances)
void read_simulation(hid_t file_id, simulation_t *simulation)
{
    children_t children;
    hsize_t i;
    char *path;

    children = read_children_name(file_id, C_SIMULATION);
    simulation->nb_simulation_instances = children.nb_children;
    simulation->simulation_instances = NULL;
    if (children.nb_children > 0)
    {
        path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        simulation->simulation_instances = (simulation_instance_t *) malloc((size_t) children.nb_children * sizeof(simulation_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_SIMULATION);
            strcat(path, children.childnames[i]);
            read_simulation_instance(file_id, path, simulation->simulation_instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
        free(path);
    }
}




// Print simulation instance
void print_simulation_instance(simulation_instance_t simulation_instance, int space)
{
    hsize_t i;

    printf("%*sInstance: %s\n", space, "", simulation_instance.name);
    print_optional_attributes(simulation_instance.optional_attributes, space + 4);
    print_str_attribute(A_MODULE, simulation_instance.module, space + 4);
    print_str_attribute(A_VERSION, simulation_instance.version, space + 4);

    if (simulation_instance.nb_inputs > 0)
    {
        printf("%*sInputs:\n", space + 2, "");
        for (i = 0; i < simulation_instance.nb_inputs; i++)
            printf("%*s%s\n", space + 5, " ", simulation_instance.inputs[i]);
    }

    if (simulation_instance.nb_outputs > 0)
    {
        printf("%*sOutputs:\n", space + 2, "");
        for (i = 0; i < simulation_instance.nb_outputs; i++)
            printf("%*s%s\n", space + 5, "", simulation_instance.outputs[i]);
    }
    printf("\n");
}


// Print simulation category (all instances)
void print_simulation(simulation_t simulation)
{
    hsize_t i;

    printf("################################  Simulation  ################################\n\n");
    for (i = 0; i < simulation.nb_simulation_instances; i++)
    {
        print_simulation_instance(simulation.simulation_instances[i], 0);
    }
    printf("\n");
}




// Free memory used by structure simulation_instance
void free_simulation_instance(simulation_instance_t *simulation_instance)
{
    if (simulation_instance->name != NULL)
    {
        free(simulation_instance->name);
        simulation_instance->name = NULL;
    }
    free_optional_attributes(&(simulation_instance->optional_attributes));
    if (simulation_instance->module != NULL)
    {
        free(simulation_instance->module);
        simulation_instance->module = NULL;
    }
    if (simulation_instance->version != NULL)
    {
        free(simulation_instance->version);
        simulation_instance->version = NULL;
    }
    if (simulation_instance->nb_inputs > 0)
    {
        free(simulation_instance->inputs[0]);
        free(simulation_instance->inputs);
        simulation_instance->inputs = NULL;
        simulation_instance->nb_inputs = 0;
    }
    if (simulation_instance->nb_outputs > 0)
    {
        free(simulation_instance->outputs[0]);
        free(simulation_instance->outputs);
        simulation_instance->outputs = NULL;
        simulation_instance->nb_outputs = 0;
    }
}


// Free memory used by structure simulation (including simulation instances)
void free_simulation(simulation_t *simulation)
{
    hsize_t i;

    for (i = 0; i < simulation->nb_simulation_instances; i++)
    {
        free_simulation_instance(simulation->simulation_instances + i);
    }
    free(simulation->simulation_instances);
    simulation->simulation_instances = NULL;
}
