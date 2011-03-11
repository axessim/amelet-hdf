#include "ah5_simulation.h"


// Read simulation instance
char AH5_read_sim_instance (hid_t file_id, const char *path, AH5_sim_instance_t *sim_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_MODULE, AH5_A_VERSION};
    char path2[AH5_ABSOLUTE_PATH_LENGTH], rdata = TRUE;
    H5T_class_t type_class;
    char success1 = FALSE, success2 = FALSE;
    size_t length;
    int nb_dims;

    sim_instance->path = strdup(path);
    sim_instance->opt_attrs.instances = NULL;
    sim_instance->module = NULL;
    sim_instance->version = NULL;
    sim_instance->inputs = NULL;
    sim_instance->outputs = NULL;

    if (AH5_path_valid(file_id, path))
    {
        AH5_read_opt_attrs(file_id, path, &(sim_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
        if (!AH5_read_str_attr(file_id, path, AH5_A_MODULE, &(sim_instance->module)))
            AH5_print_err_attr(AH5_C_SIMULATION, path, AH5_A_MODULE);
        if (!AH5_read_str_attr(file_id, path, AH5_A_VERSION, &(sim_instance->version)))
            AH5_print_err_attr(AH5_C_SIMULATION, path, AH5_A_VERSION);
        strcpy(path2, path);
        strcat(path2, AH5_G_PARAMETER);

        // inputs
        sim_instance->nb_inputs = 1;  // in case of single value
        strcpy(path2, path);
        strcat(path2, AH5_G_INPUTS);
        if (H5Lexists(file_id, path2, H5P_DEFAULT) == TRUE)
            if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
                if (nb_dims <= 1)
                    if (H5LTget_dataset_info(file_id, path2, &(sim_instance->nb_inputs), &type_class, &length) >= 0)
                        if (type_class == H5T_STRING)
                            if(AH5_read_string_dataset(file_id, path2, sim_instance->nb_inputs, length, &(sim_instance->inputs)))
                                success1 = TRUE;
        // outputs
        sim_instance->nb_outputs = 1;  // in case of single value
        strcpy(path2, path);
        strcat(path2, AH5_G_OUTPUTS);
        if (H5Lexists(file_id, path2, H5P_DEFAULT) == TRUE)
            if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
                if (nb_dims <= 1)
                    if (H5LTget_dataset_info(file_id, path2, &(sim_instance->nb_outputs), &type_class, &length) >= 0)
                        if (type_class == H5T_STRING)
                            if(AH5_read_string_dataset(file_id, path2, sim_instance->nb_outputs, length, &(sim_instance->outputs)))
                                success2 = TRUE;
    }
    if (!success1)
    {
        AH5_print_err_dset(AH5_C_SIMULATION, path2);
        sim_instance->nb_inputs = 0;
        rdata = FALSE;
    }
    if (!success2)
    {
        AH5_print_err_dset(AH5_C_SIMULATION, path2);
        sim_instance->nb_outputs = 0;
        rdata = FALSE;
    }
    return rdata;
}


// Read simulation category (all instances)
char AH5_read_simulation (hid_t file_id, AH5_simulation_t *simulation)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH], rdata = TRUE;
    AH5_children_t children;
    hsize_t i;

    simulation->instances = NULL;

    if (H5Lexists(file_id, AH5_C_SIMULATION, H5P_DEFAULT) == TRUE)
    {
        children = AH5_read_children_name(file_id, AH5_C_SIMULATION);
        simulation->nb_instances = children.nb_children;
        if (children.nb_children > 0)
        {
            simulation->instances = (AH5_sim_instance_t *) malloc(children.nb_children * sizeof(AH5_sim_instance_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path, AH5_C_SIMULATION);
                strcat(path, children.childnames[i]);
                if (!AH5_read_sim_instance(file_id, path, simulation->instances + i))
                    rdata = FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_SIMULATION, AH5_C_SIMULATION);
        rdata = FALSE;
    }
    return rdata;
}




// Print simulation instance
void AH5_print_sim_instance (const AH5_sim_instance_t *sim_instance, int space)
{
    hsize_t i;

    printf("%*sInstance: %s\n", space, "", AH5_get_name_from_path(sim_instance->path));
    AH5_print_opt_attrs(&(sim_instance->opt_attrs), space + 4);
    AH5_print_str_attr(AH5_A_MODULE, sim_instance->module, space + 4);
    AH5_print_str_attr(AH5_A_VERSION, sim_instance->version, space + 4);

    if (sim_instance->nb_inputs > 0)
    {
        printf("%*sInputs:\n", space + 2, "");
        for (i = 0; i < sim_instance->nb_inputs; i++)
            printf("%*s%s\n", space + 5, " ", sim_instance->inputs[i]);
    }

    if (sim_instance->nb_outputs > 0)
    {
        printf("%*sOutputs:\n", space + 2, "");
        for (i = 0; i < sim_instance->nb_outputs; i++)
            printf("%*s%s\n", space + 5, "", sim_instance->outputs[i]);
    }
    printf("\n");
}


// Print simulation category (all instances)
void AH5_print_simulation (const AH5_simulation_t *simulation)
{
    hsize_t i;

    printf("################################  Simulation  ################################\n\n");
    for (i = 0; i < simulation->nb_instances; i++)
    {
        AH5_print_sim_instance(&(simulation->instances[i]), 0);
    }
    printf("\n");
}




// Free memory used by structure sim_instance
void AH5_free_sim_instance (AH5_sim_instance_t *sim_instance)
{
    if (sim_instance->path != NULL)
    {
        free(sim_instance->path);
        sim_instance->path = NULL;
    }
    AH5_free_opt_attrs(&(sim_instance->opt_attrs));
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
    if (sim_instance->inputs != NULL)
    {
        free(sim_instance->inputs[0]);
        free(sim_instance->inputs);
        sim_instance->inputs = NULL;
        sim_instance->nb_inputs = 0;
    }
    if (sim_instance->outputs != NULL)
    {
        free(sim_instance->outputs[0]);
        free(sim_instance->outputs);
        sim_instance->outputs = NULL;
        sim_instance->nb_outputs = 0;
    }
}


// Free memory used by structure simulation (including simulation instances)
void AH5_free_simulation (AH5_simulation_t *simulation)
{
    hsize_t i;

    if (simulation->instances != NULL)
    {
        for (i = 0; i < simulation->nb_instances; i++)
            AH5_free_sim_instance(simulation->instances + i);
        free(simulation->instances);
        simulation->instances = NULL;
        simulation->nb_instances = 0;
    }
}
