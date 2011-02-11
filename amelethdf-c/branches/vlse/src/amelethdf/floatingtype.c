#include "floatingtype.h"
// Revision: 6.2.2011


// Read singleInteger, return TRUE (all OK) or FALSE (no malloc)
char read_ft_singleinteger (hid_t file_id, const char *path, singleinteger_t *singleinteger)
{
    char success = FALSE;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE, A_VALUE};

    if (read_int_attribute(file_id, path, A_VALUE, &(singleinteger->value)))
    {
        singleinteger->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(singleinteger->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
        success = TRUE;
    }
    else
        printf("***** ERROR: Cannot read mandatory attribute \"%s@%s\". *****\n\n", path, A_VALUE);
    return success;
}


// Read singleReal, return TRUE (all OK) or FALSE (no malloc)
char read_ft_singlereal (hid_t file_id, const char *path, singlereal_t *singlereal)
{
    char success = FALSE;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE, A_VALUE};

    if (read_flt_attribute(file_id, path, A_VALUE, &(singlereal->value)))
    {
        singlereal->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(singlereal->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
        success = TRUE;
    }
    else
        printf("***** ERROR: Cannot read mandatory attribute \"%s@%s\". *****\n\n", path, A_VALUE);
    return success;
}


// Read singleComplex, return TRUE (all OK) or FALSE (no malloc)
char read_ft_singlecomplex (hid_t file_id, const char *path, singlecomplex_t *singlecomplex)
{
    char success = FALSE;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE, A_VALUE};

    if (read_cpx_attribute(file_id, path, A_VALUE, &(singlecomplex->value)))
    {
        singlecomplex->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(singlecomplex->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
        success = TRUE;
    }
    else
        printf("***** ERROR: Cannot read mandatory attribute \"%s@%s\". *****\n\n", path, A_VALUE);
    return success;
}


// Read singleString, return TRUE (all OK) or FALSE (no malloc)
char read_ft_singlestring (hid_t file_id, const char *path, singlestring_t *singlestring)
{
    char success = FALSE;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE, A_VALUE};

    if(read_str_attribute(file_id, path, A_VALUE, &(singlestring->value)))
    {
        singlestring->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(singlestring->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
        success = TRUE;
    }
    else
        printf("***** ERROR: Cannot read mandatory attribute \"%s@%s\". *****\n\n", path, A_VALUE);
    return success;
}


// Read vector, return TRUE (all OK) or FALSE (no malloc)
char read_ft_vector (hid_t file_id, const char *path, vector_t *vector)
{
    size_t length;
    int nb_dims;
    char success = FALSE;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE};

    vector->nb_values = 1;  // in case of single value
    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims <= 1)
                if (H5LTget_dataset_info(file_id, path, &(vector->nb_values), &(vector->type_class), &length) >= 0)
                    switch (vector->type_class)
                    {
                    case H5T_INTEGER:
                        if (read_int_dataset(file_id, path, vector->nb_values, &(vector->values.i)))
                            success = TRUE;
                        break;
                    case H5T_FLOAT:
                        if (read_float_dataset(file_id, path, vector->nb_values, &(vector->values.f)))
                            success = TRUE;
                        break;
                    case H5T_COMPOUND:
                        if (read_complex_dataset(file_id, path, vector->nb_values, &(vector->values.c)))
                            success = TRUE;
                        break;
                    case H5T_STRING:
                        if (read_string_dataset(file_id, path, vector->nb_values, length, &(vector->values.s)))
                            success = TRUE;
                        break;
                    default:
                        break;
                    }
    if (success)
    {
        vector->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(vector->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    }
    else
    {
        vector->nb_values = 0;
        printf("***** ERROR: Cannot read dataset \"%s\". *****\n\n", path);
    }
    return success;
}


// Read linearListOfReal1, return TRUE (all OK) or FALSE (no malloc)
char read_ft_linearlistofreal1 (hid_t file_id, const char *path, linearlistofreal1_t *linearlistofreal1)
{
    char success = FALSE;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE};

    linearlistofreal1->first = 0;
    linearlistofreal1->last = 0;
    linearlistofreal1->number_of_values = 0;
    if (H5Aexists_by_name(file_id, path, A_FIRST, H5P_DEFAULT) > 0 && H5Aexists_by_name(file_id, path, A_LAST, H5P_DEFAULT) > 0)  // interval doesn't have A_NUMBER_OF_VALUES
        if (H5LTget_attribute_float(file_id, path, A_FIRST, &(linearlistofreal1->first)) >= 0 && H5LTget_attribute_float(file_id, path, A_LAST, &(linearlistofreal1->last)) >= 0)
            success = TRUE;
    if (H5Aexists_by_name(file_id, path, A_NUMBER_OF_VALUES, H5P_DEFAULT) > 0)
        if (H5LTget_attribute_int(file_id, path, A_NUMBER_OF_VALUES, &(linearlistofreal1->number_of_values)) < 0)  // A_NUMBER_OF_VALUES is present -> must be read
            success = FALSE;
    if (success)
    {
        linearlistofreal1->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(linearlistofreal1->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read mandatory attributes in \"%s\". *****\n\n", path);
    return success;
}


// Read linearListOfReal2, return TRUE (all OK) or FALSE (no malloc)
char read_ft_linearlistofreal2 (hid_t file_id, const char *path, linearlistofreal2_t *linearlistofreal2)
{
    char success = FALSE;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE};

    linearlistofreal2->first = 0;
    linearlistofreal2->step = 0;
    linearlistofreal2->number_of_values = 0;
    if (H5Aexists_by_name(file_id, path, A_FIRST, H5P_DEFAULT) > 0 && H5Aexists_by_name(file_id, path, A_STEP, H5P_DEFAULT) > 0 && H5Aexists_by_name(file_id, path, A_NUMBER_OF_VALUES, H5P_DEFAULT) > 0)
        if (H5LTget_attribute_float(file_id, path, A_FIRST, &(linearlistofreal2->first)) >= 0 && H5LTget_attribute_float(file_id, path, A_STEP, &(linearlistofreal2->step)) >= 0 && H5LTget_attribute_int(file_id, path, A_NUMBER_OF_VALUES, &(linearlistofreal2->number_of_values)) >= 0)
            success = TRUE;
    if (success)
    {
        linearlistofreal2->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(linearlistofreal2->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read mandatory attributes in \"%s\". *****\n\n", path);
    return success;
}


