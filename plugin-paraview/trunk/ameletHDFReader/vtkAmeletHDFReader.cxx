#include <vtkAmeletHDFReader.h>



extern "C" {
    #include "amelethdf.h"
    #include "mesh.h"
    #include "arrayset.h"
    #include "hdfpath.h"
}


#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()


#define TRUE            1
#define FALSE           0

//using namespace vtkstd;

// Standard VTK Macros for vtkObject derived Classes
vtkCxxRevisionMacro(vtkAmeletHDFReader, "$Revision: 0.1$");
vtkStandardNewMacro(vtkAmeletHDFReader);


vtkInformationKeyMacro(vtkAmeletHDFReader, IS_UMESH, Integer);
vtkInformationKeyMacro(vtkAmeletHDFReader, IS_SMESH, Integer);
vtkInformationKeyMacro(vtkAmeletHDFReader, IS_DATAONMESH, Integer);
vtkInformationKeyMacro(vtkAmeletHDFReader, IS_DATA, Integer);
vtkInformationKeyMacro(vtkAmeletHDFReader, POINTS, ObjectBase);
vtkInformationKeyMacro(vtkAmeletHDFReader, POINT_DATA, ObjectBase);

//=============================================================================
// A convenience function that gets a block from a multiblock data set,
// performing allocation if necessary.
static vtkUnstructuredGrid *AllocateGetBlock(vtkMultiBlockDataSet *blocks,
                                             unsigned int blockno,
                                             vtkInformationIntegerKey *typeKey)
{
  if (blockno > 1000)
    {
    vtkGenericWarningMacro(<< "Unexpected block number: " << blockno);
    blockno = 0;
    }

  if (blocks->GetNumberOfBlocks() <= blockno)
    {
    blocks->SetNumberOfBlocks(blockno+1);
    }

  vtkUnstructuredGrid *grid
    = vtkUnstructuredGrid::SafeDownCast(blocks->GetBlock(blockno));
  if (!grid)
    {
    grid = vtkUnstructuredGrid::New();
    blocks->SetBlock(blockno, grid);
    blocks->GetMetaData(blockno)->Set(typeKey, 1);
    grid->Delete();     // Not really deleted.
    }

  return grid;
}

// -----------------------------------------------------------------------------
//
int getAmeletHDFAttribut(hid_t loc_id, const char *attr_name, const char *attr_value)
{
	herr_t   status;
	hid_t    attr_id, attr_type, space, memtype;
	size_t sdim;
	hsize_t     dims[1]={1} ;
	int ndims, i, ret_val;
	char **rdata;

	ret_val = 0;
	attr_id = H5Aopen(loc_id,attr_name,H5P_DEFAULT);



	if (int(attr_id) >= 0)
	{
	  attr_type = H5Aget_type(attr_id);
	  sdim = H5Tget_size(attr_type);
	  sdim++; /* Make room for null terminator */
	  space = H5Aget_space (attr_id);
	  ndims = H5Sget_simple_extent_dims (space, dims, NULL);
	  rdata = (char **) malloc (dims[0] * sizeof (char *));
	  rdata[0] = (char *) malloc (dims[0] * sdim * sizeof (char));
	  /*
	   * Set the rest of the pointers to rows to the correct addresses.
	  */
	  for (i=1; i<dims[0]; i++)
	      rdata[i] = rdata[0] + i * sdim;
	  memtype = H5Tcopy (H5T_C_S1);
	  status = H5Tset_size (memtype, sdim);
	  status = H5Aread(attr_id, memtype, rdata[0]);
	  // Check the value of FORMAT attribute.
	  if(strcmp(rdata[0],attr_value)==0) ret_val=1;
	  H5Aclose(attr_id);
	  free(rdata);
	}
	return ret_val;

}
// -----------------------------------------------------------------------------
//

