#include "ah5_emsource.h"


// Read instance in /electromagneticSource/planeWave
void AH5_read_els_planewave (hid_t file_id, const char *path, AH5_planewave_t *planewave)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_XO, AH5_A_YO, AH5_A_ZO, AH5_A_THETA, AH5_A_PHI};
    char path2[AH5_ABSOLUTE_PATH_LENGTH];

    planewave->path = strdup(path);
    AH5_read_opt_attrs(file_id, path, &(planewave->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_XO, &(planewave->xo)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_XO);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_YO, &(planewave->yo)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_YO);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_ZO, &(planewave->zo)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_ZO);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_THETA, &(planewave->theta)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_THETA);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_PHI, &(planewave->phi)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_PHI);
    strcpy(path2, path);
    strcat(path2, AH5_G_MAGNITUDE);
    AH5_read_floatingtype(file_id, path2, &(planewave->magnitude));
}


// Read instance in /electromagneticSource/sphericalWave
void AH5_read_els_sphericalwave (hid_t file_id, const char *path, AH5_sphericalwave_t *sphericalwave)
{
    char path2[AH5_ABSOLUTE_PATH_LENGTH];

    sphericalwave->path = strdup(path);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_XO, &(sphericalwave->xo)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_XO);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_YO, &(sphericalwave->yo)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_YO);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_ZO, &(sphericalwave->zo)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_ZO);
    strcpy(path2, path);
    strcat(path2, AH5_G_MAGNITUDE);
    AH5_read_floatingtype(file_id, path2, &(sphericalwave->magnitude));
}


// Read instance in /electromagneticSource/generator
void AH5_read_els_generator (hid_t file_id, const char *path, AH5_generator_t *generator)
{
    char *type, path2[AH5_ABSOLUTE_PATH_LENGTH];
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_TYPE};

    generator->path = strdup(path);
    AH5_read_opt_attrs(file_id, path, &(generator->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    generator->type = GEN_INVALID;
    if (AH5_read_str_attr(file_id, path, AH5_A_TYPE, &type))
    {
        if (strcmp(type, AH5_V_VOLTAGE) == 0)
            generator->type = GEN_VOLTAGE;
        else if (strcmp(type, AH5_V_CURRENT) == 0)
            generator->type = GEN_CURRENT;
        else if (strcmp(type, AH5_V_POWER) == 0)
            generator->type = GEN_POWER;
        else if (strcmp(type, AH5_V_POWER_DENSITY) == 0)
            generator->type = GEN_POWER_DENSITY;
        free(type);
    }
    else
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_TYPE);
    strcpy(path2, path);
    strcat(path2, AH5_G_INNER_IMPEDANCE);
    AH5_read_floatingtype(file_id, path2, &(generator->inner_impedance));
    strcpy(path2, path);
    strcat(path2, AH5_G_MAGNITUDE);
    AH5_read_floatingtype(file_id, path2, &(generator->magnitude));
}


// Read instance in /electromagneticSource/dipole
void AH5_read_els_dipole (hid_t file_id, const char *path, AH5_dipole_t *dipole)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_TYPE, AH5_A_X, AH5_A_Y, AH5_A_Z, AH5_A_THETA, AH5_A_PHI, AH5_A_WIRE_RADIUS};
    char *type, path2[AH5_ABSOLUTE_PATH_LENGTH];

    dipole->path = strdup(path);
    AH5_read_opt_attrs(file_id, path, &(dipole->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    dipole->type = DIPOLE_INVALID;
    if (AH5_read_str_attr(file_id, path, AH5_A_TYPE, &type))
    {
        if (strcmp(type, AH5_V_ELECTRIC) == 0)
            dipole->type = DIPOLE_ELECTRIC;
        else if (strcmp(type, AH5_V_MAGNETIC) == 0)
            dipole->type = DIPOLE_MAGNETIC;
        free(type);
    }
    else
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_TYPE);

    if (!AH5_read_flt_attr(file_id, path, AH5_A_X, &(dipole->x)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_X);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_Y, &(dipole->y)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_Y);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_Z, &(dipole->z)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_Z);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_THETA, &(dipole->theta)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_THETA);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_PHI, &(dipole->phi)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_PHI);
    if (!AH5_read_flt_attr(file_id, path, AH5_A_WIRE_RADIUS, &(dipole->wire_radius)))
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_WIRE_RADIUS);
    strcpy(path2, path);
    strcat(path2, AH5_G_INNER_IMPEDANCE);
    AH5_read_floatingtype(file_id, path2, &(dipole->inner_impedance));
    strcpy(path2, path);
    strcat(path2, AH5_G_MAGNITUDE);
    AH5_read_floatingtype(file_id, path2, &(dipole->magnitude));
}


