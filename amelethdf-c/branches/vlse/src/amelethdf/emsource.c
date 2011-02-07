#include "emsource.h"


void read_es_planewave(hid_t file_id, const char *path, planewave_t *planewave)
{

}

void read_ess_planewave(hid_t file_id, em_source_t *em_source)
{
    hsize_t i;
    children_t children;
    char *path, *path2;

    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    strcpy(path, C_ELECTROMAGNETIC_SOURCE);
    strcat(path, "/");
    strcat(path, ES_PLANE_WAVE);
    children = read_children_name(file_id, path);
    em_source->nb_pw_sources = children.nb_children;
    em_source->pw_sources = (planewave_t *) malloc((size_t) children.nb_children * sizeof(planewave_t));
    for (i = 0; i < children.nb_children; i++)
    {
        strcpy(path2, path);
        strcat(path2, children.childnames[i]);
        read_es_planewave(file_id, path2, em_source->pw_sources +i);
        free(children.childnames[i]);
    }
    free(children.childnames);
    free(path2);
    free(path);
}



void read_es_sphericalwave(hid_t file_id, const char *path, sphericalwave_t *sphericalwave)
{

}

void read_ess_sphericalwave(hid_t file_id, em_source_t *em_source)
{
    hsize_t i;
    children_t children;
    char *path, *path2;

    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    strcpy(path, C_ELECTROMAGNETIC_SOURCE);
    strcat(path, "/");
    strcat(path, ES_SPHERICAL_WAVE);
    children = read_children_name(file_id, path);
    em_source->nb_sw_sources = children.nb_children;
    em_source->sw_sources = (sphericalwave_t *) malloc((size_t) children.nb_children * sizeof(sphericalwave_t));
    for (i = 0; i < children.nb_children; i++)
    {
        strcpy(path2, path);
        strcat(path2, children.childnames[i]);
        read_es_sphericalwave(file_id, path, em_source->sw_sources +i);
        free(children.childnames[i]);
    }
    free(children.childnames);
    free(path2);
    free(path);
}



void read_es_generator(hid_t file_id, const char *path, generator_t *generator)
{

}

void read_ess_generator(hid_t file_id, em_source_t *em_source)
{
    hsize_t i;
    children_t children;
    char *path, *path2;

    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    strcpy(path, C_ELECTROMAGNETIC_SOURCE);
    strcat(path, "/");
    strcat(path, ES_GENERATOR);
    children = read_children_name(file_id, path);
    em_source->nb_ge_sources = children.nb_children;
    em_source->ge_sources = (generator_t *) malloc((size_t) children.nb_children * sizeof(generator_t));
    for (i = 0; i < children.nb_children; i++)
    {
        strcpy(path2, path);
        strcat(path2, children.childnames[i]);
        read_es_generator(file_id, path, em_source->ge_sources +i);
        free(children.childnames[i]);
    }
    free(children.childnames);
    free(path2);
    free(path);
}



void read_es_dipole(hid_t file_id, const char *path, dipole_t *dipole)
{

}
// !!!!!!!!!!!!!!! blbì
void read_ess_dipole(hid_t file_id, em_source_t *em_source)
{
    hsize_t i;
    children_t children;
    char *path, *path2;

    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    strcpy(path, C_ELECTROMAGNETIC_SOURCE);
    strcat(path, "/");
    strcat(path, ES_DIPOLE);
    children = read_children_name(file_id, path);
    em_source->nb_di_sources = children.nb_children;
    em_source->di_sources = (dipole_t *) malloc((size_t) children.nb_children * sizeof(dipole_t));
    for (i = 0; i < children.nb_children; i++)
    {
        strcpy(path2, path);
        strcat(path2, children.childnames[i]);
        read_es_dipole(file_id, path, em_source->di_sources +i);
        free(children.childnames[i]);
    }
    free(children.childnames);
    free(path2);
    free(path);
}



void read_es_antenna(hid_t file_id, const char *path, antenna_t *antenna)
{

}

void read_ess_antenna(hid_t file_id, em_source_t *em_source)
{
    hsize_t i;
    children_t children;
    char *path, *path2;

    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    strcpy(path, C_ELECTROMAGNETIC_SOURCE);
    strcat(path, "/");
    strcat(path, ES_ANTENNA);
    children = read_children_name(file_id, path);
    em_source->nb_an_sources = children.nb_children;
    em_source->an_sources = (antenna_t *) malloc((size_t) children.nb_children * sizeof(antenna_t));
    for (i = 0; i < children.nb_children; i++)
    {
        strcpy(path2, path);
        strcat(path2, children.childnames[i]);
        read_es_antenna(file_id, path, em_source->an_sources +i);
        free(children.childnames[i]);
    }
    free(children.childnames);
    free(path2);
    free(path);
}



void read_es_sourceonmesh(hid_t file_id, const char *path, sourceonmesh_t *sourceonmesh)
{

}

