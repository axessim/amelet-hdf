#include "ah5_phmodel.h"


// Read instance in physicalModel/volume
char AH5_read_phm_volume_instance (hid_t file_id, const char *path, AH5_volume_instance_t *volume_instance)
{
    char path2[AH5_ABSOLUTE_PATH_LENGTH], rdata = TRUE;
    char mandatory[][AH5_ATTR_LENGTH] = {};

    volume_instance->path = strdup(path);
    volume_instance->opt_attrs.instances = NULL;

    if (AH5_path_valid(file_id, path))
    {
        AH5_read_opt_attrs(file_id, path, &(volume_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
        strcpy(path2, path);
        strcat(path2, AH5_G_RELATIVE_PERMITTIVITY);
        if (!AH5_read_floatingtype(file_id, path2, &(volume_instance->relative_permittivity)))
            rdata = FALSE;
        strcpy(path2, path);
        strcat(path2, AH5_G_RELATIVE_PERMEABILITY);
        if (!AH5_read_floatingtype(file_id, path2, &(volume_instance->relative_permeability)))
            rdata = FALSE;
        strcpy(path2, path);
        strcat(path2, AH5_G_ELECTRIC_CONDUCTIVITY);
        if (!AH5_read_floatingtype(file_id, path2, &(volume_instance->electric_conductivity)))
            rdata = FALSE;
        strcpy(path2, path);
        strcat(path2, AH5_G_MAGNETIC_CONDUCTIVITY);
        if (!AH5_read_floatingtype(file_id, path2, &(volume_instance->magnetic_conductivity)))
            rdata = FALSE;
    }
    else
    {
        AH5_print_err_path(AH5_C_PHYSICAL_MODEL, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read "thin dielectric layer" values of instance in physicalModel/surface
char AH5_read_phm_surface_instance_tdl (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_PHYSICAL_MODEL, AH5_A_THICKNESS};
    char rdata = TRUE;

    if (AH5_path_valid(file_id, path))
    {
        surface_instance->type = S_THIN_DIELECTRIAH5_C_LAYER;
        if(!AH5_read_str_attr(file_id, path, AH5_A_PHYSICAL_MODEL, &(surface_instance->physicalmodel)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_PHYSICAL_MODEL);
            rdata = FALSE;
        }
        if(!AH5_read_flt_attr(file_id, path, AH5_A_THICKNESS, &(surface_instance->thickness)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_THICKNESS);
            rdata = FALSE;
        }
        AH5_read_opt_attrs(file_id, path, &(surface_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
    {
        AH5_print_err_path(AH5_C_PHYSICAL_MODEL, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read "SIBC" values of instance in physicalModel/surface
char AH5_read_phm_surface_instance_sibc (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_PHYSICAL_MODEL, AH5_A_THICKNESS};
    char rdata = TRUE;

    if (AH5_path_valid(file_id, path))
    {
        surface_instance->type = S_SIBC;
        if(!AH5_read_str_attr(file_id, path, AH5_A_PHYSICAL_MODEL, &(surface_instance->physicalmodel)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_PHYSICAL_MODEL);
            rdata = FALSE;
        }
        if(!AH5_read_flt_attr(file_id, path, AH5_A_THICKNESS, &(surface_instance->thickness)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_THICKNESS);
            rdata = FALSE;
        }
        AH5_read_opt_attrs(file_id, path, &(surface_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
    {
        AH5_print_err_path(AH5_C_PHYSICAL_MODEL, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read "ZS" values of instance in physicalModel/surface
char AH5_read_phm_surface_instance_zs (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_ZS}, rdata = TRUE;

    if (AH5_path_valid(file_id, path))
    {
        surface_instance->type = S_ZS;
        if(!AH5_read_str_attr(file_id, path, AH5_A_ZS, &(surface_instance->zs)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_ZS);
            rdata = FALSE;
        }
        AH5_read_opt_attrs(file_id, path, &(surface_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
    {
        AH5_print_err_path(AH5_C_PHYSICAL_MODEL, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read "ZSZT" values of instance in physicalModel/surface
char AH5_read_phm_surface_instance_zszt (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_ZS, AH5_A_ZT}, rdata = TRUE;

    if (AH5_path_valid(file_id, path))
    {
        surface_instance->type = S_ZSZT;
        if(!AH5_read_str_attr(file_id, path, AH5_A_ZS, &(surface_instance->zs)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_ZS);
            rdata = FALSE;
        }
        if(!AH5_read_str_attr(file_id, path, AH5_A_ZT, &(surface_instance->zt)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_ZT);
            rdata = FALSE;
        }
        AH5_read_opt_attrs(file_id, path, &(surface_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
    {
        AH5_print_err_path(AH5_C_PHYSICAL_MODEL, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read "ZSZT2" values of instance in physicalModel/surface
char AH5_read_phm_surface_instance_zszt2 (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_ZS1, AH5_A_ZT1, AH5_A_ZS2, AH5_A_ZT2};
    char rdata = TRUE;

    if (AH5_path_valid(file_id, path))
    {
        surface_instance->type = S_ZSZT2;
        if(!AH5_read_str_attr(file_id, path, AH5_A_ZS1, &(surface_instance->zs1)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_ZS1);
            rdata = FALSE;
        }
        if(!AH5_read_str_attr(file_id, path, AH5_A_ZT1, &(surface_instance->zt1)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_ZT1);
            rdata = FALSE;
        }
        if(!AH5_read_str_attr(file_id, path, AH5_A_ZS2, &(surface_instance->zs2)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_ZS2);
            rdata = FALSE;
        }
        if(!AH5_read_str_attr(file_id, path, AH5_A_ZT2, &(surface_instance->zt2)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_ZT2);
            rdata = FALSE;
        }
        AH5_read_opt_attrs(file_id, path, &(surface_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
    {
        AH5_print_err_path(AH5_C_PHYSICAL_MODEL, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read instance in physicalModel/surface
char AH5_read_phm_surface_instance (hid_t file_id, const char *path, AH5_surface_instance_t *surface_instance)
{
    char *temp, rdata = TRUE;

    surface_instance->path = strdup(path);
    surface_instance->physicalmodel = NULL;
    surface_instance->thickness = 0;
    surface_instance->zs = NULL;
    surface_instance->zt = NULL;
    surface_instance->zs1 = NULL;
    surface_instance->zs2 = NULL;
    surface_instance->zt1 = NULL;
    surface_instance->zt2 = NULL;
    surface_instance->type = S_INVALID;
    surface_instance->opt_attrs.instances = NULL;

    if (AH5_path_valid(file_id, path))
    {
        if (AH5_read_str_attr(file_id, path, AH5_A_TYPE, &temp))
        {
            if (strcmp(temp, AH5_V_THIN_DIELECTRIC_LAYER) == 0)
            {
                if (!AH5_read_phm_surface_instance_tdl(file_id, path, surface_instance))
                    rdata = FALSE;
            }
            else if (strcmp(temp, AH5_V_SIBC) == 0)
            {
                if (!AH5_read_phm_surface_instance_sibc(file_id, path, surface_instance))
                    rdata = FALSE;
            }
            else if (strcmp(temp, AH5_V_ZS) == 0)
            {
                if (!AH5_read_phm_surface_instance_zs(file_id, path, surface_instance))
                    rdata = FALSE;
            }
            else if (strcmp(temp, AH5_V_ZSZT) == 0)
            {
                if (!AH5_read_phm_surface_instance_zszt(file_id, path, surface_instance))
                    rdata = FALSE;
            }
            else if (strcmp(temp, AH5_V_ZSZT2) == 0)
            {
                if (!AH5_read_phm_surface_instance_zszt2(file_id, path, surface_instance))
                    rdata = FALSE;
            }
            else
            {
                AH5_print_wrn_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_TYPE);
                rdata = FALSE;
            }
            free(temp);
        }
        else
        {
            surface_instance->type = S_INVALID;
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_TYPE);
            rdata = FALSE;
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_PHYSICAL_MODEL, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read instance in physicalModel/interface
char AH5_read_phm_interface_instance (hid_t file_id, const char *path, AH5_interface_instance_t *interface_instance)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_MEDIUM1, AH5_A_MEDIUM2};
    char rdata = TRUE;

    interface_instance->path = strdup(path);
    interface_instance->opt_attrs.instances = NULL;
    interface_instance->medium1 = NULL;
    interface_instance->medium2 = NULL;

    if (AH5_path_valid(file_id, path))
    {
        if (!AH5_read_str_attr(file_id, path, AH5_A_MEDIUM1, &(interface_instance->medium1)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_MEDIUM1);
            rdata = FALSE;
        }
        if (!AH5_read_str_attr(file_id, path, AH5_A_MEDIUM2, &(interface_instance->medium2)))
        {
            AH5_print_err_attr(AH5_C_PHYSICAL_MODEL, path, AH5_A_MEDIUM2);
            rdata = FALSE;
        }
        AH5_read_opt_attrs(file_id, path, &(interface_instance->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
    {
        AH5_print_err_path(AH5_C_PHYSICAL_MODEL, path);
        rdata = FALSE;
    }
    return rdata;
}


// Read physicalModel category
char AH5_read_physicalmodel (hid_t file_id, AH5_physicalmodel_t *physicalmodel)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH], rdata = TRUE;
    AH5_children_t children;
    hsize_t i;

    physicalmodel->volume_instances = NULL;
    physicalmodel->surface_instances = NULL;
    physicalmodel->interface_instances = NULL;

    if (H5Lexists(file_id, AH5_C_PHYSICAL_MODEL, H5P_DEFAULT) == TRUE)
    {
        strcpy(path, AH5_C_PHYSICAL_MODEL);
        strcat(path, AH5_G_VOLUME);
        children = AH5_read_children_name(file_id, path);
        physicalmodel->nb_volume_instances = children.nb_children;
        if (children.nb_children > 0)
        {
            physicalmodel->volume_instances = (AH5_volume_instance_t *) malloc(children.nb_children * sizeof(AH5_volume_instance_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path, AH5_C_PHYSICAL_MODEL);
                strcat(path, AH5_G_VOLUME);
                strcat(path, children.childnames[i]);
                if (!AH5_read_phm_volume_instance(file_id, path, physicalmodel->volume_instances + i))
                    rdata = FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }

        strcpy(path, AH5_C_PHYSICAL_MODEL);
        strcat(path, AH5_G_SURFACE);
        children = AH5_read_children_name(file_id, path);
        physicalmodel->nb_surface_instances = children.nb_children;
        if (children.nb_children > 0)
        {
            physicalmodel->surface_instances = (AH5_surface_instance_t *) malloc(children.nb_children * sizeof(AH5_surface_instance_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path, AH5_C_PHYSICAL_MODEL);
                strcat(path, AH5_G_SURFACE);
                strcat(path, children.childnames[i]);
                if (!AH5_read_phm_surface_instance(file_id, path, physicalmodel->surface_instances + i))
                    rdata = FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }

        strcpy(path, AH5_C_PHYSICAL_MODEL);
        strcat(path, AH5_G_INTERFACE);
        children = AH5_read_children_name(file_id, path);
        physicalmodel->nb_interface_instances = children.nb_children;
        if (children.nb_children > 0)
        {
            physicalmodel->interface_instances = (AH5_interface_instance_t *) malloc(children.nb_children * sizeof(AH5_interface_instance_t));
            for (i = 0; i < children.nb_children; i++)
            {
                strcpy(path, AH5_C_PHYSICAL_MODEL);
                strcat(path, AH5_G_INTERFACE);
                strcat(path, children.childnames[i]);
                if (!AH5_read_phm_interface_instance(file_id, path, physicalmodel->interface_instances + i))
                    rdata = FALSE;
                free(children.childnames[i]);
            }
            free(children.childnames);
        }
    }
    else
    {
        AH5_print_err_path(AH5_C_PHYSICAL_MODEL, AH5_C_PHYSICAL_MODEL);
        rdata = FALSE;
    }
    return rdata;
}




// Print instance in physicalModel/volume
void AH5_print_phm_volume_instance (const AH5_volume_instance_t *volume_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", AH5_get_name_from_path(volume_instance->path));
    AH5_print_opt_attrs(&(volume_instance->opt_attrs), space + 3);
    AH5_print_floatingtype(&(volume_instance->relative_permittivity), space + 3);
    AH5_print_floatingtype(&(volume_instance->relative_permeability), space + 3);
    AH5_print_floatingtype(&(volume_instance->electric_conductivity), space + 3);
    AH5_print_floatingtype(&(volume_instance->magnetic_conductivity), space + 3);
    printf("\n");
}


// Print instance in physicalModel/surface
void AH5_print_phm_surface_instance (const AH5_surface_instance_t *surface_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", AH5_get_name_from_path(surface_instance->path));
    AH5_print_opt_attrs(&(surface_instance->opt_attrs), space + 3);
    switch (surface_instance->type)
    {
    case S_THIN_DIELECTRIAH5_C_LAYER:
        AH5_print_str_attr(AH5_A_PHYSICAL_MODEL, surface_instance->physicalmodel, space + 3);
        AH5_print_flt_attr(AH5_A_THICKNESS, surface_instance->thickness, space + 3);
        break;
    case S_SIBC:
        AH5_print_str_attr(AH5_A_PHYSICAL_MODEL, surface_instance->physicalmodel, space + 3);
        AH5_print_flt_attr(AH5_A_THICKNESS, surface_instance->thickness, space + 3);
        break;
    case S_ZS:
        AH5_print_str_attr(AH5_A_ZS, surface_instance->zs, space + 3);
        break;
    case S_ZSZT:
        AH5_print_str_attr(AH5_A_ZS, surface_instance->zs, space + 3);
        AH5_print_str_attr(AH5_A_ZT, surface_instance->zt, space + 3);
        break;
    case S_ZSZT2:
        AH5_print_str_attr(AH5_A_ZS1, surface_instance->zs1, space + 3);
        AH5_print_str_attr(AH5_A_ZT1, surface_instance->zt1, space + 3);
        AH5_print_str_attr(AH5_A_ZS2, surface_instance->zs2, space + 3);
        AH5_print_str_attr(AH5_A_ZT2, surface_instance->zt2, space + 3);
        break;
    default:
        break;
    }
    printf("\n");
}


// Print instance in physicalModel/interface
void AH5_print_phm_interface_instance (const AH5_interface_instance_t *interface_instance, int space)
{
    printf("%*sInstance: %s\n", space, "", AH5_get_name_from_path(interface_instance->path));
    AH5_print_opt_attrs(&(interface_instance->opt_attrs), space + 3);
    if (interface_instance->medium1 != NULL)
        AH5_print_str_attr(AH5_A_MEDIUM1, interface_instance->medium1, space + 3);
    if (interface_instance->medium2 != NULL)
        AH5_print_str_attr(AH5_A_MEDIUM2, interface_instance->medium2, space + 3);
    printf("\n");
}


// Print physicalModel category
void AH5_print_physicalmodel (const AH5_physicalmodel_t *physicalmodel)
{
    hsize_t i;

    if (physicalmodel->nb_volume_instances > 0 || physicalmodel->nb_surface_instances > 0 || physicalmodel->nb_interface_instances > 0)
    {
        printf("##############################  Physical model  ##############################\n\n");
        if (physicalmodel->nb_volume_instances > 0)
        {
            printf("Volume:\n");
            for (i = 0; i < physicalmodel->nb_volume_instances; i++)
                AH5_print_phm_volume_instance(&(physicalmodel->volume_instances[i]), 3);
        }
        if (physicalmodel->nb_surface_instances > 0)
        {
            printf("Surface:\n");
            for (i = 0; i < physicalmodel->nb_surface_instances; i++)
                AH5_print_phm_surface_instance(&(physicalmodel->surface_instances[i]), 3);
        }
        if (physicalmodel->nb_interface_instances > 0)
        {
            printf("Interface:\n");
            for (i = 0; i < physicalmodel->nb_interface_instances; i++)
                AH5_print_phm_interface_instance(&(physicalmodel->interface_instances[i]), 3);
        }
        printf("\n");
    }
}




// Free memory used by instance in physicalModel/volume
void AH5_free_phm_volume_instance (AH5_volume_instance_t *volume_instance)
{
    if (volume_instance->path != NULL)
    {
        free(volume_instance->path);
        volume_instance->path = NULL;
    }
    AH5_free_opt_attrs(&(volume_instance->opt_attrs));
    AH5_free_floatingtype(&(volume_instance->relative_permittivity));
    AH5_free_floatingtype(&(volume_instance->relative_permeability));
    AH5_free_floatingtype(&(volume_instance->electric_conductivity));
    AH5_free_floatingtype(&(volume_instance->magnetic_conductivity));
}

// Free memory used by instance in physicalModel/surface
void AH5_free_phm_surface_instance (AH5_surface_instance_t *surface_instance)
{
    if (surface_instance->path != NULL)
    {
        free(surface_instance->path);
        surface_instance->path = NULL;
    }
    AH5_free_opt_attrs(&(surface_instance->opt_attrs));
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
    surface_instance->type = S_INVALID;
}


// Free memory used by instance in physicalModel/interface
void AH5_free_phm_interface_instance (AH5_interface_instance_t *interface_instance)
{
    if (interface_instance->path != NULL)
    {
        free(interface_instance->path);
        interface_instance->path = NULL;
    }
    AH5_free_opt_attrs(&(interface_instance->opt_attrs));
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
void AH5_free_physicalmodel (AH5_physicalmodel_t *physicalmodel)
{
    hsize_t i;

    if (physicalmodel->volume_instances != NULL)
    {
        for (i = 0; i < physicalmodel->nb_volume_instances; i++)
            AH5_free_phm_volume_instance(physicalmodel->volume_instances + i);
        free(physicalmodel->volume_instances);
        physicalmodel->volume_instances = NULL;
        physicalmodel->nb_volume_instances = 0;
    }

    if (physicalmodel->surface_instances != NULL)
    {
        for (i = 0; i < physicalmodel->nb_surface_instances; i++)
            AH5_free_phm_surface_instance(physicalmodel->surface_instances + i);
        free(physicalmodel->surface_instances);
        physicalmodel->surface_instances = NULL;
        physicalmodel->nb_surface_instances = 0;
    }

    if (physicalmodel->interface_instances != NULL)
    {
        for (i = 0; i < physicalmodel->nb_interface_instances; i++)
            AH5_free_phm_interface_instance(physicalmodel->interface_instances + i);
        free(physicalmodel->interface_instances);
        physicalmodel->interface_instances = NULL;
        physicalmodel->nb_interface_instances = 0;
    }
}