// Read instance in /electromagneticSource/antenna
void AH5_read_els_antenna (hid_t file_id, const char *path, AH5_antenna_t *antenna)
{
    char mandatory[][AH5_ATTR_LENGTH] = {}, model_man[][AH5_ATTR_LENGTH] = {AH5_A_TYPE};
    char path2[AH5_ABSOLUTE_PATH_LENGTH], *type;

    antenna->path = strdup(path);
    AH5_read_opt_attrs(file_id, path, &(antenna->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);

    strcpy(path2, path);
    strcat(path2, AH5_G_INPUT_IMPEDANCE);
    AH5_read_floatingtype(file_id, path2, &(antenna->input_impedance));

    strcpy(path2, path);
    strcat(path2, AH5_G_LOAD_IMPEDANCE);
    AH5_read_floatingtype(file_id, path2, &(antenna->load_impedance));

    strcpy(path2, path);
    strcat(path2, AH5_G_FEEDER_IMPEDANCE);
    AH5_read_floatingtype(file_id, path2, &(antenna->feeder_impedance));

    strcpy(path2, path);
    strcat(path2, AH5_G_MAGNITUDE);
    AH5_read_floatingtype(file_id, path2, &(antenna->magnitude));

    strcpy(path2, path);
    strcat(path2, AH5_G_MODEL);
    AH5_read_opt_attrs(file_id, path2, &(antenna->model.opt_attrs), model_man, sizeof(model_man)/AH5_ATTR_LENGTH);
    if (AH5_read_str_attr(file_id, path2, AH5_A_TYPE, &type))
    {
        if (strcmp(type, AH5_V_GAIN) == 0)
        {
            antenna->model.type = ANT_GAIN;
            strcat(path2, AH5_G_GAIN);
            AH5_read_floatingtype(file_id, path2, &(antenna->model.data.gain));
        }
        else if (strcmp(type, AH5_V_EFFECTIVE_AREA) == 0)
        {
            antenna->model.type = ANT_EFFECTIVE_AREA;
            strcat(path2, AH5_G_EFFECTIVE_AREA);
            AH5_read_floatingtype(file_id, path2, &(antenna->model.data.effarea));
        }
        else if (strcmp(type, AH5_V_FAR_FIELD) == 0)
        {
            antenna->model.type = ANT_FAR_FIELD;
            strcat(path2, AH5_G_FAR_FIELD);
            AH5_read_floatingtype(file_id, path2, &(antenna->model.data.farfield));
        }
        else if (strcmp(type, AH5_V_RECTANGULAR_HORN) == 0)
        {
            antenna->model.type = ANT_RECTANGULAR_HORN;
            strcat(path2, AH5_G_PARABOLIAH5_C_REFLECTOR);
            AH5_read_opt_attrs(file_id, path2, &(antenna->model.data.parreflct), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
        }
        else if (strcmp(type, AH5_V_CIRCULAR_HORN) == 0)
        {
            antenna->model.type = ANT_CIRCULAR_HORN;
            strcat(path2, AH5_G_PARABOLIAH5_C_REFLECTOR);
            AH5_read_opt_attrs(file_id, path2, &(antenna->model.data.parreflct), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
        }
        else if (strcmp(type, AH5_V_LOAH5_G_PERIODIC) == 0)
            antenna->model.type = ANT_LOAH5_G_PERIODIC;
        else if (strcmp(type, AH5_V_WHIP) == 0)
            antenna->model.type = ANT_WHIP;
        else if (strcmp(type, AH5_V_GENERIC) == 0)
            antenna->model.type = ANT_GENERIC;
        else if (strcmp(type, AH5_V_EXCHANGE_SURFACE) == 0)
            antenna->model.type = ANT_EXCHANGE_SURFACE;
        else
            antenna->model.type = ANT_INVALID;
        free(type);
    }
    else
        antenna->model.type = ANT_INVALID;
}


// Read instance in /electromagneticSource/sourceOnMesh
void AH5_read_els_sourceonmesh (hid_t file_id, const char *path, AH5_sourceonmesh_t *sourceonmesh)
{
    char *type;

    sourceonmesh->path = strdup(path);
    sourceonmesh->type = SCOM_INVALID;
    if (AH5_read_str_attr(file_id, path, AH5_A_TYPE, &type))
    {
        if (strcmp(type, AH5_V_ARRAYSET) == 0)
        {
            sourceonmesh->type = SCOM_ARRAYSET;
            AH5_read_ft_arrayset(file_id, path, &(sourceonmesh->data.arrayset));
        }
        else if (strcmp(type, AH5_V_EXCHANGE_SURFACE) == 0)
        {
            sourceonmesh->type = SCOM_EXCHANGE_SURFACE;
            AH5_read_str_attr(file_id, path, AH5_A_EXCHANGE_SURFACE, &(sourceonmesh->data.exchange_surface));
        }
        free(type);
    }
    else
        AH5_print_err_attr(AH5_C_ELECTROMAGNETIC_SOURCE, path, AH5_A_TYPE);
}


// Read electromagneticSource category
void AH5_read_electromagnetic_source (hid_t file_id, AH5_em_source_t *em_source)
{
    char path[AH5_ABSOLUTE_PATH_LENGTH], path2[AH5_ABSOLUTE_PATH_LENGTH];
    AH5_children_t children, children2;
    hsize_t i, j;

    em_source->nb_pw_sources = 0;
    em_source->nb_sw_sources = 0;
    em_source->nb_ge_sources = 0;
    em_source->nb_di_sources = 0;
    em_source->nb_an_sources = 0;
    em_source->nb_sm_sources = 0;
    em_source->pw_sources = NULL;
    em_source->sw_sources = NULL;
    em_source->ge_sources = NULL;
    em_source->di_sources = NULL;
    em_source->an_sources = NULL;
    em_source->sm_sources = NULL;

    children = AH5_read_children_name(file_id, AH5_C_ELECTROMAGNETIC_SOURCE);
    if (children.nb_children > 0)
    {
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, AH5_C_ELECTROMAGNETIC_SOURCE);
            strcat(path, children.childnames[i]);
            children2 = AH5_read_children_name(file_id, path);
            if (strcmp(children.childnames[i], AH5_G_PLANE_WAVE) == 0)
            {
                em_source->nb_pw_sources = children2.nb_children;
                em_source->pw_sources = (AH5_planewave_t *) malloc(children2.nb_children * sizeof(AH5_planewave_t));
            }
            else if (strcmp(children.childnames[i], AH5_G_SPHERICAL_WAVE) == 0)
            {
                em_source->nb_sw_sources = children2.nb_children;
                em_source->sw_sources = (AH5_sphericalwave_t *) malloc(children2.nb_children * sizeof(AH5_sphericalwave_t));
            }
            else if (strcmp(children.childnames[i], AH5_G_GENERATOR) == 0)
            {
                em_source->nb_ge_sources = children2.nb_children;
                em_source->ge_sources = (AH5_generator_t *) malloc(children2.nb_children * sizeof(AH5_generator_t));
            }
            else if (strcmp(children.childnames[i], AH5_G_DIPOLE) == 0)
            {
                em_source->nb_di_sources = children2.nb_children;
                em_source->di_sources = (AH5_dipole_t *) malloc(children2.nb_children * sizeof(AH5_dipole_t));
            }
            else if (strcmp(children.childnames[i], AH5_G_ANTENNA) == 0)
            {
                em_source->nb_an_sources = children2.nb_children;
                em_source->an_sources = (AH5_antenna_t *) malloc(children2.nb_children * sizeof(AH5_antenna_t));
            }
            else if (strcmp(children.childnames[i], AH5_G_SOURCE_ON_MESH) == 0)
            {
                em_source->nb_sm_sources = children2.nb_children;
                em_source->sm_sources = (AH5_sourceonmesh_t *) malloc(children2.nb_children * sizeof(AH5_sourceonmesh_t));
            }
            if (children2.nb_children > 0)
            {
                for (j = 0; j < children2.nb_children; j++)
                {
                    strcpy(path2, path);
                    strcat(path2, children2.childnames[j]);
                    if (strcmp(children.childnames[i], AH5_G_PLANE_WAVE) == 0)
                        AH5_read_els_planewave(file_id, path2, em_source->pw_sources +j);
                    else if (strcmp(children.childnames[i], AH5_G_SPHERICAL_WAVE) == 0)
                        AH5_read_els_sphericalwave(file_id, path2, em_source->sw_sources +j);
                    else if (strcmp(children.childnames[i], AH5_G_GENERATOR) == 0)
                        AH5_read_els_generator(file_id, path2, em_source->ge_sources +j);
                    else if (strcmp(children.childnames[i], AH5_G_DIPOLE) == 0)
                        AH5_read_els_dipole(file_id, path2, em_source->di_sources +j);
                    else if (strcmp(children.childnames[i], AH5_G_ANTENNA) == 0)
                        AH5_read_els_antenna(file_id, path2, em_source->an_sources +j);
                    else if (strcmp(children.childnames[i], AH5_G_SOURCE_ON_MESH) == 0)
                        AH5_read_els_sourceonmesh(file_id, path2, em_source->sm_sources +j);
                    free(children2.childnames[j]);
                }
                free(children2.childnames);
            }
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
}




// Print instance in /electromagneticSource/planeWave
void AH5_print_els_planewave (const AH5_planewave_t *planewave, int space)
{
    printf("%*sName: %s\n", space, "", AH5_get_name_from_path(planewave->path));
    AH5_print_opt_attrs(&(planewave->opt_attrs), space + 4);
    AH5_print_flt_attr(AH5_A_XO, planewave->xo, space + 4);
    AH5_print_flt_attr(AH5_A_YO, planewave->yo, space + 4);
    AH5_print_flt_attr(AH5_A_ZO, planewave->zo, space + 4);
    AH5_print_flt_attr(AH5_A_THETA, planewave->theta, space + 4);
    AH5_print_flt_attr(AH5_A_PHI, planewave->phi, space + 4);
    AH5_print_floatingtype(&(planewave->magnitude), space + 2);
    printf("\n");
}


// Print instance in /electromagneticSource/sphericalWave
void AH5_print_els_sphericalwave (const AH5_sphericalwave_t *sphericalwave, int space)
{
    printf("%*sName: %s\n", space, "", AH5_get_name_from_path(sphericalwave->path));
    AH5_print_flt_attr(AH5_A_XO, sphericalwave->xo, space + 4);
    AH5_print_flt_attr(AH5_A_YO, sphericalwave->yo, space + 4);
    AH5_print_flt_attr(AH5_A_ZO, sphericalwave->zo, space + 4);
    AH5_print_floatingtype(&(sphericalwave->magnitude), space + 2);
    printf("\n");
}


// Print instance in /electromagneticSource/generator
void AH5_print_els_generator (const AH5_generator_t *generator, int space)
{
    printf("%*sName: %s\n", space, "", AH5_get_name_from_path(generator->path));
    AH5_print_opt_attrs(&(generator->opt_attrs), space + 4);
    switch (generator->type)
    {
    case GEN_VOLTAGE:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_VOLTAGE, space + 4);
        break;
    case GEN_CURRENT:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_CURRENT, space + 4);
        break;
    case GEN_POWER:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_POWER, space + 4);
        break;
    case GEN_POWER_DENSITY:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_POWER_DENSITY, space + 4);
        break;
    case GEN_INVALID:
        AH5_print_str_attr(AH5_A_TYPE, "INVALID", space + 4);
        break;
    }
    AH5_print_floatingtype(&(generator->inner_impedance), space + 2);
    AH5_print_floatingtype(&(generator->magnitude), space + 2);
    printf("\n");
}


