#ifndef EMSOURCE_H
#define EMSOURCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "floatingtype.h"

#define A_X "x"
#define A_Y "y"
#define A_Z "z"
#define A_XO "xo"
#define A_YO "yo"
#define A_ZO "zo"
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
#define G_MODEL "/model"
#define G_GAIN "/gain"
#define G_EFFECTIVE_AREA "/effectiveArea"
#define G_FAR_FIELD "/farField"
#define G_PARABOLIC_REFLECTOR "/parabolicReflector"

#define V_VOLTAGE "voltage"
#define V_CURRENT "current"
#define V_POWER "power"
#define V_POWER_DENSITY "powerDensity"
#define V_ELECTRIC "electric"
#define V_MAGNETIC "magnetic"
#define V_GAIN "gain"
#define V_EFFECTIVE_AREA "effectiveArea"
#define V_FAR_FIELD "farField"
#define V_RECTANGULAR_HORN "rectangularHorn"
#define V_CIRCULAR_HORN "circularHorn"
#define V_LOG_PERIODIC "logPeriodic"
#define V_WHIP "whip"
#define V_GENERIC "generic"
#define V_EXCHANGE_SURFACE "exchangeSurface"

    typedef struct planewave_t
    {
        char        *name;
        opt_attrs_t opt_attrs;
        float       xo;
        float       yo;
        float       zo;
        float       theta;
        float       phi;
        floatingtype_t magnitude;
    } planewave_t;

    typedef struct sphericalwave_t
    {
        char        *name;
        float       xo;
        float       yo;
        float       zo;
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
        opt_attrs_t opt_attrs;
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
        opt_attrs_t opt_attrs;
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


    typedef enum antenna_class_t
    {
        ANT_INVALID             = -1,
        ANT_GAIN                = 1,
        ANT_EFFECTIVE_AREA      = 2,
        ANT_FAR_FIELD           = 3,
        ANT_RECTANGULAR_HORN    = 4,
        ANT_CIRCULAR_HORN       = 5,
        ANT_LOG_PERIODIC        = 6,
        ANT_WHIP                = 7,
        ANT_GENERIC             = 8,
        ANT_EXCHANGE_SURFACE    = 9
    } antenna_class_t;

    typedef union antennax_t
    {
        floatingtype_t gain;
        floatingtype_t effarea;
        floatingtype_t farfield;
        opt_attrs_t parreflct;
    } antennax_t;

    typedef struct antenna_model_t
    {
        antenna_class_t type;
        opt_attrs_t opt_attrs;
        antennax_t data;
    } antenna_model_t;

    typedef struct antenna_t
    {
        char *name;
        opt_attrs_t opt_attrs;
        floatingtype_t input_impedance;
        floatingtype_t load_impedance;
        floatingtype_t feeder_impedance;
        floatingtype_t magnitude;
        antenna_model_t model;
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

    void read_els_planewave (hid_t file_id, const char *path, planewave_t *planewave);
    void read_els_sphericalwave (hid_t file_id, const char *path, sphericalwave_t *sphericalwave);
    void read_els_generator (hid_t file_id, const char *path, generator_t *generator);
    void read_els_dipole (hid_t file_id, const char *path, dipole_t *dipole);
    void read_els_antenna (hid_t file_id, const char *path, antenna_t *antenna);
    void read_els_sourceonmesh (hid_t file_id, const char *path, sourceonmesh_t *sourceonmesh);
    void read_electromagnetic_source (hid_t file_id, em_source_t *em_source);

    void print_els_planewave (planewave_t planewave, int space);
    void print_els_sphericalwave (sphericalwave_t sphericalwave, int space);
    void print_els_generator (generator_t generator, int space);
    void print_els_dipole (dipole_t dipole, int space);
    void print_els_antenna (antenna_t antenna, int space);
    void print_els_sourceonmesh (sourceonmesh_t sourceonmesh, int space);
    void print_electromagnetic_source (em_source_t em_source);

    void free_els_planewave (planewave_t *planewave);
    void free_els_sphericalwave (sphericalwave_t *sphericalwave);
    void free_els_generator (generator_t *generator);
    void free_els_dipole (dipole_t *dipole);
    void free_els_antenna (antenna_t *antenna);
    void free_els_sourceonmesh (sourceonmesh_t *sourceonmesh);
    void free_electromagnetic_source (em_source_t *em_source);

#ifdef __cplusplus
}
#endif

#endif // EMSOURCE_H
