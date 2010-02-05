/*
 * datasettest.c
 *
 *  Created on: 2010/02/04
 *      Author: Cyril Giraudon
 */

#include <hdf5.h>
#include <hdf5_hl.h>
#include <dataset.h>

int main(argc,argv)
int argc;
char *argv[];
{
	char * filename;
	int values_len = 5;
	char values[4][5] = {"Go", "Toto", "Run", "Titi"};
	int rank = 1;
	hsize_t dims[] = {4};

	hid_t file_id;

    // Initialization
	H5open();
	if(argc==1)
	{
	  printf("missing input file name\n");
	  return -1;
	}
	filename = (char *)malloc(100*sizeof(char));
	strcpy(filename,argv[1]);

	// Read for the hdf5 file
	printf("Creating %s ...\n",filename);
	file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	write_string_dataset(file_id, "string_data", values_len, rank, dims, values);

	H5Fclose(file_id);
	free(filename);
	return 0;
}
