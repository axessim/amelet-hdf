#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <category.h>
#include <simulation.h>
#include <mesh.h>
#include <planewave.h>
#include <complex.h>
#include <externalelement.h>
#include <linkoutputrequest.h>

#define FALSE 0
#define TRUE 1

int main(argc, argv)
    int argc;char *argv[];
{
    hid_t status;
    char *filename;
    hid_t file_id, loc_id;
    int i, j, k, offset;
    children_t children;
    children_t children2;
    children_t children3;
    simulation_t simulation;
    char * simus;
    char * path;
    char * path2;
    char * path3;
    planewave_t pw;
    unstructured_mesh_t umesh;
    structured_mesh_t smesh;

    // Init
    H5open();
    if (argc != 3)
    {
        printf("missing input file name and the workingdirectory\n");
        return -1;
    }
    filename = (char *) malloc(100 * sizeof(char));
    strcpy(filename, argv[1]);
    // Read for the hdf5 file
    printf("Reading %s\n", filename);
    file_id = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);
    if (file_id < 0)
    {
        printf("Can't open %s !! \n", filename);
        return -1;
    }

    // All categories
    printf("\nReading categories\n");
    children = read_children_name(file_id, "/");
    for (i = 0; i < children.nbchild; i++)
        printf("    Category : %s \n", children.childnames[i]);

    // Simulations
    printf("\nReading simulation ...\n");
    free(children.childnames);
    children = read_children_name(file_id, C_SIMULATION);
    for (i = 0; i < children.nbchild; i++)
    {
        simus = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        strcpy(simus, C_SIMULATION);
        strcat(simus, "/");
        strcat(simus, children.childnames[i]);
        simulation = read_simulation(file_id, simus);
        printSimulation(simulation);
        free(simus);
    }

    // Meshes
    printf("\nReading Mesh ...\n");
    free(children.childnames);
    if (H5Lexists(file_id, C_MESH, H5P_DEFAULT) != FALSE)
    {
        children = read_children_name(file_id, C_MESH);
        path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

        for (i = 0; i < children.nbchild; i++)
        {
            strcpy(path, C_MESH);
            strcat(path, "/");
            strcat(path, children.childnames[i]);
            printf("Mesh group : %s\n", path);
            children2 = read_children_name(file_id, path);
            for (j = 0; j < children2.nbchild; j++)
            {
                path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH
                        * sizeof(char));
                strcpy(path2, path);
                strcat(path2, "/");
                strcat(path2, children2.childnames[j]);
                printf("Reading %s\n", path2);
                if (meshtype(file_id, path2) == UNSTRUCTURED)
                {
                    printf("Unstructured Mesh\n");
                    umesh = read_unstructured_mesh(file_id, path2);
                    umesh.name = (char *) malloc(ELEMENT_NAME_LENGTH
                            * sizeof(char));
                    strcpy(umesh.name, children2.childnames[j]);
                    print_unstructured_mesh(umesh);
                }
                else if (meshtype(file_id, path2) == STRUCTURED)
                {
                    printf("Structured Mesh\n");
                    smesh = read_structured_mesh(file_id, path2);
                    smesh.name = (char *) malloc(ELEMENT_NAME_LENGTH
                            * sizeof(char));
                    strcpy(smesh.name, children2.childnames[j]);
                    print_structured_mesh(smesh);
                }
                free(path2);
            }
            free(children2.childnames);
        }
    }

    // physical Models
    printf("\nReading Physical models ...\n");
    if (H5Lexists(file_id, C_PHYSICAL_MODEL, H5P_DEFAULT) != FALSE)
    {
        free(children.childnames);
        children = read_children_name(file_id, C_PHYSICAL_MODEL);
        for (i = 0; i < children.nbchild; i++)
            printf("Physical models : %s\n", children.childnames[i]);
    }
    // Electromagnetic sources
    printf("\nReading Electromagnetic Source ...\n");
    if (H5Lexists(file_id, C_ELECTROMAGNETIC_SOURCE, H5P_DEFAULT) != FALSE)
    {
        free(children.childnames);
        children = read_children_name(file_id, C_ELECTROMAGNETIC_SOURCE);
        path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        for (i = 0; i < children.nbchild; i++)
        {
            strcpy(path, C_ELECTROMAGNETIC_SOURCE);
            strcat(path, "/");
            strcat(path, children.childnames[i]);
            printf("Electromagnetic Sources : %s\n", path);
            children2 = read_children_name(file_id, path);

            if (strcmp(C_PLANE_WAVE, children.childnames[i]) == 0)
            {
                for (j = 0; j < children2.nbchild; j++)
                {
                    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH
                            * sizeof(char));
                    strcpy(path2, path);
                    strcat(path2, "/");
                    strcat(path2, children2.childnames[j]);
                    printf("%s\n", path2);
                    pw = read_planewave(file_id, path2);
                    printf("Theta = %f\n", pw.theta);
                    printf("Phi = %f\n", pw.phi);
                    printf("Polarization = %i\n", pw.polarization);
                    printf("Polarization is linear = %i\n", islinear(pw));
                    if (islinear(pw))
                        printf(" Linear Polarisation = %f\n",
                                pw.linear_polarization);
                    printf("Polarization is elliptic = %i\n", iselliptic(pw));
                    if (iselliptic(pw))
                    {
                        printf(" Etheta = %f + j*%f\n", crealf(
                                pw.elliptical_polarization_etheta), cimagf(
                                pw.elliptical_polarization_etheta));
                        printf(" Ephi = %f + j*%f\n", crealf(
                                pw.elliptical_polarization_ephi), cimagf(
                                pw.elliptical_polarization_ephi));
                    }
                    printf("Magnitude : %f + i*%f\n", crealf(
                            pw.magnitude.singlecomplex.value), cimagf(
                            pw.magnitude.singlecomplex.value));
                    free(path2);
                }

            }
            free(children2.childnames);

        }
    }
    // external elements
    printf("\nExternal elements ...\n");
    if (H5Lexists(file_id, "/externalElement", H5P_DEFAULT) != FALSE)
    {
        free(children.childnames);
        children = read_children_name(file_id, "/externalElement");
        for (i = 0; i < children.nbchild; i++)
        {
            external_element_t extelt;
            strcpy(path, "/externalElement/");
            strcat(path, children.childnames[i]);
            extelt = read_external_elements(file_id, path);
            for (j = 0; j < extelt.nb_ext_el; j++)
            {
                printf("external element %i : \n", j);
                printf("  %s %s %s\n", extelt.external_name[j],
                        extelt.file_name[j], extelt.internal_name[j]);
            }
        }
    }

    // Label
    printf("\nReading label ...\n");
    if (H5Lexists(file_id, C_LABEL, H5P_DEFAULT) != FALSE)
    {
        free(children.childnames);
        children = read_children_name(file_id, C_LABEL);
        for (i = 0; i < children.nbchild; i++)
        {
            strcpy(path, C_LABEL);
            strcat(path, children.childnames[i]);
            children_t children4;
            children4 = read_string_vector(file_id, path);
            printf("\nLabel %i :\n", i);
            for (j = 0; j < children4.nbchild; j++)
                printf("%s \n", children4.childnames[j]);
            free(children4.childnames);
        }
    }

    // Link
    printf("\nReading links ...\n");
    if (H5Lexists(file_id, C_LINK, H5P_DEFAULT) != FALSE)
    {
        free(children.childnames);
        children = read_children_name(file_id, C_LINK);
        for (i = 0; i < children.nbchild; i++)
        {
            strcpy(path, C_LINK);
            strcat(path, children.childnames[i]);
            printf("Link group : %s\n", path);
            children2 = read_children_name(file_id, path);
            for (j = 0; j < children2.nbchild; j++)
            {
                path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH
                        * sizeof(char));
                strcpy(path2, path);
                strcat(path2, "/");
                strcat(path2, children2.childnames[j]);
                printf("Link : %s\n", path2);
                link_t lnk;
                lnk = read_link(file_id, path2);
                printf("Subject : %s\n", lnk.subject);
                printf("Object : %s\n", lnk.object);
                printf("Is dataonmesh : %i\n", isdataonmesh(lnk));
                free(path2);
            }
            free(children2.childnames);
            children2.nbchild = 0;
        }
    }

    // Output Request
    printf("\nReading output requests ...\n");
    if (H5Lexists(file_id, C_OUTPUT_REQUEST, H5P_DEFAULT) != FALSE)
    {
        free(children.childnames);
        children = read_children_name(file_id, C_OUTPUT_REQUEST);
        for (i = 0; i < children.nbchild; i++)
        {
            strcpy(path, C_OUTPUT_REQUEST);
            strcat(path, children.childnames[i]);
            printf("Output request group : %s\n", path);
            children2 = read_children_name(file_id, path);
            for (j = 0; j < children2.nbchild; j++)
            {
                path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH
                        * sizeof(char));
                strcpy(path2, path);
                strcat(path2, "/");
                strcat(path2, children2.childnames[j]);
                printf("Output request : %s\n", path2);
                link_t lnk;
                lnk = read_link(file_id, path2);
                printf("Subject : %s\n", lnk.subject);
                printf("Object : %s\n", lnk.object);
                free(path2);
            }
            free(children2.childnames);
            children2.nbchild = 0;
        }
    }

    free(path);

    free(children.childnames);

    free(filename);
    status = H5Fclose(file_id);
    return 0;
}
