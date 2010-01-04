#include <hdf5.h>
#include <hdf5_hl.h>
#include <stdlib.h>
#include <string.h>
#include "stringdataset.h"

// Read a long string dataset
char ** read_string_dataset2(hid_t file_id,const char * path, int l, int nb)
{
	char **buf;
	herr_t status;
	hid_t filetype, dset,space, memtype;
	size_t sdim;
	int i, ndims;
	hsize_t *dims;

	dims =(hsize_t *)malloc(sizeof(hsize_t));
	dset=H5Dopen(file_id,path,H5P_DEFAULT);
	filetype = H5Dget_type (dset);
    sdim = H5Tget_size (filetype);
    sdim++;
    space = H5Dget_space (dset);
    ndims = H5Sget_simple_extent_dims (space, dims, NULL);
	buf=(char **)malloc(nb*sizeof(char*));
	buf[0]=(char *)malloc(nb *sdim *sizeof(char));
	for (i=1; i<nb; i++)
	        buf[i] = buf[0] + i * sdim;
	memtype = H5Tcopy (H5T_C_S1);
	status = H5Tset_size (memtype, sdim);
	status = H5Dread (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, buf[0]);
	H5Dclose(dset);
    free(dims);
	return buf;
}

// Read a (m x n) string datset (rows x columns) of l characters
char *** read_string_dataset1(hid_t file_id,const char * path, int l, int m, int n)
{
	char **buf;
	char ***data;
	int i, j, k,offset;

    buf = (char **)malloc(m*n*sizeof(char*));
	buf[0] = (char *)malloc(l*m*n*sizeof(char));
	for (i=1; i<m*n; i++)
		        buf[i] = buf[0] + i * l;

	data = (char ***)malloc(n*sizeof(char**));
	for (i=0; i<n;i++)
	{
		data[i] = (char**) malloc(n*m*sizeof(char*));
                data[i][0] = (char *)malloc(n*m*l*sizeof(char));
		for(j=1;j<m;j++)
			data[i][j] = data[i][0] + j*l;
	}
	buf = read_string_dataset2(file_id,path,l,m*n);
        offset=0;
	for(i=0;i<n;i++)
        {
		for(j=0;j<m;j++)
		{
			data[i][j]=strdup(buf[j+offset]);
		}
                offset=offset+m;
       }
	free(buf);
	return data;
}

// Reads the length of strings contains in a dataset
// The dataset must be a string dataset (no check)
int *get_dataset_lmn(hid_t file_id, char* path)
{
	int *lmn;
	H5T_class_t type_class;
	int m=0, n=0;
	herr_t status;
	size_t type_size;
	hsize_t dims[2];

    lmn = (int *)malloc(3*sizeof(int));
    dims[0]=0;
    dims[1]=0;
	status = H5LTget_dataset_info(file_id,path,dims,&type_class,&type_size);
	m = dims[0];
	n = dims[1];
	lmn[0]=type_size;
	lmn[1]=n;
	lmn[2]=m;
	return lmn;
}

children_t read_string_vector(hid_t file_id, char* path)
{
	char ***vector;
	int *lmn;
	int i,j;
	children_t child;

	lmn = (int *)malloc(3*sizeof(int));
	lmn = get_dataset_lmn(file_id, path);
	if((lmn[1]!=0) && (lmn[1]!=1)) printf("%s must be a (n x 1) or (n) dataset\n",path);
	vector =(char ***) malloc(sizeof(char **));
	for (i=0; i<1;i++)
	{
		vector[i] = (char**) malloc(lmn[2]*sizeof(char*));
	    vector[i][0] = (char *)malloc(lmn[2]*lmn[0]*sizeof(char));
		for(j=1;j<lmn[2];j++)
			vector[i][j] = vector[i][0] + j*lmn[0];
	}
	vector = read_string_dataset1(file_id, path, lmn[0],lmn[2],1);
	child.childnames = (char **) malloc(lmn[2]*sizeof(char *));
	child.childnames[0] = (char*) malloc(lmn[0]*lmn[2]*sizeof(char));
	for(i=0;i<lmn[2];i++)
		strcpy(child.childnames[i],vector[0][i]);
	child.nbchild = lmn[2];
	return child;
}

//return the index of an element
int str_index(char ** arr, char *aelement, int nbel)
{
	int index = -1;
	int i;

	for(i=0;i<nbel;i++)
		if(strcmp(arr[i],aelement)==0) index=i;
	return index;
}