// Read logarithmListOfReal, return TRUE (all OK) or FALSE (no malloc)
char read_ft_logarithmlistofreal (hid_t file_id, const char *path, logarithmlistofreal_t *logarithmlistofreal)
{
    char success = FALSE;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE};

    logarithmlistofreal->first = 0;
    logarithmlistofreal->last = 0;
    logarithmlistofreal->number_of_values = 0;
    if (H5Aexists_by_name(file_id, path, A_FIRST, H5P_DEFAULT) > 0 && H5Aexists_by_name(file_id, path, A_LAST, H5P_DEFAULT) > 0 && H5Aexists_by_name(file_id, path, A_NUMBER_OF_VALUES, H5P_DEFAULT) > 0)
        if (H5LTget_attribute_float(file_id, path, A_FIRST, &(logarithmlistofreal->first)) >= 0 && H5LTget_attribute_float(file_id, path, A_LAST, &(logarithmlistofreal->last)) >= 0 && H5LTget_attribute_int(file_id, path, A_NUMBER_OF_VALUES, &(logarithmlistofreal->number_of_values)) >= 0)
            success = TRUE;
    if (success)
    {
        logarithmlistofreal->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(logarithmlistofreal->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read mandatory attributes in \"%s\". *****\n\n", path);
    return success;
}


// Read perDecadeListOfReal, return TRUE (all OK) or FALSE (no malloc)
char read_ft_perdecadelistofreal (hid_t file_id, const char *path, perdecadelistofreal_t *perdecadelistofreal)
{
    char success = FALSE;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE};

    perdecadelistofreal->first = 0;
    perdecadelistofreal->number_of_decades = 0;
    perdecadelistofreal->number_of_values_per_decade = 0;
    if (H5Aexists_by_name(file_id, path, A_FIRST, H5P_DEFAULT) > 0 && H5Aexists_by_name(file_id, path, A_NUMBER_OF_DECADES, H5P_DEFAULT) > 0 && H5Aexists_by_name(file_id, path, A_NUMBER_OF_VALUES_PER_DECADE, H5P_DEFAULT) > 0)
        if (H5LTget_attribute_float(file_id, path, A_FIRST, &(perdecadelistofreal->first)) >= 0 && H5LTget_attribute_int(file_id, path, A_NUMBER_OF_DECADES, &(perdecadelistofreal->number_of_decades)) >= 0 && H5LTget_attribute_int(file_id, path, A_NUMBER_OF_VALUES_PER_DECADE, &(perdecadelistofreal->number_of_values_per_decade)) >= 0)
            success = TRUE;
    if (success)
    {
        perdecadelistofreal->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(perdecadelistofreal->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read mandatory attributes in \"%s\". *****\n\n", path);
    return success;
}


// Read linearListOfInteger2, return TRUE (all OK) or FALSE (no malloc)
char read_ft_linearlistofinteger2 (hid_t file_id, const char *path, linearlistofinteger2_t *linearlistofinteger2)
{
    char success = FALSE;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE};

    linearlistofinteger2->first = 0;
    linearlistofinteger2->step = 0;
    linearlistofinteger2->number_of_values = 0;
    if (H5Aexists_by_name(file_id, path, A_FIRST, H5P_DEFAULT) > 0 && H5Aexists_by_name(file_id, path, A_STEP, H5P_DEFAULT) > 0 && H5Aexists_by_name(file_id, path, A_NUMBER_OF_VALUES, H5P_DEFAULT) > 0)
        if (H5LTget_attribute_int(file_id, path, A_FIRST, &(linearlistofinteger2->first)) >= 0 && H5LTget_attribute_int(file_id, path, A_STEP, &(linearlistofinteger2->step)) >= 0 && H5LTget_attribute_int(file_id, path, A_NUMBER_OF_VALUES, &(linearlistofinteger2->number_of_values)) >= 0)
            success = TRUE;
    if (success)
    {
        linearlistofinteger2->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(linearlistofinteger2->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read mandatory attributes in \"%s\". *****\n\n", path);
    return success;
}


// Read rationalFunction, return TRUE (all OK) or FALSE (no malloc)
char read_ft_rationalfunction (hid_t file_id, const char *path, rationalfunction_t *rationalfunction)
{
    hsize_t nfields, i;
    char success = FALSE;
    char **field_names;
    size_t *field_sizes;
    size_t *field_offsets;
    size_t type_size;
    int type = 0, a = 1, b = 2, f = 3;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE};

    if (H5Lexists(file_id, path, H5P_DEFAULT) != FALSE)
        if (H5TBget_table_info(file_id, path, &nfields, &(rationalfunction->nb_types)) >= 0)
            if (nfields == 4 && rationalfunction->nb_types > 0)
            {
                field_names = (char **) malloc(nfields * sizeof(char *));
                field_names[0] = (char *) malloc(TABLE_FIELD_NAME_LENGTH * nfields * sizeof(char));
                for (i = 0; i < nfields; i++)
                    field_names[i] = field_names[0] + i * TABLE_FIELD_NAME_LENGTH;
                field_sizes = (size_t *) malloc((size_t) sizeof(size_t *) * nfields);
                field_offsets = (size_t *) malloc((size_t) sizeof(size_t *) * nfields);

                if (H5TBget_field_info(file_id, path, field_names, field_sizes, field_offsets, &type_size) >= 0)
                    if (strcmp(field_names[0], F_TYPE) == 0 && strcmp(field_names[1], F_A) == 0 && strcmp(field_names[2], F_B) == 0 && strcmp(field_names[3], F_F) == 0)
                    {
                        rationalfunction->types = (int *) malloc((size_t) rationalfunction->nb_types * sizeof(int));
                        rationalfunction->a = (float *) malloc((size_t) rationalfunction->nb_types * sizeof(float));
                        rationalfunction->b = (float *) malloc((size_t) rationalfunction->nb_types * sizeof(float));
                        rationalfunction->f = (float *) malloc((size_t) rationalfunction->nb_types * sizeof(float));
                        if (H5TBread_fields_index(file_id, path, 1, &type, 0, rationalfunction->nb_types, sizeof(int), field_offsets, field_sizes, rationalfunction->types) >= 0
                                &&
                                H5TBread_fields_index(file_id, path, 1, &a, 0, rationalfunction->nb_types, sizeof(float), field_offsets, field_sizes, rationalfunction->a) >= 0
                                &&
                                H5TBread_fields_index(file_id, path, 1, &b, 0, rationalfunction->nb_types, sizeof(float), field_offsets, field_sizes, rationalfunction->b) >= 0
                                &&
                                H5TBread_fields_index(file_id, path, 1, &f, 0, rationalfunction->nb_types, sizeof(float), field_offsets, field_sizes, rationalfunction->f) >= 0)
                            success = TRUE;
                        else
                        {
                            free(rationalfunction->types);
                            free(rationalfunction->a);
                            free(rationalfunction->b);
                            free(rationalfunction->f);
                        }
                    }
                free(field_names[0]);
                free(field_names);
                free(field_sizes);
                free(field_offsets);
            }
    if (success)
    {
        rationalfunction->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(rationalfunction->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read table \"%s\". *****\n\n", path);
    return success;
}