int getAmeletHDFDataType(char* fileName)
{
	int fileType=0;
	hid_t    file_id;

	file_id = H5Fopen (fileName, H5F_ACC_RDONLY, H5P_DEFAULT);

	// test if arrayset group is present


	if(H5Lexists(file_id,"/floatingType",H5P_DEFAULT)!=FALSE)
	{
		// Is a data on mesh ?
		if (H5Lexists(file_id,"/mesh",H5P_DEFAULT)!=FALSE) fileType = 1; // data on mesh
		else fileType = 2; // data
	}
	else
	{
		if (H5Lexists(file_id,"/mesh",H5P_DEFAULT)!=FALSE) fileType = 3; // mesh

	}
	H5Fclose(file_id);
    return fileType;

}
//----------------------------------------------------------------------------
class WithinTolerance: public std::binary_function<double, double, bool>
{
public:
  WithinTolerance(double tol) { this->tolerance = tol; }
  double tolerance;
  //
    result_type operator()(first_argument_type a, second_argument_type b) const
    {
      bool result = (fabs(a-b)<=(a*this->tolerance));
      return (result_type)result;
    }
};
// -----------------------------------------------------------------------------
//
int vtkAmeletHDFReader::ReadDataOnMesh(hid_t file_id, vtkMultiBlockDataSet *output)
{
	vtkUnstructuredGrid *grid = AllocateGetBlock(output, 0,IS_DATAONMESH());
	hid_t loc_id, mesh_id;
	children_t meshChild, child;
	vtkAmeletHDFMeshReader ahdfmesh;
	int meshType;
    char  attr_value[ELEMENT_NAME_LENGTH];
    char  path[ABSOLUTE_PATH_NAME_LENGTH];
    char  attr[ELEMENT_NAME_LENGTH];
    arrayset_t ars;

	//read mesh under "/mesh"
    loc_id = H5Gopen(file_id, "/mesh",H5P_DEFAULT);
    child = read_children_name(file_id,"/mesh");
    if(child.nbchild>1)
    {
     	   vtkErrorMacro("This is more than one mesh in the ameletHDF file.");
     	   return 0;
    }
    mesh_id =  H5Gopen(loc_id, child.childnames[0],H5P_DEFAULT);
    meshChild = read_children_name(loc_id,child.childnames[0]);
    if(meshChild.nbchild>1)
    {
      	   vtkErrorMacro("This is more than one mesh in the ameletHDF file.");
       	   return 0;
    }
    meshType = meshtype(mesh_id,meshChild.childnames[0]);
    if(meshType==UNSTRUCTURED)
    {
       ahdfmesh.readUmesh(mesh_id,meshChild.childnames[0],grid);
    }
    else if(meshType==STRUCTURED)
    {
       ahdfmesh.readSmesh(mesh_id,meshChild.childnames[0],grid);
    }

    H5Gclose(mesh_id);
    H5Gclose(loc_id);
    delete(meshChild.childnames);
    child = read_children_name(file_id,"/floatingType");
    if(child.nbchild>1)
    {
    	   vtkErrorMacro("This is more than one arrayset in the ameletHDF file.");
       	   return 0;
    }
    strcpy(attr,"floatingType");
    strcpy(path,"/floatingType");
    strcat(path,"/");
    strcat(path,child.childnames[0]);
    strcpy(attr_value,read_string_attribute(file_id,path,attr));
    if(strcmp(attr_value,"arraySet")!=0)
    {
    	vtkErrorMacro("This is not an arrayset in the ameletHDF file.");
    	return 0;
    }


    ars = read_arrayset(file_id,path);
    if(strcmp(ars.dims[0].single.physical_nature,"meshEntity")!=0)
    {
    	vtkErrorMacro("This is not a data on mesh file !!!");
    	return 0;
    }



    char **dataname;
    if(ars.nbdims>1)
    {
    	int nbdataarray=1;
    	int max=ars.nbdims-1;
    	if(strcmp(ars.dims[1].single.physical_nature,"component")==0)max=ars.nbdims-2;
    	else if(strcmp(ars.dims[1].single.physical_nature,"time")==0)max=ars.nbdims-2;
    	for(int i=0;i<max;i++)
    			nbdataarray = nbdataarray*ars.dims[ars.nbdims-i-1].nbvalues;

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
        char meshEntity[ABSOLUTE_PATH_NAME_LENGTH];
        strcat(path,"/ds/dim1");
        strcpy(attr,"meshEntity");
        strcpy(meshEntity,read_string_attribute(file_id,path,attr));
        cout<<meshEntity<<endl;
        char * grp = path_element(meshEntity,1,1);

        if(strcmp(grp,"group")==0)
        {
        	hid_t grp_id = H5Dopen1(file_id,meshEntity);
        	ugroup_t grp;
        	sgroup_t sgrp;
        	char *grpname = path_element(meshEntity,0,1);
        	char *meshname = path_element(meshEntity,2,1);
        	char *meshgrp = path_element(meshEntity,3,1);
        	char pathmesh[ABSOLUTE_PATH_NAME_LENGTH];
        	strcpy(pathmesh,"/mesh/");
        	strcat(pathmesh,meshgrp);
        	strcat(pathmesh,"/");
        	strcat(pathmesh,meshname);
        	int meshType =  meshtype(file_id,pathmesh);
        	if(meshType == 1)
        		{
        		 grp = readUGroup(grp_id,grpname);
        		}
        	else sgrp = readSGroup(grp_id,grpname);

        	strcpy(attr,"type");
        	char *type;
            cout<<nbdataarray<<endl;
        	int offset=0;
        	int nbeltgrp;
        	if(meshType==1)nbeltgrp=grp.nbeltgroup;
        	else nbeltgrp=sgrp.nbelt;
        	for(int i=0;i<nbdataarray;i++)
        	{
       			vtkFloatArray *floatscalar = vtkFloatArray::New();
       			floatscalar->SetName(dataname[i].c_str());
       			if(strcmp(ars.dims[1].single.physical_nature,"component")==0)
       			{
       				cout<<"component"<<endl;
       				floatscalar->SetNumberOfComponents(3);
       				for(int j=0;j<ars.dims[1].nbvalues;j++)
       				{
       					for(int k=0;k<nbeltgrp;k++)
       					{
       					  if(j<3)
       					  {
       						  if(ars.data.rvalue!=NULL)
       						  {
       							  if(meshType==1) floatscalar->InsertComponent(grp.eltgroup[k],j,ars.data.rvalue[k+j+offset]);
       							  else floatscalar->InsertComponent(k,j,ars.data.rvalue[k+j+offset]);
       						  }
       						  else if(ars.data.cvalue!=NULL)
       						  {
       							  float module;
       							  module=cabs(ars.data.cvalue[k+offset]);
       							  if(meshType==1) floatscalar->InsertComponent(grp.eltgroup[k],j,module);
       							  else floatscalar->InsertComponent(k,j,module);
       						  }
       					  }
        				}
        				offset=offset+nbeltgrp;
        			}
       				if(strcmp(read_string_attribute(file_id,meshEntity,attr),"node")==0) grid->GetPointData()->AddArray(floatscalar);
       				if(strcmp(read_string_attribute(file_id,meshEntity,attr),"element")==0) grid->GetCellData()->AddArray(floatscalar);
        		}
       			else if(strcmp(ars.dims[1].single.physical_nature,"time")==0)
       			{
					this->TimeStepMode = true;
					for(int j=0;j<ars.dims[1].nbvalues;j++)
					{
						for(int k=0;k<nbeltgrp;k++)
						{
							if(ars.data.rvalue!=NULL)
					       	{
								if(this->ActualTimeStep==j)
								{
								if(meshType==1) floatscalar->InsertTuple1(grp.eltgroup[k],ars.data.rvalue[k+offset]);
					       		else floatscalar->InsertTuple1(k,ars.data.rvalue[k+offset]);
								}
					       	}
					       	else if(ars.data.cvalue!=NULL)
					       	{
					       		if(this->ActualTimeStep==j)
					       		{
					       		float module;
					       		module=cabs(ars.data.cvalue[k+offset]);
					       		if(meshType==1) floatscalar->InsertTuple1(grp.eltgroup[k],module);
					       		else floatscalar->InsertTuple1(k,module);
					       		}
					       	}

						}
					    offset=offset+nbeltgrp;
					    if(this->ActualTimeStep==j)
					    {
					    if(strcmp(read_string_attribute(file_id,meshEntity,attr),"node")==0)
						grid->GetPointData()->AddArray(floatscalar);
					    if(strcmp(read_string_attribute(file_id,meshEntity,attr),"element")==0)
					    {
					    	grid->GetCellData()->AddArray(floatscalar);
					    	double timevalue = (double)ars.dims[1].rvalue[j];
					    	output->GetInformation()->Set(vtkDataObject::DATA_TIME_STEPS(),&timevalue,1);
					    }
					    }
					}



       			}
       			else
       			{
       				for(int k=0;k<nbeltgrp;k++)
       				{
       					if(ars.data.rvalue!=NULL)
       					{
       						if(meshType==1) floatscalar->InsertTuple1(grp.eltgroup[k],ars.data.rvalue[k+offset]);
       						else floatscalar->InsertTuple1(k,ars.data.rvalue[k+offset]);
       					}
       					else if(ars.data.cvalue!=NULL)
       				   	{
       				   		float module;
       				   		module=cabs(ars.data.cvalue[k+offset]);
       				   		if(meshType==1) floatscalar->InsertTuple1(grp.eltgroup[k],module);
       				   		else floatscalar->InsertTuple1(k,module);
       				   	}

       				}
       				 offset=offset+nbeltgrp;
       				 if(strcmp(read_string_attribute(file_id,meshEntity,attr),"node")==0) grid->GetPointData()->AddArray(floatscalar);
       				 if(strcmp(read_string_attribute(file_id,meshEntity,attr),"element")==0) grid->GetCellData()->AddArray(floatscalar);

       			}
        	}
            H5Dclose(grp_id);
        }
    }




    delete(child.childnames);
	return 1;
}

