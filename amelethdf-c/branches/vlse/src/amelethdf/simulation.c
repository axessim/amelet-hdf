#include "simulation.h"


// Read simulation instance
void read_sim_instance(hid_t file_id, const char *path, sim_instance_t *sim_instance)
{
    char mandatory[][ATTR_LENGTH] = {A_MODULE, A_VERSION};
    char path2[ABSOLUTE_PATH_NAME_LENGTH];
    H5T_class_t type_class;
    char success = FALSE;
    size_t length;
    int nb_dims;

    sim_instance->path = strdup(path);
    read_opt_attrs(file_id, path2, &(sim_instance->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
    if (!read_str_attr(file_id, path, A_MODULE, &(sim_instance->module)))
        print_err_attr(C_SIMULATION, path, A_MODULE);
    if (!read_str_attr(file_id, path, A_VERSION, &(sim_instance->version)))
        print_err_attr(C_SIMULATION, path, A_VERSION);
    strcpy(path2, path);
    strcat(path2, G_PARAMETER);

    // inputs
    sim_instance->nb_inputs = 1;  // in case of single value
    strcpy(path2, path);
    strcat(path2, G_INPUTS);
    if (H5Lexists(file_id, path2, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path2, &(sim_instance->nb_inputs), &type_class, &length) >= 0)
                    if (type_class == H5T_STRING)
                        if(read_string_dataset(file_id, path2, sim_instance->nb_inputs, length, &(sim_instance->inputs)))
                            success = TRUE;
    if (!success)
    {
        print_err_dset(C_SIMULATION, path2);
        sim_instance->nb_inputs = 0;
        sim_instance->inputs = NULL;
    }

    // outputs
    sim_instance->nb_outputs = 1;  // in case of single value
    success = FALSE;
    strcpy(path2, path);
    strcat(path2, G_OUTPUTS);
    if (H5Lexists(file_id, path2, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path2, &(sim_instance->nb_outputs), &type_class, &length) >= 0)
                    if (type_class == H5T_STRING)
                        if(read_string_dataset(file_id, path2, sim_instance->nb_outputs, length, &(sim_instance->outputs)))
                            success = TRUE;
    if (!success)
    {
        print_err_dset(C_SIMULATION, path2);
        sim_instance->nb_outputs = 0;
        sim_instance->outputs = NULL;
    }
}


// Read simulation category (all instances)
void read_simulation(hid_t file_id, simulation_t *simulation)
{
    char path[ABSOLUTE_PATH_NAME_LENGTH];
    children_t children;
    hsize_t i;

    children = read_children_name(file_id, C_SIMULATION);
    simulation->nb_instances = children.nb_children;
    simulation->instances = NULL;
    if (children.nb_children > 0)
    {
        simulation->instances = (sim_instance_t *) malloc(children.nb_children * sizeof(sim_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_SIMULATION);
            strcat(path, children.childnames[i]);
            read_sim_instance(file_id, path, simulation->instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}




// Print simulation instance
void print_sim_instance(sim_instance_t sim_instance, int space)
{
    hsize_t i;

    printf("%*sInstance: %s\n", space, "", get_name_from_path(sim_instance.path));
    print_opt_attrs(sim_instance.opt_attrs, space + 4);
    print_str_attr(A_MODULE, sim_instance.module, space + 4);
    print_str_attr(A_VERSION, sim_instance.version, space + 4);

    if (sim_instance.nb_inputs > 0)
    {
        printf("%*sInputs:\n", space + 2, "");
        for (i = 0; i < sim_instance.nb_inputs; i++)
            printf("%*s%s\n", space + 5, " ", sim_instance.inputs[i]);
    }

    if (sim_instance.nb_outputs > 0)
    {
        printf("%*sOutputs:\n", space + 2, "");
        for (i = 0; i < sim_instance.nb_outputs; i++)
            printf("%*s%s\n", space + 5, "", sim_instance.outputs[i]);
    }
    printf("\n");
}


// Print simulation category (all instances)
void print_simulation(simulation_t simulation)
{
    hsize_t i;

    printf("################################  Simulation  ################################\n\n");
    for (i = 0; i < simulation.nb_instances; i++)
    {
        print_sim_instance(simulation.instances[i], 0);
    }
    printf("\n");
}




// Free memory used by structure sim_instance
void free_sim_instance(sim_instance_t *sim_instance)
{
    if (sim_instance->path != NULL)
    {
        free(sim_instance->path);
        sim_instance->path = NULL;
    }
    free_opt_attrs(&(sim_instance->opt_attrs));
    if (sim_instance->module != NULL)
    {
        free(sim_instance->module);
        sim_instance->module = NULL;
    }
    if (sim_instance->version != NULL)
    {
        free(sim_instance->version);
        sim_instance->version = NULL;
    }
    if (sim_instance->nb_inputs > 0)
    {
        free(sim_instance->inputs[0]);
        free(sim_instance->inputs);
        sim_instance->inputs = NULL;
        sim_instance->nb_inputs = 0;
    }
    if (sim_instance->nb_outputs > 0)
    {
        free(sim_instance->outputs[0]);
        free(sim_instance->outputs);
        sim_instance->outputs = NULL;
        sim_instance->nb_outputs = 0;
    }
}


// Free memory used by structure simulation (including simulation instances)
void free_simulation(simulation_t *simulation)
{
    hsize_t i;

    for (i = 0; i < simulation->nb_instances; i++)
    {
        free_sim_instance(simulation->instances + i);
    }
    free(simulation->instances);
    simulation->instances = NULL;
}