// Read generalRationalFunction, return TRUE (all OK) or FALSE (no malloc)
char read_ft_generalrationalfunction (hid_t file_id, const char *path, generalrationalfunction_t *generalrationalfunction)
{
    hsize_t dims[2], i;
    int nb_dims;
    H5T_class_t type_class;
    size_t length;
    char success = FALSE;
    complex float *buf;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE};

    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
            if (nb_dims == 2)
                if (H5LTget_dataset_info(file_id, path, dims, &type_class, &length) >= 0)
                    if (dims[0] > 0 && dims[1] == 2 && type_class == H5T_COMPOUND)
                    {
                        generalrationalfunction->numerator = (complex float *) malloc((size_t) dims[0] * sizeof(complex float));
                        generalrationalfunction->denominator = (complex float *) malloc((size_t) dims[0] * sizeof(complex float));
                        if (read_complex_dataset(file_id, path, dims[0] * dims[1], &(buf)))
                        {
                            for (i = 0; i < dims[0]; i++)
                            {
                                generalrationalfunction->numerator[i] = buf[2*i];
                                generalrationalfunction->denominator[i] = buf[2*i + 1];
                            }
                            success = TRUE;
                            free(buf);
                        }
                        else
                        {
                            free(generalrationalfunction->numerator);
                            free(generalrationalfunction->denominator);
                        }
                    }
    if (success)
    {
        generalrationalfunction->nb_degrees = dims[0];
        generalrationalfunction->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(generalrationalfunction->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read dataset \"%s\". *****\n\n", path);
    return success;
}


// Read rational, return TRUE (all OK) or FALSE (no malloc)
char read_ft_rational (hid_t file_id, const char *path, rational_t *rational)
{
    char *path2, *buf, success = FALSE;
    hsize_t i, invalid = 0;
    int nb_dims;
    size_t length;
    H5T_class_t type_class;
    children_t children;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE};

    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    strcpy(path2, path);
    strcat(path2, G_FUNCTION);
    children = read_children_name(file_id, path2);
    rational->nb_functions = children.nb_children;
    if (children.nb_children > 0)
    {
        // Read rational/function until error
        rational->functions = (floatingtyper_t *) malloc((size_t) children.nb_children * sizeof(floatingtyper_t));
        for (i = 0; i < children.nb_children; i++)
        {
            if (!invalid)
            {
                strcpy(path2, path);
                strcat(path2, G_FUNCTION);
                strcat(path2, children.childnames[i]);
                if (read_str_attribute(file_id, path2, A_FLOATING_TYPE, &buf))
                {
                    if (strcmp(buf, V_RATIONAL_FUNCTION) == 0)
                    {
                        rational->functions[i].type = FT_RATIONAL_FUNCTION;
                        if(!read_ft_rationalfunction(file_id, path2, &(rational->functions[i].data.rf)))
                            invalid = i;
                    }
                    else if (strcmp(buf, V_GENERAL_RATIONAL_FUNCTION) == 0)
                    {
                        rational->functions[i].type = FT_GENERAL_RATIONAL_FUNCTION;
                        if(!read_ft_generalrationalfunction(file_id, path2, &(rational->functions[i].data.grf)))
                            invalid = i;
                    }
                    else
                    {
                        printf("***** WARNING: Invalid attribute \"floatingType\" in \"%s\". *****\n\n", path2);
                        invalid = i;
                    }
                    free(buf);
                    buf = NULL;
                }
            }
            free(children.childnames[i]);
        }
        free(children.childnames);

        // Free allocated memory in case of error
        if (invalid)
        {
            for (i = 0; i < invalid; i++)
            {
                switch (rational->functions[i].type)
                {
                case FT_RATIONAL_FUNCTION:
                    free_ft_rationalfunction(&(rational->functions[i].data.rf));
                    break;
                case FT_GENERAL_RATIONAL_FUNCTION:
                    free_ft_generalrationalfunction(&(rational->functions[i].data.grf));
                    break;
                default:
                    break;
                }
            }
            free(rational->functions);
        }
        else
        {
            // Read rational/data
            strcpy(path2, path);
            strcat(path2, G_DATA);
            if (H5Lexists(file_id, path2, H5P_DEFAULT) > 0)
                if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
                    if (nb_dims == 2)
                        if (H5LTget_dataset_info(file_id, path2, rational->dims, &type_class, &length) >= 0)
                            if (type_class == H5T_STRING)
                                if (read_string_dataset(file_id, path2, (rational->dims[0]) * (rational->dims[1]), length, &(rational->data)))
                                    success = TRUE;
        }
    }
    free(path2);
    if (success)
    {
        rational->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(rational->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read dataset \"%s\". *****\n\n", path2);
    return success;
}


