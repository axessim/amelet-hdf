#ifndef EMSOURCE_H
#define EMSOURCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "floatingtype.h"

#define A_X "x"
#define A_Y "y"
#define A_Z "z"
#define A_X0 "xo"
#define A_Y0 "yo"
#define A_Z0 "zo"
#define A_THETA "theta"
#define A_PHI "phi"
#define A_TYPE "type"
#define A_WIRE_RADIUS "wireRadius"
#define A_EXCHANGE_SURFACE "exchangeSurface"

#define G_PLANE_WAVE "/planeWave"
#define G_SPHERICAL_WAVE "/sphericalWave"
#define G_GENERATOR "/generator"
#define G_DIPOLE "/dipole"
#define G_ANTENNA "/antenna"
#define G_SOURCE_ON_MESH "/sourceOnMesh"
#define G_INNER_IMPEDANCE "/innerImpedance"
#define G_MAGNITUDE "/magnitude"
#define G_INPUT_IMPEDANCE "/inputImpedance"
#define G_LOAD_IMPEDANCE "/loadImpedance"
#define G_FEEDER_IMPEDANCE "/feederImpedance"

#define V_VOLTAGE "voltage"
#define V_CURRENT "current"
#define V_POWER "power"
#define V_POWER_DENSITY "powerDensity"
#define V_ELECTRIC "electric"
#define V_MAGNETIC "magnetic"
#define V_EXCHANGE_SURFACE "exchangeSurface"

    typedef struct planewave_t
    {
        char        *name;
        optional_attributes_t optional_attributes;
        float       x0;
        float       y0;
        float       z0;
        float       theta;
        float       phi;
        floatingtype_t magnitude;
    } planewave_t;

    typedef struct sphericalwave_t
    {
        char        *name;
        float       x0;
        float       y0;
        float       z0;
        floatingtype_t magnitude;
    } sphericalwave_t;

    typedef enum generator_class_t
    {
        GEN_INVALID             = -1,
        GEN_VOLTAGE             = 1,
        GEN_CURRENT             = 2,
        GEN_POWER               = 3,
        GEN_POWER_DENSITY       = 4
    } generator_class_t;

    typedef struct generator_t
    {
        char        *name;
        optional_attributes_t optional_attributes;
        generator_class_t type;
        floatingtype_t inner_impedance;
        floatingtype_t magnitude;
    } generator_t;

    typedef enum dipole_class_t
    {
        DIPOLE_INVALID          = -1,
        DIPOLE_ELECTRIC         = 1,
        DIPOLE_MAGNETIC         = 2
    } dipole_class_t;

    typedef struct dipole_t
    {
        char        *name;
        optional_attributes_t optional_attributes;
        dipole_class_t type;
        float       x;
        float       y;
        float       z;
        float       theta;
        float       phi;
        float       wire_radius;
        floatingtype_t inner_impedance;
        floatingtype_t magnitude;
    } dipole_t;

    typedef struct antenna_t
    {
        char *name;
        optional_attributes_t optional_attributes;
        floatingtype_t input_impedance;
        floatingtype_t load_impedance;
        floatingtype_t feeder_impedance;
        floatingtype_t magnitude;
        // TO BE CONTINUED...
    } antenna_t;

    typedef enum sourceonmesh_class_t
    {
        SCOM_INVALID            = -1,
        SCOM_ARRAYSET           = 1,
        SCOM_EXCHANGE_SURFACE   = 2
    } sourceonmesh_class_t;

    typedef union sourceonmeshx_t
    {
        arrayset_t  arrayset;
        char        *exchange_surface;
    } sourceonmeshx_t;

    typedef struct sourceonmesh_t
    {
        char        *name;
        sourceonmesh_class_t type;
        sourceonmeshx_t data;
    } sourceonmesh_t;

    typedef struct em_source_t
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

    void read_es_planewave (hid_t file_id, const char *path, planewave_t *planewave);
    void read_es_sphericalwave (hid_t file_id, const char *path, sphericalwave_t *sphericalwave);
    void read_es_generator (hid_t file_id, const char *path, generator_t *generator);
    void read_es_dipole (hid_t file_id, const char *path, dipole_t *dipole);
    void read_es_antenna (hid_t file_id, const char *path, antenna_t *antenna);
    void read_es_sourceonmesh (hid_t file_id, const char *path, sourceonmesh_t *sourceonmesh);
    void read_electromagnetic_source (hid_t file_id, em_source_t *em_source);

    void print_es_planewave (planewave_t planewave, int space);
    void print_es_sphericalwave (sphericalwave_t sphericalwave, int space);
    void print_es_generator (generator_t generator, int space);
    void print_es_dipole (dipole_t dipole, int space);
    void print_es_antenna (antenna_t antenna, int space);
    void print_es_sourceonmesh (sourceonmesh_t sourceonmesh, int space);
    void print_electromagnetic_source (em_source_t em_source);

    void free_es_planewave (planewave_t *planewave);
    void free_es_sphericalwave (sphericalwave_t *sphericalwave);
    void free_es_generator (generator_t *generator);
    void free_es_dipole (dipole_t *dipole);
    void free_es_antenna (antenna_t *antenna);
    void free_es_sourceonmesh (sourceonmesh_t *sourceonmesh);
    void free_electromagnetic_source (em_source_t *em_source);

#ifdef __cplusplus
}
#endif

#endif // EMSOURCE_H
