/*
 * vtkAmeletHDFDataReader.cxx
 *
 *  Created on: 8 janv. 2010
 *      Author: didier
 */

#include "vtkAmeletHDFDataReader.h"



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
    int xdim = -1; int ydim = -1; int zdim = -1;
    int nbdataarray=1;
    int max=1;
    
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

    for(int i=0;i<ars.nbdims;i++)
            nbdataarray = nbdataarray*ars.dims[i].nbvalues;
    vtkstd::string dataname;
    dataname= child.childnames[0];
    int temp=nbdataarray;
    
    int offset = 0;

    for(int i=0;i<ars.nbdims;i++)
    {
      if(ars.dims[i].rvalue!=NULL)
      {
        array = vtkFloatArray::New();
        array->SetName(ars.dims[i].single.label);
        table->AddColumn(array);
        array->Delete();
      }
      else if(ars.dims[i].svalue!=NULL)
      {
        array = vtkFloatArray::New();
        std::cout<<ars.dims[i].single.label<<std::endl;
        array->SetName(ars.dims[i].single.label);
        table->AddColumn(array);
        array->Delete();
      }
    }
    array = vtkFloatArray::New();
    array->SetName(child.childnames[0]);
    table->AddColumn(array);
    array->Delete();
    table->SetNumberOfRows(nbdataarray);
    
    int offsettemp=1;
    for(int j=0;j<ars.nbdims;j++)
    {
      offset=0;
      for(int k=0;k<ars.dims[j].nbvalues;k++)
      {
        for(int itemp=0;itemp<offsettemp;itemp++)
        {
          //std::cout<<"offset="<<offset<<std::endl;
          if(ars.dims[j].rvalue!=NULL)
          {
            table->SetValue(offset,j,ars.dims[j].rvalue[k]);
          }
          else if(ars.dims[j].svalue!=NULL)
          {
            table->SetValue(offset,j,float(k));
          }
          offset=offset+1;
        }
        if(offset<nbdataarray){
          if(k==(ars.dims[j].nbvalues-1)) k=-1;}
      }
      offsettemp=offsettemp*(ars.dims[j].nbvalues);
    }
    
    for(int i=0;i<nbdataarray;i++)
    {
      if(ars.data.rvalue!=NULL)
        table->SetValue(i,ars.nbdims,ars.data.rvalue[i]);
      else if(ars.data.cvalue!=NULL)
      {
        float module;
        module=abs_complex(ars.data.cvalue[i]);
        table->SetValue(i,ars.nbdims,module);
      }  
    }
   
    if(child.nbchild>1) free(child.childnames[0]);
        free(child.childnames);
    return 1;
}