// Read dataset, return TRUE (all OK) or FALSE (no malloc)
char read_ft_dataset (hid_t file_id, const char *path, dataset_t *dataset)
{
    char success = FALSE;
    size_t length;
    hsize_t total_size = 1;
    int i;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE};

    if (H5Lexists(file_id, path, H5P_DEFAULT) > 0)
        if (H5LTget_dataset_ndims(file_id, path, &(dataset->nb_dims)) >= 0)
            if (dataset->nb_dims > 0)
            {
                dataset->dims = (hsize_t *) malloc((size_t) (dataset->nb_dims * sizeof(hsize_t)));
                if (H5LTget_dataset_info(file_id, path, dataset->dims, &(dataset->type_class), &length) >= 0)
                {
                    for (i = 0; i < dataset->nb_dims; i++)
                        total_size *= dataset->dims[i];
                    switch (dataset->type_class)
                    {
                    case H5T_INTEGER:
                        if (read_int_dataset(file_id, path, total_size, &(dataset->values.i)))
                            success = TRUE;
                        break;
                    case H5T_FLOAT:
                        if (read_float_dataset(file_id, path, total_size, &(dataset->values.f)))
                            success = TRUE;
                        break;
                    case H5T_COMPOUND:
                        if (read_complex_dataset(file_id, path, total_size, &(dataset->values.c)))
                            success = TRUE;
                        break;
                    case H5T_STRING:
                        if (read_string_dataset(file_id, path, total_size, length, &(dataset->values.s)))
                            success = TRUE;
                        break;
                    default:
                        break;
                    }
                }
                if (!success)
                    free(dataset->dims);
            }
    if (success)
    {
        dataset->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(dataset->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read dataset \"%s\". *****\n\n", path);
    return success;
}


// Read arraySet, return TRUE (all OK) or FALSE (no malloc)
char read_ft_arrayset (hid_t file_id, const char *path, arrayset_t *arrayset)
{
    char success = FALSE;
    children_t children;
    char *path2;
    hsize_t i, invalid = 0;
    char mandatory[][ATTRIBUTE_LENGTH] = {A_FLOATING_TYPE};

    path2 = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));

    strcpy(path2, path);
    strcat(path2, "/data");
    if (read_ft_dataset(file_id, path2, &(arrayset->data)))
    {
        strcpy(path2, path);
        strcat(path2, "/ds");
        children = read_children_name(file_id, path2);
        arrayset->nb_dims = children.nb_children;
        arrayset->dims = (vector_t *) malloc((size_t) children.nb_children * sizeof(vector_t));
        for (i = 0; i < children.nb_children; i++)
        {
            if (!invalid)
            {
                strcpy(path2, path);
                strcat(path2, "/ds");
                strcat(path2, children.childnames[i]);
                if(read_ft_vector(file_id, path2, arrayset->dims + i))
                    invalid = i;
            }
            free(children.childnames[i]);
        }
        free(children.childnames);

        if (invalid)
        {
            for (i = 0; i < invalid; i++)
                free_ft_vector(arrayset->dims + i);
            free(arrayset->dims);
        }
        else
            success = TRUE;
    }
    free(path2);
    if (success)
    {
        arrayset->name = get_name_from_path(path);
        read_optional_attributes(file_id, path, &(arrayset->optional_attributes), mandatory, sizeof(mandatory)/ATTRIBUTE_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read arraySet \"%s\". *****\n\n", path);
    return success;
}

// Read floatingType structure, return TRUE (all OK) or FALSE (no malloc)
char read_floatingtype(hid_t file_id, const char *path, floatingtype_t *floatingtype)
{
    char success = FALSE;
    char* buf;

    if (read_str_attribute(file_id, path, A_FLOATING_TYPE, &buf))
    {
        if (strcmp(buf, V_SINGLE_INTEGER) == 0)
        {
            floatingtype->type = FT_SINGLE_INTEGER;
            if (read_ft_singleinteger(file_id, path, &(floatingtype->data.singleinteger)))
                success = TRUE;
        }
        else if (strcmp(buf, V_SINGLE_REAL) == 0)
        {
            floatingtype->type = FT_SINGLE_REAL;
            if (read_ft_singlereal(file_id, path, &(floatingtype->data.singlereal)))
                success = TRUE;
        }
        else if (strcmp(buf, V_SINGLE_COMPLEX) == 0)
        {
            floatingtype->type = FT_SINGLE_COMPLEX;
            if (read_ft_singlecomplex(file_id, path, &(floatingtype->data.singlecomplex)))
                success = TRUE;
        }
        else if (strcmp(buf, V_SINGLE_STRING) == 0)
        {
            floatingtype->type = FT_SINGLE_STRING;
            if (read_ft_singlestring(file_id, path, &(floatingtype->data.singlestring)))
                success = TRUE;
        }
        else if (strcmp(buf, V_VECTOR) == 0)
        {
            floatingtype->type = FT_VECTOR;
            if (read_ft_vector(file_id, path, &(floatingtype->data.vector)))
                success = TRUE;
        }
        else if (strcmp(buf, V_LINEARLISTOFREAL1) == 0)
        {
            floatingtype->type = FT_LINEARLISTOFREAL1;
            if (read_ft_linearlistofreal1(file_id, path, &(floatingtype->data.linearlistofreal1)))
                success = TRUE;
        }
        else if (strcmp(buf, V_LINEARLISTOFREAL2) == 0)
        {
            floatingtype->type = FT_LINEARLISTOFREAL2;
            if (read_ft_linearlistofreal2(file_id, path, &(floatingtype->data.linearlistofreal2)))
                success = TRUE;
        }
        else if (strcmp(buf, V_LOGARITHMLISTOFREAL) == 0)
        {
            floatingtype->type = FT_LOGARITHMLISTOFREAL;
            if (read_ft_logarithmlistofreal(file_id, path, &(floatingtype->data.logarithmlistofreal)))
                success = TRUE;
        }
        else if (strcmp(buf, V_PERDECADELISTOFREAL) == 0)
        {
            floatingtype->type = FT_PERDECADELISTOFREAL;
            if (read_ft_perdecadelistofreal(file_id, path, &(floatingtype->data.perdecadelistofreal)))
                success = TRUE;
        }
        else if (strcmp(buf, V_LINEARLISTOFINTEGER2) == 0)
        {
            floatingtype->type = FT_LINEARLISTOFINTEGER2;
            if (read_ft_linearlistofinteger2(file_id, path, &(floatingtype->data.linearlistofinteger2)))
                success = TRUE;
        }
        else if (strcmp(buf, V_RATIONAL_FUNCTION) == 0)
        {
            floatingtype->type = FT_RATIONAL_FUNCTION;
            if (read_ft_rationalfunction(file_id, path, &(floatingtype->data.rationalfunction)))
                success = TRUE;
        }
        else if (strcmp(buf, V_GENERAL_RATIONAL_FUNCTION) == 0)
        {
            floatingtype->type = FT_GENERAL_RATIONAL_FUNCTION;
            if (read_ft_generalrationalfunction(file_id, path, &(floatingtype->data.generalrationalfunction)))
                success = TRUE;
        }
        else if (strcmp(buf, V_RATIONAL) == 0)
        {
            floatingtype->type = FT_RATIONAL;
            if (read_ft_rational (file_id, path, &(floatingtype->data.rational)))
                success = TRUE;
        }
        else if (strcmp(buf, V_DATASET) == 0)
        {
            floatingtype->type = FT_DATASET;
            if (read_ft_dataset(file_id, path, &(floatingtype->data.dataset)))
                success = TRUE;
        }
        else if (strcmp(buf, V_ARRAYSET) == 0)
        {
            floatingtype->type = FT_ARRAYSET;
            if (read_ft_arrayset(file_id, path, &(floatingtype->data.arrayset)))
                success = TRUE;
        }
        else
            printf("***** ERROR: Invalid attribute \"floatingType\" in \"%s\". *****\n\n", path);
        free(buf);
        buf = NULL;
    }
    else
        printf("***** ERROR: Cannot read mandatory attribute \"%s@%s\". *****\n\n", path, A_FLOATING_TYPE);
    if (!success)
        floatingtype->type = FT_INVALID;
    return success;
}




