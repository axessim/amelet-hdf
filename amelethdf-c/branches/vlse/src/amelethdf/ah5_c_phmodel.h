#ifndef AH5_C_PHMODEL_H
#define AH5_C_PHMODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ah5_c_fltype.h"

    typedef enum _AH5_surface_class_t
    {
        S_INVALID               = -1,
        S_THIN_DIELECTRIAH5_C_LAYER = 1,
        S_SIBC                  = 2,
        S_ZS                    = 3,
        S_ZSZT                  = 4,
        S_ZSZT2                 = 5
    } AH5_surface_class_t;

    typedef struct _AH5_debye_t
    {
        float           limit;
        float           stat;
        hsize_t         nb_gtau;
        float           *gtau;
    } AH5_debye_t;

    typedef struct _AH5_lorentz_t
    {
        float           limit;
        float           stat;
        hsize_t         nb_god;
        float           *god;
    } AH5_lorentz_t;

    typedef enum _AH5_material_prop_class_t
    {
        MP_INVALID              = -1,
        MP_SINGLE_REAL          = 1,
        MP_SINGLE_COMPLEX       = 2,
        MP_GENERAL_RATIONAL_FUNCTION = 3,
        MP_ARRAYSET             = 4,
        MP_DEBYE                = 5,
        MP_LORENTZ              = 6
    } AH5_material_prop_class_t;

    typedef union _AH5_material_prop_data_t
    {
        AH5_singlereal_t singlereal;
        AH5_singlecomplex_t singlecomplex;
        AH5_arrayset_t  arrayset;
        AH5_generalrationalfunction_t generalrationalfunction;
        AH5_debye_t     debye;
        AH5_lorentz_t   lorentz;
    } AH5_material_prop_data_t;

    typedef struct _AH5_vol_inst_props_t
    {
        AH5_material_prop_class_t type;
        AH5_material_prop_data_t data;
    } AH5_material_prop_t;

    typedef struct _AH5_volume_instance_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        AH5_material_prop_t relative_permittivity;
        AH5_material_prop_t relative_permeability;
        AH5_material_prop_t electric_conductivity;
        AH5_material_prop_t magnetic_conductivity;
    } AH5_volume_instance_t;

    typedef struct _AH5_surface_instance_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        AH5_surface_class_t type;
        char            *physicalmodel;
        float           thickness;
        char            *zs;
        char            *zt;
        char            *zs1;
        char            *zt1;
        char            *zs2;
        char            *zt2;
    } AH5_surface_instance_t;

    typedef struct _AH5_interface_instance_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        char            *medium1;
        char            *medium2;
    } AH5_interface_instance_t;

    typedef struct _AH5_physicalmodel_t
    {
        hsize_t         nb_volume_instances;
        AH5_volume_instance_t *volume_instances;
        hsize_t         nb_surface_instances;
        AH5_surface_instance_t *surface_instances;
        hsize_t         nb_interface_instances;
        AH5_interface_instance_t *interface_instances;
    } AH5_physicalmodel_t;

    char AH5_read_phm_vimp (hid_t file_id, const char *path, AH5_material_prop_t *material_prop);
    char AH5_read_phm_volume_instance (hid_t file_id, const char *path, AH5_volume_instance_t *volume_instance);
    char AH5_read_phm_surface_instance_tdl (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    char AH5_read_phm_surface_instance_sibc (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    char AH5_read_phm_surface_instance_zs (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    char AH5_read_phm_surface_instance_zszt (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    char AH5_read_phm_surface_instance_zszt2 (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    char AH5_read_phm_surface_instance (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    char AH5_read_phm_interface_instance (hid_t file_id, const char *path, AH5_interface_instance_t *interface_instance);
    char AH5_read_physicalmodel (hid_t file_id, AH5_physicalmodel_t *physicalmodel);

    void AH5_print_phm_volume_instance (const AH5_volume_instance_t *volume_instance, int space);
    void AH5_print_phm_surface_instance (const AH5_surface_instance_t *surface_instance, int space);
    void AH5_print_phm_interface_instance (const AH5_interface_instance_t *interface_instance, int space);
    void AH5_print_physicalmodel (const AH5_physicalmodel_t *physicalmodel);

    void AH5_free_phm_vimp (AH5_material_prop_t *material_prop);
    void AH5_free_phm_volume_instance (AH5_volume_instance_t *volume_instance);
    void AH5_free_phm_surface_instance (AH5_surface_instance_t *surface_instance);
    void AH5_free_phm_interface_instance (AH5_interface_instance_t *interface_instance);
    void AH5_free_physicalmodel (AH5_physicalmodel_t *physicalmodel);

#ifdef __cplusplus
}
#endif

#endif // AH5_C_PHMODEL_H
