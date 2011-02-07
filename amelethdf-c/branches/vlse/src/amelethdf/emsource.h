#ifndef EMSOURCE_H
#define EMSOURCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amelethdf.h"

#define ES_PLANE_WAVE "planeWave"
#define ES_SPHERICAL_WAVE "sphericalWave"
#define ES_GENERATOR "generator"
#define ES_DIPOLE "dipole"
#define ES_ANTENNA "antenna"
#define ES_SOURCE_ON_MESH "sourceOnMesh"


typedef struct
{
        char *name;
        float x0;
        float y0;
        float z0;
        float theta;
        float phi;
        float linear_polarization;
//        int polarization;
//        complex float elliptical_polarization_etheta;
//        complex float elliptical_polarization_ephi;
        //floatingtype_t magnitude;
} planewave_t;

typedef struct
{
        char *name;
        float x0;
        float y0;
        float z0;
//        floatingtype_t magnitude;
} sphericalwave_t;

typedef struct
{
        char *name;
        char *type;
} generator_t;

typedef struct
{
        char *name;
} dipole_t;

typedef struct
{
        char *name;
} antenna_t;

typedef struct
{
        char *name;
} sourceonmesh_t;

typedef struct
{
        hsize_t nb_pw_sources;
        planewave_t *pw_sources;

        hsize_t nb_sw_sources;
        sphericalwave_t *sw_sources;

        hsize_t nb_ge_sources;
        generator_t *ge_sources;

        hsize_t nb_di_sources;
        dipole_t *di_sources;

        hsize_t nb_an_sources;
        antenna_t *an_sources;

        hsize_t nb_sm_sources;
        sourceonmesh_t *sm_sources;
} em_source_t;

void read_es_planewave(hid_t file_id, const char *path, planewave_t *planewave);
void read_ess_planewave(hid_t file_id, em_source_t *em_source);

void read_ess_sphericalwave(hid_t file_id, em_source_t *em_source);
void read_es_generator(hid_t file_id, const char *path, generator_t *generator);

void read_ess_generator(hid_t file_id, em_source_t *em_source);
void read_es_dipole(hid_t file_id, const char *path, dipole_t *dipole);

void read_ess_dipole(hid_t file_id, em_source_t *em_source);
void read_es_antenna(hid_t file_id, const char *path, antenna_t *antenna);

void read_ess_antenna(hid_t file_id, em_source_t *em_source);
void read_es_sourceonmesh(hid_t file_id, const char *path, sourceonmesh_t *sourceonmesh);

void read_ess_sourceonmesh(hid_t file_id, em_source_t *em_source);
void read_electromagnetic_source(hid_t file_id, em_source_t *em_source);

void print_electromagnetic_source(em_source_t em_source);
void free_electromagnetic_source(em_source_t em_source);

#ifdef __cplusplus
}
#endif

#endif // EMSOURCE_H
