#ifndef AH5_EMSOURCE_H
#define AH5_EMSOURCE_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "ah5_fltype.h"

    typedef struct AH5_planewave_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        float           xo;
        float           yo;
        float           zo;
        float           theta;
        float           phi;
        AH5_ft_t magnitude;
    } AH5_planewave_t;

    typedef struct AH5_sphericalwave_t
    {
        char            *path;
        float           xo;
        float           yo;
        float           zo;
        AH5_ft_t magnitude;
    } AH5_sphericalwave_t;

    typedef enum AH5_gen_class_t
    {
        GEN_INVALID             = -1,
        GEN_VOLTAGE             = 1,
        GEN_CURRENT             = 2,
        GEN_POWER               = 3,
        GEN_POWER_DENSITY       = 4
    } AH5_gen_class_t;

    typedef struct AH5_generator_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        AH5_gen_class_t type;
        AH5_ft_t inner_impedance;
        AH5_ft_t magnitude;
    } AH5_generator_t;

    typedef enum AH5_dip_class_t
    {
        DIPOLE_INVALID          = -1,
        DIPOLE_ELECTRIC         = 1,
        DIPOLE_MAGNETIC         = 2
    } AH5_dip_class_t;

    typedef struct AH5_dipole_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        AH5_dip_class_t type;
        float           x;
        float           y;
        float           z;
        float           theta;
        float           phi;
        float           wire_radius;
        AH5_ft_t        inner_impedance;
        AH5_ft_t        magnitude;
    } AH5_dipole_t;

    typedef enum AH5_ant_class_t
    {
        ANT_INVALID             = -1,
        ANT_GAIN                = 1,
        ANT_EFFECTIVE_AREA      = 2,
        ANT_FAR_FIELD           = 3,
        ANT_RECTANGULAR_HORN    = 4,
        ANT_CIRCULAR_HORN       = 5,
        ANT_LOAH5_G_PERIODIC        = 6,
        ANT_WHIP                = 7,
        ANT_GENERIC             = 8,
        ANT_EXCHANGE_SURFACE    = 9
    } AH5_ant_class_t;

    typedef union AH5_antennax_t
    {
        AH5_ft_t        gain;
        AH5_ft_t        effarea;
        AH5_ft_t        farfield;
        AH5_opt_attrs_t parreflct;
    } AH5_antennax_t;

    typedef struct AH5_ant_model_t
    {
        AH5_ant_class_t type;
        AH5_opt_attrs_t opt_attrs;
        AH5_antennax_t  data;
    } AH5_ant_model_t;

    typedef struct AH5_antenna_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        AH5_ft_t        input_impedance;
        AH5_ft_t        load_impedance;
        AH5_ft_t        feeder_impedance;
        AH5_ft_t        magnitude;
        AH5_ant_model_t model;
    } AH5_antenna_t;

    typedef enum AH5_scm_class_t
    {
        SCOM_INVALID            = -1,
        SCOM_ARRAYSET           = 1,
        SCOM_EXCHANGE_SURFACE   = 2
    } AH5_scm_class_t;

    typedef union AH5_scmx_t
    {
        AH5_arrayset_t  arrayset;
        char            *exchange_surface;
    } AH5_scmx_t;

    typedef struct AH5_sourceonmesh_t
    {
        char            *path;
        AH5_scm_class_t type;
        AH5_scmx_t      data;
    } AH5_sourceonmesh_t;

    typedef struct AH5_em_source_t
    {
        hsize_t         nb_pw_sources;
        AH5_planewave_t *pw_sources;

        hsize_t         nb_sw_sources;
        AH5_sphericalwave_t *sw_sources;

        hsize_t         nb_ge_sources;
        AH5_generator_t *ge_sources;

        hsize_t         nb_di_sources;
        AH5_dipole_t    *di_sources;

        hsize_t         nb_an_sources;
        AH5_antenna_t   *an_sources;

        hsize_t         nb_sm_sources;
        AH5_sourceonmesh_t *sm_sources;
    } AH5_em_source_t;

    char AH5_read_els_planewave (hid_t file_id, const char *path, AH5_planewave_t *planewave);
    char AH5_read_els_sphericalwave (hid_t file_id, const char *path, AH5_sphericalwave_t *sphericalwave);
    char AH5_read_els_generator (hid_t file_id, const char *path, AH5_generator_t *generator);
    char AH5_read_els_dipole (hid_t file_id, const char *path, AH5_dipole_t *dipole);
    char AH5_read_els_antenna (hid_t file_id, const char *path, AH5_antenna_t *antenna);
    char AH5_read_els_sourceonmesh (hid_t file_id, const char *path, AH5_sourceonmesh_t *sourceonmesh);
    char AH5_read_electromagnetic_source (hid_t file_id, AH5_em_source_t *em_source);

    void AH5_print_els_planewave (const AH5_planewave_t *planewave, int space);
    void AH5_print_els_sphericalwave (const AH5_sphericalwave_t *sphericalwave, int space);
    void AH5_print_els_generator (const AH5_generator_t *generator, int space);
    void AH5_print_els_dipole (const AH5_dipole_t *dipole, int space);
    void AH5_print_els_antenna (const AH5_antenna_t *antenna, int space);
    void AH5_print_els_sourceonmesh (const AH5_sourceonmesh_t *sourceonmesh, int space);
    void AH5_print_electromagnetic_source (const AH5_em_source_t *em_source);

    void AH5_free_els_planewave (AH5_planewave_t *planewave);
    void AH5_free_els_sphericalwave (AH5_sphericalwave_t *sphericalwave);
    void AH5_free_els_generator (AH5_generator_t *generator);
    void AH5_free_els_dipole (AH5_dipole_t *dipole);
    void AH5_free_els_antenna (AH5_antenna_t *antenna);
    void AH5_free_els_sourceonmesh (AH5_sourceonmesh_t *sourceonmesh);
    void AH5_free_electromagnetic_source (AH5_em_source_t *em_source);

#ifdef __cplusplus
}
#endif

#endif // AH5_EMSOURCE_H