void read_ess_sourceonmesh(hid_t file_id, em_source_t *em_source)
{
    hsize_t i;
    children_t children;
    char *path, *path2;

    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    strcpy(path, C_ELECTROMAGNETIC_SOURCE);
    strcat(path, "/");
    strcat(path, ES_PLANE_WAVE);
    children = read_children_name(file_id, path);
    em_source->nb_sm_sources = children.nb_children;
    em_source->sm_sources = (sourceonmesh_t *) malloc((size_t) children.nb_children * sizeof(sourceonmesh_t));
    for (i = 0; i < children.nb_children; i++)
    {
        strcpy(path2, path);
        strcat(path2, children.childnames[i]);
        read_es_sourceonmesh(file_id, path, em_source->sm_sources +i);
        free(children.childnames[i]);
    }
    free(children.childnames);
    free(path2);
    free(path);
}



void read_electromagnetic_source(hid_t file_id, em_source_t *em_source)
{
    hsize_t i;
    children_t children;
    char *path;

    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    children = read_children_name(file_id, C_ELECTROMAGNETIC_SOURCE);
    for (i = 0; i < children.nb_children; i++)
    {
        strcpy(path, C_ELECTROMAGNETIC_SOURCE);
        strcat(path, children.childnames[i]);
        if (strcmp(children.childnames[i], ES_PLANE_WAVE) == 0)
            read_ess_planewave(file_id, em_source);
        else if (strcmp(children.childnames[i], ES_SPHERICAL_WAVE) == 0)
            read_ess_sphericalwave(file_id, em_source);
        else if (strcmp(children.childnames[i], ES_GENERATOR) == 0)
            read_ess_generator(file_id, em_source);
        else if (strcmp(children.childnames[i], ES_DIPOLE) == 0)
            read_ess_dipole(file_id, em_source);
        else if (strcmp(children.childnames[i], ES_ANTENNA) == 0)
            read_ess_antenna(file_id, em_source);
        else if (strcmp(children.childnames[i], ES_SOURCE_ON_MESH) == 0)
            read_ess_sourceonmesh(file_id, em_source);
        free(children.childnames[i]);
    }
    free(children.childnames);
    free(path);
}


void print_electromagnetic_source(em_source_t em_source)
{

}


void free_electromagnetic_source(em_source_t em_source)
{

}










/*



            children2 = read_children_name(file_id, path);

            if (strcmp(C_PLANE_WAVE, children.childnames[i]) == 0)
            {
                for (j = 0; j < children2.nbchild; j++)
                {
                    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH
                            * sizeof(char));
                    strcpy(path2, path);
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
            for (idel=0;idel<children2.nbchild; idel++)
                free(*(children2.childnames + idel));

        }

void print_electromagnetic_source(electromagneticsource_t electromagneticsource)
{
            printf("Electromagnetic Sources : %s\n", path);
}

void free_electromagnetic_source(electromagneticsource_t electromagneticsource)
{

}





    em_source->nb_pw_sources = 0;
    em_source->nb_sw_sources = 0;
    em_source->nb_ge_sources = 0;
    em_source->nb_di_sources = 0;
    em_source->nb_an_sources = 0;
    em_source->nb_sm_sources = 0;

    for (i = 0; i < children.nb_children; i++)
    {
        strcpy(path, C_ELECTROMAGNETIC_SOURCE);
        strcat(path, "/");
        strcat(path, children.childnames[i]);
        if (strcmp(children.childnames[i], ES_PLANE_WAVE) == 0)
            em_source->nb_pw_sources++;
        else if (strcmp(children.childnames[i], ES_SPHERICAL_WAVE) == 0)
            em_source->nb_sw_sources++;
        else if (strcmp(children.childnames[i], ES_GENERATOR) == 0)
            em_source->nb_ge_sources++;
        else if (strcmp(children.childnames[i], ES_DIPOLE) == 0)
            em_source->nb_di_sources++;
        else if (strcmp(children.childnames[i], ES_ANTENNA) == 0)
            em_source->nb_an_sources++;
        else if (strcmp(children.childnames[i], ES_SOURCE_ON_MESH) == 0)
            em_source->nb_sm_sources++;
    }
        if (em_source->nb_pw_sources > 0)

        if (em_source->nb_sw_sources > 0)
            em_source->sw_sources = (sphericalwave_t *) malloc((size_t) nb_sw * sizeof(sphericalwave_t));
        if (em_source->nb_ge_sources > 0)
            em_source->ge_sources = (generator_t *) malloc((size_t) nb_ge * sizeof(generator_t));
        if (em_source->nb_di_sources > 0)
            em_source->di_sources = (dipole_t *) malloc((size_t) nb_di * sizeof(dipole_t));
        if (em_source->nb_an_sources > 0)
            em_source->an_sources = (antenna_t *) malloc((size_t) nb_an * sizeof(antenna_t));
        if (em_source->nb_sm_sources > 0)
            em_source->sm_sources = (sourceonmesh_t *) malloc((size_t) nb_sm * sizeof(sourceonmesh_t));
*/