// Print instance in /electromagneticSource/dipole
void AH5_print_els_dipole (const AH5_dipole_t *dipole, int space)
{
    printf("%*sName: %s\n", space, "", AH5_get_name_from_path(dipole->path));
    switch (dipole->type)
    {
    case DIPOLE_ELECTRIC:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_ELECTRIC, space + 4);
        break;
    case DIPOLE_MAGNETIC:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_MAGNETIC, space + 4);
        break;
    case DIPOLE_INVALID:
        AH5_print_str_attr(AH5_A_TYPE, "INVALID", space + 4);
        break;
    }
    AH5_print_opt_attrs(&(dipole->opt_attrs), space + 4);
    AH5_print_flt_attr(AH5_A_X, dipole->x, space + 4);
    AH5_print_flt_attr(AH5_A_Y, dipole->y, space + 4);
    AH5_print_flt_attr(AH5_A_Z, dipole->z, space + 4);
    AH5_print_flt_attr(AH5_A_THETA, dipole->theta, space + 4);
    AH5_print_flt_attr(AH5_A_PHI, dipole->phi, space + 4);
    AH5_print_flt_attr(AH5_A_WIRE_RADIUS, dipole->wire_radius, space + 4);
    printf("\n");
}


// Print instance in /electromagneticSource/antenna
void AH5_print_els_antenna (const AH5_antenna_t *antenna, int space)
{
    printf("%*sName: %s\n", space, "", AH5_get_name_from_path(antenna->path));
    AH5_print_opt_attrs(&(antenna->opt_attrs), space + 5);
    AH5_print_floatingtype(&(antenna->input_impedance), space + 2);
    AH5_print_floatingtype(&(antenna->load_impedance), space + 2);
    AH5_print_floatingtype(&(antenna->feeder_impedance), space + 2);
    AH5_print_floatingtype(&(antenna->magnitude), space + 2);
    printf("%*s-model:\n", space + 2, "");
    switch (antenna->model.type)
    {
    case ANT_GAIN:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_GAIN, space + 6);
        AH5_print_opt_attrs(&(antenna->model.opt_attrs), space + 6);
        AH5_print_floatingtype(&(antenna->model.data.gain), space + 4);
        break;
    case ANT_EFFECTIVE_AREA:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_EFFECTIVE_AREA, space + 6);
        AH5_print_opt_attrs(&(antenna->model.opt_attrs), space + 6);
        AH5_print_floatingtype(&(antenna->model.data.effarea), space + 4);
        break;
    case ANT_FAR_FIELD:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_FAR_FIELD, space + 6);
        AH5_print_opt_attrs(&(antenna->model.opt_attrs), space + 6);
        AH5_print_floatingtype(&(antenna->model.data.farfield), space + 4);
        break;
    case ANT_RECTANGULAR_HORN:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_RECTANGULAR_HORN, space + 6);
        AH5_print_opt_attrs(&(antenna->model.opt_attrs), space + 6);
        if (antenna->model.data.parreflct.nb_instances > 0)
        {
            printf("%*s-parabolicReflector:\n", space + 4, "");
            AH5_print_opt_attrs(&(antenna->model.data.parreflct), space + 7);
        }
        break;
    case ANT_CIRCULAR_HORN:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_CIRCULAR_HORN, space + 6);
        AH5_print_opt_attrs(&(antenna->model.opt_attrs), space + 6);
        if (antenna->model.data.parreflct.nb_instances > 0)
        {
            printf("%*s-parabolicReflector:\n", space + 4, "");
            AH5_print_opt_attrs(&(antenna->model.data.parreflct), space + 7);
        }
        break;
    case ANT_LOAH5_G_PERIODIC:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_LOAH5_G_PERIODIC, space + 5);
        AH5_print_opt_attrs(&(antenna->model.opt_attrs), space + 5);
        break;
    case ANT_WHIP:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_WHIP, space + 5);
        AH5_print_opt_attrs(&(antenna->model.opt_attrs), space + 5);
        break;
    case ANT_GENERIC:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_GENERIC, space + 5);
        AH5_print_opt_attrs(&(antenna->model.opt_attrs), space + 5);
        break;
    case ANT_EXCHANGE_SURFACE:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_EXCHANGE_SURFACE, space + 5);
        AH5_print_opt_attrs(&(antenna->model.opt_attrs), space + 5);
    break;
    default:
        AH5_print_str_attr(AH5_A_TYPE, "INVALID", space + 5);
        break;
    }
    printf("\n");
}