// Print singleInteger
void print_ft_singleinteger (singleinteger_t singleinteger, int space)
{
    printf("%*s-%s:\n", space, "", singleinteger.name);
    print_int_attribute(A_VALUE, singleinteger.value, space + 3);
    print_optional_attributes(singleinteger.optional_attributes, space + 3);
}


// Print singleReal
void print_ft_singlereal (singlereal_t singlereal, int space)
{
    printf("%*s-%s:\n", space, "", singlereal.name);
    print_flt_attribute(A_VALUE, singlereal.value, space + 3);
    print_optional_attributes(singlereal.optional_attributes, space + 3);
}


// Print singleComplex
void print_ft_singlecomplex (singlecomplex_t singlecomplex, int space)
{
    printf("%*s-%s:\n", space, "", singlecomplex.name);
    print_cpx_attribute(A_VALUE, singlecomplex.value, space + 3);
    print_optional_attributes(singlecomplex.optional_attributes, space + 3);
}


// Print singleString
void print_ft_singlestring (singlestring_t singlestring, int space)
{
    printf("%*s-%s:\n", space, "", singlestring.name);
    print_str_attribute(A_VALUE, singlestring.value, space + 3);
    print_optional_attributes(singlestring.optional_attributes, space + 3);
}


// Print vector
void print_ft_vector (vector_t vector, int space)
{
    hsize_t i;

    printf("%*s-%s [%lu]: {", space, "", vector.name,(long unsigned) vector.nb_values);
    switch (vector.type_class)
    {
    case H5T_INTEGER:
        for (i = 0; i < vector.nb_values - 1; i++)
            printf("%i, ", vector.values.i[i]);
        printf("%i}\n", vector.values.i[vector.nb_values - 1]);
        break;
    case H5T_FLOAT:
        for (i = 0; i < vector.nb_values - 1; i++)
            printf("%g, ", vector.values.f[i]);
        printf("%g}\n", vector.values.f[vector.nb_values - 1]);
        break;
    case H5T_COMPOUND:
        for (i = 0; i < vector.nb_values - 1; i++)
            printf("%g%+gi, ", creal(vector.values.c[i]), cimag(vector.values.c[i]));
        printf("%g%+gi}\n", creal(vector.values.c[vector.nb_values - 1]), cimag(vector.values.c[vector.nb_values - 1]));
        break;
    case H5T_STRING:
        for (i = 0; i < vector.nb_values - 1; i++)
            printf("\"%s\", ", vector.values.s[i]);
        printf("\"%s\"}\n", vector.values.s[vector.nb_values - 1]);
        break;
    default:
        break;
    }
    print_optional_attributes(vector.optional_attributes, space + 3);
}


// Print linearListOfReal1
void print_ft_linearlistofreal1 (linearlistofreal1_t linearlistofreal1, int space)
{
    printf("%*s-%s: %s=%g, %s=%g, %s=%i\n", space, "",linearlistofreal1.name, A_FIRST, linearlistofreal1.first, A_LAST, linearlistofreal1.last, A_NUMBER_OF_VALUES, linearlistofreal1.number_of_values);
    print_optional_attributes(linearlistofreal1.optional_attributes, space + 3);
}


// Print linearListOfReal2
void print_ft_linearlistofreal2 (linearlistofreal2_t linearlistofreal2, int space)
{
    printf("%*s-%s: %s=%g, %s=%g, %s=%i\n", space, "",linearlistofreal2.name, A_FIRST, linearlistofreal2.first, A_STEP, linearlistofreal2.step, A_NUMBER_OF_VALUES, linearlistofreal2.number_of_values);
    print_optional_attributes(linearlistofreal2.optional_attributes, space + 3);
}


// Print logarithmListOfReal
void print_ft_logarithmlistofreal (logarithmlistofreal_t logarithmlistofreal, int space)
{
    printf("%*s-%s: %s=%g, %s=%g, %s=%i\n", space, "", logarithmlistofreal.name, A_FIRST, logarithmlistofreal.first, A_LAST, logarithmlistofreal.last, A_NUMBER_OF_VALUES, logarithmlistofreal.number_of_values);
    print_optional_attributes(logarithmlistofreal.optional_attributes, space + 3);
}


