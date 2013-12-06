#include "test.h"
#include "utest.h"

#define FILE        "../../../data/tst_attribute.h5"
#define FILE_IN     "../../../data/tst_attribute_in.h5"
#define FILE_OUT    "../../../data/tst_attribute_out.h5"
#define EXTRA_LONG_PATH    "this_is_invalid_path_because_it_is_just_way_too_long"

int test_attribute()
{
    hid_t       file, group, invalid_id = -3;         /* Handles */
    char        status;
    int         k;
    int         tst_int[3] = {1, 2, 3};
    float       tst_flt[3] = {1.1, 2.2, 3.3};
    char        tst_str[4][60] = {"Amelet", "is just", "beautiful", "but number of characters of given attribute is limited"};
    AH5_complex_t tst_cpx[4];
    char        command[100];
    char        path[100];
    char        groups[16][100] = {"/integer", "/integer/G2", "/integer/G2/G3", "/integer/", "/float", "/float/G2", "/float/G2/G3", "/float/", "/string", "/string/G2", "/string/G2/G3", "/string/", "/complex", "/complex/G2", "/complex/G2/G3", "/complex/"};

    strcat(groups[3], EXTRA_LONG_PATH);
    strcat(groups[7], EXTRA_LONG_PATH);
    strcat(groups[11], EXTRA_LONG_PATH);
    strcat(groups[15], EXTRA_LONG_PATH);

    tst_cpx[0] = AH5_set_complex(1.2, 3.4);
    tst_cpx[1] = AH5_set_complex(5.6, 7.8);
    tst_cpx[2] = AH5_set_complex(9.0, 8.1);

/*
    file = H5Fcreate(FILE_IN, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    for (k = 0; k < 16; k++)
    {
        group = H5Gcreate(file, groups[k], H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Gclose(group);
    };
    H5Fclose(file);
*/

    filecopy(FILE_IN, FILE_OUT);

// TEST AH5_write_int_attr()

    strcpy(path, "integer/");
    strcat(path, EXTRA_LONG_PATH);
    file = H5Fopen(FILE_OUT, H5F_ACC_RDWR, H5P_DEFAULT);
    group = H5Oopen(file, "integer", H5P_DEFAULT);
    status = AH5_write_int_attr(file, "integer", "int1", tst_int[0]);
    status *= AH5_write_int_attr(group, ".", "int2", tst_int[1]);
    status *= AH5_write_int_attr(group, "G2/G3", "int3", tst_int[2]);
    status *= !AH5_write_int_attr(file, "invalid_path", "int4", tst_int[0]);
    status *= !AH5_write_int_attr(invalid_id, ".", "int5", tst_int[1]);
    status *= !AH5_write_int_attr(file, path, "int6", tst_int[2]);
    H5Oclose(group);
    mu_assert("AH5_write_int_attr() writing data...", status);
    H5Fclose(file);
    sprintf(command, "h5diffdll %s %s /integer", FILE_OUT, FILE);
    mu_assert("AH5_write_int_attr() checking data...", system(command) == 0);


// TEST AH5_write_flt_attr()

    // VALID
    strcpy(path, "float/");
    strcat(path, EXTRA_LONG_PATH);
    file = H5Fopen(FILE_OUT, H5F_ACC_RDWR, H5P_DEFAULT);
    group = H5Oopen(file, "float", H5P_DEFAULT);
    status = AH5_write_flt_attr(file, "float", "float1", tst_flt[0]);
    status *= AH5_write_flt_attr(group, ".", "float2", tst_flt[1]);
    status *= AH5_write_flt_attr(group, "G2/G3", "float3", tst_flt[2]);
    status *= !AH5_write_flt_attr(file, "invalid_path", "float4", tst_flt[0]);
    status *= !AH5_write_flt_attr(invalid_id, ".", "float5", tst_flt[1]);
    status *= !AH5_write_flt_attr(file, path, "float6", tst_flt[2]);
    H5Oclose(group);
    mu_assert("AH5_write_flt_attr() writing data...", status);
    H5Fclose(file);
    sprintf(command, "h5diffdll %s %s /float", FILE_OUT, FILE);
    mu_assert("AH5_write_flt_attr() checking data...", system(command) == 0);


// TEST AH5_write_str_attr()

    // VALID
    strcpy(path, "string/");
    strcat(path, EXTRA_LONG_PATH);
    file = H5Fopen(FILE_OUT, H5F_ACC_RDWR, H5P_DEFAULT);
    group = H5Oopen(file, "string", H5P_DEFAULT);
    status = AH5_write_str_attr(file, "string", "string1", tst_str[0]);
    status *= AH5_write_str_attr(group, ".", "string2", tst_str[1]);
    status *= AH5_write_str_attr(group, "G2/G3", "string3", tst_str[2]);
    status *= !AH5_write_str_attr(file, "invalid_path", "string4", tst_str[0]);
    status *= !AH5_write_str_attr(invalid_id, ".", "string5", tst_str[1]);
    status *= !AH5_write_str_attr(file, path, "string6", tst_str[2]);
    status *= !AH5_write_str_attr(file, "string", "string7", tst_str[3]);
    H5Oclose(group);
    mu_assert("AH5_write_str_attr() writing data...", status);
    H5Fclose(file);
    sprintf(command, "h5diffdll %s %s /string", FILE_OUT, FILE);
    mu_assert("AH5_write_str_attr() checking data...", system(command) == 0);


// TEST AH5_write_cpx_attr()

    // VALID
    strcpy(path, "complex/");
    strcat(path, EXTRA_LONG_PATH);
    file = H5Fopen(FILE_OUT, H5F_ACC_RDWR, H5P_DEFAULT);
    group = H5Oopen(file, "complex", H5P_DEFAULT);
    status = AH5_write_cpx_attr(file, "complex", "complex1", tst_cpx[0]);
    status *= AH5_write_cpx_attr(group, ".", "complex2", tst_cpx[1]);
    status *= AH5_write_cpx_attr(group, "G2/G3", "complex3", tst_cpx[2]);
    status *= !AH5_write_cpx_attr(file, "invalid_path", "complex4", tst_cpx[0]);
    status *= !AH5_write_cpx_attr(invalid_id, ".", "complex5", tst_cpx[1]);
    status *= !AH5_write_cpx_attr(file, path, "complex6", tst_cpx[2]);
    H5Oclose(group);
    mu_assert("AH5_write_cpx_attr() writing data...", status);
    H5Fclose(file);
    sprintf(command, "h5diffdll %s %s /complex", FILE_OUT, FILE);
    mu_assert("AH5_write_cpx_attr() checking data...", system(command) == 0);

    return 0;
}
