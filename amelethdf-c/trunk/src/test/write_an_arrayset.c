/*
 *  write_an_arrayset.c
 *
 *  Created on: 26 janv. 2010
 *  Author: Didier Roisse
 */

#include <hdf5.h>
#include <hdf5_hl.h>
#include <dataset.h>

/* This program write an HDF5 file with the following arraySet structure

 data.h5
 `-- floatingType
     `-- an_arrayset
         |-- data[@physicaNature=electricField
         |        @unit=voltPerMeter]
         `-- ds
             |-- dim1[@physicalNature=length
             |        @unit=meter]
             `-- dim2[@physicalNature=length
                      @unit=meter]

    float, dimension(10, 20) :: data
    float, dimension(20) :: dim1
    float, dimension(10) :: dim2
*/
int set_floating_type_attr(hid_t file_id, char* path, char* label,
        char* physical_nature, char *unit, char* comment)
{
    herr_t status;
    printf(" writes /%s@label ...\n", path);
    status = H5LTset_attribute_string(file_id, path, "label", label);
    if (status < 0)
    {
        printf("Can't create attribute !!!\n");
        exit(-1);
    }
    printf(" writes /%s@physicalNature ...\n", path);
    status = H5LTset_attribute_string(file_id, path, "physicalNature",
            physical_nature);
    if (status < 0)
    {
        printf("Can't create attribute !!!\n");
        exit(-1);
    }
    printf(" writes /%s@unit ...\n", path);
    status = H5LTset_attribute_string(file_id, path, "unit", unit);
    if (status < 0)
    {
        printf("Can't create attribute !!!\n");
        exit(-1);
    }
    printf(" writes /%s@comment ...\n", path);
    status = H5LTset_attribute_string(file_id, path, "comment", comment);
    if (status < 0)
    {
        printf("Can't create attribute !!!\n");
        exit(-1);
    }
    return 0;
}

int main(argc, argv)
    int argc;char *argv[];
{
    char * filename;
    hid_t file_id, grp_id, ds_id;
    herr_t status;
    int rank = 2;
    hsize_t dims[2] =
    { 10, 20 };
    hsize_t dims1[1] =
    { 20 };
    hsize_t dims2[1] =
    { 10 };
    float data[10][20];
    float dim1[20], dim2[10];
    char *label;
    char *physical_nature;
    char *unit;
    char *comment;
    float value;
    int i, j;

    value = 0;
    for (j = 0; j < 10; j++)
    {
        value = value + 1;
        dim2[j] = j;
        for (i = 0; i < 20; i++)
        {
            data[j][i] = value;
            printf("data[%i][%i]=%f\n", j, i, value);
        }
    }
    for (i = 0; i < 20; i++)
        dim1[i] = i;
    // init
    H5open();
    if (argc == 1)
    {
        printf("missing input file name\n");
        return -1;
    }
    filename = (char *) malloc(100 * sizeof(char));
    strcpy(filename, argv[1]);

    // File creation
    file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file_id < 0)
    {
        printf("Can't create file !!!\n");
        return -1;
    }

    // Writes / attribute
    printf("Write / attribute\n");
    status = H5LTset_attribute_string(file_id, "/", "FORMAT", "AMELET-HDF");
    if (status < 0)
    {
        printf("Can't create attribute !!!\n");
        exit(-1);
    }
    status = H5LTset_attribute_string(file_id, "/", "AMELETHDF_FORMAT_VERSION",
            "1.0.0");
    if (status < 0)
    {
        printf("Can't create attribute !!!\n");
        exit(-1);
    }

    // floatingType group creation
    printf("/floatingType group creation ...");
    grp_id = H5Gcreate(file_id, "floatingType", H5P_DEFAULT, H5P_DEFAULT,
            H5P_DEFAULT);
    if (grp_id < 0)
    {
        printf("Can't create group !!!\n");
        exit(-1);
    }

    // arraySet group creation
    printf("/floatingType/an_arrayset group creation ...\n");
    grp_id = H5Gcreate(grp_id, "an_arrayset", H5P_DEFAULT, H5P_DEFAULT,
            H5P_DEFAULT);
    if (grp_id < 0)
    {
        printf("Can't create group !!!\n");
        exit(-1);
    }

    // ds group creation
    printf("/floatingType/an_arrayset/ds group creation ...\n");
    ds_id = H5Gcreate(grp_id, "ds", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (grp_id < 0)
    {
        printf("Can't create group !!!\n");
        exit(-1);
    }

    // Writes floatingType attribute
    printf("Write floatingType attribute\n");
    status = H5LTset_attribute_string(file_id, "floatingType/an_arrayset",
            "floatingType", "arraySet");
    if (status < 0)
    {
        printf("Can't create attribute !!!\n");
        exit(-1);
    }

    // writes /floatingType/an_arrayset/data
    printf("/floatingType/an_arrayset/data creation ...\n");
    status = H5LTmake_dataset_float(file_id, "floatingType/an_arrayset/data",
            rank, dims, data[0]);
    if (status < 0)
    {
        printf("Can't create dataset !!!\n");
        exit(-1);
    }

    // writes data attribute
    set_floating_type_attr(file_id, "floatingType/an_arrayset/data",
            "Electric Field", "electricField", "voltPerMeter", "electric field");

    // writes /floatingType/an_arrayset/ds/dim1
    printf("/floatingType/an_arrayset/ds/dim1 creation ...\n");
    status = H5LTmake_dataset_float(file_id,
            "floatingType/an_arrayset/ds/dim1", 1, dims1, dim1);
    if (status < 0)
    {
        printf("Can't create dataset !!!\n");
        exit(-1);
    }

    // writes dim1 attribute
    set_floating_type_attr(file_id, "floatingType/an_arrayset/ds/dim1", "x",
            "length", "meter", "The X axis");

    // writes /floatingType/an_arrayset/ds/dim2
    printf("/floatingType/an_arrayset/ds/dim2 creation ...\n");
    status = H5LTmake_dataset_float(file_id,
            "floatingType/an_arrayset/ds/dim2", 1, dims2, dim2);
    if (status < 0)
    {
        printf("Can't create dataset !!!\n");
        exit(-1);
    }

    // writes dim1 attribute
    set_floating_type_attr(file_id, "floatingType/an_arrayset/ds/dim2", "y",
            "length", "meter", "The Y axis");

    H5Fclose(file_id);

    return 0;
}