// Print perDecadeListOfReal
void print_ft_perdecadelistofreal (perdecadelistofreal_t perdecadelistofreal, int space)
{
    printf("%*s-%s: %s=%g, %s=%i, %s=%i\n", space, "", perdecadelistofreal.name, A_FIRST, perdecadelistofreal.first, A_NUMBER_OF_DECADES, perdecadelistofreal.number_of_decades, A_NUMBER_OF_VALUES_PER_DECADE, perdecadelistofreal.number_of_values_per_decade);
    print_optional_attributes(perdecadelistofreal.optional_attributes, space + 3);
}


// Print linearListOfInteger2
void print_ft_linearlistofinteger2 (linearlistofinteger2_t linearlistofinteger2, int space)
{
    printf("%*s-%s: %s=%i, %s=%i, %s=%i\n", space, "",linearlistofinteger2.name, A_FIRST, linearlistofinteger2.first, A_STEP, linearlistofinteger2.step, A_NUMBER_OF_VALUES, linearlistofinteger2.number_of_values);
    print_optional_attributes(linearlistofinteger2.optional_attributes, space + 3);
}


// Print rationalFunction
void print_ft_rationalfunction (rationalfunction_t rationalfunction, int space)
{
    hsize_t i;

    printf("%*s-%s [%lu]: ", space, "",rationalfunction.name, (long unsigned) rationalfunction.nb_types);
    for (i = 0; i < rationalfunction.nb_types - 1; i++)
    {
        printf("type%i=%g|%g|%g, ", rationalfunction.types[i], rationalfunction.a[i], rationalfunction.b[i], rationalfunction.f[i]);
    }
    printf("type%i=%g|%g|%g\n", rationalfunction.types[rationalfunction.nb_types - 1], rationalfunction.a[rationalfunction.nb_types - 1], rationalfunction.b[rationalfunction.nb_types - 1], rationalfunction.f[rationalfunction.nb_types - 1]);
    print_optional_attributes(rationalfunction.optional_attributes, space + 3);
}


// Print generalRationalFunction
void print_ft_generalrationalfunction (generalrationalfunction_t generalrationalfunction, int space)
{
    int i;

    printf("%*s-%s [%i]:\n", space, "",generalrationalfunction.name, generalrationalfunction.nb_degrees);
    for (i = 0; i < generalrationalfunction.nb_degrees; i++)
        printf("%*s-degree %i: numerator=%g%+gi, denominator=%g%+gi\n", space + 3, "", i, creal(generalrationalfunction.numerator[i]), cimag(generalrationalfunction.numerator[i]), creal(generalrationalfunction.denominator[i]), cimag(generalrationalfunction.denominator[i]));
    print_optional_attributes(generalrationalfunction.optional_attributes, space + 3);
}


// Print rational
void print_ft_rational (rational_t rational, int space)
{
    hsize_t i, total;
    int j;

    printf("%*s-%s:\n", space, "", rational.name);
    print_optional_attributes(rational.optional_attributes, space + 3);
    printf("%*s-@%s [%lux%lu]: {", space + 3, "", A_VALUE, (long unsigned) rational.dims[0], (long unsigned) rational.dims[1]);
    if (rational.data != NULL)
    {
        total = (rational.dims[0]) * (rational.dims[1]);
        for (i = 0; i < total - 1; i++)
        {
            printf("\"%s\", ", rational.data[i]);
        }
        printf("\"%s\"", rational.data[total - 1]);
    }
    printf("}\n");
    // Print functions inside rational
    for (j = 0; j < rational.nb_functions; j++)
    {
        switch (rational.functions[j].type)
        {
        case FT_RATIONAL_FUNCTION:
            print_ft_rationalfunction(rational.functions[j].data.rf, space + 6);
            break;
        case FT_GENERAL_RATIONAL_FUNCTION:
            print_ft_generalrationalfunction(rational.functions[j].data.grf, space + 6);
            break;
        default:
            break;
        }
    }

}


// Print dataset
void print_ft_dataset (dataset_t dataset, int space)
{
    hsize_t i, total = 1;
    int j;

    printf("%*s-%s [", space, "", dataset.name);
    for (j = 0; j < dataset.nb_dims - 1; j++)
    {
        printf("%lux", (long unsigned) dataset.dims[j]);
        total *= dataset.dims[j];
    }
    printf("%lu]: {", (long unsigned) dataset.dims[dataset.nb_dims - 1]);
    total *= dataset.dims[dataset.nb_dims - 1];

    switch (dataset.type_class)
    {
    case H5T_INTEGER:
        for (i = 0; i < total - 1; i++)
        {
            printf("%i, ", dataset.values.i[i]);
        }
        printf("%i}\n", dataset.values.i[total - 1]);
        break;
    case H5T_FLOAT:
        for (i = 0; i < total - 1; i++)
        {
            printf("%g, ", dataset.values.f[i]);
        }
        printf("%g}\n", dataset.values.f[total - 1]);
        break;
    case H5T_COMPOUND:
        for (i = 0; i < total - 1; i++)
        {
            printf("%g%+gi, ", creal(dataset.values.c[i]), cimag(dataset.values.c[i]));
        }
        printf("%g%+gi}\n", creal(dataset.values.c[total - 1]), cimag(dataset.values.c[total - 1]));
        break;
    case H5T_STRING:
        if (dataset.values.s != NULL)
        {
            for (i = 0; i < total - 1; i++)
            {
                printf("\"%s\", ", dataset.values.s[i]);
            }
            printf("\"%s\"}\n", dataset.values.s[total - 1]);
        }
        break;
    default:
        break;
    }
    print_optional_attributes(dataset.optional_attributes, space + 3);
}


// Print arrayset
void print_ft_arrayset (arrayset_t arrayset, int space)
{
    hsize_t i;

    printf("%*s-%s:\n", space, "", arrayset.name);
    print_optional_attributes(arrayset.optional_attributes, space + 4);
    print_ft_dataset(arrayset.data, space + 2);
    for (i = 0; i < arrayset.nb_dims; i++)
        print_ft_vector(arrayset.dims[i], space + 2);
}


