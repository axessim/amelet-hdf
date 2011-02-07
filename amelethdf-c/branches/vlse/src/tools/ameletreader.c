#include <stdlib.h>

#include "category.h"
#include "simulation.h"
#include "mesh.h"
#include "physicalmodel.h"
#include "externalelement.h"
#include "label.h"
#include "link.h"
//#include "emsource.h"
#include "outputrequest.h"

#define FALSE 0
#define TRUE 1

int main(argc, argv)
int argc;
char *argv[];
{
    hid_t status;
    char *filename;
    hid_t file_id;
    hsize_t i;
    children_t children;
    char *path;
    char *path2;
    simulation_t simulation;
    mesh_t mesh;
    external_element_t external_element;
    label_t label;
    link_t link;
    outputrequest_t outputrequest;
    physicalmodel_t physicalmodel;
//    em_source_t em_source;

    // Init
    H5open();
    if (argc < 2)
    {
        printf("***** ERROR: Missing input filename. *****\n\n");
        return -1;
    }
    filename = (char *) malloc(100 * sizeof(char));
    strcpy(filename, argv[1]);


    // Read for the hdf5 file
    printf("Reading %s\n", filename);
    file_id = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);
    if (file_id < 0)
    {
        printf("***** ERROR: Cannot open file %s. *****\n\n", filename);
        return -1;
    }

    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    // All categories
    printf("\nReading categories ...\n");
    children = read_children_name(file_id, "/");
    if (children.nb_children > 0)
    {
        for (i = 0; i < children.nb_children; i++)
        {
            printf("    Category: %s \n", children.childnames[i]);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
    printf ("\n\n");


    // Read categories
    // Simulations
    printf("Reading simulations ...\n");
    if(H5Lexists(file_id, C_SIMULATION, H5P_DEFAULT) != FALSE)
        read_simulation(file_id, &simulation);

    // Meshes
    printf("Reading mesh ...\n");
    if (H5Lexists(file_id, C_MESH, H5P_DEFAULT) != FALSE)
        read_mesh(file_id, &mesh);

    // physical Models
    printf("Reading physical models ...\n");
    if (H5Lexists(file_id, C_PHYSICAL_MODEL, H5P_DEFAULT) != FALSE)
        read_physicalmodel(file_id, &physicalmodel);

    // Electromagnetic sources
//    printf("\nReading Electromagnetic source ...\n");
//    if (H5Lexists(file_id, C_ELECTROMAGNETIC_SOURCE, H5P_DEFAULT) != FALSE)
//        read_electromagnetic_source(file_id, &em_source);

    // External element
    printf("Reading external elements ...\n");
    if (H5Lexists(file_id, C_EXTERNAL_ELEMENT, H5P_DEFAULT) != FALSE)
        read_external_element(file_id, &external_element);

    // Label
    printf("Reading labels ...\n");
    if (H5Lexists(file_id, C_LABEL, H5P_DEFAULT) != FALSE)
        read_label(file_id, &label);

    // Link
    printf("Reading links ...\n");
    if (H5Lexists(file_id, C_LINK, H5P_DEFAULT) != FALSE)
        read_link(file_id, &link);

    // Output Request
    printf("Reading ouput requests ...\n");
    if (H5Lexists(file_id, C_OUTPUT_REQUEST, H5P_DEFAULT) != FALSE)
        read_outputrequest(file_id, &outputrequest);
    printf("\n");


    // Print & free


    if(H5Lexists(file_id, C_SIMULATION, H5P_DEFAULT) != FALSE)
    {
        print_simulation(simulation);
        free_simulation(&simulation);
    }
    if(H5Lexists(file_id, C_MESH, H5P_DEFAULT) != FALSE)
    {
        print_mesh(mesh);
        free_mesh(&mesh);
    }

    if (H5Lexists(file_id, C_PHYSICAL_MODEL, H5P_DEFAULT) != FALSE)
    {
        print_physicalmodel(physicalmodel);
        free_physicalmodel(&physicalmodel);
    }

//    if (H5Lexists(file_id, C_ELECTROMAGNETIC_SOURCE, H5P_DEFAULT) != FALSE)
    //{
//        print_electromagnetic_source(em_source);
//        free_electromagnetic_source(&em_source);
    //}

    if (H5Lexists(file_id, C_EXTERNAL_ELEMENT, H5P_DEFAULT) != FALSE)
    {
        print_external_element(external_element);
        free_external_element(&external_element);
    }

    if (H5Lexists(file_id, C_LABEL, H5P_DEFAULT) != FALSE)
    {
        print_label(label);
        free_label(&label);
    }

    if (H5Lexists(file_id, C_LINK, H5P_DEFAULT) != FALSE)
    {
        print_link(link);
        free_link(&link);
    }
    if (H5Lexists(file_id, C_OUTPUT_REQUEST, H5P_DEFAULT) != FALSE)
    {
        print_outputrequest(outputrequest);
        free_outputrequest(&outputrequest);
    }

    free(path2);
    free(path);
    free(filename);
    status = H5Fclose(file_id);
    return 0;
}
