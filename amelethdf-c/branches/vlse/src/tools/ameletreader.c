#include <stdlib.h>

#include "ah5.h"

int main(argc, argv)
int argc;
char *argv[];
{
    hid_t file_id;
//    hsize_t i;
//    AH5_children_t children;
    AH5_simulation_t simulation;
    AH5_mesh_t mesh;
    AH5_external_element_t external_element;
    AH5_label_t label;
    AH5_link_t link;
    AH5_outputrequest_t outputrequest;
    AH5_physicalmodel_t physicalmodel;
    AH5_em_source_t em_source;
    AH5_global_environment_t global_environment;
    AH5_localization_system_t localization_system;
    AH5_exchange_surface_t exchange_surface;


    if (argc < 2)
    {
        printf("***** ERROR: Missing input filename. *****\n\n");
        return -1;
    }

    // Init
    H5open();
    file_id = H5Fopen(argv[1], H5F_ACC_RDWR, H5P_DEFAULT);  // Read for the HDF5 file
    if (file_id < 0)
    {
        printf("***** ERROR: Cannot open file %s. *****\n\n", argv[1]);
        H5close();
        return -1;
    }

    printf("\n#############################  Amelet-HDF Reader  ############################\n");
    printf("##############################################################################\n\n");
/*
    printf("Main categories:\n");
    children = AH5_read_children_name(file_id, "/");
    if (children.nb_children > 0)
    {
        for (i = 0; i < children.nb_children; i++)
        {
            printf("   %s\n", children.childnames[i]);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }

    printf ("\n\n");
*/


    /* ################################  Read categories  ################################ */
    /* ################################################################################### */

    // Simulation
    printf("Reading simulations ...\n");
    if(H5Lexists(file_id, AH5_C_SIMULATION, H5P_DEFAULT) != FALSE)
        AH5_read_simulation(file_id, &simulation);

    // Global environment
    printf("Reading global environment ...\n");
    if(H5Lexists(file_id, AH5_C_GLOBAL_ENVIRONMENT, H5P_DEFAULT) != FALSE)
        AH5_read_global_environment(file_id, &global_environment);

    // Localization system
    printf("Reading localization system ...\n");
    if(H5Lexists(file_id, AH5_C_LOCALIZATION_SYSTEM, H5P_DEFAULT) != FALSE)
        AH5_read_localization_system(file_id, &localization_system);

    // Mesh
    printf("Reading meshes ...\n");
    if (H5Lexists(file_id, AH5_C_MESH, H5P_DEFAULT) != FALSE)
        AH5_read_mesh(file_id, &mesh);

    // Physical model
    printf("Reading physical models ...\n");
    if (H5Lexists(file_id, AH5_C_PHYSICAL_MODEL, H5P_DEFAULT) != FALSE)
        AH5_read_physicalmodel(file_id, &physicalmodel);

    // Electromagnetic source
    printf("Reading electromagnetic sources ...\n");
    if (H5Lexists(file_id, AH5_C_ELECTROMAGNETIC_SOURCE, H5P_DEFAULT) != FALSE)
        AH5_read_electromagnetic_source(file_id, &em_source);

    // External element
    printf("Reading external elements ...\n");
    if (H5Lexists(file_id, AH5_C_EXTERNAL_ELEMENT, H5P_DEFAULT) != FALSE)
        AH5_read_external_element(file_id, &external_element);

    // Label
    printf("Reading labels ...\n");
    if (H5Lexists(file_id, AH5_C_LABEL, H5P_DEFAULT) != FALSE)
        AH5_read_label(file_id, &label);

    // Link
    printf("Reading links ...\n");
    if (H5Lexists(file_id, AH5_C_LINK, H5P_DEFAULT) != FALSE)
        AH5_read_link(file_id, &link);

    // Output request
    printf("Reading ouput requests ...\n");
    if (H5Lexists(file_id, AH5_C_OUTPUT_REQUEST, H5P_DEFAULT) != FALSE)
        AH5_read_outputrequest(file_id, &outputrequest);

    // Exchange surface
    printf("Reading exchange surface ...\n");
    if (H5Lexists(file_id, AH5_C_EXCHANGE_SURFACE, H5P_DEFAULT) != FALSE)
        AH5_read_exchange_surface(file_id, &exchange_surface);

    printf("\n\n");



    /* ############################  Print & free categories  ############################ */
    /* ################################################################################### */

    // Simulation
    if(H5Lexists(file_id, AH5_C_SIMULATION, H5P_DEFAULT) != FALSE)
    {
        AH5_print_simulation(&simulation);
        AH5_free_simulation(&simulation);
    }

    // Global environment
    if(H5Lexists(file_id, AH5_C_GLOBAL_ENVIRONMENT, H5P_DEFAULT) != FALSE)
    {
        AH5_print_global_environment(&global_environment);
        AH5_free_global_environment(&global_environment);
    }

    // Localization system
    if(H5Lexists(file_id, AH5_C_LOCALIZATION_SYSTEM, H5P_DEFAULT) != FALSE)
    {
        AH5_print_localization_system(&localization_system);
        AH5_free_localization_system(&localization_system);
    }

    // Mesh
    if(H5Lexists(file_id, AH5_C_MESH, H5P_DEFAULT) != FALSE)
    {
        AH5_print_mesh(&mesh);
        AH5_free_mesh(&mesh);
    }

    // Physical model
    if (H5Lexists(file_id, AH5_C_PHYSICAL_MODEL, H5P_DEFAULT) != FALSE)
    {
        AH5_print_physicalmodel(&physicalmodel);
        AH5_free_physicalmodel(&physicalmodel);
    }

    // Electromagnetic source
    if (H5Lexists(file_id, AH5_C_ELECTROMAGNETIC_SOURCE, H5P_DEFAULT) != FALSE)
    {
        AH5_print_electromagnetic_source(&em_source);
        AH5_free_electromagnetic_source(&em_source);
    }

    // External element
    if (H5Lexists(file_id, AH5_C_EXTERNAL_ELEMENT, H5P_DEFAULT) != FALSE)
    {
        AH5_print_external_element(&external_element);
        AH5_free_external_element(&external_element);
    }

    // Label
    if (H5Lexists(file_id, AH5_C_LABEL, H5P_DEFAULT) != FALSE)
    {
        AH5_print_label(&label);
        AH5_free_label(&label);
    }

    // Link
    if (H5Lexists(file_id, AH5_C_LINK, H5P_DEFAULT) != FALSE)
    {
        AH5_print_link(&link);
        AH5_free_link(&link);
    }

    // Output request
    if (H5Lexists(file_id, AH5_C_OUTPUT_REQUEST, H5P_DEFAULT) != FALSE)
    {
        AH5_print_outputrequest(&outputrequest);
        AH5_free_outputrequest(&outputrequest);
    }

    // Exchange surface
    if (H5Lexists(file_id, AH5_C_EXCHANGE_SURFACE, H5P_DEFAULT) != FALSE)
    {
        AH5_print_exchange_surface(&exchange_surface);
        AH5_free_exchange_surface(&exchange_surface);
    }


    H5Fclose(file_id);
    H5close();
    return 0;
}