// Print floatingType structure
void print_floatingtype (floatingtype_t floatingtype, int space)
{
    switch(floatingtype.type)
    {
    case FT_SINGLE_INTEGER:
        print_ft_singleinteger(floatingtype.data.singleinteger, space);
        break;
    case FT_SINGLE_REAL:
        print_ft_singlereal(floatingtype.data.singlereal, space);
        break;
    case FT_SINGLE_COMPLEX:
        print_ft_singlecomplex(floatingtype.data.singlecomplex, space);
        break;
    case FT_SINGLE_STRING:
        print_ft_singlestring(floatingtype.data.singlestring, space);
        break;
    case FT_VECTOR:
        print_ft_vector(floatingtype.data.vector, space);
        break;
    case FT_LINEARLISTOFREAL1:
        print_ft_linearlistofreal1(floatingtype.data.linearlistofreal1, space);
        break;
    case FT_LINEARLISTOFREAL2:
        print_ft_linearlistofreal2(floatingtype.data.linearlistofreal2, space);
        break;
    case FT_LOGARITHMLISTOFREAL:
        print_ft_logarithmlistofreal(floatingtype.data.logarithmlistofreal, space);
        break;
    case FT_PERDECADELISTOFREAL:
        print_ft_perdecadelistofreal(floatingtype.data.perdecadelistofreal, space);
        break;
    case FT_LINEARLISTOFINTEGER2:
        print_ft_linearlistofinteger2(floatingtype.data.linearlistofinteger2, space);
        break;
    case FT_RATIONAL_FUNCTION:
        print_ft_rationalfunction(floatingtype.data.rationalfunction, space);
        break;
    case FT_GENERAL_RATIONAL_FUNCTION:
        print_ft_generalrationalfunction(floatingtype.data.generalrationalfunction, space);
        break;
    case FT_RATIONAL:
        print_ft_rational(floatingtype.data.rational, space);
        break;
    case FT_DATASET:
        print_ft_dataset(floatingtype.data.dataset, space);
        break;
    case FT_ARRAYSET:
        print_ft_arrayset(floatingtype.data.arrayset, space);
        break;
    default:
        break;
    }
}




// Free memory used by singleInteger
void free_ft_singleinteger (singleinteger_t *singleinteger)
{
    if (singleinteger->name != NULL)
    {
        free(singleinteger->name);
        singleinteger->name = NULL;
    }
    free_optional_attributes(&(singleinteger->optional_attributes));
}


// Free memory used by singleReal
void free_ft_singlereal (singlereal_t *singlereal)
{
    if (singlereal->name != NULL)
    {
        free(singlereal->name);
        singlereal->name = NULL;
    }
    free_optional_attributes(&(singlereal->optional_attributes));
}


// Free memory used by singleComplex
void free_ft_singlecomplex (singlecomplex_t *singlecomplex)
{
    if (singlecomplex->name != NULL)
    {
        free(singlecomplex->name);
        singlecomplex->name = NULL;
    }
    free_optional_attributes(&(singlecomplex->optional_attributes));
}


// Free memory used by singleString
void free_ft_singlestring (singlestring_t *singlestring)
{
    if (singlestring->name != NULL)
    {
        free(singlestring->name);
        singlestring->name = NULL;
    }
    if (singlestring->value != NULL)
    {
        free(singlestring->value);
        singlestring->value = NULL;
    }
    free_optional_attributes(&(singlestring->optional_attributes));
}


// Free memory used by vector
void free_ft_vector (vector_t *vector)
{
    if (vector->name != NULL)
    {
        free(vector->name);
        vector->name = NULL;
    }
    free_optional_attributes(&(vector->optional_attributes));
    switch (vector->type_class)
    {
    case H5T_INTEGER:
        if (vector->values.i != NULL)
        {
            free(vector->values.i);
            vector->values.i = NULL;
        }
        break;
    case H5T_FLOAT:
        if (vector->values.f != NULL)
        {
            free(vector->values.f);
            vector->values.f = NULL;
        }
        break;
    case H5T_COMPOUND:
        if (vector->values.c != NULL)
        {
            free(vector->values.c);
            vector->values.c = NULL;
        }
        break;
    case H5T_STRING:
        if (vector->values.s != NULL)
        {
            free(vector->values.s);
            vector->values.s = NULL;
        }
        break;
    default:
        break;
    }
    vector->nb_values = 0;
}


// Free memory used by linearListOfReal1
void free_ft_linearlistofreal1 (linearlistofreal1_t *linearlistofreal1)
{
    if (linearlistofreal1->name != NULL)
    {
        free(linearlistofreal1->name);
        linearlistofreal1->name = NULL;
    }
    free_optional_attributes(&(linearlistofreal1->optional_attributes));
}


// Free memory used by linearListOfReal2
void free_ft_linearlistofreal2 (linearlistofreal2_t *linearlistofreal2)
{
    if (linearlistofreal2->name != NULL)
    {
        free(linearlistofreal2->name);
        linearlistofreal2->name = NULL;
    }
    free_optional_attributes(&(linearlistofreal2->optional_attributes));
}


// Free memory used by logarithmListOfReal
void free_ft_logarithmlistofreal (logarithmlistofreal_t *logarithmlistofreal)
{
    if (logarithmlistofreal->name != NULL)
    {
        free(logarithmlistofreal->name);
        logarithmlistofreal->name = NULL;
    }
    free_optional_attributes(&(logarithmlistofreal->optional_attributes));
}


// Free memory used by perDecadeListOfReal
void free_ft_perdecadelistofreal (perdecadelistofreal_t *perdecadelistofreal)
{
    if (perdecadelistofreal->name != NULL)
    {
        free(perdecadelistofreal->name);
        perdecadelistofreal->name = NULL;
    }
    free_optional_attributes(&(perdecadelistofreal->optional_attributes));
}


// Free memory used by linearListOfInteger2
void free_ft_linearlistofinteger2 (linearlistofinteger2_t *linearlistofinteger2)
{
    if (linearlistofinteger2->name != NULL)
    {
        free(linearlistofinteger2->name);
        linearlistofinteger2->name = NULL;
    }
    free_optional_attributes(&(linearlistofinteger2->optional_attributes));
}