// Print instance in /electromagneticSource/sourceOnMesh
void AH5_print_els_sourceonmesh (const AH5_sourceonmesh_t *sourceonmesh, int space)
{
    hsize_t i;

    printf("%*sName: %s\n", space, "", AH5_get_name_from_path(sourceonmesh->path));
    switch (sourceonmesh->type)
    {
    case SCOM_ARRAYSET:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_ARRAYSET, space + 4);
        AH5_print_ft_dataset(&(sourceonmesh->data.arrayset.data), space + 2);
        for (i = 0; i < sourceonmesh->data.arrayset.nb_dims; i++)
            AH5_print_ft_vector(&(sourceonmesh->data.arrayset.dims[i]), space + 2);
        break;
    case SCOM_EXCHANGE_SURFACE:
        AH5_print_str_attr(AH5_A_TYPE, AH5_V_EXCHANGE_SURFACE, space + 3);
        AH5_print_str_attr(AH5_A_EXCHANGE_SURFACE, sourceonmesh->data.exchange_surface, space + 3);
        break;
    case SCOM_INVALID:
        AH5_print_str_attr(AH5_A_TYPE, "INVALID", space + 3);
        break;
    }
    printf("\n");
}

// Print electromagneticSource category
void AH5_print_electromagnetic_source (const AH5_em_source_t *em_source)
{
    hsize_t i;

    printf("##########################  Electromagnetic source  ##########################\n\n");
    if (em_source->nb_pw_sources)
        printf("Plane wave:\n");
    for (i = 0; i < em_source->nb_pw_sources; i++)
        AH5_print_els_planewave(&(em_source->pw_sources[i]), 3);

    if (em_source->nb_sw_sources)
        printf("Spherical wave:\n");
    for (i = 0; i < em_source->nb_sw_sources; i++)
        AH5_print_els_sphericalwave(&(em_source->sw_sources[i]), 3);

    if (em_source->nb_ge_sources)
        printf("Generator:\n");
    for (i = 0; i < em_source->nb_ge_sources; i++)
        AH5_print_els_generator(&(em_source->ge_sources[i]), 3);

    if (em_source->nb_di_sources)
        printf("Dipole:\n");
    for (i = 0; i < em_source->nb_di_sources; i++)
        AH5_print_els_dipole(&(em_source->di_sources[i]), 3);

    if (em_source->nb_an_sources)
        printf("Antenna:\n");
    for (i = 0; i < em_source->nb_an_sources; i++)
        AH5_print_els_antenna(&(em_source->an_sources[i]), 3);

    if (em_source->nb_sm_sources)
        printf("Source on mesh:\n");
    for (i = 0; i < em_source->nb_sm_sources; i++)
        AH5_print_els_sourceonmesh(&(em_source->sm_sources[i]), 3);
    printf("\n");
}




