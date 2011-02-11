#include "emsource.h"


// Read instance in /electromagneticSource/planeWave
void read_es_planewave (hid_t file_id, const char *path, planewave_t *planewave)
{
    char *path2, mandatory[][ATTRIBUTE_LENGTH] = {A_X0, A_Y0, A_Z0, A_THETA, A_PHI};

    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    planewave->name = get_name_from_path(path);
    read_optional_attributes(file_id, path, &(planewave->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    if (!read_flt_attribute(file_id, path, A_X0, &(planewave->x0)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_X0);
    if (!read_flt_attribute(file_id, path, A_Y0, &(planewave->y0)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_Y0);
    if (!read_flt_attribute(file_id, path, A_Z0, &(planewave->z0)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_Z0);
    if (!read_flt_attribute(file_id, path, A_THETA, &(planewave->theta)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_THETA);
    if (!read_flt_attribute(file_id, path, A_PHI, &(planewave->phi)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_PHI);
    strcpy(path2, path);
    strcat(path2, G_MAGNITUDE);
    read_floatingtype(file_id, path2, &(planewave->magnitude));
    free(path2);
}


// Read instance in /electromagneticSource/sphericalWave
void read_es_sphericalwave (hid_t file_id, const char *path, sphericalwave_t *sphericalwave)
{
    char *path2;

    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    sphericalwave->name = get_name_from_path(path);
    if (!read_flt_attribute(file_id, path, A_X0, &(sphericalwave->x0)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_X0);
    if (!read_flt_attribute(file_id, path, A_Y0, &(sphericalwave->y0)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_Y0);
    if (!read_flt_attribute(file_id, path, A_Z0, &(sphericalwave->z0)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_Z0);
    strcpy(path2, path);
    strcat(path2, G_MAGNITUDE);
    read_floatingtype(file_id, path2, &(sphericalwave->magnitude));
    free(path2);
}


// Read instance in /electromagneticSource/generator
void read_es_generator (hid_t file_id, const char *path, generator_t *generator)
{
    char *type, *path2, mandatory[][ATTRIBUTE_LENGTH] = {A_TYPE};

    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    generator->name = get_name_from_path(path);
    read_optional_attributes(file_id, path, &(generator->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    generator->type = GEN_INVALID;
    if (read_str_attribute(file_id, path, A_TYPE, &type))
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
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_TYPE);
    strcpy(path2, path);
    strcat(path2, G_INNER_IMPEDANCE);
    read_floatingtype(file_id, path2, &(generator->inner_impedance));
    strcpy(path2, path);
    strcat(path2, G_MAGNITUDE);
    read_floatingtype(file_id, path2, &(generator->magnitude));
    free(path2);
}


// Read instance in /electromagneticSource/dipole
void read_es_dipole (hid_t file_id, const char *path, dipole_t *dipole)
{
    char *type, *path2, mandatory[][ATTRIBUTE_LENGTH] = {A_TYPE, A_X, A_Y, A_Z, A_THETA, A_PHI, A_WIRE_RADIUS};

    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    dipole->name = get_name_from_path(path);
    read_optional_attributes(file_id, path, &(dipole->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    dipole->type = DIPOLE_INVALID;
    if (read_str_attribute(file_id, path, A_TYPE, &type))
    {
        if (strcmp(type, V_ELECTRIC) == 0)
            dipole->type = DIPOLE_ELECTRIC;
        else if (strcmp(type, V_MAGNETIC) == 0)
            dipole->type = DIPOLE_MAGNETIC;
        free(type);
    }
    else
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_TYPE);

    if (!read_flt_attribute(file_id, path, A_X, &(dipole->x)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_X);
    if (!read_flt_attribute(file_id, path, A_Y, &(dipole->y)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_Y);
    if (!read_flt_attribute(file_id, path, A_Z, &(dipole->z)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_Z);
    if (!read_flt_attribute(file_id, path, A_THETA, &(dipole->theta)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_THETA);
    if (!read_flt_attribute(file_id, path, A_PHI, &(dipole->phi)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_PHI);
    if (!read_flt_attribute(file_id, path, A_WIRE_RADIUS, &(dipole->wire_radius)))
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_WIRE_RADIUS);
    strcpy(path2, path);
    strcat(path2, G_INNER_IMPEDANCE);
    read_floatingtype(file_id, path2, &(dipole->inner_impedance));
    strcpy(path2, path);
    strcat(path2, G_MAGNITUDE);
    read_floatingtype(file_id, path2, &(dipole->magnitude));
    free(path2);
}


// Read instance in /electromagneticSource/antenna
void read_es_antenna (hid_t file_id, const char *path, antenna_t *antenna)
{
    char *path2, mandatory[][ATTRIBUTE_LENGTH] = {};

    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    antenna->name = get_name_from_path(path);
    read_optional_attributes(file_id, path, &(antenna->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);

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
    free(path2);
}


// Read instance in /electromagneticSource/sourceOnMesh
void read_es_sourceonmesh (hid_t file_id, const char *path, sourceonmesh_t *sourceonmesh)
{
    char *type;

    sourceonmesh->name = get_name_from_path(path);
    sourceonmesh->type = SCOM_INVALID;
    if (read_str_attribute(file_id, path, A_TYPE, &type))
    {
        if (strcmp(type, V_ARRAYSET) == 0)
        {
            sourceonmesh->type = SCOM_ARRAYSET;
            read_ft_arrayset(file_id, path, &(sourceonmesh->data.arrayset));
        }
        else if (strcmp(type, V_EXCHANGE_SURFACE) == 0)
        {
            sourceonmesh->type = SCOM_EXCHANGE_SURFACE;
            read_str_attribute(file_id, path, A_EXCHANGE_SURFACE, &(sourceonmesh->data.exchange_surface));
        }
        free(type);
    }
    else
        printf("***** ERROR(%s): Cannot read mandatory attribute \"%s@%s\". *****\n\n", C_ELECTROMAGNETIC_SOURCE, path, A_TYPE);
}


// Read electromagneticSource category
void read_electromagnetic_source (hid_t file_id, em_source_t *em_source)
{
    hsize_t i, j;
    children_t children, children2;
    char *path, *path2;

    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

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
                em_source->pw_sources = (planewave_t *) malloc((size_t) children2.nb_children * sizeof(planewave_t));
            }
            else if (strcmp(children.childnames[i], G_SPHERICAL_WAVE) == 0)
            {
                em_source->nb_sw_sources = children2.nb_children;
                em_source->sw_sources = (sphericalwave_t *) malloc((size_t) children2.nb_children * sizeof(sphericalwave_t));
            }
            else if (strcmp(children.childnames[i], G_GENERATOR) == 0)
            {
                em_source->nb_ge_sources = children2.nb_children;
                em_source->ge_sources = (generator_t *) malloc((size_t) children2.nb_children * sizeof(generator_t));
            }
            else if (strcmp(children.childnames[i], G_DIPOLE) == 0)
            {
                em_source->nb_di_sources = children2.nb_children;
                em_source->di_sources = (dipole_t *) malloc((size_t) children2.nb_children * sizeof(dipole_t));
            }
            else if (strcmp(children.childnames[i], G_ANTENNA) == 0)
            {
                em_source->nb_an_sources = children2.nb_children;
                em_source->an_sources = (antenna_t *) malloc((size_t) children2.nb_children * sizeof(antenna_t));
            }
            else if (strcmp(children.childnames[i], G_SOURCE_ON_MESH) == 0)
            {
                em_source->nb_sm_sources = children2.nb_children;
                em_source->sm_sources = (sourceonmesh_t *) malloc((size_t) children2.nb_children * sizeof(sourceonmesh_t));
            }
            if (children2.nb_children > 0)
            {
                for (j = 0; j < children2.nb_children; j++)
                {
                    strcpy(path2, path);
                    strcat(path2, children2.childnames[j]);
                    if (strcmp(children.childnames[i], G_PLANE_WAVE) == 0)
                        read_es_planewave(file_id, path2, em_source->pw_sources +j);
                    else if (strcmp(children.childnames[i], G_SPHERICAL_WAVE) == 0)
                        read_es_sphericalwave(file_id, path2, em_source->sw_sources +j);
                    else if (strcmp(children.childnames[i], G_GENERATOR) == 0)
                        read_es_generator(file_id, path2, em_source->ge_sources +j);
                    else if (strcmp(children.childnames[i], G_DIPOLE) == 0)
                        read_es_dipole(file_id, path2, em_source->di_sources +j);
                    else if (strcmp(children.childnames[i], G_ANTENNA) == 0)
                        read_es_antenna(file_id, path2, em_source->an_sources +j);
                    else if (strcmp(children.childnames[i], G_SOURCE_ON_MESH) == 0)
                        read_es_sourceonmesh(file_id, path2, em_source->sm_sources +j);
                    free(children2.childnames[j]);
                }
                free(children2.childnames);
            }
            free(children.childnames[i]);
        }
        free(children.childnames);
    }
    free(path2);
    free(path);
}




// Print instance in /electromagneticSource/planeWave
void print_es_planewave (planewave_t planewave, int space)
{
    printf("%*sName: %s\n", space, "", planewave.name);
    print_optional_attributes(planewave.optional_attributes, space + 4);
    print_flt_attribute(A_X0, planewave.x0, space + 4);
    print_flt_attribute(A_Y0, planewave.y0, space + 4);
    print_flt_attribute(A_Z0, planewave.z0, space + 4);
    print_flt_attribute(A_THETA, planewave.theta, space + 4);
    print_flt_attribute(A_PHI, planewave.phi, space + 4);
    print_floatingtype(planewave.magnitude, space + 2);
    printf("\n");
}


// Print instance in /electromagneticSource/sphericalWave
void print_es_sphericalwave (sphericalwave_t sphericalwave, int space)
{
    printf("%*sName: %s\n", space, "", sphericalwave.name);
    print_flt_attribute(A_X0, sphericalwave.x0, space + 4);
    print_flt_attribute(A_Y0, sphericalwave.y0, space + 4);
    print_flt_attribute(A_Z0, sphericalwave.z0, space + 4);
    print_floatingtype(sphericalwave.magnitude, space + 2);
    printf("\n");
}


// Print instance in /electromagneticSource/generator
void print_es_generator (generator_t generator, int space)
{
    printf("%*sName: %s\n", space, "", generator.name);
    print_optional_attributes(generator.optional_attributes, space + 4);
    switch (generator.type)
    {
        case GEN_VOLTAGE:
            print_str_attribute(A_TYPE, V_VOLTAGE, space + 4);
        break;
        case GEN_CURRENT:
            print_str_attribute(A_TYPE, V_CURRENT, space + 4);
        break;
        case GEN_POWER:
            print_str_attribute(A_TYPE, V_POWER, space + 4);
        break;
        case GEN_POWER_DENSITY:
            print_str_attribute(A_TYPE, V_POWER_DENSITY, space + 4);
        break;
        case GEN_INVALID:
            print_str_attribute(A_TYPE, "INVALID", space + 4);
        break;
    }
    print_floatingtype(generator.inner_impedance, space + 2);
    print_floatingtype(generator.magnitude, space + 2);
    printf("\n");
}


// Print instance in /electromagneticSource/dipole
void print_es_dipole (dipole_t dipole, int space)
{
    printf("%*sName: %s\n", space, "", dipole.name);
    switch (dipole.type)
    {
        case DIPOLE_ELECTRIC:
            print_str_attribute(A_TYPE, V_ELECTRIC, space + 4);
        break;
        case DIPOLE_MAGNETIC:
            print_str_attribute(A_TYPE, V_MAGNETIC, space + 4);
        break;
        case DIPOLE_INVALID:
            print_str_attribute(A_TYPE, "INVALID", space + 4);
        break;
    }
    print_optional_attributes(dipole.optional_attributes, space + 4);
    print_flt_attribute(A_X, dipole.x, space + 4);
    print_flt_attribute(A_Y, dipole.y, space + 4);
    print_flt_attribute(A_Z, dipole.z, space + 4);
    print_flt_attribute(A_THETA, dipole.theta, space + 4);
    print_flt_attribute(A_PHI, dipole.phi, space + 4);
    print_flt_attribute(A_WIRE_RADIUS, dipole.wire_radius, space + 4);
    printf("\n");
}


// Print instance in /electromagneticSource/antenna
void print_es_antenna (antenna_t antenna, int space)
{
    printf("%*sName: %s\n", space, "", antenna.name);
    print_optional_attributes(antenna.optional_attributes, space + 4);
    print_floatingtype(antenna.input_impedance, space + 2);
    print_floatingtype(antenna.load_impedance, space + 2);
    print_floatingtype(antenna.feeder_impedance, space + 2);
    print_floatingtype(antenna.magnitude, space + 2);
    printf("\n");
}


// Print instance in /electromagneticSource/sourceOnMesh
void print_es_sourceonmesh (sourceonmesh_t sourceonmesh, int space)
{
    hsize_t i;

    printf("%*sName: %s\n", space, "", sourceonmesh.name);
    switch (sourceonmesh.type)
    {
        case SCOM_ARRAYSET:
            print_str_attribute(A_TYPE, V_ARRAYSET, space + 4);
            print_ft_dataset(sourceonmesh.data.arrayset.data, space + 2);
            for (i = 0; i < sourceonmesh.data.arrayset.nb_dims; i++)
                print_ft_vector(sourceonmesh.data.arrayset.dims[i], space + 2);
        break;
        case SCOM_EXCHANGE_SURFACE:
            print_str_attribute(A_TYPE, V_EXCHANGE_SURFACE, space + 3);
            print_str_attribute(A_EXCHANGE_SURFACE, sourceonmesh.data.exchange_surface, space + 3);
        break;
        case SCOM_INVALID:
            print_str_attribute(A_TYPE, "INVALID", space + 3);
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
        print_es_planewave(em_source.pw_sources[i], 3);

    if (em_source.nb_sw_sources)
        printf("Spherical wave:\n");
    for (i = 0; i < em_source.nb_sw_sources; i++)
        print_es_sphericalwave(em_source.sw_sources[i], 3);

    if (em_source.nb_ge_sources)
        printf("Generator:\n");
    for (i = 0; i < em_source.nb_ge_sources; i++)
        print_es_generator(em_source.ge_sources[i], 3);

    if (em_source.nb_di_sources)
        printf("Dipole:\n");
    for (i = 0; i < em_source.nb_di_sources; i++)
        print_es_dipole(em_source.di_sources[i], 3);

    if (em_source.nb_an_sources)
        printf("Antenna:\n");
    for (i = 0; i < em_source.nb_an_sources; i++)
        print_es_antenna(em_source.an_sources[i], 3);

    if (em_source.nb_sm_sources)
        printf("Source on mesh:\n");
    for (i = 0; i < em_source.nb_sm_sources; i++)
        print_es_sourceonmesh(em_source.sm_sources[i], 3);
    printf("\n");
}




// Free memory used by planeWave instance
void free_es_planewave (planewave_t *planewave)
{
    if (planewave->name != NULL)
    {
        free(planewave->name);
        planewave->name = NULL;
    }
    free_optional_attributes(&(planewave->optional_attributes));
    free_floatingtype(&(planewave->magnitude));
}


// Free memory used by sphericalWave instance
void free_es_sphericalwave (sphericalwave_t *sphericalwave)
{
    if (sphericalwave->name != NULL)
    {
        free(sphericalwave->name);
        sphericalwave->name = NULL;
    }
    free_floatingtype(&(sphericalwave->magnitude));
}


// Free memory used by planeWave instance
void free_es_generator (generator_t *generator)
{
    if (generator->name != NULL)
    {
        free(generator->name);
        generator->name = NULL;
    }
    free_optional_attributes(&(generator->optional_attributes));
    free_floatingtype(&(generator->inner_impedance));
    free_floatingtype(&(generator->magnitude));
}


// Free memory used by dipole instance
void free_es_dipole (dipole_t *dipole)
{
    if (dipole->name != NULL)
    {
        free(dipole->name);
        dipole->name = NULL;
    }
    free_optional_attributes(&(dipole->optional_attributes));
    free_floatingtype(&(dipole->inner_impedance));
    free_floatingtype(&(dipole->magnitude));
}


// Free memory used by antenna instance
void free_es_antenna (antenna_t *antenna)
{
    if (antenna->name != NULL)
    {
        free(antenna->name);
        antenna->name = NULL;
    }
    free_optional_attributes(&(antenna->optional_attributes));
    free_floatingtype(&(antenna->input_impedance));
    free_floatingtype(&(antenna->load_impedance));
    free_floatingtype(&(antenna->feeder_impedance));
    free_floatingtype(&(antenna->magnitude));
}


// Free memory used by sourceOnMesh instance
void free_es_sourceonmesh (sourceonmesh_t *sourceonmesh)
{
    if (sourceonmesh->name != NULL)
    {
        free(sourceonmesh->name);
        sourceonmesh->name = NULL;
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
            free_es_planewave(em_source->pw_sources + i);
        free(em_source->pw_sources);
    }
    if (em_source->nb_sw_sources > 0)
    {
        for (i = 0; i < em_source->nb_sw_sources; i++)
            free_es_sphericalwave(em_source->sw_sources + i);
        free(em_source->sw_sources);
    }
    if (em_source->nb_ge_sources > 0)
    {
        for (i = 0; i < em_source->nb_ge_sources; i++)
            free_es_generator(em_source->ge_sources + i);
        free(em_source->ge_sources);
    }
    if (em_source->nb_di_sources > 0)
    {
        for (i = 0; i < em_source->nb_di_sources; i++)
            free_es_dipole(em_source->di_sources + i);
        free(em_source->di_sources);
    }
    if (em_source->nb_an_sources > 0)
    {
        for (i = 0; i < em_source->nb_an_sources; i++)
            free_es_antenna(em_source->an_sources + i);
        free(em_source->an_sources);
    }
    if (em_source->nb_sm_sources > 0)
    {
        for (i = 0; i < em_source->nb_sm_sources; i++)
            free_es_sourceonmesh(em_source->sm_sources + i);
        free(em_source->sm_sources);
    }
}

