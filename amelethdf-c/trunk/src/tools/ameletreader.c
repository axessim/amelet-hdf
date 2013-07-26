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

    // Electromagnetic source
    if (AH5_path_valid(file_id, AH5_C_ELECTROMAGNETIC_SOURCE))
    {
        printf("Reading electromagnetic sources... \n");
        if (AH5_read_electromagnetic_source(file_id, &em_source))
            printf("%*ssuccess\n", 50, "");
        else
            printf("%*sfailed!\n", 50, "");
    }

    // Exchange surface
    if (AH5_path_valid(file_id, AH5_C_EXCHANGE_SURFACE))
    {
        printf("Reading exchange surface... \n");
        if (AH5_read_exchange_surface(file_id, &exchange_surface))
            printf("%*ssuccess\n", 50, "");
        else
            printf("%*sfailed!\n", 50, "");
    }

    // External element
    if (AH5_path_valid(file_id, AH5_C_EXTERNAL_ELEMENT))
    {
        printf("Reading external elements... \n");
        if (AH5_read_external_element(file_id, &external_element))
            printf("%*ssuccess\n", 50, "");
        else
            printf("%*sfailed!\n", 50, "");
    }

    // Global environment
    if (AH5_path_valid(file_id, AH5_C_GLOBAL_ENVIRONMENT))
    {
        printf("Reading global environment... \n");
        if (AH5_read_global_environment(file_id, &global_environment))
            printf("%*ssuccess\n", 50, "");
        else
            printf("%*sfailed!\n", 50, "");
    }

    // Label
    if (AH5_path_valid(file_id, AH5_C_LABEL))
    {
        printf("Reading labels... \n");
        if (AH5_read_label(file_id, &label))
            printf("%*ssuccess\n", 50, "");
        else
            printf("%*sfailed!\n", 50, "");
    }

    // Link
    if (AH5_path_valid(file_id, AH5_C_LINK))
    {
        printf("Reading links... \n");
        if (AH5_read_link(file_id, &link))
            printf("%*ssuccess\n", 50, "");
        else
            printf("%*sfailed!\n", 50, "");
    }

    // Localization system
    if (AH5_path_valid(file_id, AH5_C_LOCALIZATION_SYSTEM))
    {
        printf("Reading localization system... \n");
        if (AH5_read_localization_system(file_id, &localization_system))
            printf("%*ssuccess\n", 50, "");
        else
            printf("%*sfailed!\n", 50, "");
    }

    // Mesh
    if (AH5_path_valid(file_id, AH5_C_MESH))
    {
        printf("Reading meshes... \n");
        if (AH5_read_mesh(file_id, &mesh))
            printf("%*ssuccess\n", 50, "");
        else
            printf("%*sfailed!\n", 50, "");
    }

    // Output request
    if (AH5_path_valid(file_id, AH5_C_OUTPUT_REQUEST))
    {
        printf("Reading ouput requests... \n");
        if (AH5_read_outputrequest(file_id, &outputrequest))
            printf("%*ssuccess\n", 50, "");
        else
            printf("%*sfailed!\n", 50, "");
    }

    // Physical model
    if (AH5_path_valid(file_id, AH5_C_PHYSICAL_MODEL))
    {
        printf("Reading physical models... \n");
        if (AH5_read_physicalmodel(file_id, &physicalmodel))
            printf("%*ssuccess\n", 50, "");
        else
            printf("%*sfailed!\n", 50, "");
    }

   // Simulation
    if (AH5_path_valid(file_id, AH5_C_SIMULATION))
    {
        printf("Reading simulations... \n");
        if (AH5_read_simulation(file_id, &simulation))
            printf("%*ssuccess\n", 50, "");
        else
            printf("%*sfailed!\n", 50, "");
    }

    printf("\n\n");



    /* ############################  Print & free categories  ############################ */
    /* ################################################################################### */

    // Electromagnetic source
    if (AH5_path_valid(file_id, AH5_C_ELECTROMAGNETIC_SOURCE))
    {
        AH5_print_electromagnetic_source(&em_source);
        AH5_free_electromagnetic_source(&em_source);
    }

    // Exchange surface
    if (AH5_path_valid(file_id, AH5_C_EXCHANGE_SURFACE))
    {
        AH5_print_exchange_surface(&exchange_surface);
        AH5_free_exchange_surface(&exchange_surface);
    }

    // External element
    if (AH5_path_valid(file_id, AH5_C_EXTERNAL_ELEMENT))
    {
        AH5_print_external_element(&external_element);
        AH5_free_external_element(&external_element);
    }

    // Global environment
    if (AH5_path_valid(file_id, AH5_C_GLOBAL_ENVIRONMENT))
    {
        AH5_print_global_environment(&global_environment);
        AH5_free_global_environment(&global_environment);
    }

    // Label
    if (AH5_path_valid(file_id, AH5_C_LABEL))
    {
        AH5_print_label(&label);
        AH5_free_label(&label);
    }

    // Link
    if (AH5_path_valid(file_id, AH5_C_LINK))
    {
        AH5_print_link(&link);
        AH5_free_link(&link);
    }

    // Localization system
    if (AH5_path_valid(file_id, AH5_C_LOCALIZATION_SYSTEM))
    {
        AH5_print_localization_system(&localization_system);
        AH5_free_localization_system(&localization_system);
    }

    // Mesh
    if (AH5_path_valid(file_id, AH5_C_MESH))
    {
        AH5_print_mesh(&mesh);
        AH5_free_mesh(&mesh);
    }

    // Output request
    if (AH5_path_valid(file_id, AH5_C_OUTPUT_REQUEST))
    {
        AH5_print_outputrequest(&outputrequest);
        AH5_free_outputrequest(&outputrequest);
    }

    // Physical model
    if (AH5_path_valid(file_id, AH5_C_PHYSICAL_MODEL))
    {
        AH5_print_physicalmodel(&physicalmodel);
        AH5_free_physicalmodel(&physicalmodel);
    }

    // Simulation
    if (AH5_path_valid(file_id, AH5_C_SIMULATION))
    {
        AH5_print_simulation(&simulation);
        AH5_free_simulation(&simulation);
    }

    H5Fclose(file_id);
    H5close();
    return 0;
}