// Free memory used by planeWave instance
void AH5_free_els_planewave (AH5_planewave_t *planewave)
{
    if (planewave->path != NULL)
    {
        free(planewave->path);
        planewave->path = NULL;
    }
    AH5_free_opt_attrs(&(planewave->opt_attrs));
    AH5_free_floatingtype(&(planewave->magnitude));
}


// Free memory used by sphericalWave instance
void AH5_free_els_sphericalwave (AH5_sphericalwave_t *sphericalwave)
{
    if (sphericalwave->path != NULL)
    {
        free(sphericalwave->path);
        sphericalwave->path = NULL;
    }
    AH5_free_floatingtype(&(sphericalwave->magnitude));
}


// Free memory used by planeWave instance
void AH5_free_els_generator (AH5_generator_t *generator)
{
    if (generator->path != NULL)
    {
        free(generator->path);
        generator->path = NULL;
    }
    AH5_free_opt_attrs(&(generator->opt_attrs));
    AH5_free_floatingtype(&(generator->inner_impedance));
    AH5_free_floatingtype(&(generator->magnitude));
}


// Free memory used by dipole instance
void AH5_free_els_dipole (AH5_dipole_t *dipole)
{
    if (dipole->path != NULL)
    {
        free(dipole->path);
        dipole->path = NULL;
    }
    AH5_free_opt_attrs(&(dipole->opt_attrs));
    AH5_free_floatingtype(&(dipole->inner_impedance));
    AH5_free_floatingtype(&(dipole->magnitude));
}


