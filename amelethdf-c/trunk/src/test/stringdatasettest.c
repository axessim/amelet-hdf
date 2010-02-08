/*
 * datasettest.c
 *
 *  Created on: 2010/02/04
 *      Author: Cyril Giraudon
 */

#include <hdf5.h>
#include <dataset.h>

int main(argc,argv)
int argc;
char *argv[];
{
	char * filename;
	int values_len = 34;

	char values[2][3][34] = {{"/floatingType/far_field_theta_phi", "farfield.h5",
                                  "/floatingType/far_field_theta_phi"},
		                  {"/floatingType/Enear_field_plane1",
		                   "monitor_plane1_efield.h5",
		                   "/floatingType/Enear_field_plane1"}};
	int rank = 2;
	hsize_t dims[2] = {2,3};

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
