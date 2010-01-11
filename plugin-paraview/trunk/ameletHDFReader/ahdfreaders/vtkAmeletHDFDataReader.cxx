/*
 * vtkAmeletHDFDataReader.cxx
 *
 *  Created on: 8 janv. 2010
 *      Author: didier
 */

#include "vtkAmeletHDFDataReader.h"


extern "C" {
    #include "amelethdf.h"
    #include "arrayset.h"
}

#define TRUE            1
#define FALSE           0

int vtkAmeletHDFDataReader::readData(hid_t file_id, vtkTable *table)
{
    vtkFloatArray *array;
    arrayset_t ars;
    children_t child;
    char  path[ABSOLUTE_PATH_NAME_LENGTH];
    char  attr_value[ELEMENT_NAME_LENGTH];
    char  attr[ELEMENT_NAME_LENGTH];

    child = read_children_name(file_id,"/floatingType");
    if(child.nbchild>1)
    {
    	cout<<"This is more than one arrayset in the ameletHDF file."<<endl;
        return 0;
    }
    strcpy(attr,"floatingType");
    strcpy(path,"/floatingType");
    strcat(path,"/");
    strcat(path,child.childnames[0]);
    strcpy(attr_value,read_string_attribute(file_id,path,attr));
    if(strcmp(attr_value,"arraySet")!=0)
    {
      	cout<<("This is not an arrayset in the ameletHDF file.")<<endl;
      	return 0;
    }

    ars = read_arrayset(file_id,path);

    int nbdataarray=1;
    int max;
    if(ars.nbdims>1)
    {
        	nbdataarray=1;
        	max=ars.nbdims-1;
        	for(int i=0;i<max;i++)
        			nbdataarray = nbdataarray*ars.dims[ars.nbdims-i-1].nbvalues;
    }
    //set data name
    vtkstd::string dataname[nbdataarray];
    for(int i=0;i<nbdataarray;i++)
    	dataname[i]= child.childnames[0];
    int temp =  nbdataarray;
    for(int i=0;i<max;i++)
    	{
    	   	temp = (int)temp/ars.dims[ars.nbdims-i-1].nbvalues;
          	int j=0;
           	while(j<nbdataarray)
           	{
        	  if(ars.dims[ars.nbdims-i-1].cvalue!=NULL)
        		  for(int l=0;l<ars.dims[ars.nbdims-i-1].nbvalues;l++)
        		  {
        			  std::ostringstream buf;
        			  buf<<"_"<<crealf(ars.dims[ars.nbdims-i-1].cvalue[l])<<"_j"<<cimagf(ars.dims[ars.nbdims-i-1].cvalue[l]);
        			  for(int k=0;k<temp;k++)
        			  dataname[j+k]=dataname[j+k]
        			                           +"_"+ars.dims[ars.nbdims-i-1].single.label
        			                           +buf.str();
        			  j=j+temp;
        		  }
              else if(ars.dims[ars.nbdims-i-1].rvalue!=NULL)
            	for(int l=0;l<ars.dims[ars.nbdims-i-1].nbvalues;l++)
            	{
            	    std::ostringstream buf;
            		buf<<"_"<<ars.dims[ars.nbdims-i-1].rvalue[l];
            		for(int k=0;k<temp;k++)
            		dataname[j+k]=dataname[j+k]
            		                +"_"+ars.dims[ars.nbdims-i-1].single.label
            		                +buf.str();
            		j=j+temp;
            	}
              else if(ars.dims[ars.nbdims-i-1].ivalue!=NULL)
                for(int l=0;l<ars.dims[ars.nbdims-i-1].nbvalues;l++)
            	{
            	    std::ostringstream buf;
            		buf<<"_"<<ars.dims[ars.nbdims-i-1].ivalue[l];
             		for(int k=0;k<temp;k++)
           				  dataname[j+k]=dataname[j+k]
           				                           +"_"+ars.dims[ars.nbdims-i-1].single.label
           				                           +buf.str();
           				  j=j+temp;
           			  }
       		  else if(ars.dims[ars.nbdims-i-1].svalue!=NULL)
       			  for(int l=0;l<ars.dims[ars.nbdims-i-1].nbvalues;l++)
       			  {
       				  for(int k=0;k<temp;k++)
       				  dataname[j+k]=dataname[j+k]
           						  +"_"+ars.dims[ars.nbdims-i-1].single.label
           						  +"_"+ars.dims[ars.nbdims-i-1].svalue[l];
       				  j=j+temp;
       			  }
         	}

    }
    int offset=0;
    for(int i=0;i<nbdataarray;i++)
    {
    	array = vtkFloatArray::New();
        // get the column name
        array->SetName(dataname[i].c_str());
        for (int j=0;j<ars.dims[0].nbvalues;j++)
        {
        	if(ars.data.rvalue!=NULL)
        	{
        		array->InsertTuple1(j,ars.data.rvalue[j+offset]);
        	}
        	else if(ars.data.cvalue!=NULL)
        	{
        		float module;
        		module=cabs(ars.data.cvalue[j+offset]);
        		array->InsertTuple1(j,module);
        	}

        }
        offset=offset+ars.dims[0].nbvalues;
        table->AddColumn(array);
    }
    //add column for abs
    array = vtkFloatArray::New();
    array->SetName(ars.dims[0].single.label);
    for (int j=0;j<ars.dims[0].nbvalues;j++)
    {
    	if(ars.dims[0].rvalue!=NULL)
    	    array->InsertTuple1(j,ars.dims[0].rvalue[j]);
    	else if(ars.dims[0].ivalue!=NULL)
    		array->InsertTuple1(j,float(ars.dims[0].ivalue[j]));
    }

    table->AddColumn(array);






	return 1;
}