// Free memory used by antenna instance
void AH5_free_els_antenna (AH5_antenna_t *antenna)
{
    if (antenna->path != NULL)
    {
        free(antenna->path);
        antenna->path = NULL;
    }
    AH5_free_opt_attrs(&(antenna->opt_attrs));
    AH5_free_floatingtype(&(antenna->input_impedance));
    AH5_free_floatingtype(&(antenna->load_impedance));
    AH5_free_floatingtype(&(antenna->feeder_impedance));
    AH5_free_floatingtype(&(antenna->magnitude));
    AH5_free_opt_attrs(&(antenna->model.opt_attrs));
    switch (antenna->model.type)
    {
    case ANT_GAIN:
        AH5_free_floatingtype(&(antenna->model.data.gain));
        break;
    case ANT_EFFECTIVE_AREA:
        AH5_free_floatingtype(&(antenna->model.data.effarea));
        break;
    case ANT_FAR_FIELD:
        AH5_free_floatingtype(&(antenna->model.data.farfield));
        break;
    case ANT_RECTANGULAR_HORN:
        AH5_free_opt_attrs(&(antenna->model.data.parreflct));
        break;
    case ANT_CIRCULAR_HORN:
        AH5_free_opt_attrs(&(antenna->model.data.parreflct));
        break;
    default:
        break;
    }
}


