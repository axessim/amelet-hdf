#include "physicalmodel.h"


// Read instance in physicalModel/volume
void read_physicalmodel_volume_instance (hid_t file_id, const char *path, volume_instance_t *volume_instance)
{
    char path2[ABSOLUTE_PATH_NAME_LENGTH];
    char mandatory[][ATTR_LENGTH] = {};

    volume_instance->name = get_name_from_path(path);
    read_opt_attrs(file_id, path, &(volume_instance->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
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
}


// Read "thin dielectric layer" values of instance in physicalModel/surface
void read_physicalmodel_surface_instance_tdl (hid_t file_id, const char *path, surface_instance_t *surface_instance)
{
    char mandatory[][ATTR_LENGTH] = {A_PHYSICAL_MODEL, A_THICKNESS};

    surface_instance->type = S_THIN_DIELECTRIC_LAYER;
    if(!read_str_attr(file_id, path, A_PHYSICAL_MODEL, &(surface_instance->physicalmodel)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_PHYSICAL_MODEL);
    if(!read_flt_attr(file_id, path, A_THICKNESS, &(surface_instance->thickness)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_THICKNESS);
    read_opt_attrs(file_id, path, &(surface_instance->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
}


// Read "SIBC" values of instance in physicalModel/surface
void read_physicalmodel_surface_instance_sibc (hid_t file_id, const char *path, surface_instance_t *surface_instance)
{
    char mandatory[][ATTR_LENGTH] = {A_PHYSICAL_MODEL, A_THICKNESS};

    surface_instance->type = S_SIBC;
    if(!read_str_attr(file_id, path, A_PHYSICAL_MODEL, &(surface_instance->physicalmodel)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_PHYSICAL_MODEL);
    if(!read_flt_attr(file_id, path, A_THICKNESS, &(surface_instance->thickness)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_THICKNESS);
    read_opt_attrs(file_id, path, &(surface_instance->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
}


// Read "ZS" values of instance in physicalModel/surface
void read_physicalmodel_surface_instance_zs (hid_t file_id, const char *path, surface_instance_t *surface_instance)
{
    char mandatory[][ATTR_LENGTH] = {A_ZS};

    surface_instance->type = S_ZS;
    if(!read_str_attr(file_id, path, A_ZS, &(surface_instance->zs)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_ZS);
    read_opt_attrs(file_id, path, &(surface_instance->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
}


// Read "ZSZT" values of instance in physicalModel/surface
void read_physicalmodel_surface_instance_zszt (hid_t file_id, const char *path, surface_instance_t *surface_instance)
{
    char mandatory[][ATTR_LENGTH] = {A_ZS, A_ZT};

    surface_instance->type = S_ZSZT;
    if(!read_str_attr(file_id, path, A_ZS, &(surface_instance->zs)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_ZS);
    if(!read_str_attr(file_id, path, A_ZT, &(surface_instance->zt)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_ZT);
    read_opt_attrs(file_id, path, &(surface_instance->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
}


// Read "ZSZT2" values of instance in physicalModel/surface
void read_physicalmodel_surface_instance_zszt2 (hid_t file_id, const char *path, surface_instance_t *surface_instance)
{
    char mandatory[][ATTR_LENGTH] = {A_ZS1, A_ZT1, A_ZS2, A_ZT2};

    surface_instance->type = S_ZSZT2;
    if(!read_str_attr(file_id, path, A_ZS1, &(surface_instance->zs1)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_ZS1);
    if(!read_str_attr(file_id, path, A_ZT1, &(surface_instance->zt1)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_ZT1);
    if(!read_str_attr(file_id, path, A_ZS2, &(surface_instance->zs2)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_ZS2);
    if(!read_str_attr(file_id, path, A_ZT2, &(surface_instance->zt2)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_ZT2);
    read_opt_attrs(file_id, path, &(surface_instance->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
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
    if (read_str_attr(file_id, path, A_TYPE, &temp))
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
            print_wrn_attr(C_PHYSICAL_MODEL, path, A_TYPE);

        }
        free(temp);
    }
    else
    {
        surface_instance->type = S_INVALID;
        print_err_attr(C_PHYSICAL_MODEL, path, A_TYPE);
    }
}


// Read instance in physicalModel/interface
void read_physicalmodel_interface_instance (hid_t file_id, const char *path, interface_instance_t *interface_instance)
{
    char mandatory[][ATTR_LENGTH] = {A_MEDIUM1, A_MEDIUM2};

    interface_instance->name = get_name_from_path(path);
    if (!read_str_attr(file_id, path, A_MEDIUM1, &(interface_instance->medium1)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_MEDIUM1);
    if (!read_str_attr(file_id, path, A_MEDIUM2, &(interface_instance->medium2)))
        print_err_attr(C_PHYSICAL_MODEL, path, A_MEDIUM2);
    read_opt_attrs(file_id, path, &(interface_instance->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
}


// Read physicalModel category
void read_physicalmodel (hid_t file_id, physicalmodel_t *physicalmodel)
{
    children_t children;
    hsize_t i;
    char path[ABSOLUTE_PATH_NAME_LENGTH];

    strcpy(path, C_PHYSICAL_MODEL);
    strcat(path, G_VOLUME);
    children = read_children_name(file_id, path);
    physicalmodel->nb_volume_instances = children.nb_children;
    physicalmodel->volume_instances = NULL;
    if (children.nb_children > 0)
    {
        physicalmodel->volume_instances = (volume_instance_t *) malloc(children.nb_children * sizeof(volume_instance_t));
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
        physicalmodel->surface_instances = (surface_instance_t *) malloc(children.nb_children * sizeof(surface_instance_t));
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
        physicalmodel->interface_instances = (interface_instance_t *) malloc(children.nb_children * sizeof(interface_instance_t));
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
}




// Print instance in physicalModel/volume
void print_physicalmodel_volume_instance (volume_instance_t volume_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", volume_instance.name);
    print_opt_attrs(volume_instance.opt_attrs, space + 3);
    print_floatingtype(volume_instance.relative_permittivity, space + 3);
    print_floatingtype(volume_instance.relative_permeability, space + 3);
    print_floatingtype(volume_instance.electric_conductivity, space + 3);
    print_floatingtype(volume_instance.magnetic_conductivity, space + 3);
    printf("\n");
}


// Print instance in physicalModel/surface
void print_physicalmodel_surface_instance (surface_instance_t surface_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", surface_instance.name);
    print_opt_attrs(surface_instance.opt_attrs, space + 3);
    switch (surface_instance.type)
    {
    case S_THIN_DIELECTRIC_LAYER:
        print_str_attr(A_PHYSICAL_MODEL, surface_instance.physicalmodel, space + 3);
        print_flt_attr(A_THICKNESS, surface_instance.thickness, space + 3);
        break;
    case S_SIBC:
        print_str_attr(A_PHYSICAL_MODEL, surface_instance.physicalmodel, space + 3);
        print_flt_attr(A_THICKNESS, surface_instance.thickness, space + 3);
        break;
    case S_ZS:
        print_str_attr(A_ZS, surface_instance.zs, space + 3);
        break;
    case S_ZSZT:
        print_str_attr(A_ZS, surface_instance.zs, space + 3);
        print_str_attr(A_ZT, surface_instance.zt, space + 3);
        break;
    case S_ZSZT2:
        print_str_attr(A_ZS1, surface_instance.zs1, space + 3);
        print_str_attr(A_ZT1, surface_instance.zt1, space + 3);
        print_str_attr(A_ZS2, surface_instance.zs2, space + 3);
        print_str_attr(A_ZT2, surface_instance.zt2, space + 3);
        break;
    default:
        break;
    }
    printf("\n");
}


// Print instance in physicalModel/interface
void print_physicalmodel_interface_instance (interface_instance_t interface_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", interface_instance.name);
    print_opt_attrs(interface_instance.opt_attrs, space + 3);
    if (interface_instance.medium1 != NULL)
        print_str_attr(A_MEDIUM1, interface_instance.medium1, space + 3);
    if (interface_instance.medium2 != NULL)
        print_str_attr(A_MEDIUM2, interface_instance.medium2, space + 3);
    printf("\n");
}


// Print physicalModel category
void print_physicalmodel (physicalmodel_t physicalmodel)
{
    hsize_t i;

    if (physicalmodel.nb_volume_instances > 0 || physicalmodel.nb_surface_instances > 0 || physicalmodel.nb_interface_instances > 0)
    {
        printf("##############################  Physical model  ##############################\n\n");
        if (physicalmodel.nb_volume_instances > 0)
        {
            printf("Volume:\n");
            for (i = 0; i < physicalmodel.nb_volume_instances; i++)
                print_physicalmodel_volume_instance(physicalmodel.volume_instances[i], 3);
        }
        if (physicalmodel.nb_surface_instances > 0)
        {
            printf("Surface:\n");
            for (i = 0; i < physicalmodel.nb_surface_instances; i++)
                print_physicalmodel_surface_instance(physicalmodel.surface_instances[i], 3);
        }
        if (physicalmodel.nb_interface_instances > 0)
        {
            printf("Interface:\n");
            for (i = 0; i < physicalmodel.nb_interface_instances; i++)
                print_physicalmodel_interface_instance(physicalmodel.interface_instances[i], 3);
        }
        printf("\n");
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
    free_opt_attrs(&(volume_instance->opt_attrs));
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
    free_opt_attrs(&(surface_instance->opt_attrs));
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
    free_opt_attrs(&(interface_instance->opt_attrs));
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