// -----------------------------------------------------------------------------
//
vtkAmeletHDFReader::vtkAmeletHDFReader()
{
	this->SetNumberOfInputPorts(0);

	this->FileName = NULL;
	this->TimeStep = 0;
	this->ActualTimeStep = 0;
	this->NumberOfTimeSteps = 0;
	this->TimeStepRange[0] = 0;
	this->TimeStepRange[1] = 0;
	this->TimeStepMode = false;

}

// -----------------------------------------------------------------------------
//
vtkAmeletHDFReader::~vtkAmeletHDFReader()
{
	this->SetFileName(NULL);
}

// -----------------------------------------------------------------------------
//
void vtkAmeletHDFReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "FileName: " << this->FileName << endl;
}

// -----------------------------------------------------------------------------
//
int vtkAmeletHDFReader::FillInputPortInformation(int, vtkInformation *info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkMultiBlockDataSetAlgorithm");
  return 1;
}

// -----------------------------------------------------------------------------
//
int vtkAmeletHDFReader::CanReadFile(const char *filename)
{

  hid_t    file_id, group_id;
  int ret_val;


  ret_val = 0;
  file_id = H5Fopen (filename, H5F_ACC_RDONLY, H5P_DEFAULT);
  // Check FORMAT and AMELETHDF_FORMAT_VERSION attributes values
  group_id = H5Gopen(file_id,"/",H5P_DEFAULT);
  ret_val = getAmeletHDFAttribut(group_id, "FORMAT", "AMELETHDF");
  ret_val = getAmeletHDFAttribut(group_id, "AMELETHDF_FORMAT_VERSION", "1.0.0");

  H5Gclose(group_id);

  H5Fclose(file_id);
  return ret_val;


}

