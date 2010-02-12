#include <hdf5.h>
#include <hdf5_hl.h>
#include <string.h>
#include <amelethdf.h>
#include <simpletype.h>
#include <stdlib.h>

int main(argc, argv)
    int argc;char *argv[];
{
    hid_t status;
    const char *filename;
    single_t single;
    hid_t file_id;

    if (argc == 1)
    {
        printf("missing input file name\n");
        return -1;
    }
    filename = strdup(argv[1]);

    //Write in the hdf5 file
    printf("Write %s \n", filename);
    strcpy(single.label, "A single label");
    file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Fclose(file_id);

    // Read for the hdf5 file
    printf("Reading %s\n", filename);
    file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    status = H5Fclose(file_id);

    printf("End\n");
    return 0;
}
