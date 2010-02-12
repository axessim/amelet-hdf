/*
 * datasettest.c
 *
 *  Created on: 7 déc. 2009
 *      Author: Didier
 */

#include <hdf5.h>
#include <hdf5_hl.h>
#include <string.h>
#include <dataset.h>
#include <complex.h>
#include <stdio.h>

int main(argc, argv)
    int argc;char *argv[];
{
    char * filename;
    dataset_t ds;

    hid_t file_id;
    // init
    H5open();
    if (argc == 1)
    {
        printf("missing input file name\n");
        return -1;
    }
    filename = (char *) malloc(100 * sizeof(char));
    strcpy(filename, argv[1]);
    // Read for the hdf5 file
    printf("Reading %s\n", filename);
    file_id = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);

    printf("Reading dataset ...\n");
    //Integer dataset
    printf("/int_dataset\n");
    ds = read_dataset(file_id, "/int_dataset");
    printf(" %s", dataset_to_string(ds));

    H5Fclose(file_id);
    free(filename);
    return 0;
}