// Free memory used by sourceOnMesh instance
void AH5_free_els_sourceonmesh (AH5_sourceonmesh_t *sourceonmesh)
{
    if (sourceonmesh->path != NULL)
    {
        free(sourceonmesh->path);
        sourceonmesh->path = NULL;
    }
    switch (sourceonmesh->type)
    {
    case SCOM_ARRAYSET:
        AH5_free_ft_arrayset(&(sourceonmesh->data.arrayset));
        break;
    case SCOM_EXCHANGE_SURFACE:
        free(sourceonmesh->data.exchange_surface);
        break;
    default:
        break;
    }
}


// Free memory used by whole electromagneticSource category
void AH5_free_electromagnetic_source (AH5_em_source_t *em_source)
{
    hsize_t i;

    if (em_source->nb_pw_sources > 0)
    {
        for (i = 0; i < em_source->nb_pw_sources; i++)
            AH5_free_els_planewave(em_source->pw_sources + i);
        free(em_source->pw_sources);
    }
    if (em_source->nb_sw_sources > 0)
    {
        for (i = 0; i < em_source->nb_sw_sources; i++)
            AH5_free_els_sphericalwave(em_source->sw_sources + i);
        free(em_source->sw_sources);
    }
    if (em_source->nb_ge_sources > 0)
    {
        for (i = 0; i < em_source->nb_ge_sources; i++)
            AH5_free_els_generator(em_source->ge_sources + i);
        free(em_source->ge_sources);
    }
    if (em_source->nb_di_sources > 0)
    {
        for (i = 0; i < em_source->nb_di_sources; i++)
            AH5_free_els_dipole(em_source->di_sources + i);
        free(em_source->di_sources);
    }
    if (em_source->nb_an_sources > 0)
    {
        for (i = 0; i < em_source->nb_an_sources; i++)
            AH5_free_els_antenna(em_source->an_sources + i);
        free(em_source->an_sources);
    }
    if (em_source->nb_sm_sources > 0)
    {
        for (i = 0; i < em_source->nb_sm_sources; i++)
            AH5_free_els_sourceonmesh(em_source->sm_sources + i);
        free(em_source->sm_sources);
    }
}

