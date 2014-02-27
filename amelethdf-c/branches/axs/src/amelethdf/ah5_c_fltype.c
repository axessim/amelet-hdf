#include "ah5_c_fltype.h"


// Read singleInteger, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_singleinteger (hid_t file_id, const char *path, AH5_singleinteger_t *singleinteger)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE, AH5_A_VALUE};
    char rdata = AH5_TRUE;

    if (AH5_read_int_attr(file_id, path, AH5_A_VALUE, &(singleinteger->value)))
    {
        singleinteger->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(singleinteger->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
    {
        AH5_print_err_attr("", path, AH5_A_VALUE);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read singleReal, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_singlereal (hid_t file_id, const char *path, AH5_singlereal_t *singlereal)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE, AH5_A_VALUE};
    char rdata = AH5_TRUE;

    if (AH5_read_flt_attr(file_id, path, AH5_A_VALUE, &(singlereal->value)))
    {
        singlereal->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(singlereal->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
    {
        AH5_print_err_attr("", path, AH5_A_VALUE);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read singleComplex, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_singlecomplex (hid_t file_id, const char *path, AH5_singlecomplex_t *singlecomplex)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE, AH5_A_VALUE};
    char rdata = AH5_TRUE;

    if (AH5_read_cpx_attr(file_id, path, AH5_A_VALUE, &(singlecomplex->value)))
    {
        singlecomplex->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(singlecomplex->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
    {
        AH5_print_err_attr("", path, AH5_A_VALUE);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read singleString, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_singlestring (hid_t file_id, const char *path, AH5_singlestring_t *singlestring)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE, AH5_A_VALUE};
    char rdata = AH5_TRUE;

    if(AH5_read_str_attr(file_id, path, AH5_A_VALUE, &(singlestring->value)))
    {
        singlestring->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(singlestring->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
    {
        AH5_print_err_attr("", path, AH5_A_VALUE);
        rdata = AH5_FALSE;
    }
    return rdata;
}


// Read vector, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_vector (hid_t file_id, const char *path, AH5_vector_t *vector)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE};
    char rdata = AH5_FALSE;
    size_t length;
    int nb_dims;

    vector->nb_values = 1;  // in case of single value
    if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
        if (nb_dims <= 1)
            if (H5LTget_dataset_info(file_id, path, &(vector->nb_values), &(vector->type_class), &length) >= 0)
                switch (vector->type_class)
                {
                case H5T_INTEGER:
                    if (AH5_read_int_dataset(file_id, path, vector->nb_values, &(vector->values.i)))
                        rdata = AH5_TRUE;
                    break;
                case H5T_FLOAT:
                    if (AH5_read_flt_dataset(file_id, path, vector->nb_values, &(vector->values.f)))
                        rdata = AH5_TRUE;
                    break;
                case H5T_COMPOUND:
                    if (AH5_read_cpx_dataset(file_id, path, vector->nb_values, &(vector->values.c)))
                        rdata = AH5_TRUE;
                    break;
                case H5T_STRING:
                    if (AH5_read_str_dataset(file_id, path, vector->nb_values, length, &(vector->values.s)))
                        rdata = AH5_TRUE;
                    break;
                default:
                    break;
                }
    if (rdata)
    {
        vector->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(vector->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
    {
        AH5_print_err_dset("", path);
        vector->nb_values = 0;
    }
    return rdata;
}


// Read linearListOfReal1, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_linearlistofreal1 (hid_t file_id, const char *path, AH5_linearlistofreal1_t *linearlistofreal1)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE, AH5_A_FIRST, AH5_A_LAST, AH5_A_NUMBER_OF_VALUES};
    char rdata = AH5_FALSE;

    linearlistofreal1->first = 0;
    linearlistofreal1->last = 0;
    linearlistofreal1->number_of_values = 0;
    if (H5Aexists_by_name(file_id, path, AH5_A_FIRST, H5P_DEFAULT) > 0 && H5Aexists_by_name(file_id, path, AH5_A_LAST, H5P_DEFAULT) > 0)  // interval doesn't have AH5_A_NUMBER_OF_VALUES
        if (H5LTget_attribute_float(file_id, path, AH5_A_FIRST, &(linearlistofreal1->first)) >= 0 && H5LTget_attribute_float(file_id, path, AH5_A_LAST, &(linearlistofreal1->last)) >= 0)
            rdata = AH5_TRUE;
    if (H5Aexists_by_name(file_id, path, AH5_A_NUMBER_OF_VALUES, H5P_DEFAULT) > 0)
        if (H5LTget_attribute_int(file_id, path, AH5_A_NUMBER_OF_VALUES, &(linearlistofreal1->number_of_values)) < 0)  // AH5_A_NUMBER_OF_VALUES is present -> must be read
            rdata = AH5_FALSE;
    if (rdata)
    {
        linearlistofreal1->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(linearlistofreal1->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read mandatory attributes in \"%s\". *****\n\n", path);
    return rdata;
}


// Read linearListOfReal2, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_linearlistofreal2 (hid_t file_id, const char *path, AH5_linearlistofreal2_t *linearlistofreal2)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE, AH5_A_FIRST, AH5_A_STEP, AH5_A_NUMBER_OF_VALUES};
    char rdata = AH5_FALSE;

    linearlistofreal2->first = 0;
    linearlistofreal2->step = 0;
    linearlistofreal2->number_of_values = 0;
    if (H5Aexists_by_name(file_id, path, AH5_A_FIRST, H5P_DEFAULT) > 0
            && H5Aexists_by_name(file_id, path, AH5_A_STEP, H5P_DEFAULT) > 0
            && H5Aexists_by_name(file_id, path, AH5_A_NUMBER_OF_VALUES, H5P_DEFAULT) > 0)
        if (H5LTget_attribute_float(file_id, path, AH5_A_FIRST, &(linearlistofreal2->first)) >= 0
                && H5LTget_attribute_float(file_id, path, AH5_A_STEP, &(linearlistofreal2->step)) >= 0
                && H5LTget_attribute_int(file_id, path, AH5_A_NUMBER_OF_VALUES, &(linearlistofreal2->number_of_values)) >= 0)
            rdata = AH5_TRUE;
    if (rdata)
    {
        linearlistofreal2->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(linearlistofreal2->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read mandatory attributes in \"%s\". *****\n\n", path);
    return rdata;
}


// Read logarithmListOfReal, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_logarithmlistofreal (hid_t file_id, const char *path, AH5_logarithmlistofreal_t *logarithmlistofreal)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE, AH5_A_FIRST, AH5_A_LAST, AH5_A_NUMBER_OF_VALUES};
    char rdata = AH5_FALSE;

    logarithmlistofreal->first = 0;
    logarithmlistofreal->last = 0;
    logarithmlistofreal->number_of_values = 0;
    if (H5Aexists_by_name(file_id, path, AH5_A_FIRST, H5P_DEFAULT) > 0
            && H5Aexists_by_name(file_id, path, AH5_A_LAST, H5P_DEFAULT) > 0
            && H5Aexists_by_name(file_id, path, AH5_A_NUMBER_OF_VALUES, H5P_DEFAULT) > 0)
        if (H5LTget_attribute_float(file_id, path, AH5_A_FIRST, &(logarithmlistofreal->first)) >= 0
                && H5LTget_attribute_float(file_id, path, AH5_A_LAST, &(logarithmlistofreal->last)) >= 0
                && H5LTget_attribute_int(file_id, path, AH5_A_NUMBER_OF_VALUES, &(logarithmlistofreal->number_of_values)) >= 0)
            rdata = AH5_TRUE;
    if (rdata)
    {
        logarithmlistofreal->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(logarithmlistofreal->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read mandatory attributes in \"%s\". *****\n\n", path);
    return rdata;
}


// Read perDecadeListOfReal, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_perdecadelistofreal (hid_t file_id, const char *path, AH5_perdecadelistofreal_t *perdecadelistofreal)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE, AH5_A_NUMBER_OF_DECADES, AH5_A_NUMBER_OF_VALUES_PER_DECADE};
    char rdata = AH5_FALSE;

    perdecadelistofreal->first = 0;
    perdecadelistofreal->number_of_decades = 0;
    perdecadelistofreal->number_of_values_per_decade = 0;
    if (H5Aexists_by_name(file_id, path, AH5_A_FIRST, H5P_DEFAULT) > 0
            && H5Aexists_by_name(file_id, path, AH5_A_NUMBER_OF_DECADES, H5P_DEFAULT) > 0
            && H5Aexists_by_name(file_id, path, AH5_A_NUMBER_OF_VALUES_PER_DECADE, H5P_DEFAULT) > 0)
        if (H5LTget_attribute_float(file_id, path, AH5_A_FIRST, &(perdecadelistofreal->first)) >= 0
                && H5LTget_attribute_int(file_id, path, AH5_A_NUMBER_OF_DECADES, &(perdecadelistofreal->number_of_decades)) >= 0
                && H5LTget_attribute_int(file_id, path, AH5_A_NUMBER_OF_VALUES_PER_DECADE, &(perdecadelistofreal->number_of_values_per_decade)) >= 0)
            rdata = AH5_TRUE;
    if (rdata)
    {
        perdecadelistofreal->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(perdecadelistofreal->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read mandatory attributes in \"%s\". *****\n\n", path);
    return rdata;
}


// Read linearListOfInteger2, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_linearlistofinteger2 (hid_t file_id, const char *path, AH5_linearlistofinteger2_t *linearlistofinteger2)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE, AH5_A_FIRST, AH5_A_STEP, AH5_A_NUMBER_OF_VALUES};
    char rdata = AH5_FALSE;

    linearlistofinteger2->first = 0;
    linearlistofinteger2->step = 0;
    linearlistofinteger2->number_of_values = 0;
    if (H5Aexists_by_name(file_id, path, AH5_A_FIRST, H5P_DEFAULT) > 0
            && H5Aexists_by_name(file_id, path, AH5_A_STEP, H5P_DEFAULT) > 0
            && H5Aexists_by_name(file_id, path, AH5_A_NUMBER_OF_VALUES, H5P_DEFAULT) > 0)
        if (H5LTget_attribute_int(file_id, path, AH5_A_FIRST, &(linearlistofinteger2->first)) >= 0
                && H5LTget_attribute_int(file_id, path, AH5_A_STEP, &(linearlistofinteger2->step)) >= 0
                && H5LTget_attribute_int(file_id, path, AH5_A_NUMBER_OF_VALUES, &(linearlistofinteger2->number_of_values)) >= 0)
            rdata = AH5_TRUE;
    if (rdata)
    {
        linearlistofinteger2->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(linearlistofinteger2->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read mandatory attributes in \"%s\". *****\n\n", path);
    return rdata;
}


// Read rationalFunction, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_rationalfunction (hid_t file_id, const char *path, AH5_rationalfunction_t *rationalfunction)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE};
    int type = 0, a = 1, b = 2, f = 3;
    size_t *field_offsets;
    char rdata = AH5_FALSE;
    size_t *field_sizes;
    char **field_names;
    hsize_t nfields, i;
    size_t type_size;

    if (H5TBget_table_info(file_id, path, &nfields, &(rationalfunction->nb_types)) >= 0)
        if (nfields == 4 && rationalfunction->nb_types > 0)
        {
            field_names = (char **) malloc((size_t) nfields * sizeof(char *));
			field_names[0] = (char *) malloc((size_t) nfields * AH5_TABLE_FIELD_NAME_LENGTH * sizeof(char));
            for (i = 0; i < nfields; i++)
                field_names[i] = field_names[0] + i * AH5_TABLE_FIELD_NAME_LENGTH;
			field_sizes = (size_t *) malloc((size_t ) nfields * sizeof(size_t *));
			field_offsets = (size_t *) malloc((size_t) nfields * sizeof(size_t *));

            if (H5TBget_field_info(file_id, path, field_names, field_sizes, field_offsets, &type_size) >= 0)
                if (strcmp(field_names[0], AH5_F_TYPE) == 0 && strcmp(field_names[1], AH5_F_A) == 0 && strcmp(field_names[2], AH5_F_B) == 0 && strcmp(field_names[3], AH5_F_F) == 0)
                {
                    rationalfunction->types = (int *) malloc((size_t) rationalfunction->nb_types * sizeof(int));
                    rationalfunction->a = (float *) malloc((size_t) rationalfunction->nb_types * sizeof(float));
                    rationalfunction->b = (float *) malloc((size_t) rationalfunction->nb_types * sizeof(float));
                    rationalfunction->f = (float *) malloc((size_t) rationalfunction->nb_types * sizeof(float));
                    if (H5TBread_fields_index(file_id, path, 1, &type, 0, rationalfunction->nb_types, sizeof(int), field_offsets, field_sizes, rationalfunction->types) >= 0
                            && H5TBread_fields_index(file_id, path, 1, &a, 0, rationalfunction->nb_types, sizeof(float), field_offsets, field_sizes, rationalfunction->a) >= 0
                            && H5TBread_fields_index(file_id, path, 1, &b, 0, rationalfunction->nb_types, sizeof(float), field_offsets, field_sizes, rationalfunction->b) >= 0
                            && H5TBread_fields_index(file_id, path, 1, &f, 0, rationalfunction->nb_types, sizeof(float), field_offsets, field_sizes, rationalfunction->f) >= 0)
                        rdata = AH5_TRUE;
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
    if (rdata)
    {
        rationalfunction->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(rationalfunction->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read table \"%s\". *****\n\n", path);
    return rdata;
}


// Read generalRationalFunction, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_generalrationalfunction (hid_t file_id, const char *path, AH5_generalrationalfunction_t *generalrationalfunction)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE};
    H5T_class_t type_class;
    char rdata = AH5_FALSE;
    AH5_complex_t *buf;
    hsize_t dims[2], i;
    size_t length;
    int nb_dims;

    if (H5LTget_dataset_ndims(file_id, path, &nb_dims) >= 0)
        if (nb_dims == 2)
            if (H5LTget_dataset_info(file_id, path, dims, &type_class, &length) >= 0)
                if (dims[0] > 0 && dims[1] == 2 && type_class == H5T_COMPOUND)
                {
                    generalrationalfunction->numerator = (AH5_complex_t *) malloc((size_t) dims[0] * sizeof(AH5_complex_t));
					generalrationalfunction->denominator = (AH5_complex_t *) malloc((size_t) dims[0] * sizeof(AH5_complex_t));
                    if (AH5_read_cpx_dataset(file_id, path, dims[0] * dims[1], &(buf)))
                    {
                        for (i = 0; i < dims[0]; i++)
                        {
                            generalrationalfunction->numerator[i] = buf[2*i];
                            generalrationalfunction->denominator[i] = buf[2*i + 1];
                        }
                        rdata = AH5_TRUE;
                        free(buf);
                    }
                    else
                    {
                        free(generalrationalfunction->numerator);
                        free(generalrationalfunction->denominator);
                    }
                }
    if (rdata)
    {
        generalrationalfunction->nb_degrees = dims[0];
        generalrationalfunction->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(generalrationalfunction->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read dataset \"%s\". *****\n\n", path);
    return rdata;
}


// Read rational, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_rational (hid_t file_id, const char *path, AH5_rational_t *rational)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE};
    char path2[AH5_ABSOLUTE_PATH_LENGTH];
    char *buf, rdata = AH5_FALSE;
    hsize_t i, invalid_nb = -1;
    char invalid = AH5_FALSE;
    H5T_class_t type_class;
    AH5_children_t children;
    size_t length;
    int nb_dims;

    strcpy(path2, path);
    strcat(path2, AH5_G_FUNCTION);
    children = AH5_read_children_name(file_id, path2);
    rational->nb_functions = children.nb_children;
    if (children.nb_children > 0)
    {
        // Read rational/function until error
        rational->functions = (AH5_ftr_t *) malloc((size_t) children.nb_children * sizeof(AH5_ftr_t));
        for (i = 0; i < children.nb_children; i++)
        {
            if (!invalid)
            {
                strcpy(path2, path);
                strcat(path2, AH5_G_FUNCTION);
                strcat(path2, children.childnames[i]);
                if (AH5_read_str_attr(file_id, path2, AH5_A_FLOATING_TYPE, &buf))
                {
                    if (strcmp(buf, AH5_V_RATIONAL_FUNCTION) == 0)
                    {
                        rational->functions[i].type = FT_RATIONAL_FUNCTION;
                        if(!AH5_read_ft_rationalfunction(file_id, path2, &(rational->functions[i].data.rf)))
                        {
                            invalid_nb = i;
                            invalid = AH5_TRUE;
                        }
                    }
                    else if (strcmp(buf, AH5_V_GENERAL_RATIONAL_FUNCTION) == 0)
                    {
                        rational->functions[i].type = FT_GENERAL_RATIONAL_FUNCTION;
                        if(!AH5_read_ft_generalrationalfunction(file_id, path2, &(rational->functions[i].data.grf)))
                        {
                            invalid_nb = i;
                            invalid = AH5_TRUE;
                        }                    }
                    else
                    {
                        printf("***** WARNING: Invalid attribute \"floatingType\" in \"%s\". *****\n\n", path2);
                        invalid_nb = i;
                        invalid = AH5_TRUE;
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
            for (i = 0; i < invalid_nb; i++)
            {
                switch (rational->functions[i].type)
                {
                case FT_RATIONAL_FUNCTION:
                    AH5_free_ft_rationalfunction(&(rational->functions[i].data.rf));
                    break;
                case FT_GENERAL_RATIONAL_FUNCTION:
                    AH5_free_ft_generalrationalfunction(&(rational->functions[i].data.grf));
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
            strcat(path2, AH5_G_DATA);
            if (AH5_path_valid(file_id, path2))
                if (H5LTget_dataset_ndims(file_id, path2, &nb_dims) >= 0)
                    if (nb_dims == 2)
                        if (H5LTget_dataset_info(file_id, path2, rational->dims, &type_class, &length) >= 0)
                            if (type_class == H5T_STRING)
                                if (AH5_read_str_dataset(file_id, path2, (rational->dims[0]) * (rational->dims[1]), length, &(rational->data)))
                                    rdata = AH5_TRUE;
        }
    }
    if (rdata)
    {
        rational->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(rational->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read dataset \"%s\". *****\n\n", path2);
    return rdata;
}


// Read dataset, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_dataset (hid_t file_id, const char *path, AH5_dataset_t *dataset)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE};
    hsize_t total_size = 1;
    char rdata = AH5_FALSE;
    size_t length;
    int i;

    if (H5LTget_dataset_ndims(file_id, path, &(dataset->nb_dims)) >= 0)
        if (dataset->nb_dims > 0)
        {
            dataset->dims = (hsize_t *) malloc((dataset->nb_dims * sizeof(hsize_t)));
            if (H5LTget_dataset_info(file_id, path, dataset->dims, &(dataset->type_class), &length) >= 0)
            {
                for (i = 0; i < dataset->nb_dims; i++)
                    total_size *= dataset->dims[i];
                switch (dataset->type_class)
                {
                case H5T_INTEGER:
                    if (AH5_read_int_dataset(file_id, path, total_size, &(dataset->values.i)))
                        rdata = AH5_TRUE;
                    break;
                case H5T_FLOAT:
                    if (AH5_read_flt_dataset(file_id, path, total_size, &(dataset->values.f)))
                        rdata = AH5_TRUE;
                    break;
                case H5T_COMPOUND:
                    if (AH5_read_cpx_dataset(file_id, path, total_size, &(dataset->values.c)))
                        rdata = AH5_TRUE;
                    break;
                case H5T_STRING:
                    if (AH5_read_str_dataset(file_id, path, total_size, length, &(dataset->values.s)))
                        rdata = AH5_TRUE;
                    break;
                default:
                    break;
                }
            }
            if (!rdata)
                free(dataset->dims);
        }
    if (rdata)
    {
        dataset->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(dataset->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
        AH5_print_err_dset("", path);
    return rdata;
}


// Read arraySet, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_ft_arrayset (hid_t file_id, const char *path, AH5_arrayset_t *arrayset)
{
    char mandatory[][AH5_ATTR_LENGTH] = {AH5_A_FLOATING_TYPE};
    char path2[AH5_ABSOLUTE_PATH_LENGTH];
    hsize_t i, invalid_nb = -1;
    char invalid = AH5_FALSE;
    char rdata = AH5_FALSE;
    AH5_children_t children;

    strcpy(path2, path);
    strcat(path2, AH5_G_DATA);
    if (AH5_read_ft_dataset(file_id, path2, &(arrayset->data)))
    {
        strcpy(path2, path);
        strcat(path2, AH5_G_DS);
        children = AH5_read_children_name(file_id, path2);
        arrayset->nb_dims = children.nb_children;
        arrayset->dims = (AH5_vector_t *) malloc((size_t) children.nb_children * sizeof(AH5_vector_t));
        for (i = 0; i < children.nb_children; i++)
        {
            if (!invalid)
            {
                strcpy(path2, path);
                strcat(path2, AH5_G_DS);
                strcat(path2, children.childnames[i]);
                if(!AH5_read_ft_vector(file_id, path2, arrayset->dims + i))
                {
                    invalid_nb = i;
                    invalid = AH5_TRUE;
                }
            }
            free(children.childnames[i]);
        }
        free(children.childnames);

        if (invalid)
        {
            for (i = 0; i < invalid_nb; i++)
                AH5_free_ft_vector(arrayset->dims + i);
            free(arrayset->dims);
        }
        else
            rdata = AH5_TRUE;
    }
    if (rdata)
    {
        arrayset->path = strdup(path);
        AH5_read_opt_attrs(file_id, path, &(arrayset->opt_attrs), mandatory, sizeof(mandatory)/AH5_ATTR_LENGTH);
    }
    else
        printf("***** ERROR: Cannot read arraySet \"%s\". *****\n\n", path);
    return rdata;
}

// Read floatingType structure, return AH5_TRUE (all OK) or AH5_FALSE (no malloc)
char AH5_read_floatingtype(hid_t file_id, const char *path, AH5_ft_t *floatingtype)
{
    char rdata = AH5_FALSE;
    char* buf;

    if (AH5_path_valid(file_id, path))
    {
        if (AH5_read_str_attr(file_id, path, AH5_A_FLOATING_TYPE, &buf))
        {
            if (strcmp(buf, AH5_V_SINGLE_INTEGER) == 0)
            {
                floatingtype->type = FT_SINGLE_INTEGER;
                if (AH5_read_ft_singleinteger(file_id, path, &(floatingtype->data.singleinteger)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_SINGLE_REAL) == 0)
            {
                floatingtype->type = FT_SINGLE_REAL;
                if (AH5_read_ft_singlereal(file_id, path, &(floatingtype->data.singlereal)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_SINGLE_COMPLEX) == 0)
            {
                floatingtype->type = FT_SINGLE_COMPLEX;
                if (AH5_read_ft_singlecomplex(file_id, path, &(floatingtype->data.singlecomplex)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_SINGLE_STRING) == 0)
            {
                floatingtype->type = FT_SINGLE_STRING;
                if (AH5_read_ft_singlestring(file_id, path, &(floatingtype->data.singlestring)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_VECTOR) == 0)
            {
                floatingtype->type = FT_VECTOR;
                if (AH5_read_ft_vector(file_id, path, &(floatingtype->data.vector)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_LINEARLISTOFREAL1) == 0)
            {
                floatingtype->type = FT_LINEARLISTOFREAL1;
                if (AH5_read_ft_linearlistofreal1(file_id, path, &(floatingtype->data.linearlistofreal1)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_LINEARLISTOFREAL2) == 0)
            {
                floatingtype->type = FT_LINEARLISTOFREAL2;
                if (AH5_read_ft_linearlistofreal2(file_id, path, &(floatingtype->data.linearlistofreal2)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_LOGARITHMLISTOFREAL) == 0)
            {
                floatingtype->type = FT_LOGARITHMLISTOFREAL;
                if (AH5_read_ft_logarithmlistofreal(file_id, path, &(floatingtype->data.logarithmlistofreal)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_PERDECADELISTOFREAL) == 0)
            {
                floatingtype->type = FT_PERDECADELISTOFREAL;
                if (AH5_read_ft_perdecadelistofreal(file_id, path, &(floatingtype->data.perdecadelistofreal)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_LINEARLISTOFINTEGER2) == 0)
            {
                floatingtype->type = FT_LINEARLISTOFINTEGER2;
                if (AH5_read_ft_linearlistofinteger2(file_id, path, &(floatingtype->data.linearlistofinteger2)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_RATIONAL_FUNCTION) == 0)
            {
                floatingtype->type = FT_RATIONAL_FUNCTION;
                if (AH5_read_ft_rationalfunction(file_id, path, &(floatingtype->data.rationalfunction)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_GENERAL_RATIONAL_FUNCTION) == 0)
            {
                floatingtype->type = FT_GENERAL_RATIONAL_FUNCTION;
                if (AH5_read_ft_generalrationalfunction(file_id, path, &(floatingtype->data.generalrationalfunction)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_RATIONAL) == 0)
            {
                floatingtype->type = FT_RATIONAL;
                if (AH5_read_ft_rational (file_id, path, &(floatingtype->data.rational)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_DATASET) == 0)
            {
                floatingtype->type = FT_DATASET;
                if (AH5_read_ft_dataset(file_id, path, &(floatingtype->data.dataset)))
                    rdata = AH5_TRUE;
            }
            else if (strcmp(buf, AH5_V_ARRAYSET) == 0)
            {
                floatingtype->type = FT_ARRAYSET;
                if (AH5_read_ft_arrayset(file_id, path, &(floatingtype->data.arrayset)))
                    rdata = AH5_TRUE;
            }
            else
                printf("***** ERROR: Invalid attribute \"floatingType\" in \"%s\". *****\n\n", path);
            free(buf);
            buf = NULL;
        }
        else
            AH5_print_err_attr("", path, AH5_A_FLOATING_TYPE);
    }
    else
        AH5_print_err_path("", path);
    if (!rdata)
        floatingtype->type = FT_INVALID;
    return rdata;
}


/*******************************************************************************
 * Write functions
 ******************************************************************************/
/** 
 * Initialized the floatingType.
 *
 * This function have two main rules:
 *  - check the floatingType's path
 *  - and create the floatingType category if needed.
 * 
 * @param file_id 
 * @param path 
 * 
 * @return AH5_TRUE if all ok else AH5_FALSE
 */
char AH5_init_floatingtype (hid_t file_id, const char* path)
{
  char success = AH5_FALSE;
  char *path2;

  if (path != NULL && !AH5_path_valid(file_id, path))
  {
    path2 = AH5_get_base_from_path(path);
    
    if (AH5_path_valid(file_id, path2))
      success = AH5_TRUE;
    else
      if (strcmp(path2, AH5_C_FLOATING_TYPE) == 0)
        success = H5Gcreate(file_id, AH5_C_FLOATING_TYPE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT) >= 0;
    
    free(path2);
  }
  
  return success;
}


char AH5_write_ft_singleinteger (hid_t file_id, AH5_singleinteger_t *singleinteger)
{
  char success = AH5_FALSE;

  if (AH5_init_floatingtype(file_id, singleinteger->path))
    if (H5Gcreate(file_id, singleinteger->path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT) >= 0)
      if (AH5_write_int_attr(file_id, singleinteger->path, AH5_A_VALUE, singleinteger->value))
        if (AH5_write_str_attr(file_id, singleinteger->path, AH5_A_FLOATING_TYPE, AH5_V_SINGLE_INTEGER))
          success = AH5_write_opt_attrs(file_id, singleinteger->path, &(singleinteger->opt_attrs));

  if (!success)
    AH5_print_err_dset("", singleinteger->path);
  
  return success;
}

char AH5_write_ft_singlereal (hid_t file_id, AH5_singlereal_t *singlereal)
{
  char success = AH5_FALSE;
  
  if (AH5_init_floatingtype(file_id, singlereal->path))
    if (H5Gcreate(file_id, singlereal->path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT) >= 0)
      if (AH5_write_flt_attr(file_id, singlereal->path, AH5_A_VALUE, singlereal->value))
        if (AH5_write_str_attr(file_id, singlereal->path, AH5_A_FLOATING_TYPE, AH5_V_SINGLE_REAL))
          success = AH5_write_opt_attrs(file_id, singlereal->path, &(singlereal->opt_attrs));
  
  if (!success)
    AH5_print_err_dset("", singlereal->path);
  
  return success;
}

char AH5_write_ft_singlecomplex (hid_t file_id, AH5_singlecomplex_t *singlecomplex)
{
  char success = AH5_FALSE;
  
  if (AH5_init_floatingtype(file_id, singlecomplex->path))
    if (H5Gcreate(file_id, singlecomplex->path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT) >= 0)
      if (AH5_write_cpx_attr(file_id, singlecomplex->path, AH5_A_VALUE, singlecomplex->value))
        if (AH5_write_str_attr(file_id, singlecomplex->path, AH5_A_FLOATING_TYPE, AH5_V_SINGLE_COMPLEX))
          success = AH5_write_opt_attrs(file_id, singlecomplex->path, &(singlecomplex->opt_attrs));
  
  if (!success)
    AH5_print_err_dset("", singlecomplex->path);
  
  return success;
}

char AH5_write_ft_singlestring (hid_t file_id, AH5_singlestring_t *singlestring)
{
  char success = AH5_FALSE;
  
  if (AH5_init_floatingtype(file_id, singlestring->path))
    if (AH5_write_str_attr(file_id, singlestring->path, AH5_A_VALUE, singlestring->value))
      if (AH5_write_str_attr(file_id, singlestring->path, AH5_A_FLOATING_TYPE, AH5_V_SINGLE_STRING))
        success = AH5_write_opt_attrs(file_id, singlestring->path, &(singlestring->opt_attrs));
  
  if (!success)
    AH5_print_err_dset("", singlestring->path);
  
  return success;
}

char AH5_write_ft_vector (hid_t file_id, AH5_vector_t *vector)
{
  char success = AH5_FALSE;

  if (AH5_init_floatingtype(file_id, vector->path) && vector->nb_values > 0)
  {
    switch (vector->type_class)
    {
      case H5T_INTEGER:
        if (AH5_write_int_dataset(file_id, vector->path, vector->nb_values, vector->values.i))
          success = AH5_TRUE;
        break;
      case H5T_FLOAT:
        if (AH5_write_flt_dataset(file_id, vector->path, vector->nb_values, vector->values.f))
          success = AH5_TRUE;
        break;
      case H5T_COMPOUND:
        if (AH5_write_cpx_dataset(file_id, vector->path, vector->nb_values, vector->values.c))
          success = AH5_TRUE;
        break;
      case H5T_STRING:
        if (AH5_write_str_dataset(file_id, vector->path, vector->nb_values, strlen(vector->values.s[0])+1, vector->values.s))
          success = AH5_TRUE;
        break;
      default:
        break;
    }
  }

  if (success)
    success = AH5_write_str_attr(file_id, vector->path, AH5_A_FLOATING_TYPE, AH5_V_VECTOR);
        
  if (success)
    success = AH5_write_opt_attrs(file_id, vector->path, &(vector->opt_attrs));

  if (!success)
    AH5_print_err_dset("", vector->path);
  
  return success;
}

char AH5_write_ft_linearlistofreal1 (hid_t file_id, AH5_linearlistofreal1_t *linearlistofreal1)
{
  char success = AH5_FALSE;

  // TO BE IMPLEMENTED...
  AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED("FloatingType", linearlistofreal1->path);
  
  return success;
}

char AH5_write_ft_linearlistofreal2 (hid_t file_id, AH5_linearlistofreal2_t *linearlistofreal2)
{
  char success = AH5_FALSE;

  // TO BE IMPLEMENTED...
  AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED("FloatingType", linearlistofreal2->path);
  
  return success;
}

char AH5_write_ft_logarithmlistofreal (hid_t file_id, AH5_logarithmlistofreal_t *logarithmlistofreal)
{
  char success = AH5_FALSE;

  // TO BE IMPLEMENTED...
  AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED("FloatingType", logarithmlistofreal->path);

  return success;
}

char AH5_write_ft_perdecadelistofreal (hid_t file_id, AH5_perdecadelistofreal_t *perdecadelistofreal)
{
  char success = AH5_FALSE;

  // TO BE IMPLEMENTED...
  AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED("FloatingType", perdecadelistofreal->path);
  
  return success;
}

char AH5_write_ft_linearlistofinteger2 (hid_t file_id, AH5_linearlistofinteger2_t *linearlistofinteger2)
{
  char success = AH5_FALSE;

  // TO BE IMPLEMENTED...
  AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED("FloatingType", linearlistofinteger2->path);
  
  return success;
}

char AH5_write_ft_rationalfunction (hid_t file_id, AH5_rationalfunction_t *rationalfunction)
{
  char success = AH5_FALSE;

  // TO BE IMPLEMENTED...
  AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED("FloatingType", rationalfunction->path);
  
  return success;
}

char AH5_write_ft_generalrationalfunction (hid_t file_id, AH5_generalrationalfunction_t *generalrationalfunction)
{
  char success = AH5_FALSE;

  // TO BE IMPLEMENTED...
  AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED("FloatingType", generalrationalfunction->path);
  
  return success;
}

char AH5_write_ft_rational (hid_t file_id, AH5_rational_t *rational)
{
  char success = AH5_FALSE;

  // TO BE IMPLEMENTED...
  AH5_PRINT_ERR_FUNC_NOT_IMPLEMENTED("FloatingType", rational->path);
  
  return success;
}

char AH5_write_ft_dataset (hid_t file_id, AH5_dataset_t *dataset)
{
  char success = AH5_FALSE;
  hsize_t total_size = 1;
  int i;
  
  if (AH5_init_floatingtype(file_id, dataset->path) && dataset->nb_dims > 0)
  {
    switch (dataset->type_class)
    {
      for (i = 0; i < dataset->nb_dims; ++i)
        total_size *= dataset->dims[i];
      
      case H5T_INTEGER:
        if (AH5_write_int_array(file_id, dataset->path, dataset->nb_dims, dataset->dims, dataset->values.i))
          success = AH5_TRUE;
        break;
      case H5T_FLOAT:
        if (AH5_write_flt_array(file_id, dataset->path, dataset->nb_dims, dataset->dims, dataset->values.f))
          success = AH5_TRUE;
        break;
      case H5T_COMPOUND:
        if (AH5_write_cpx_array(file_id, dataset->path, dataset->nb_dims, dataset->dims, dataset->values.c))
          success = AH5_TRUE;
        break;
      case H5T_STRING:
        if (AH5_write_str_dataset(file_id, dataset->path, total_size, strlen(dataset->values.s), dataset->values.s))
          success = AH5_TRUE;
        break;
      default:
        break;
    }
  }
  
  if (success)
    success = AH5_write_str_attr(file_id, dataset->path, AH5_A_FLOATING_TYPE, AH5_V_DATASET);
              
  if (success)
    success = AH5_write_opt_attrs(file_id, dataset->path, &(dataset->opt_attrs));

  if (!success)
    AH5_print_err_dset("", dataset->path);
  
  return success;
}

char AH5_write_ft_arrayset (hid_t file_id, AH5_arrayset_t *arrayset)
{
  char success = AH5_FALSE;

  char path2[AH5_ABSOLUTE_PATH_LENGTH], dimname[AH5_ABSOLUTE_PATH_LENGTH];
  char *tmp, *tmp2;

  unsigned int i;

  /*
    The data's path and dimensions' path are not directly use to ensure the
    Amelet-HDF data file integrity. However, the initial path must not be
    altered by the arrayset write function. Finally, in future version the path
    could be checked to raise warning.
  */

  
  if (AH5_init_floatingtype(file_id, arrayset->path) && arrayset->nb_dims > 0)
  {
    if (H5Gcreate(file_id, arrayset->path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT) >= 0)
    {
      strcpy(path2, arrayset->path);
      strcat(path2, AH5_G_DATA);
      tmp = arrayset->data.path;
      arrayset->data.path = path2;
    
      if (AH5_write_ft_dataset(file_id, &(arrayset->data)))
      {
        strcpy(path2, arrayset->path);
        strcat(path2, AH5_G_DS);
        if (H5Gcreate(file_id, path2, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT) >= 0)
        {
          success = AH5_TRUE;
          for (i = 0; i < arrayset->nb_dims && success; ++i)
          {
            sprintf(dimname, "/dim%d", i + 1);
            strcpy(path2, arrayset->path);
            strcat(path2, AH5_G_DS);
            strcat(path2, dimname);
            tmp2 = arrayset->dims[i].path;
            arrayset->dims[i].path = path2;
            success = AH5_write_ft_vector(file_id, arrayset->dims + i);
            arrayset->dims[i].path = tmp2;
          }
        }
      }
    
      arrayset->data.path = tmp;
    }
  }

  if (success)
    success = AH5_write_str_attr(file_id, arrayset->path, AH5_A_FLOATING_TYPE, AH5_V_ARRAYSET);
              
  if (success)
    success = AH5_write_opt_attrs(file_id, arrayset->path, &(arrayset->opt_attrs));

  if (!success)
    AH5_print_err_dset("", arrayset->path);
  
  return success;
}

char AH5_write_floatingtype (hid_t file_id, AH5_ft_t *floatingtype)
{
  char success = AH5_FALSE;

  switch (floatingtype->type)
  {
    case FT_SINGLE_INTEGER:
      return AH5_write_ft_singleinteger(file_id, &(floatingtype->data.singleinteger));
    case FT_SINGLE_REAL:
      return AH5_write_ft_singlereal(file_id, &(floatingtype->data.singlereal));
    case FT_SINGLE_COMPLEX:
      return AH5_write_ft_singlecomplex(file_id, &(floatingtype->data.singlecomplex));
    case FT_SINGLE_STRING:
      return AH5_write_ft_singlestring(file_id, &(floatingtype->data.singlestring));
    case FT_VECTOR:
      return AH5_write_ft_vector(file_id, &(floatingtype->data.vector));
    case FT_LINEARLISTOFREAL1:
      return AH5_write_ft_linearlistofreal1(file_id, &(floatingtype->data.linearlistofreal1));
    case FT_LINEARLISTOFREAL2:
      return AH5_write_ft_linearlistofreal2(file_id, &(floatingtype->data.linearlistofreal2));
    case FT_LOGARITHMLISTOFREAL:
      return AH5_write_ft_logarithmlistofreal(file_id, &(floatingtype->data.logarithmlistofreal));
    case FT_PERDECADELISTOFREAL:
      return AH5_write_ft_perdecadelistofreal(file_id, &(floatingtype->data.perdecadelistofreal));
    case FT_LINEARLISTOFINTEGER2:
      return AH5_write_ft_linearlistofinteger2(file_id, &(floatingtype->data.linearlistofinteger2));
    case FT_RATIONAL_FUNCTION:
      return AH5_write_ft_rationalfunction(file_id, &(floatingtype->data.rationalfunction));
    case FT_GENERAL_RATIONAL_FUNCTION:
      return AH5_write_ft_generalrationalfunction(file_id, &(floatingtype->data.generalrationalfunction));
    case FT_RATIONAL:
      return AH5_write_ft_rational(file_id, &(floatingtype->data.rational));
    case FT_DATASET:
      return AH5_write_ft_dataset(file_id, &(floatingtype->data.dataset));
    case FT_ARRAYSET:
      return AH5_write_ft_arrayset(file_id, &(floatingtype->data.arrayset));
  }
  
  return success;
}



/*******************************************************************************
 * Print functions
 ******************************************************************************/
// Print singleInteger
void AH5_print_ft_singleinteger (const AH5_singleinteger_t *singleinteger, int space)
{
    printf("%*s-%s:\n", space, "", AH5_get_name_from_path(singleinteger->path));
    AH5_print_int_attr(AH5_A_VALUE, singleinteger->value, space + 3);
    AH5_print_opt_attrs(&(singleinteger->opt_attrs), space + 3);
}


// Print singleReal
void AH5_print_ft_singlereal (const AH5_singlereal_t *singlereal, int space)
{
    printf("%*s-%s:\n", space, "", AH5_get_name_from_path(singlereal->path));
    AH5_print_flt_attr(AH5_A_VALUE, singlereal->value, space + 3);
    AH5_print_opt_attrs(&(singlereal->opt_attrs), space + 3);
}


// Print singleComplex
void AH5_print_ft_singlecomplex (const AH5_singlecomplex_t *singlecomplex, int space)
{
    printf("%*s-%s:\n", space, "", AH5_get_name_from_path(singlecomplex->path));
    AH5_print_cpx_attr(AH5_A_VALUE, singlecomplex->value, space + 3);
    AH5_print_opt_attrs(&(singlecomplex->opt_attrs), space + 3);
}


// Print singleString
void AH5_print_ft_singlestring (const AH5_singlestring_t *singlestring, int space)
{
    printf("%*s-%s:\n", space, "", AH5_get_name_from_path(singlestring->path));
    AH5_print_str_attr(AH5_A_VALUE, singlestring->value, space + 3);
    AH5_print_opt_attrs(&(singlestring->opt_attrs), space + 3);
}


// Print vector
void AH5_print_ft_vector (const AH5_vector_t *vector, int space)
{
    hsize_t i;

    printf("%*s-%s [%lu]: {", space, "", AH5_get_name_from_path(vector->path),(long unsigned) vector->nb_values);
    switch (vector->type_class)
    {
    case H5T_INTEGER:
        for (i = 0; i < vector->nb_values - 1; i++)
            printf("%i, ", vector->values.i[i]);
        printf("%i}\n", vector->values.i[vector->nb_values - 1]);
        break;
    case H5T_FLOAT:
        for (i = 0; i < vector->nb_values - 1; i++)
            printf("%g, ", vector->values.f[i]);
        printf("%g}\n", vector->values.f[vector->nb_values - 1]);
        break;
    case H5T_COMPOUND:
        for (i = 0; i < vector->nb_values - 1; i++)
            printf("%g%+gi, ", creal(vector->values.c[i]), cimag(vector->values.c[i]));
        printf("%g%+gi}\n", creal(vector->values.c[vector->nb_values - 1]), cimag(vector->values.c[vector->nb_values - 1]));
        break;
    case H5T_STRING:
        for (i = 0; i < vector->nb_values - 1; i++)
            printf("\"%s\", ", vector->values.s[i]);
        printf("\"%s\"}\n", vector->values.s[vector->nb_values - 1]);
        break;
    default:
        break;
    }
    AH5_print_opt_attrs(&(vector->opt_attrs), space + 3);
}


// Print linearListOfReal1
void AH5_print_ft_linearlistofreal1 (const AH5_linearlistofreal1_t *linearlistofreal1, int space)
{
    printf("%*s-%s: %s=%g, %s=%g, %s=%i\n", space, "",AH5_get_name_from_path(linearlistofreal1->path), AH5_A_FIRST, linearlistofreal1->first, AH5_A_LAST, linearlistofreal1->last, AH5_A_NUMBER_OF_VALUES, linearlistofreal1->number_of_values);
    AH5_print_opt_attrs(&(linearlistofreal1->opt_attrs), space + 3);
}


// Print linearListOfReal2
void AH5_print_ft_linearlistofreal2 (const AH5_linearlistofreal2_t *linearlistofreal2, int space)
{
    printf("%*s-%s: %s=%g, %s=%g, %s=%i\n", space, "",AH5_get_name_from_path(linearlistofreal2->path), AH5_A_FIRST, linearlistofreal2->first, AH5_A_STEP, linearlistofreal2->step, AH5_A_NUMBER_OF_VALUES, linearlistofreal2->number_of_values);
    AH5_print_opt_attrs(&(linearlistofreal2->opt_attrs), space + 3);
}


// Print logarithmListOfReal
void AH5_print_ft_logarithmlistofreal (const AH5_logarithmlistofreal_t *logarithmlistofreal, int space)
{
    printf("%*s-%s: %s=%g, %s=%g, %s=%i\n", space, "", AH5_get_name_from_path(logarithmlistofreal->path), AH5_A_FIRST, logarithmlistofreal->first, AH5_A_LAST, logarithmlistofreal->last, AH5_A_NUMBER_OF_VALUES, logarithmlistofreal->number_of_values);
    AH5_print_opt_attrs(&(logarithmlistofreal->opt_attrs), space + 3);
}


// Print perDecadeListOfReal
void AH5_print_ft_perdecadelistofreal (const AH5_perdecadelistofreal_t *perdecadelistofreal, int space)
{
    printf("%*s-%s: %s=%g, %s=%i, %s=%i\n", space, "", AH5_get_name_from_path(perdecadelistofreal->path), AH5_A_FIRST, perdecadelistofreal->first, AH5_A_NUMBER_OF_DECADES, perdecadelistofreal->number_of_decades, AH5_A_NUMBER_OF_VALUES_PER_DECADE, perdecadelistofreal->number_of_values_per_decade);
    AH5_print_opt_attrs(&(perdecadelistofreal->opt_attrs), space + 3);
}


// Print linearListOfInteger2
void AH5_print_ft_linearlistofinteger2 (const AH5_linearlistofinteger2_t *linearlistofinteger2, int space)
{
    printf("%*s-%s: %s=%i, %s=%i, %s=%i\n", space, "",AH5_get_name_from_path(linearlistofinteger2->path), AH5_A_FIRST, linearlistofinteger2->first, AH5_A_STEP, linearlistofinteger2->step, AH5_A_NUMBER_OF_VALUES, linearlistofinteger2->number_of_values);
    AH5_print_opt_attrs(&(linearlistofinteger2->opt_attrs), space + 3);
}


// Print rationalFunction
void AH5_print_ft_rationalfunction (const AH5_rationalfunction_t *rationalfunction, int space)
{
    hsize_t i;

    printf("%*s-%s [%lu]: ", space, "", AH5_get_name_from_path(rationalfunction->path), (long unsigned) rationalfunction->nb_types);
    for (i = 0; i < rationalfunction->nb_types - 1; i++)
    {
        printf("type%i=%g|%g|%g, ", rationalfunction->types[i], rationalfunction->a[i], rationalfunction->b[i], rationalfunction->f[i]);
    }
    printf("type%i=%g|%g|%g\n", rationalfunction->types[rationalfunction->nb_types - 1], rationalfunction->a[rationalfunction->nb_types - 1], rationalfunction->b[rationalfunction->nb_types - 1], rationalfunction->f[rationalfunction->nb_types - 1]);
    AH5_print_opt_attrs(&(rationalfunction->opt_attrs), space + 3);
}


// Print generalRationalFunction
void AH5_print_ft_generalrationalfunction (const AH5_generalrationalfunction_t *generalrationalfunction, int space)
{
    hsize_t i;

    printf("%*s-%s [%lu]:\n", space, "", AH5_get_name_from_path(generalrationalfunction->path), (long unsigned) generalrationalfunction->nb_degrees);
    for (i = 0; i < generalrationalfunction->nb_degrees; i++)
        printf("%*s-degree %lu: numerator=%g%+gi, denominator=%g%+gi\n", space + 3, "", (long unsigned) i, creal(generalrationalfunction->numerator[i]), cimag(generalrationalfunction->numerator[i]), creal(generalrationalfunction->denominator[i]), cimag(generalrationalfunction->denominator[i]));
    AH5_print_opt_attrs(&(generalrationalfunction->opt_attrs), space + 3);
}


// Print rational
void AH5_print_ft_rational (const AH5_rational_t *rational, int space)
{
    hsize_t i, j, total;

    printf("%*s-%s:\n", space, "", AH5_get_name_from_path(rational->path));
    AH5_print_opt_attrs(&(rational->opt_attrs), space + 3);
    printf("%*s-@%s [%lux%lu]: {", space + 3, "", AH5_A_VALUE, (long unsigned) rational->dims[0], (long unsigned) rational->dims[1]);
    if (rational->data != NULL)
    {
        total = (rational->dims[0]) * (rational->dims[1]);
        for (i = 0; i < total - 1; i++)
        {
            printf("\"%s\", ", rational->data[i]);
        }
        printf("\"%s\"", rational->data[total - 1]);
    }
    printf("}\n");
    // Print functions inside rational
    for (j = 0; j < rational->nb_functions; j++)
    {
        switch (rational->functions[j].type)
        {
        case FT_RATIONAL_FUNCTION:
            AH5_print_ft_rationalfunction(&(rational->functions[j].data.rf), space + 6);
            break;
        case FT_GENERAL_RATIONAL_FUNCTION:
            AH5_print_ft_generalrationalfunction(&(rational->functions[j].data.grf), space + 6);
            break;
        default:
            break;
        }
    }

}


// Print dataset
void AH5_print_ft_dataset (const AH5_dataset_t *dataset, int space)
{
    hsize_t i, total = 1;
    int j;

    printf("%*s-%s [", space, "", AH5_get_name_from_path(dataset->path));
    for (j = 0; j < dataset->nb_dims - 1; j++)
    {
        printf("%lux", (long unsigned) dataset->dims[j]);
        total *= dataset->dims[j];
    }
    printf("%lu]: {", (long unsigned) dataset->dims[dataset->nb_dims - 1]);
    total *= dataset->dims[dataset->nb_dims - 1];

    switch (dataset->type_class)
    {
    case H5T_INTEGER:
        for (i = 0; i < total - 1; i++)
        {
            printf("%i, ", dataset->values.i[i]);
        }
        printf("%i}\n", dataset->values.i[total - 1]);
        break;
    case H5T_FLOAT:
        for (i = 0; i < total - 1; i++)
        {
            printf("%g, ", dataset->values.f[i]);
        }
        printf("%g}\n", dataset->values.f[total - 1]);
        break;
    case H5T_COMPOUND:
        for (i = 0; i < total - 1; i++)
        {
            printf("%g%+gi, ", creal(dataset->values.c[i]), cimag(dataset->values.c[i]));
        }
        printf("%g%+gi}\n", creal(dataset->values.c[total - 1]), cimag(dataset->values.c[total - 1]));
        break;
    case H5T_STRING:
        if (dataset->values.s != NULL)
        {
            for (i = 0; i < total - 1; i++)
            {
                printf("\"%s\", ", dataset->values.s[i]);
            }
            printf("\"%s\"}\n", dataset->values.s[total - 1]);
        }
        break;
    default:
        break;
    }
    AH5_print_opt_attrs(&(dataset->opt_attrs), space + 3);
}


// Print arrayset
void AH5_print_ft_arrayset (const AH5_arrayset_t *arrayset, int space)
{
    hsize_t i;

    printf("%*s-%s:\n", space, "", AH5_get_name_from_path(arrayset->path));
    AH5_print_opt_attrs(&(arrayset->opt_attrs), space + 4);
    AH5_print_ft_dataset(&(arrayset->data), space + 2);
    for (i = 0; i < arrayset->nb_dims; i++)
        AH5_print_ft_vector(&(arrayset->dims[i]), space + 2);
}


// Print floatingType structure
void AH5_print_floatingtype (const AH5_ft_t *floatingtype, int space)
{
    switch(floatingtype->type)
    {
    case FT_SINGLE_INTEGER:
        AH5_print_ft_singleinteger(&(floatingtype->data.singleinteger), space);
        break;
    case FT_SINGLE_REAL:
        AH5_print_ft_singlereal(&(floatingtype->data.singlereal), space);
        break;
    case FT_SINGLE_COMPLEX:
        AH5_print_ft_singlecomplex(&(floatingtype->data.singlecomplex), space);
        break;
    case FT_SINGLE_STRING:
        AH5_print_ft_singlestring(&(floatingtype->data.singlestring), space);
        break;
    case FT_VECTOR:
        AH5_print_ft_vector(&(floatingtype->data.vector), space);
        break;
    case FT_LINEARLISTOFREAL1:
        AH5_print_ft_linearlistofreal1(&(floatingtype->data.linearlistofreal1), space);
        break;
    case FT_LINEARLISTOFREAL2:
        AH5_print_ft_linearlistofreal2(&(floatingtype->data.linearlistofreal2), space);
        break;
    case FT_LOGARITHMLISTOFREAL:
        AH5_print_ft_logarithmlistofreal(&(floatingtype->data.logarithmlistofreal), space);
        break;
    case FT_PERDECADELISTOFREAL:
        AH5_print_ft_perdecadelistofreal(&(floatingtype->data.perdecadelistofreal), space);
        break;
    case FT_LINEARLISTOFINTEGER2:
        AH5_print_ft_linearlistofinteger2(&(floatingtype->data.linearlistofinteger2), space);
        break;
    case FT_RATIONAL_FUNCTION:
        AH5_print_ft_rationalfunction(&(floatingtype->data.rationalfunction), space);
        break;
    case FT_GENERAL_RATIONAL_FUNCTION:
        AH5_print_ft_generalrationalfunction(&(floatingtype->data.generalrationalfunction), space);
        break;
    case FT_RATIONAL:
        AH5_print_ft_rational(&(floatingtype->data.rational), space);
        break;
    case FT_DATASET:
        AH5_print_ft_dataset(&(floatingtype->data.dataset), space);
        break;
    case FT_ARRAYSET:
        AH5_print_ft_arrayset(&(floatingtype->data.arrayset), space);
        break;
    default:
        break;
    }
}



/*******************************************************************************
 * Free functions
 ******************************************************************************/
// Free memory used by singleInteger
void AH5_free_ft_singleinteger (AH5_singleinteger_t *singleinteger)
{
    if (singleinteger->path != NULL)
    {
        free(singleinteger->path);
        singleinteger->path = NULL;
    }
    AH5_free_opt_attrs(&(singleinteger->opt_attrs));
}


// Free memory used by singleReal
void AH5_free_ft_singlereal (AH5_singlereal_t *singlereal)
{
    if (singlereal->path != NULL)
    {
        free(singlereal->path);
        singlereal->path = NULL;
    }
    AH5_free_opt_attrs(&(singlereal->opt_attrs));
}


// Free memory used by singleComplex
void AH5_free_ft_singlecomplex (AH5_singlecomplex_t *singlecomplex)
{
    if (singlecomplex->path != NULL)
    {
        free(singlecomplex->path);
        singlecomplex->path = NULL;
    }
    AH5_free_opt_attrs(&(singlecomplex->opt_attrs));
}


// Free memory used by singleString
void AH5_free_ft_singlestring (AH5_singlestring_t *singlestring)
{
    if (singlestring->path != NULL)
    {
        free(singlestring->path);
        singlestring->path = NULL;
    }
    if (singlestring->value != NULL)
    {
        free(singlestring->value);
        singlestring->value = NULL;
    }
    AH5_free_opt_attrs(&(singlestring->opt_attrs));
}


// Free memory used by vector
void AH5_free_ft_vector (AH5_vector_t *vector)
{
    if (vector->path != NULL)
    {
        free(vector->path);
        vector->path = NULL;
    }
    AH5_free_opt_attrs(&(vector->opt_attrs));
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
            free(vector->values.s[0]);
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
void AH5_free_ft_linearlistofreal1 (AH5_linearlistofreal1_t *linearlistofreal1)
{
    if (linearlistofreal1->path != NULL)
    {
        free(linearlistofreal1->path);
        linearlistofreal1->path = NULL;
    }
    AH5_free_opt_attrs(&(linearlistofreal1->opt_attrs));
}


// Free memory used by linearListOfReal2
void AH5_free_ft_linearlistofreal2 (AH5_linearlistofreal2_t *linearlistofreal2)
{
    if (linearlistofreal2->path != NULL)
    {
        free(linearlistofreal2->path);
        linearlistofreal2->path = NULL;
    }
    AH5_free_opt_attrs(&(linearlistofreal2->opt_attrs));
}


// Free memory used by logarithmListOfReal
void AH5_free_ft_logarithmlistofreal (AH5_logarithmlistofreal_t *logarithmlistofreal)
{
    if (logarithmlistofreal->path != NULL)
    {
        free(logarithmlistofreal->path);
        logarithmlistofreal->path = NULL;
    }
    AH5_free_opt_attrs(&(logarithmlistofreal->opt_attrs));
}


// Free memory used by perDecadeListOfReal
void AH5_free_ft_perdecadelistofreal (AH5_perdecadelistofreal_t *perdecadelistofreal)
{
    if (perdecadelistofreal->path != NULL)
    {
        free(perdecadelistofreal->path);
        perdecadelistofreal->path = NULL;
    }
    AH5_free_opt_attrs(&(perdecadelistofreal->opt_attrs));
}


// Free memory used by linearListOfInteger2
void AH5_free_ft_linearlistofinteger2 (AH5_linearlistofinteger2_t *linearlistofinteger2)
{
    if (linearlistofinteger2->path != NULL)
    {
        free(linearlistofinteger2->path);
        linearlistofinteger2->path = NULL;
    }
    AH5_free_opt_attrs(&(linearlistofinteger2->opt_attrs));
}


// Free memory used by rationalFunction
void AH5_free_ft_rationalfunction (AH5_rationalfunction_t *rationalfunction)
{
    if (rationalfunction->path != NULL)
    {
        free(rationalfunction->path);
        rationalfunction->path = NULL;
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
    AH5_free_opt_attrs(&(rationalfunction->opt_attrs));
}


// Free memory used by generalRationalFunction
void AH5_free_ft_generalrationalfunction (AH5_generalrationalfunction_t *generalrationalfunction)
{
    if (generalrationalfunction->path != NULL)
    {
        free(generalrationalfunction->path);
        generalrationalfunction->path = NULL;
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
    AH5_free_opt_attrs(&(generalrationalfunction->opt_attrs));
}


// Free memory used by rational
void AH5_free_ft_rational (AH5_rational_t *rational)
{
    hsize_t j;

    if (rational->path != NULL)
    {
        free(rational->path);
        rational->path = NULL;
    }
    AH5_free_opt_attrs(&(rational->opt_attrs));

    if (rational->functions != NULL)
    {
        for (j = 0; j < rational->nb_functions; j++)
        {
            switch (rational->functions[j].type)
            {
            case FT_RATIONAL_FUNCTION:
                AH5_free_ft_rationalfunction(&(rational->functions[j].data.rf));
                break;
            case FT_GENERAL_RATIONAL_FUNCTION:
                AH5_free_ft_generalrationalfunction(&(rational->functions[j].data.grf));
                break;
            default:
                break;
            }
            rational->functions[j].type = FT_INVALID;
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
void AH5_free_ft_dataset (AH5_dataset_t *dataset)
{
    if (dataset->path != NULL)
    {
        free(dataset->path);
        dataset->path = NULL;
    }
    AH5_free_opt_attrs(&(dataset->opt_attrs));
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
void AH5_free_ft_arrayset (AH5_arrayset_t *arrayset)
{
    hsize_t i;

    if (arrayset->path != NULL)
    {
        free(arrayset->path);
        arrayset->path = NULL;
    }
    AH5_free_opt_attrs(&(arrayset->opt_attrs));
    if (arrayset->dims != NULL)
    {
        AH5_free_ft_dataset(&(arrayset->data));
        for (i = 0; i < arrayset->nb_dims; i++)
            AH5_free_ft_vector(arrayset->dims + i);
        free(arrayset->dims);
        arrayset->dims = NULL;
        arrayset->nb_dims = 0;
    }
}


// Free memory used by floatingType structure
void AH5_free_floatingtype (AH5_ft_t *floatingtype)
{
    switch(floatingtype->type)
    {
    case FT_SINGLE_INTEGER:
        AH5_free_ft_singleinteger(&(floatingtype->data.singleinteger));
        break;
    case FT_SINGLE_REAL:
        AH5_free_ft_singlereal(&(floatingtype->data.singlereal));
        break;
    case FT_SINGLE_COMPLEX:
        AH5_free_ft_singlecomplex(&(floatingtype->data.singlecomplex));
        break;
    case FT_SINGLE_STRING:
        AH5_free_ft_singlestring(&(floatingtype->data.singlestring));
        break;
    case FT_VECTOR:
        AH5_free_ft_vector(&(floatingtype->data.vector));
        break;
    case FT_LINEARLISTOFREAL1:
        AH5_free_ft_linearlistofreal1(&(floatingtype->data.linearlistofreal1));
        break;
    case FT_LINEARLISTOFREAL2:
        AH5_free_ft_linearlistofreal2(&(floatingtype->data.linearlistofreal2));
        break;
    case FT_LOGARITHMLISTOFREAL:
        AH5_free_ft_logarithmlistofreal(&(floatingtype->data.logarithmlistofreal));
        break;
    case FT_PERDECADELISTOFREAL:
        AH5_free_ft_perdecadelistofreal(&(floatingtype->data.perdecadelistofreal));
        break;
    case FT_LINEARLISTOFINTEGER2:
        AH5_free_ft_linearlistofinteger2(&(floatingtype->data.linearlistofinteger2));
        break;
    case FT_RATIONAL_FUNCTION:
        AH5_free_ft_rationalfunction(&(floatingtype->data.rationalfunction));
        break;
    case FT_GENERAL_RATIONAL_FUNCTION:
        AH5_free_ft_generalrationalfunction(&(floatingtype->data.generalrationalfunction));
        break;
    case FT_RATIONAL:
        AH5_free_ft_rational(&(floatingtype->data.rational));
        break;
    case FT_DATASET:
        AH5_free_ft_dataset(&(floatingtype->data.dataset));
        break;
    case FT_ARRAYSET:
        AH5_free_ft_arrayset(&(floatingtype->data.arrayset));
        break;
    default:
        break;
    }
    floatingtype->type = FT_INVALID;
}

