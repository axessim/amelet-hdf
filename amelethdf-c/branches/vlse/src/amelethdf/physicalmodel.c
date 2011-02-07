#include "physicalmodel.h"
// Revision: 7.2.2011

// Read instance in physicalModel/volume
void read_physicalmodel_volume_instance (hid_t file_id, const char *path, volume_instance_t *volume_instance)
{
    char *path2;
    char mandatory[][ATTRIBUTE_LENGTH] = {};

    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    volume_instance->name = get_name_from_path(path);
    read_optional_attributes(file_id, path, &(volume_instance->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    strcpy(path2, path);
    strcat(path2, G_RELATIVE_PERMITTIVITY);
    read_floatingtype(file_id, path2, &(volume_instance->relative_permittivity));
    strcpy(path2, path);
    strcat(path2, G_RELATIVE_PERMEABILITY);
    read_floatingtype(file_id, path2, &(volume_instance->relative_permeability));
    strcpy(path2, path);
    strcat(path2, G_ELECTRIC_CONDUCTIVITY);
    read_floatingtype(file_id, path2, &(volume_instance->electric_conductivity));
    strcpy(path2, path);
    strcat(path2, G_MAGNETIC_CONDUCTIVITY);
    read_floatingtype(file_id, path2, &(volume_instance->magnetic_conductivity));
    free(path2);
}


// Read "thin dielectric layer" values of instance in physicalModel/surface
void read_physicalmodel_surface_instance_tdl (hid_t file_id, const char *path, surface_instance_t *surface_instance)
{
    char mandatory[][ATTRIBUTE_LENGTH] = {A_PHYSICAL_MODEL, A_THICKNESS};

    surface_instance->type = S_THIN_DIELECTRIC_LAYER;
    if(!read_string_attribute(file_id, path, A_PHYSICAL_MODEL, &(surface_instance->physicalmodel)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_PHYSICAL_MODEL);
    if(!read_float_attribute(file_id, path, A_THICKNESS, &(surface_instance->thickness)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_THICKNESS);
    read_optional_attributes(file_id, path, &(surface_instance->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
}


// Read "SIBC" values of instance in physicalModel/surface
void read_physicalmodel_surface_instance_sibc (hid_t file_id, const char *path, surface_instance_t *surface_instance)
{
    char mandatory[][ATTRIBUTE_LENGTH] = {A_PHYSICAL_MODEL, A_THICKNESS};

    surface_instance->type = S_SIBC;
    if(!read_string_attribute(file_id, path, A_PHYSICAL_MODEL, &(surface_instance->physicalmodel)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_PHYSICAL_MODEL);
    if(!read_float_attribute(file_id, path, A_THICKNESS, &(surface_instance->thickness)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_THICKNESS);
    read_optional_attributes(file_id, path, &(surface_instance->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
}


// Read "ZS" values of instance in physicalModel/surface
void read_physicalmodel_surface_instance_zs (hid_t file_id, const char *path, surface_instance_t *surface_instance)
{
    char mandatory[][ATTRIBUTE_LENGTH] = {A_ZS};

    surface_instance->type = S_ZS;
    if(!read_string_attribute(file_id, path, A_ZS, &(surface_instance->zs)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_ZS);
    read_optional_attributes(file_id, path, &(surface_instance->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
}


// Read "ZSZT" values of instance in physicalModel/surface
void read_physicalmodel_surface_instance_zszt (hid_t file_id, const char *path, surface_instance_t *surface_instance)
{
    char mandatory[][ATTRIBUTE_LENGTH] = {A_ZS, A_ZT};

    surface_instance->type = S_ZSZT;
    if(!read_string_attribute(file_id, path, A_ZS, &(surface_instance->zs)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_ZS);
    if(!read_string_attribute(file_id, path, A_ZT, &(surface_instance->zt)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_ZT);
    read_optional_attributes(file_id, path, &(surface_instance->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
}


// Read "ZSZT2" values of instance in physicalModel/surface
void read_physicalmodel_surface_instance_zszt2 (hid_t file_id, const char *path, surface_instance_t *surface_instance)
{
    char mandatory[][ATTRIBUTE_LENGTH] = {A_ZS1, A_ZT1, A_ZS2, A_ZT2};

    surface_instance->type = S_ZSZT2;
    if(!read_string_attribute(file_id, path, A_ZS1, &(surface_instance->zs1)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_ZS1);
    if(!read_string_attribute(file_id, path, A_ZT1, &(surface_instance->zt1)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_ZT1);
    if(!read_string_attribute(file_id, path, A_ZS2, &(surface_instance->zs2)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_ZS2);
    if(!read_string_attribute(file_id, path, A_ZT2, &(surface_instance->zt2)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_ZT2);
    read_optional_attributes(file_id, path, &(surface_instance->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
}


// Read instance in physicalModel/surface
void read_physicalmodel_surface_instance (hid_t file_id, const char *path, surface_instance_t *surface_instance)
{
    char *temp;

    surface_instance->physicalmodel = NULL;
    surface_instance->thickness = 0;
    surface_instance->zs = NULL;
    surface_instance->zt = NULL;
    surface_instance->zs1 = NULL;
    surface_instance->zs2 = NULL;
    surface_instance->zt1 = NULL;
    surface_instance->zt2 = NULL;
    surface_instance->name = get_name_from_path(path);
    if (read_string_attribute(file_id, path, A_TYPE, &temp))
    {
        if (strcmp(temp, V_THIN_DIELECTRIC_LAYER) == 0)
            read_physicalmodel_surface_instance_tdl(file_id, path, surface_instance);
        else if (strcmp(temp, V_SIBC) == 0)
            read_physicalmodel_surface_instance_sibc(file_id, path, surface_instance);
        else if (strcmp(temp, V_ZS) == 0)
            read_physicalmodel_surface_instance_zs(file_id, path, surface_instance);
        else if (strcmp(temp, V_ZSZT) == 0)
            read_physicalmodel_surface_instance_zszt(file_id, path, surface_instance);
        else if (strcmp(temp, V_ZSZT2) == 0)
            read_physicalmodel_surface_instance_zszt2(file_id, path, surface_instance);
        else
        {
            surface_instance->type = S_INVALID;
            printf("***** WARNING: Invalid attribute \"type\" in \"%s\". *****\n\n", path);
        }
        free(temp);
    }
    else
    {
        surface_instance->type = S_INVALID;
        printf("***** ERROR: Cannot find mandatory attribute \"%s@%s\". *****\n\n", path, A_TYPE);
    }
}


// Read instance in physicalModel/interface
void read_physicalmodel_interface_instance (hid_t file_id, const char *path, interface_instance_t *interface_instance)
{
    char mandatory[][ATTRIBUTE_LENGTH] = {A_MEDIUM1, A_MEDIUM2};

    interface_instance->name = get_name_from_path(path);
    if (!read_string_attribute(file_id, path, A_MEDIUM1, &(interface_instance->medium1)))
        printf("***** ERROR(%s): Cannot find mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_MEDIUM1);
    if (!read_string_attribute(file_id, path, A_MEDIUM2, &(interface_instance->medium2)))
        printf("***** ERROR(%s): Cannot find mandatory attribute \"%s@%s\". *****\n\n", C_PHYSICAL_MODEL, path, A_MEDIUM2);
    read_optional_attributes(file_id, path, &(interface_instance->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
}


// Read physicalModel category
void read_physicalmodel (hid_t file_id, physicalmodel_t *physicalmodel)
{
    children_t children;
    hsize_t i;
    char *path;

    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    strcpy(path, C_PHYSICAL_MODEL);
    strcat(path, G_VOLUME);
    children = read_children_name(file_id, path);
    physicalmodel->nb_volume_instances = children.nb_children;
    physicalmodel->volume_instances = NULL;
    if (children.nb_children > 0)
    {
        physicalmodel->volume_instances = (volume_instance_t *) malloc((size_t) children.nb_children * sizeof(volume_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_PHYSICAL_MODEL);
            strcat(path, G_VOLUME);
            strcat(path, children.childnames[i]);
            read_physicalmodel_volume_instance(file_id, path, physicalmodel->volume_instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }

    strcpy(path, C_PHYSICAL_MODEL);
    strcat(path, G_SURFACE);
    children = read_children_name(file_id, path);
    physicalmodel->nb_surface_instances = children.nb_children;
    physicalmodel->surface_instances = NULL;
    if (children.nb_children > 0)
    {
        physicalmodel->surface_instances = (surface_instance_t *) malloc((size_t) children.nb_children * sizeof(surface_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_PHYSICAL_MODEL);
            strcat(path, G_SURFACE);
            strcat(path, children.childnames[i]);
            read_physicalmodel_surface_instance(file_id, path, physicalmodel->surface_instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }

    strcpy(path, C_PHYSICAL_MODEL);
    strcat(path, G_INTERFACE);
    children = read_children_name(file_id, path);
    physicalmodel->nb_interface_instances = children.nb_children;
    physicalmodel->interface_instances = NULL;
    if (children.nb_children > 0)
    {
        physicalmodel->interface_instances = (interface_instance_t *) malloc((size_t) children.nb_children * sizeof(interface_instance_t));
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_PHYSICAL_MODEL);
            strcat(path, G_INTERFACE);
            strcat(path, children.childnames[i]);
            read_physicalmodel_interface_instance(file_id, path, physicalmodel->interface_instances + i);
            free(children.childnames[i]);
        }
        free(children.childnames);
    }

    free(path);
}




// Print instance in physicalModel/volume
void print_physicalmodel_volume_instance (volume_instance_t volume_instance)
{
    printf("-name: %s\n", volume_instance.name);
    print_optional_attributes(volume_instance.optional_attributes);
    print_floatingtype(volume_instance.relative_permittivity);
    print_floatingtype(volume_instance.relative_permeability);
    print_floatingtype(volume_instance.electric_conductivity);
    print_floatingtype(volume_instance.magnetic_conductivity);
}


// Print instance in physicalModel/surface
void print_physicalmodel_surface_instance (surface_instance_t surface_instance)
{
    printf("-name: %s\n", surface_instance.name);
    print_optional_attributes(surface_instance.optional_attributes);
    switch (surface_instance.type)
    {
    case S_THIN_DIELECTRIC_LAYER:
        printf("   -physicalModel: %s\n", surface_instance.physicalmodel);
        printf("   -thickness: %g\n",surface_instance.thickness);
        break;
    case S_SIBC:
        printf("   -physicalModel: %s\n", surface_instance.physicalmodel);
        printf("   -thickness: %g\n",surface_instance.thickness);
        break;
    case S_ZS:
        printf("   -Zs: %s\n", surface_instance.zs);
        break;
    case S_ZSZT:
        printf("   -Zs: %s\n", surface_instance.zs);
        printf("   -Zt: %s\n", surface_instance.zt);
        break;
    case S_ZSZT2:
        printf("   -Zs1: %s\n", surface_instance.zs1);
        printf("   -Zt1: %s\n", surface_instance.zt1);
        printf("   -Zs2: %s\n", surface_instance.zs2);
        printf("   -Zt2: %s\n", surface_instance.zt2);
        break;
    default:
        break;
    }
}


// Print instance in physicalModel/interface
void print_physicalmodel_interface_instance (interface_instance_t interface_instance)
{
    printf("-name: %s\n", interface_instance.name);
    print_optional_attributes(interface_instance.optional_attributes);
    if (interface_instance.medium1 != NULL)
        printf("      -medium1: %s\n", interface_instance.medium1);
    if (interface_instance.medium2 != NULL)
        printf("      -medium2: %s\n", interface_instance.medium2);
}


// Print physicalModel category
void print_physicalmodel (physicalmodel_t physicalmodel)
{
    hsize_t i;

    printf("\n##############################  Physical model  ##############################\n\n");
    printf("Volume instances:\n");
    for (i = 0; i < physicalmodel.nb_volume_instances; i++)
    {
        print_physicalmodel_volume_instance(physicalmodel.volume_instances[i]);
    }
    printf("\nSurface instances:\n");
    for (i = 0; i < physicalmodel.nb_surface_instances; i++)
    {
        print_physicalmodel_surface_instance(physicalmodel.surface_instances[i]);
    }
    printf("\nInterface instances:\n");
    for (i = 0; i < physicalmodel.nb_interface_instances; i++)
    {
        print_physicalmodel_interface_instance(physicalmodel.interface_instances[i]);
    }
}




// Free memory used by instance in physicalModel/volume
void free_physicalmodel_volume_instance (volume_instance_t *volume_instance)
{
    if (volume_instance->name != NULL)
    {
        free(volume_instance->name);
        volume_instance->name = NULL;
    }
    free_optional_attributes(&(volume_instance->optional_attributes));
    free_floatingtype(&(volume_instance->relative_permittivity));
    free_floatingtype(&(volume_instance->relative_permeability));
    free_floatingtype(&(volume_instance->electric_conductivity));
    free_floatingtype(&(volume_instance->magnetic_conductivity));
}

// Free memory used by instance in physicalModel/surface
void free_physicalmodel_surface_instance (surface_instance_t *surface_instance)
{
    if (surface_instance->name != NULL)
    {
        free(surface_instance->name);
        surface_instance->name = NULL;
    }
    free_optional_attributes(&(surface_instance->optional_attributes));
    if (surface_instance->physicalmodel != NULL)
    {
        free(surface_instance->physicalmodel);
        surface_instance->physicalmodel = NULL;
    }
    if (surface_instance->zs != NULL)
    {
        free(surface_instance->zs);
        surface_instance->zs = NULL;
    }
    if (surface_instance->zt != NULL)
    {
        free(surface_instance->zt);
        surface_instance->zt = NULL;
    }
    if (surface_instance->zs1 != NULL)
    {
        free(surface_instance->zs1);
        surface_instance->zs1 = NULL;
    }
    if (surface_instance->zt1 != NULL)
    {
        free(surface_instance->zt1);
        surface_instance->zt1 = NULL;
    }
    if (surface_instance->zs2 != NULL)
    {
        free(surface_instance->zs2);
        surface_instance->zs2 = NULL;
    }
    if (surface_instance->zt2 != NULL)
    {
        free(surface_instance->zt2);
        surface_instance->zt2 = NULL;
    }
}


// Free memory used by instance in physicalModel/interface
void free_physicalmodel_interface_instance (interface_instance_t *interface_instance)
{
    if (interface_instance->name != NULL)
    {
        free(interface_instance->name);
        interface_instance->name = NULL;
    }
    free_optional_attributes(&(interface_instance->optional_attributes));
    if (interface_instance->medium1 != NULL)
    {
        free(interface_instance->medium1);
        interface_instance->medium1 = NULL;
    }
    if (interface_instance->medium2 != NULL)
    {
        free(interface_instance->medium2);
        interface_instance->medium2 = NULL;
    }
}


// Free memory used by physicalModel category
void free_physicalmodel (physicalmodel_t *physicalmodel)
{
    hsize_t i;

    if (physicalmodel->nb_volume_instances > 0)
    {
        for (i = 0; i < physicalmodel->nb_volume_instances; i++)
            free_physicalmodel_volume_instance(physicalmodel->volume_instances + i);
        free(physicalmodel->volume_instances);
        physicalmodel->volume_instances = NULL;
        physicalmodel->nb_volume_instances = 0;
    }

    if (physicalmodel->nb_surface_instances > 0)
    {
        for (i = 0; i < physicalmodel->nb_surface_instances; i++)
            free_physicalmodel_surface_instance(physicalmodel->surface_instances + i);
        free(physicalmodel->surface_instances);
        physicalmodel->surface_instances = NULL;
        physicalmodel->nb_surface_instances = 0;
    }

    if (physicalmodel->nb_interface_instances > 0)
    {
        for (i = 0; i < physicalmodel->nb_interface_instances; i++)
            free_physicalmodel_interface_instance(physicalmodel->interface_instances + i);
        free(physicalmodel->interface_instances);
        physicalmodel->interface_instances = NULL;
        physicalmodel->nb_interface_instances = 0;
    }
}
