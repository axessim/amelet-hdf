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
    int max=1;
    if(ars.nbdims>1)
    {
        max=ars.nbdims-1;
        for(int i=1;i<ars.nbdims;i++)
            nbdataarray = nbdataarray*ars.dims[ars.nbdims-i].nbvalues;
    }
    //set data name
    vtkstd::string dataname[nbdataarray];
    for(int i=0;i<nbdataarray;i++)
    	dataname[i]= child.childnames[0];
    int temp=nbdataarray;
    for(int i=0;i<max;i++)
    {
        temp = (int)temp/ars.dims[ars.nbdims-i-1].nbvalues;
        if(temp<1) temp=1;
        int j=0;
        while(j<nbdataarray)
        {
            if(ars.dims[ars.nbdims-i-1].cvalue!=NULL)
            {
                for(int l=0;l<ars.dims[ars.nbdims-i-1].nbvalues;l++)
                {
                    std::ostringstream buf;
                    buf<<"_"<<crealf(ars.dims[ars.nbdims-i-1].cvalue[l])<<"_j"<<cimagf(ars.dims[ars.nbdims-i-1].cvalue[l]);
                    for(int k=0;k<temp;k++)
                        dataname[j+k]=dataname[j+k]
                                      +"_"+ars.dims[ars.nbdims-i-1].single.label
                                      +buf.str();
                 }
                    j=j+temp;
            }
            else if(ars.dims[ars.nbdims-i-1].rvalue!=NULL)
            {
                for(int l=0;l<ars.dims[ars.nbdims-i-1].nbvalues;l++)
                {
                    std::ostringstream buf;
                    buf<<"_"<<ars.dims[ars.nbdims-i-1].rvalue[l];
                    for(int k=0;k<temp;k++)
                        dataname[j+k]=dataname[j+k]
                                      +"_"+ars.dims[ars.nbdims-i-1].single.label
                                      +buf.str();
                }
                
                j=j+temp;
                
            }
            else if(ars.dims[ars.nbdims-i-1].ivalue!=NULL)
            {
                for(int l=0;l<ars.dims[ars.nbdims-i-1].nbvalues;l++)
                {
                    std::ostringstream buf;
                    buf<<"_"<<ars.dims[ars.nbdims-i-1].ivalue[l];
                    for(int k=0;k<temp;k++)
                      dataname[j+k]=dataname[j+k]
                                      +"_"+ars.dims[ars.nbdims-i-1].single.label
                                      +buf.str();
                }
                j=j+temp;
                
            }
            else if(ars.dims[ars.nbdims-i-1].svalue!=NULL)
            {
                for(int l=0;l<ars.dims[ars.nbdims-i-1].nbvalues;l++)
                {
                    for(int k=0;k<temp;k++)
                        dataname[j+k]=dataname[j+k]
                                      +"_"+ars.dims[ars.nbdims-i-1].single.label
                                      +"_"+ars.dims[ars.nbdims-i-1].svalue[l];
                }
                j=j+temp;
                
            }
        }

    }
    int offset = 0;
    for(int i=0;i<ars.nbdims;i++)
    {
        array = vtkFloatArray::New();
        array->SetName(ars.dims[i].single.label);
        offset = 0;
        int offsettemp=0;
        int nbtest = nbdataarray*ars.dims[0].nbvalues;
        while(offset<nbtest)
        {
            if(i>0)
            {
                int offset2=0;
                for(int k=0;k<i;k++)
                    offset2=offset2+ars.dims[k].nbvalues;
                cout<<"dims.nbvalues = "<<ars.dims[i].nbvalues<<endl;
                for(int k=0;k<ars.dims[i].nbvalues;k++)
                {
                    for(int j=0;j<offset2;j++)
                    {
                        if(ars.dims[i].rvalue!=NULL)
                            array->InsertTuple1(j+k+offsettemp,ars.dims[i].rvalue[k]);
                        else
                            cout<<"Attention !!!!"<<endl;
                        offset=offset+1;
                    } 
                    offsettemp=offsettemp+offset2-1;
                }
            }
            else if(i==0)
            {
                for(int j=0;j<ars.dims[i].nbvalues;j++)
                    array->InsertTuple1(j+offset,ars.dims[i].rvalue[j]);
                offset +=ars.dims[0].nbvalues;
            }
        }
        table->AddColumn(array);
        array->Delete();
    }
    offset=0;
    array = vtkFloatArray::New();
    array->SetName(dataname[0].c_str());
    for(int i=0;i<nbdataarray;i++)
    {
        // get the column name
        for (int j=0;j<ars.dims[0].nbvalues;j++)
        {
        	if(ars.data.rvalue!=NULL)
        	{
        		array->InsertTuple1(j+offset,ars.data.rvalue[j+offset]);
        	}
        	else if(ars.data.cvalue!=NULL)
        	{
        		float module;
        		module=abs_complex(ars.data.cvalue[j+offset]);
        		array->InsertTuple1(j+offset,module);
        	}

        }
        offset=offset+ars.dims[0].nbvalues;
        //table->AddColumn(array);
    }
    table->AddColumn(array);
    array->Delete();
    //add ia dummy column 
    array = vtkFloatArray::New();
    array->SetName("dummy");
    for (int j=0;j<(ars.dims[0].nbvalues*nbdataarray);j++)
    {
        array->InsertTuple1(j,0.0);
    }
    table->AddColumn(array);
    array->Delete();

    return 1;
}