// -----------------------------------------------------------------------------
//
int vtkAmeletHDFReader::RequestData( vtkInformation *request,
                                      vtkInformationVector **inputVector,
                                      vtkInformationVector *outputVector)
{
  int dataType = 0;
  hid_t file_id, loc_id;
  herr_t status;


  // get the info objects
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkMultiBlockDataSet *output = vtkMultiBlockDataSet::GetData(outInfo);
  this->UpdateProgress(0.);

  if (!this->FileName)
     {
     vtkErrorMacro("No filename specified.");
     return 0;
     }
  // Test data type (unstructured mesh, structured mesh, data on mesh, or data ?)
  dataType = getAmeletHDFDataType(this->FileName);
  if (dataType==0)
      {
	  vtkErrorMacro("This is not a mesh, data or data on mesh ameletHDF file.");
	  return 0;
	  }
  children_t child;
  file_id = H5Fopen (this->FileName, H5F_ACC_RDONLY, H5P_DEFAULT);

  if(dataType==1)
  {

	  //read mesh under "/mesh"
	  loc_id = H5Gopen(file_id, "/mesh",H5P_DEFAULT);
	  child = read_children_name(file_id,"/mesh");
	  if(child.nbchild>1)
	  {
	   	   vtkErrorMacro("This is more than one mesh in the ameletHDF file.");
	   	   return 0;
	  }

	  //vtkUnstructuredGrid *grid = AllocateGetBlock(output, 0,IS_DATAONMESH());
	  if(outInfo->Has(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS()) && this->TimeStepMode )
	  {
		  double requestedTimeValue = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS())[0];
		  int nSteps =
		        outInfo->Length(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
		  double* steps =
		        outInfo->Get(vtkStreamingDemandDrivenPipeline::TIME_STEPS());

		  this->ActualTimeStep =
		        vtkstd::find_if(this->TimeStepValues.begin(), this->TimeStepValues.end(),
		                        vtkstd::bind2nd( WithinTolerance(1e-9), requestedTimeValue ))
		        - this->TimeStepValues.begin();


		 output->GetInformation()->Set(vtkDataObject::DATA_TIME_STEPS(),
		                                    &TimeStepValues[this->ActualTimeStep], 1);
	  }
	  this->ReadDataOnMesh(file_id,output);
      this->UpdateProgress(1);
   	  //loc_id = H5Gopen(file_id,"/floatingType",H5P_DEFAULT);


  }
  else if(dataType==2)
  {
   	  cout<<"data conversion"<<endl;
   	  vtkTable *table = vtkTable::New();
   	  output->SetBlock(0,table);
   	  vtkAmeletHDFDataReader ahdfdata;
   	  ahdfdata.readData(file_id,table);


  }
  else if(dataType==3) //mesh
  {
	  vtkAmeletHDFMeshReader ahdfmesh;
	  //read mesh under "/mesh"
      loc_id = H5Gopen(file_id, "/mesh",H5P_DEFAULT);
      child = read_children_name(file_id,"/mesh");
      if(child.nbchild>1)
      {
       	   vtkErrorMacro("This is more than one mesh in the ameletHDF file.");
       	   return 0;
      }
      hid_t mesh_id;
      mesh_id =  H5Gopen(loc_id, child.childnames[0],H5P_DEFAULT);
      children_t meshChild;
      meshChild = read_children_name(loc_id,child.childnames[0]);
      for(int i=0;i<meshChild.nbchild;i++)
      {
       	   int meshType;
       	   meshType = meshtype(mesh_id,meshChild.childnames[i]);
       	   if(meshType==UNSTRUCTURED)
       	   {
       	       vtkUnstructuredGrid *ugrid = AllocateGetBlock(output, i,IS_UMESH());
       	       ahdfmesh.readUmesh(mesh_id,meshChild.childnames[i],ugrid);
       		   //this->readUmesh(mesh_id,meshChild.childnames[i],ugrid);
       	   }
       	   else if(meshType==STRUCTURED)
       	   {
       		   vtkUnstructuredGrid *sgrid = AllocateGetBlock(output, i , IS_SMESH());
       		   ahdfmesh.readSmesh(mesh_id,meshChild.childnames[i],sgrid);
       		   //this->readSmesh(mesh_id,meshChild.childnames[i],sgrid);
       	   }
       	   this->UpdateProgress(this->GetProgress()+i/(meshChild.nbchild));
      }
      H5Gclose(mesh_id);
      H5Gclose(loc_id);
  }
  else if(dataType>3 || dataType<1)
  {
	  vtkErrorMacro("This is not an ameletHDF data or mesh file .");
	  return 0;
  }
  H5Fclose(file_id);
  return 1;
}
// -----------------------------------------------------------------------------
//
int vtkAmeletHDFReader::RequestInformation(vtkInformation *vtkNotUsed(request),
                                           vtkInformationVector **vtkNotUsed(inputVector),
                                           vtkInformationVector *outputVector)
{
	vtkInformation* outInfo = outputVector->GetInformationObject(0);
	int dataType=0;


	vtkDebugMacro("RequestInformation");
	if (!this->FileName)
	    {
	    vtkErrorMacro("No filename specified.");
	    return 0;
	    }
	// Test data type (unstructured mesh, structured mesh, data on mesh, or data ?)
    dataType = getAmeletHDFDataType(this->FileName);
    if(dataType==1)
    {
    	//time series ?
    	hid_t file_id=H5Fopen(this->FileName,H5F_ACC_RDONLY, H5P_DEFAULT);
    	char path[ABSOLUTE_PATH_NAME_LENGTH];
    	strcpy(path,"/floatingType");
    	children_t child;
    	child=read_children_name(file_id,path);
    	strcat(path,"/");
    	strcat(path,child.childnames[0]);
    	strcat(path,"/ds/dim2");
    	vector_t vec=read_vector(file_id,path);
    	this->TimeStepMode = false;
    	if(strcmp(vec.single.physical_nature,"time")==0)
    	{
    		outInfo->Remove(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
    		this->TimeStepMode = true;

    		//this->TimeStepValues=new double[vec.nbvalues];
    		for(int i=0;i<vec.nbvalues;i++)
    		{
    		   //outInfo->Append(vtkStreamingDemandDrivenPipeline::TIME_STEPS(),vec.rvalue[i]);
    		   this->TimeStepValues.push_back(vec.rvalue[i]);

    		}
    		this->TimeStepRange[0] = 0;
    		this->TimeStepRange[1] = vec.nbvalues - 1;
    		outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_STEPS(),
    		               &this->TimeStepValues[0], this->TimeStepValues.size());
    		double timeRange[2];
    		timeRange[0]=this->TimeStepValues.front();
    		timeRange[1]=this->TimeStepValues.back();
    		outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_RANGE(), timeRange, 2);

    	}
    	H5Fclose(file_id);

    }
    if (dataType==0) return 0;

	return 1;
}


