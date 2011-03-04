#include "emsource.h"


// Read instance in /electromagneticSource/planeWave
void read_els_planewave (hid_t file_id, const char *path, planewave_t *planewave)
{
    char mandatory[][ATTR_LENGTH] = {A_XO, A_YO, A_ZO, A_THETA, A_PHI};
    char path2[ABSOLUTE_PATH_NAME_LENGTH];

    planewave->path = strdup(path);
    read_opt_attrs(file_id, path, &(planewave->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
    if (!read_flt_attr(file_id, path, A_XO, &(planewave->xo)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_XO);
    if (!read_flt_attr(file_id, path, A_YO, &(planewave->yo)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_YO);
    if (!read_flt_attr(file_id, path, A_ZO, &(planewave->zo)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_ZO);
    if (!read_flt_attr(file_id, path, A_THETA, &(planewave->theta)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_THETA);
    if (!read_flt_attr(file_id, path, A_PHI, &(planewave->phi)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_PHI);
    strcpy(path2, path);
    strcat(path2, G_MAGNITUDE);
    read_floatingtype(file_id, path2, &(planewave->magnitude));
}


// Read instance in /electromagneticSource/sphericalWave
void read_els_sphericalwave (hid_t file_id, const char *path, sphericalwave_t *sphericalwave)
{
    char path2[ABSOLUTE_PATH_NAME_LENGTH];

    sphericalwave->path = strdup(path);
    if (!read_flt_attr(file_id, path, A_XO, &(sphericalwave->xo)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_XO);
    if (!read_flt_attr(file_id, path, A_YO, &(sphericalwave->yo)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_YO);
    if (!read_flt_attr(file_id, path, A_ZO, &(sphericalwave->zo)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_ZO);
    strcpy(path2, path);
    strcat(path2, G_MAGNITUDE);
    read_floatingtype(file_id, path2, &(sphericalwave->magnitude));
}


// Read instance in /electromagneticSource/generator
void read_els_generator (hid_t file_id, const char *path, generator_t *generator)
{
    char *type, path2[ABSOLUTE_PATH_NAME_LENGTH];
    char mandatory[][ATTR_LENGTH] = {A_TYPE};

    generator->path = strdup(path);
    read_opt_attrs(file_id, path, &(generator->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
    generator->type = GEN_INVALID;
    if (read_str_attr(file_id, path, A_TYPE, &type))
    {
        if (strcmp(type, V_VOLTAGE) == 0)
            generator->type = GEN_VOLTAGE;
        else if (strcmp(type, V_CURRENT) == 0)
            generator->type = GEN_CURRENT;
        else if (strcmp(type, V_POWER) == 0)
            generator->type = GEN_POWER;
        else if (strcmp(type, V_POWER_DENSITY) == 0)
            generator->type = GEN_POWER_DENSITY;
        free(type);
    }
    else
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_TYPE);
    strcpy(path2, path);
    strcat(path2, G_INNER_IMPEDANCE);
    read_floatingtype(file_id, path2, &(generator->inner_impedance));
    strcpy(path2, path);
    strcat(path2, G_MAGNITUDE);
    read_floatingtype(file_id, path2, &(generator->magnitude));
}


// Read instance in /electromagneticSource/dipole
void read_els_dipole (hid_t file_id, const char *path, dipole_t *dipole)
{
    char mandatory[][ATTR_LENGTH] = {A_TYPE, A_X, A_Y, A_Z, A_THETA, A_PHI, A_WIRE_RADIUS};
    char *type, path2[ABSOLUTE_PATH_NAME_LENGTH];

    dipole->path = strdup(path);
    read_opt_attrs(file_id, path, &(dipole->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);
    dipole->type = DIPOLE_INVALID;
    if (read_str_attr(file_id, path, A_TYPE, &type))
    {
        if (strcmp(type, V_ELECTRIC) == 0)
            dipole->type = DIPOLE_ELECTRIC;
        else if (strcmp(type, V_MAGNETIC) == 0)
            dipole->type = DIPOLE_MAGNETIC;
        free(type);
    }
    else
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_TYPE);

    if (!read_flt_attr(file_id, path, A_X, &(dipole->x)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_X);
    if (!read_flt_attr(file_id, path, A_Y, &(dipole->y)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_Y);
    if (!read_flt_attr(file_id, path, A_Z, &(dipole->z)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_Z);
    if (!read_flt_attr(file_id, path, A_THETA, &(dipole->theta)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_THETA);
    if (!read_flt_attr(file_id, path, A_PHI, &(dipole->phi)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_PHI);
    if (!read_flt_attr(file_id, path, A_WIRE_RADIUS, &(dipole->wire_radius)))
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_WIRE_RADIUS);
    strcpy(path2, path);
    strcat(path2, G_INNER_IMPEDANCE);
    read_floatingtype(file_id, path2, &(dipole->inner_impedance));
    strcpy(path2, path);
    strcat(path2, G_MAGNITUDE);
    read_floatingtype(file_id, path2, &(dipole->magnitude));
}


// Read instance in /electromagneticSource/antenna
void read_els_antenna (hid_t file_id, const char *path, antenna_t *antenna)
{
    char mandatory[][ATTR_LENGTH] = {}, model_man[][ATTR_LENGTH] = {A_TYPE};
    char path2[ABSOLUTE_PATH_NAME_LENGTH], *type;

    antenna->path = strdup(path);
    read_opt_attrs(file_id, path, &(antenna->opt_attrs), mandatory, sizeof(mandatory)/ATTR_LENGTH);

    strcpy(path2, path);
    strcat(path2, G_INPUT_IMPEDANCE);
    read_floatingtype(file_id, path2, &(antenna->input_impedance));

    strcpy(path2, path);
    strcat(path2, G_LOAD_IMPEDANCE);
    read_floatingtype(file_id, path2, &(antenna->load_impedance));

    strcpy(path2, path);
    strcat(path2, G_FEEDER_IMPEDANCE);
    read_floatingtype(file_id, path2, &(antenna->feeder_impedance));

    strcpy(path2, path);
    strcat(path2, G_MAGNITUDE);
    read_floatingtype(file_id, path2, &(antenna->magnitude));

    strcpy(path2, path);
    strcat(path2, G_MODEL);
    read_opt_attrs(file_id, path2, &(antenna->model.opt_attrs), model_man, sizeof(model_man)/ATTR_LENGTH);
    if (read_str_attr(file_id, path2, A_TYPE, &type))
    {
        if (strcmp(type, V_GAIN) == 0)
        {
            antenna->model.type = ANT_GAIN;
            strcat(path2, G_GAIN);
            read_floatingtype(file_id, path2, &(antenna->model.data.gain));
        }
        else if (strcmp(type, V_EFFECTIVE_AREA) == 0)
        {
            antenna->model.type = ANT_EFFECTIVE_AREA;
            strcat(path2, G_EFFECTIVE_AREA);
            read_floatingtype(file_id, path2, &(antenna->model.data.effarea));
        }
        else if (strcmp(type, V_FAR_FIELD) == 0)
        {
            antenna->model.type = ANT_FAR_FIELD;
            strcat(path2, G_FAR_FIELD);
            read_floatingtype(file_id, path2, &(antenna->model.data.farfield));
        }
        else if (strcmp(type, V_RECTANGULAR_HORN) == 0)
        {
            antenna->model.type = ANT_RECTANGULAR_HORN;
            strcat(path2, G_PARABOLIC_REFLECTOR);
            read_opt_attrs(file_id, path2, &(antenna->model.data.parreflct), mandatory, sizeof(mandatory)/ATTR_LENGTH);
        }
        else if (strcmp(type, V_CIRCULAR_HORN) == 0)
        {
            antenna->model.type = ANT_CIRCULAR_HORN;
            strcat(path2, G_PARABOLIC_REFLECTOR);
            read_opt_attrs(file_id, path2, &(antenna->model.data.parreflct), mandatory, sizeof(mandatory)/ATTR_LENGTH);
        }
        else if (strcmp(type, V_LOG_PERIODIC) == 0)
            antenna->model.type = ANT_LOG_PERIODIC;
        else if (strcmp(type, V_WHIP) == 0)
            antenna->model.type = ANT_WHIP;
        else if (strcmp(type, V_GENERIC) == 0)
            antenna->model.type = ANT_GENERIC;
        else if (strcmp(type, V_EXCHANGE_SURFACE) == 0)
            antenna->model.type = ANT_EXCHANGE_SURFACE;
        else
            antenna->model.type = ANT_INVALID;
        free(type);
    }
    else
        antenna->model.type = ANT_INVALID;
}


// Read instance in /electromagneticSource/sourceOnMesh
void read_els_sourceonmesh (hid_t file_id, const char *path, sourceonmesh_t *sourceonmesh)
{
    char *type;

    sourceonmesh->path = strdup(path);
    sourceonmesh->type = SCOM_INVALID;
    if (read_str_attr(file_id, path, A_TYPE, &type))
    {
        if (strcmp(type, V_ARRAYSET) == 0)
        {
            sourceonmesh->type = SCOM_ARRAYSET;
            read_ft_arrayset(file_id, path, &(sourceonmesh->data.arrayset));
        }
        else if (strcmp(type, V_EXCHANGE_SURFACE) == 0)
        {
            sourceonmesh->type = SCOM_EXCHANGE_SURFACE;
            read_str_attr(file_id, path, A_EXCHANGE_SURFACE, &(sourceonmesh->data.exchange_surface));
        }
        free(type);
    }
    else
        print_err_attr(C_ELECTROMAGNETIC_SOURCE, path, A_TYPE);
}


// Read electromagneticSource category
void read_electromagnetic_source (hid_t file_id, em_source_t *em_source)
{
    char path[ABSOLUTE_PATH_NAME_LENGTH], path2[ABSOLUTE_PATH_NAME_LENGTH];
    children_t children, children2;
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

    children = read_children_name(file_id, C_ELECTROMAGNETIC_SOURCE);
    if (children.nb_children > 0)
    {
        for (i = 0; i < children.nb_children; i++)
        {
            strcpy(path, C_ELECTROMAGNETIC_SOURCE);
            strcat(path, children.childnames[i]);
            children2 = read_children_name(file_id, path);
            if (strcmp(children.childnames[i], G_PLANE_WAVE) == 0)
            {
                em_source->nb_pw_sources = children2.nb_children;
                em_source->pw_sources = (planewave_t *) malloc(children2.nb_children * sizeof(planewave_t));
            }
            else if (strcmp(children.childnames[i], G_SPHERICAL_WAVE) == 0)
            {
                em_source->nb_sw_sources = children2.nb_children;
                em_source->sw_sources = (sphericalwave_t *) malloc(children2.nb_children * sizeof(sphericalwave_t));
            }
            else if (strcmp(children.childnames[i], G_GENERATOR) == 0)
            {
                em_source->nb_ge_sources = children2.nb_children;
                em_source->ge_sources = (generator_t *) malloc(children2.nb_children * sizeof(generator_t));
            }
            else if (strcmp(children.childnames[i], G_DIPOLE) == 0)
            {
                em_source->nb_di_sources = children2.nb_children;
                em_source->di_sources = (dipole_t *) malloc(children2.nb_children * sizeof(dipole_t));
            }
            else if (strcmp(children.childnames[i], G_ANTENNA) == 0)
            {
                em_source->nb_an_sources = children2.nb_children;
                em_source->an_sources = (antenna_t *) malloc(children2.nb_children * sizeof(antenna_t));
            }
            else if (strcmp(children.childnames[i], G_SOURCE_ON_MESH) == 0)
            {
                em_source->nb_sm_sources = children2.nb_children;
                em_source->sm_sources = (sourceonmesh_t *) malloc(children2.nb_children * sizeof(sourceonmesh_t));
            }
            if (children2.nb_children > 0)
            {
                for (j = 0; j < children2.nb_children; j++)
                {
                    strcpy(path2, path);
                    strcat(path2, children2.childnames[j]);
                    if (strcmp(children.childnames[i], G_PLANE_WAVE) == 0)
                        read_els_planewave(file_id, path2, em_source->pw_sources +j);
                    else if (strcmp(children.childnames[i], G_SPHERICAL_WAVE) == 0)
                        read_els_sphericalwave(file_id, path2, em_source->sw_sources +j);
                    else if (strcmp(children.childnames[i], G_GENERATOR) == 0)
                        read_els_generator(file_id, path2, em_source->ge_sources +j);
                    else if (strcmp(children.childnames[i], G_DIPOLE) == 0)
                        read_els_dipole(file_id, path2, em_source->di_sources +j);
                    else if (strcmp(children.childnames[i], G_ANTENNA) == 0)
                        read_els_antenna(file_id, path2, em_source->an_sources +j);
                    else if (strcmp(children.childnames[i], G_SOURCE_ON_MESH) == 0)
                        read_els_sourceonmesh(file_id, path2, em_source->sm_sources +j);
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
void print_els_planewave (planewave_t planewave, int space)
{
    printf("%*sName: %s\n", space, "", get_name_from_path(planewave.path));
    print_opt_attrs(planewave.opt_attrs, space + 4);
    print_flt_attr(A_XO, planewave.xo, space + 4);
    print_flt_attr(A_YO, planewave.yo, space + 4);
    print_flt_attr(A_ZO, planewave.zo, space + 4);
    print_flt_attr(A_THETA, planewave.theta, space + 4);
    print_flt_attr(A_PHI, planewave.phi, space + 4);
    print_floatingtype(planewave.magnitude, space + 2);
    printf("\n");
}


// Print instance in /electromagneticSource/sphericalWave
void print_els_sphericalwave (sphericalwave_t sphericalwave, int space)
{
    printf("%*sName: %s\n", space, "", get_name_from_path(sphericalwave.path));
    print_flt_attr(A_XO, sphericalwave.xo, space + 4);
    print_flt_attr(A_YO, sphericalwave.yo, space + 4);
    print_flt_attr(A_ZO, sphericalwave.zo, space + 4);
    print_floatingtype(sphericalwave.magnitude, space + 2);
    printf("\n");
}


// Print instance in /electromagneticSource/generator
void print_els_generator (generator_t generator, int space)
{
    printf("%*sName: %s\n", space, "", get_name_from_path(generator.path));
    print_opt_attrs(generator.opt_attrs, space + 4);
    switch (generator.type)
    {
    case GEN_VOLTAGE:
        print_str_attr(A_TYPE, V_VOLTAGE, space + 4);
        break;
    case GEN_CURRENT:
        print_str_attr(A_TYPE, V_CURRENT, space + 4);
        break;
    case GEN_POWER:
        print_str_attr(A_TYPE, V_POWER, space + 4);
        break;
    case GEN_POWER_DENSITY:
        print_str_attr(A_TYPE, V_POWER_DENSITY, space + 4);
        break;
    case GEN_INVALID:
        print_str_attr(A_TYPE, "INVALID", space + 4);
        break;
    }
    print_floatingtype(generator.inner_impedance, space + 2);
    print_floatingtype(generator.magnitude, space + 2);
    printf("\n");
}


// Print instance in /electromagneticSource/dipole
void print_els_dipole (dipole_t dipole, int space)
{
    printf("%*sName: %s\n", space, "", get_name_from_path(dipole.path));
    switch (dipole.type)
    {
    case DIPOLE_ELECTRIC:
        print_str_attr(A_TYPE, V_ELECTRIC, space + 4);
        break;
    case DIPOLE_MAGNETIC:
        print_str_attr(A_TYPE, V_MAGNETIC, space + 4);
        break;
    case DIPOLE_INVALID:
        print_str_attr(A_TYPE, "INVALID", space + 4);
        break;
    }
    print_opt_attrs(dipole.opt_attrs, space + 4);
    print_flt_attr(A_X, dipole.x, space + 4);
    print_flt_attr(A_Y, dipole.y, space + 4);
    print_flt_attr(A_Z, dipole.z, space + 4);
    print_flt_attr(A_THETA, dipole.theta, space + 4);
    print_flt_attr(A_PHI, dipole.phi, space + 4);
    print_flt_attr(A_WIRE_RADIUS, dipole.wire_radius, space + 4);
    printf("\n");
}


// Print instance in /electromagneticSource/antenna
void print_els_antenna (antenna_t antenna, int space)
{
    printf("%*sName: %s\n", space, "", get_name_from_path(antenna.path));
    print_opt_attrs(antenna.opt_attrs, space + 5);
    print_floatingtype(antenna.input_impedance, space + 2);
    print_floatingtype(antenna.load_impedance, space + 2);
    print_floatingtype(antenna.feeder_impedance, space + 2);
    print_floatingtype(antenna.magnitude, space + 2);
    printf("%*s-model:\n", space + 2, "");
    switch (antenna.model.type)
    {
    case ANT_GAIN:
        print_str_attr(A_TYPE, V_GAIN, space + 6);
        print_opt_attrs(antenna.model.opt_attrs, space + 6);
        print_floatingtype(antenna.model.data.gain, space + 4);
        break;
    case ANT_EFFECTIVE_AREA:
        print_str_attr(A_TYPE, V_EFFECTIVE_AREA, space + 6);
        print_opt_attrs(antenna.model.opt_attrs, space + 6);
        print_floatingtype(antenna.model.data.effarea, space + 4);
        break;
    case ANT_FAR_FIELD:
        print_str_attr(A_TYPE, V_FAR_FIELD, space + 6);
        print_opt_attrs(antenna.model.opt_attrs, space + 6);
        print_floatingtype(antenna.model.data.farfield, space + 4);
        break;
    case ANT_RECTANGULAR_HORN:
        print_str_attr(A_TYPE, V_RECTANGULAR_HORN, space + 6);
        print_opt_attrs(antenna.model.opt_attrs, space + 6);
        if (antenna.model.data.parreflct.nb_instances > 0)
        {
            printf("%*s-parabolicReflector:\n", space + 4, "");
            print_opt_attrs(antenna.model.data.parreflct, space + 7);
        }
        break;
    case ANT_CIRCULAR_HORN:
        print_str_attr(A_TYPE, V_CIRCULAR_HORN, space + 6);
        print_opt_attrs(antenna.model.opt_attrs, space + 6);
        if (antenna.model.data.parreflct.nb_instances > 0)
        {
            printf("%*s-parabolicReflector:\n", space + 4, "");
            print_opt_attrs(antenna.model.data.parreflct, space + 7);
        }
        break;
    case ANT_LOG_PERIODIC:
        print_str_attr(A_TYPE, V_LOG_PERIODIC, space + 5);
        print_opt_attrs(antenna.model.opt_attrs, space + 5);
        break;
    case ANT_WHIP:
        print_str_attr(A_TYPE, V_WHIP, space + 5);
        print_opt_attrs(antenna.model.opt_attrs, space + 5);
        break;
    case ANT_GENERIC:
        print_str_attr(A_TYPE, V_GENERIC, space + 5);
        print_opt_attrs(antenna.model.opt_attrs, space + 5);
        break;
    case ANT_EXCHANGE_SURFACE:
        print_str_attr(A_TYPE, V_EXCHANGE_SURFACE, space + 5);
        print_opt_attrs(antenna.model.opt_attrs, space + 5);
    break;
    default:
        print_str_attr(A_TYPE, "INVALID", space + 5);
        break;
    }
    printf("\n");
}


// Print instance in /electromagneticSource/sourceOnMesh
void print_els_sourceonmesh (sourceonmesh_t sourceonmesh, int space)
{
    hsize_t i;

    printf("%*sName: %s\n", space, "", get_name_from_path(sourceonmesh.path));
    switch (sourceonmesh.type)
    {
    case SCOM_ARRAYSET:
        print_str_attr(A_TYPE, V_ARRAYSET, space + 4);
        print_ft_dataset(sourceonmesh.data.arrayset.data, space + 2);
        for (i = 0; i < sourceonmesh.data.arrayset.nb_dims; i++)
            print_ft_vector(sourceonmesh.data.arrayset.dims[i], space + 2);
        break;
    case SCOM_EXCHANGE_SURFACE:
        print_str_attr(A_TYPE, V_EXCHANGE_SURFACE, space + 3);
        print_str_attr(A_EXCHANGE_SURFACE, sourceonmesh.data.exchange_surface, space + 3);
        break;
    case SCOM_INVALID:
        print_str_attr(A_TYPE, "INVALID", space + 3);
        break;
    }
    printf("\n");
}

// Print electromagneticSource category
void print_electromagnetic_source (em_source_t em_source)
{
    hsize_t i;

    printf("##########################  Electromagnetic source  ##########################\n\n");
    if (em_source.nb_pw_sources)
        printf("Plane wave:\n");
    for (i = 0; i < em_source.nb_pw_sources; i++)
        print_els_planewave(em_source.pw_sources[i], 3);

    if (em_source.nb_sw_sources)
        printf("Spherical wave:\n");
    for (i = 0; i < em_source.nb_sw_sources; i++)
        print_els_sphericalwave(em_source.sw_sources[i], 3);

    if (em_source.nb_ge_sources)
        printf("Generator:\n");
    for (i = 0; i < em_source.nb_ge_sources; i++)
        print_els_generator(em_source.ge_sources[i], 3);

    if (em_source.nb_di_sources)
        printf("Dipole:\n");
    for (i = 0; i < em_source.nb_di_sources; i++)
        print_els_dipole(em_source.di_sources[i], 3);

    if (em_source.nb_an_sources)
        printf("Antenna:\n");
    for (i = 0; i < em_source.nb_an_sources; i++)
        print_els_antenna(em_source.an_sources[i], 3);

    if (em_source.nb_sm_sources)
        printf("Source on mesh:\n");
    for (i = 0; i < em_source.nb_sm_sources; i++)
        print_els_sourceonmesh(em_source.sm_sources[i], 3);
    printf("\n");
}




// Free memory used by planeWave instance
void free_els_planewave (planewave_t *planewave)
{
    if (planewave->path != NULL)
    {
        free(planewave->path);
        planewave->path = NULL;
    }
    free_opt_attrs(&(planewave->opt_attrs));
    free_floatingtype(&(planewave->magnitude));
}


// Free memory used by sphericalWave instance
void free_els_sphericalwave (sphericalwave_t *sphericalwave)
{
    if (sphericalwave->path != NULL)
    {
        free(sphericalwave->path);
        sphericalwave->path = NULL;
    }
    free_floatingtype(&(sphericalwave->magnitude));
}


// Free memory used by planeWave instance
void free_els_generator (generator_t *generator)
{
    if (generator->path != NULL)
    {
        free(generator->path);
        generator->path = NULL;
    }
    free_opt_attrs(&(generator->opt_attrs));
    free_floatingtype(&(generator->inner_impedance));
    free_floatingtype(&(generator->magnitude));
}


// Free memory used by dipole instance
void free_els_dipole (dipole_t *dipole)
{
    if (dipole->path != NULL)
    {
        free(dipole->path);
        dipole->path = NULL;
    }
    free_opt_attrs(&(dipole->opt_attrs));
    free_floatingtype(&(dipole->inner_impedance));
    free_floatingtype(&(dipole->magnitude));
}


// Free memory used by antenna instance
void free_els_antenna (antenna_t *antenna)
{
    if (antenna->path != NULL)
    {
        free(antenna->path);
        antenna->path = NULL;
    }
    free_opt_attrs(&(antenna->opt_attrs));
    free_floatingtype(&(antenna->input_impedance));
    free_floatingtype(&(antenna->load_impedance));
    free_floatingtype(&(antenna->feeder_impedance));
    free_floatingtype(&(antenna->magnitude));
    free_opt_attrs(&(antenna->model.opt_attrs));
    switch (antenna->model.type)
    {
    case ANT_GAIN:
        free_floatingtype(&(antenna->model.data.gain));
        break;
    case ANT_EFFECTIVE_AREA:
        free_floatingtype(&(antenna->model.data.effarea));
        break;
    case ANT_FAR_FIELD:
        free_floatingtype(&(antenna->model.data.farfield));
        break;
    case ANT_RECTANGULAR_HORN:
        free_opt_attrs(&(antenna->model.data.parreflct));
        break;
    case ANT_CIRCULAR_HORN:
        free_opt_attrs(&(antenna->model.data.parreflct));
        break;
    default:
        break;
    }
}


// Free memory used by sourceOnMesh instance
void free_els_sourceonmesh (sourceonmesh_t *sourceonmesh)
{
    if (sourceonmesh->path != NULL)
    {
        free(sourceonmesh->path);
        sourceonmesh->path = NULL;
    }
    switch (sourceonmesh->type)
    {
    case SCOM_ARRAYSET:
        free_ft_arrayset(&(sourceonmesh->data.arrayset));
        break;
    case SCOM_EXCHANGE_SURFACE:
        free(sourceonmesh->data.exchange_surface);
        break;
    default:
        break;
    }
}


// Free memory used by whole electromagneticSource category
void free_electromagnetic_source (em_source_t *em_source)
{
    hsize_t i;

    if (em_source->nb_pw_sources > 0)
    {
        for (i = 0; i < em_source->nb_pw_sources; i++)
            free_els_planewave(em_source->pw_sources + i);
        free(em_source->pw_sources);
    }
    if (em_source->nb_sw_sources > 0)
    {
        for (i = 0; i < em_source->nb_sw_sources; i++)
            free_els_sphericalwave(em_source->sw_sources + i);
        free(em_source->sw_sources);
    }
    if (em_source->nb_ge_sources > 0)
    {
        for (i = 0; i < em_source->nb_ge_sources; i++)
            free_els_generator(em_source->ge_sources + i);
        free(em_source->ge_sources);
    }
    if (em_source->nb_di_sources > 0)
    {
        for (i = 0; i < em_source->nb_di_sources; i++)
            free_els_dipole(em_source->di_sources + i);
        free(em_source->di_sources);
    }
    if (em_source->nb_an_sources > 0)
    {
        for (i = 0; i < em_source->nb_an_sources; i++)
            free_els_antenna(em_source->an_sources + i);
        free(em_source->an_sources);
    }
    if (em_source->nb_sm_sources > 0)
    {
        for (i = 0; i < em_source->nb_sm_sources; i++)
            free_els_sourceonmesh(em_source->sm_sources + i);
        free(em_source->sm_sources);
    }
}