// Free memory used by rationalFunction
void free_ft_rationalfunction (rationalfunction_t *rationalfunction)
{
    if (rationalfunction->name != NULL)
    {
        free(rationalfunction->name);
        rationalfunction->name = NULL;
    }
    if (rationalfunction->types != NULL)
    {
        free(rationalfunction->types);
        rationalfunction->types = NULL;
    }
    if (rationalfunction->a != NULL)
    {
        free(rationalfunction->a);
        rationalfunction->a = NULL;
    }
    if (rationalfunction->b != NULL)
    {
        free(rationalfunction->b);
        rationalfunction->b = NULL;
    }
    if (rationalfunction->f != NULL)
    {
        free(rationalfunction->f);
        rationalfunction->f = NULL;
    }
    rationalfunction->nb_types = 0;
    free_optional_attributes(&(rationalfunction->optional_attributes));
}


// Free memory used by generalRationalFunction
void free_ft_generalrationalfunction (generalrationalfunction_t *generalrationalfunction)
{
    if (generalrationalfunction->name != NULL)
    {
        free(generalrationalfunction->name);
        generalrationalfunction->name = NULL;
    }

    if (generalrationalfunction->numerator != NULL)
    {
        free(generalrationalfunction->numerator);
        generalrationalfunction->numerator = NULL;
    }

    if (generalrationalfunction->denominator != NULL)
    {
        free(generalrationalfunction->denominator);
        generalrationalfunction->denominator = NULL;
    }
    generalrationalfunction->nb_degrees = 0;
    free_optional_attributes(&(generalrationalfunction->optional_attributes));
}


// Free memory used by rational
void free_ft_rational (rational_t *rational)
{
    int j;

    if (rational->name != NULL)
    {
        free(rational->name);
        rational->name = NULL;
    }
    free_optional_attributes(&(rational->optional_attributes));

    if (rational->nb_functions > 0)
    {
        for (j = 0; j < rational->nb_functions; j++)
        {
            switch (rational->functions[j].type)
            {
            case FT_RATIONAL_FUNCTION:
                free_ft_rationalfunction(&(rational->functions[j].data.rf));
                break;
            case FT_GENERAL_RATIONAL_FUNCTION:
                free_ft_generalrationalfunction(&(rational->functions[j].data.grf));
                break;
            default:
                break;
            }
        }
        free(rational->functions);
        rational->functions = NULL;
        rational->nb_functions = 0;
    }
    if (rational->data != NULL)
    {
        free(*(rational->data));
        free(rational->data);
        rational->dims[0] = 0;
        rational->dims[1] = 0;
    }
}


// Free memory used by dataset
void free_ft_dataset (dataset_t *dataset)
{
    if (dataset->name != NULL)
    {
        free(dataset->name);
        dataset->name = NULL;
    }
    free_optional_attributes(&(dataset->optional_attributes));
    if (dataset->dims != NULL)
    {
        free(dataset->dims);
        dataset->dims = NULL;
        dataset->nb_dims = 0;
    }
    switch (dataset->type_class)
    {
    case H5T_INTEGER:
        if (dataset->values.i != NULL)
        {
            free(dataset->values.i);
            dataset->values.i = NULL;
        }
        break;
    case H5T_FLOAT:
        if (dataset->values.f != NULL)
        {
            free(dataset->values.f);
            dataset->values.f = NULL;
        }
        break;
    case H5T_COMPOUND:
        if (dataset->values.c != NULL)
        {
            free(dataset->values.c);
            dataset->values.c = NULL;
        }
        break;
    case H5T_STRING:
        if (dataset->values.s != NULL)
        {
            free(dataset->values.s);
            dataset->values.s = NULL;
        }
        break;
    default:
        break;
    }
}


// Free memory used by arrayset
void free_ft_arrayset (arrayset_t *arrayset)
{
    hsize_t i;

    if (arrayset->name != NULL)
    {
        free(arrayset->name);
        arrayset->name = NULL;
    }
    free_optional_attributes(&(arrayset->optional_attributes));
    if (arrayset->nb_dims > 0)
    {
        free_ft_dataset(&(arrayset->data));
        for (i = 0; i < arrayset->nb_dims; i++)
            free_ft_vector(arrayset->dims + i);
        free(arrayset->dims);
        arrayset->nb_dims = 0;
    }
}


// Free memory used by floatingType structure
void free_floatingtype (floatingtype_t *floatingtype)
{
    switch(floatingtype->type)
    {
    case FT_SINGLE_INTEGER:
        free_ft_singleinteger(&(floatingtype->data.singleinteger));
        break;
    case FT_SINGLE_REAL:
        free_ft_singlereal(&(floatingtype->data.singlereal));
        break;
    case FT_SINGLE_COMPLEX:
        free_ft_singlecomplex(&(floatingtype->data.singlecomplex));
        break;
    case FT_SINGLE_STRING:
        free_ft_singlestring(&(floatingtype->data.singlestring));
        break;
    case FT_VECTOR:
        free_ft_vector(&(floatingtype->data.vector));
        break;
    case FT_LINEARLISTOFREAL1:
        free_ft_linearlistofreal1(&(floatingtype->data.linearlistofreal1));
        break;
    case FT_LINEARLISTOFREAL2:
        free_ft_linearlistofreal2(&(floatingtype->data.linearlistofreal2));
        break;
    case FT_LOGARITHMLISTOFREAL:
        free_ft_logarithmlistofreal(&(floatingtype->data.logarithmlistofreal));
        break;
    case FT_PERDECADELISTOFREAL:
        free_ft_perdecadelistofreal(&(floatingtype->data.perdecadelistofreal));
        break;
    case FT_LINEARLISTOFINTEGER2:
        free_ft_linearlistofinteger2(&(floatingtype->data.linearlistofinteger2));
        break;
    case FT_RATIONAL_FUNCTION:
        free_ft_rationalfunction(&(floatingtype->data.rationalfunction));
        break;
    case FT_GENERAL_RATIONAL_FUNCTION:
        free_ft_generalrationalfunction(&(floatingtype->data.generalrationalfunction));
        break;
    case FT_RATIONAL:
        free_ft_rational(&(floatingtype->data.rational));
        break;
    case FT_DATASET:
        free_ft_dataset(&(floatingtype->data.dataset));
        break;
    case FT_ARRAYSET:
        free_ft_arrayset(&(floatingtype->data.arrayset));
        break;
    default:
        break;
    }
}

