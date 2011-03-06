#ifndef AH5_PHMODEL_H
#define AH5_PHMODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ah5_fltype.h"

    typedef enum AH5_surface_class_t
    {
        S_INVALID               = -1,
        S_THIN_DIELECTRIAH5_C_LAYER = 1,
        S_SIBC                  = 2,
        S_ZS                    = 3,
        S_ZSZT                  = 4,
        S_ZSZT2                 = 5
    } AH5_surface_class_t;

    typedef struct AH5_volume_instance_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        AH5_ft_t        relative_permittivity;
        AH5_ft_t        relative_permeability;
        AH5_ft_t        electric_conductivity;
        AH5_ft_t        magnetic_conductivity;
    } AH5_volume_instance_t;

    typedef struct AH5_surface_instance_t
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

    typedef struct AH5_interface_instance_t
    {
        char            *path;
        AH5_opt_attrs_t opt_attrs;
        char            *medium1;
        char            *medium2;
    } AH5_interface_instance_t;

    typedef struct AH5_physicalmodel_t
    {
        hsize_t         nb_volume_instances;
        AH5_volume_instance_t *volume_instances;
        hsize_t         nb_surface_instances;
        AH5_surface_instance_t *surface_instances;
        hsize_t         nb_interface_instances;
        AH5_interface_instance_t *interface_instances;
    } AH5_physicalmodel_t;

    void AH5_read_phm_volume_instance (hid_t file_id, const char *path, AH5_volume_instance_t *volume_instance);
    void AH5_read_phm_surface_instance_tdl (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    void AH5_read_phm_surface_instance_sibc (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    void AH5_read_phm_surface_instance_zs (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    void AH5_read_phm_surface_instance_zszt (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    void AH5_read_phm_surface_instance_zszt2 (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    void AH5_read_phm_surface_instance (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance);
    void AH5_read_phm_interface_instance (hid_t file_id, const char *path, AH5_interface_instance_t *interface_instance);
    void AH5_read_physicalmodel (hid_t file_id, AH5_physicalmodel_t *physicalmodel);

    void AH5_print_phm_volume_instance (const AH5_volume_instance_t *volume_instance, int space);
    void AH5_print_phm_surface_instance (const AH5_surface_instance_t *surface_instance, int space);
    void AH5_print_phm_interface_instance (const AH5_interface_instance_t *interface_instance, int space);
    void AH5_print_physicalmodel (const AH5_physicalmodel_t *physicalmodel);

    void AH5_free_phm_volume_instance (AH5_volume_instance_t *volume_instance);
    void AH5_free_phm_surface_instance (AH5_surface_instance_t *surface_instance);
    void AH5_free_phm_interface_instance (AH5_interface_instance_t *interface_instance);
    void AH5_free_physicalmodel (AH5_physicalmodel_t *physicalmodel);

#ifdef __cplusplus
}
#endif

#endif // AH5_PHMODEL_H
