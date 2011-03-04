#ifndef PHYSICALMODEL_H
#define PHYSICALMODEL_H

#ifdef __cplusplus
extern "C" {
#endif

//#include "amelethdf.h"
#include "floatingtype.h"

#define G_RELATIVE_PERMITTIVITY "/relativePermittivity"
#define G_RELATIVE_PERMEABILITY "/relativePermeability"
#define G_ELECTRIC_CONDUCTIVITY "/electricConductivity"
#define G_MAGNETIC_CONDUCTIVITY "/magneticConductivity"
#define G_VOLUME "/volume"
#define G_VACUUM "/vacuum"
#define G_PERFECT_ELECTRIC_CONDUCTOR "/perfectElectricConductor"
#define G_PERFECT_MAGNETIC_CONDUCTOR "/perfectMagneticConductor"
#define G_SURFACE "/surface"
#define G_INTERFACE "/interface"

#define A_F_MIN "frequency_validity_min"
#define A_F_MAX "frequency_validity_max"
#define A_TYPE "type"
#define A_PHYSICAL_MODEL "physicalModel"
#define A_THICKNESS "thickness"
#define A_ZS "Zs"
#define A_ZT "Zt"
#define A_ZS1 "Zs1"
#define A_ZT1 "Zt1"
#define A_ZS2 "Zs2"
#define A_ZT2 "Zt2"
#define A_MEDIUM1 "medium1"
#define A_MEDIUM2 "medium2"

#define V_THIN_DIELECTRIC_LAYER "thinDielectricLayer"
#define V_SIBC "SIBC"
#define V_ZS "Zs"
#define V_ZSZT "ZsZt"
#define V_ZSZT2 "ZsZt2"


    typedef enum surface_class_t
    {
        S_INVALID               = -1,
        S_THIN_DIELECTRIC_LAYER = 1,
        S_SIBC                  = 2,
        S_ZS                    = 3,
        S_ZSZT                  = 4,
        S_ZSZT2                 = 5
    } surface_class_t;

    typedef struct volume_instance_t
    {
        char *path;
        opt_attrs_t opt_attrs;
        floatingtype_t relative_permittivity;
        floatingtype_t relative_permeability;
        floatingtype_t electric_conductivity;
        floatingtype_t magnetic_conductivity;
    } volume_instance_t;


    typedef struct surface_instance_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        surface_class_t type;
        char        *physicalmodel;
        float       thickness;
        char        *zs;
        char        *zt;
        char        *zs1;
        char        *zt1;
        char        *zs2;
        char        *zt2;
    } surface_instance_t;

    typedef struct interface_instance_t
    {
        char        *path;
        opt_attrs_t opt_attrs;
        char        *medium1;
        char        *medium2;
    } interface_instance_t;

    typedef struct physicalmodel_t
    {
        hsize_t     nb_volume_instances;
        volume_instance_t *volume_instances;
        hsize_t     nb_surface_instances;
        surface_instance_t *surface_instances;
        hsize_t     nb_interface_instances;
        interface_instance_t *interface_instances;
    } physicalmodel_t;

    void read_physicalmodel_volume_instance (hid_t file_id, const char *path, volume_instance_t *volume_instance);
    void read_physicalmodel_surface_instance_tdl (hid_t file_id, const char *path, surface_instance_t *surface_instance);
    void read_physicalmodel_surface_instance_sibc (hid_t file_id, const char *path, surface_instance_t *surface_instance);
    void read_physicalmodel_surface_instance_zs (hid_t file_id, const char *path, surface_instance_t *surface_instance);
    void read_physicalmodel_surface_instance_zszt (hid_t file_id, const char *path, surface_instance_t *surface_instance);
    void read_physicalmodel_surface_instance_zszt2 (hid_t file_id, const char *path, surface_instance_t *surface_instance);
    void read_physicalmodel_surface_instance (hid_t file_id, const char *path, surface_instance_t *surface_instance);
    void read_physicalmodel_interface_instance (hid_t file_id, const char *path, interface_instance_t *interface_instance);
    void read_physicalmodel (hid_t file_id, physicalmodel_t *physicalmodel);

    void print_physicalmodel_volume_instance (volume_instance_t volume_instance, int space);
    void print_physicalmodel_surface_instance (surface_instance_t surface_instance, int space);
    void print_physicalmodel_interface_instance (interface_instance_t interface_instance, int space);
    void print_physicalmodel (physicalmodel_t physicalmodel);

    void free_physicalmodel_volume_instance (volume_instance_t *volume_instance);
    void free_physicalmodel_surface_instance (surface_instance_t *surface_instance);
    void free_physicalmodel_interface_instance (interface_instance_t *interface_instance);
    void free_physicalmodel (physicalmodel_t *physicalmodel);

#ifdef __cplusplus
}
#endif

#endif // PHYSICALMODEL_H
