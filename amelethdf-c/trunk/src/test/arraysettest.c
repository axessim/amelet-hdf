#include <hdf5.h>
#include <hdf5_hl.h>
#include <string.h>
#include <arrayset.h>
#include <complex.h>
#include <stdio.h>

int main(argc,argv)
int argc;
char *argv[];
{
	  hid_t status;
	  char *filename;
	  arrayset_t ars;
	  arrayset2_t ars2,ars2c;
	  hid_t file_id;
	  int i,j,offset;

      // initialisatin
	  H5open();
	  if(argc==1)
		  {
		  printf("missing input file name\n");
		  return -1;
		  }
	  filename = (char *)malloc(100*sizeof(char));
	  strcpy(filename,argv[1]);
	  // Read for the hdf5 file
	  printf("Reading %s\n",filename);
	  file_id = H5Fopen(filename,H5F_ACC_RDWR,H5P_DEFAULT);

	  //read a real arrayset2_t
	  ars2 = read_arrayset2(file_id,"/a_real_arrayset2");
	  printf("\n/a_real_arrayset2\n");
	  printf("floatingType : \n");
	  printf("%s",single_to_string(ars2.single));
	  printf ("Data : \n");
	  for(i=0;i<ars2.data.nbvalues;i++)
		  printf("%f ",ars2.data.rvalue[i]);
	  printf ("\nDim : \n");
	  for(i=0;i<ars2.dim1.nbvalues;i++)
	    printf("%f ",ars2.dim1.rvalue[i]);
	  printf("\n");
	  ars2=clear_content_ars2(ars2);

	  // Read a complex arrayser2_t
	  ars2 = read_arrayset2(file_id,"/a_complex_arrayset2");
	  printf("\n/a_complex_arrayset2\n");
	  printf("floatingType :\n");
	  printf("%s",single_to_string(ars2.single));
	  printf ("Data : \n");
	  for(i=0;i<ars2.data.nbvalues;i++)
		  printf("%f +j*%f ; ",crealf(ars2.data.cvalue[i]),cimagf(ars2.data.cvalue[i]));
	  printf ("\nDim : \n");
	  for(i=0;i<ars2.dim1.nbvalues;i++)
		  printf("%f ",ars2.dim1.rvalue[i]);
	  printf("\n");

	  ars2=clear_content_ars2(ars2);

	  // Read a real arrayset_t
	  ars = read_arrayset(file_id,"/a_real_arrayset");
	  printf("\n/a real arrayset\n");
	  printf("floatingType :\n");
	  printf("%s",single_to_string(ars.single));
	  printf ("Data : \n");
	  printf("dimensions %i (%ix%i)\n",ars.nbdims,(int)ars.data.dims[1],(int)ars.data.dims[0]);
	  offset=0;
	  for(i=0;i<ars.data.dims[1];i++)
	  {
		  for(j=0;j<ars.data.dims[0];j++)
			  printf("%f ",ars.data.rvalue[j+offset]);
		  offset = offset + ars.data.dims[1]+1;
		  printf("\n");
	  }

	  for(i=0;i<ars.data.nbdims;i++)
	  {
		  printf("dim %i : ",i);
		  for(j=0;j<ars.data.dims[i];j++)
			  printf("%f ",ars.dims[i].rvalue[j]);
		  printf("\n");
	  }


	  status = H5Fclose(file_id);
      free(filename);
	  return 0;
}
