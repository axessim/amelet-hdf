#include <vtkAmeletHDFReader.h>
#include <sstream>



#include <ah5.h>
#include <ah5_category.h>




#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()


#define TRUE            1
#define FALSE           0

//using namespace vtkstd;

// Standard VTK Macros for vtkObject derived Classes
vtkCxxRevisionMacro(vtkAmeletHDFReader, "$Revision: 0.2.0$");
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

float abs_complex(AH5_complex_t complex)
{
	float module;

	module = (complex.re*complex.re)+(complex.im+complex.im);
	module = sqrt(module);
	return module;
}
// -----------------------------------------------------------------------------
// test if the file is readable

bool is_readable( const std:: string & file)
{
  std::ifstream ameletfile( file.c_str());
  return !ameletfile.fail();
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
	vtkAmeletHDFMeshReader ahdfmesh;
	char* entryPoint = NULL;
	AH5_ft_t ft;
	int timedim=-1;
	int componentdim=-1;
	int meshentitydim=-1;
	int nbdataarray=1;
    cout<<"[vtkAmeletHDFReader::ReadDataOnMesh] begin"<<endl;

	AH5_read_str_attr(file_id, ".", AH5_A_ENTRY_POINT, &entryPoint);
	AH5_read_floatingtype(file_id, entryPoint, &ft);

	for (int i=0;i<ft.data.arrayset.nb_dims;i++)
	{
		for (int j=0;j<ft.data.arrayset.dims[i].opt_attrs.nb_instances;j++)
		{
			if(strcmp(ft.data.arrayset.dims[i].opt_attrs.instances[j].name,"physicalNature")==0)
			{
				if(strcmp(ft.data.arrayset.dims[i].opt_attrs.instances[j].value.s,"time")==0)
					timedim=i;
				else if(strcmp(ft.data.arrayset.dims[i].opt_attrs.instances[j].value.s,"frequency")==0)
					timedim=i;
				else if(strcmp(ft.data.arrayset.dims[i].opt_attrs.instances[j].value.s,"component")==0)
					componentdim=i;
				else if(strcmp(ft.data.arrayset.dims[i].opt_attrs.instances[j].value.s,"meshEntity")==0)
					meshentitydim=i;
			}
		}
	}
    //compute nbdataarray
	for (int i=0;i<ft.data.arrayset.nb_dims;i++)
	{
		if(i!=meshentitydim)
			if(i!=componentdim)
				if(i!=timedim)
					nbdataarray = nbdataarray*ft.data.arrayset.dims[i].nb_values;
	}

	// set data name
	vtkstd::string dataname[nbdataarray];
	for (int i=0;i<nbdataarray;i++)
		dataname[i]=AH5_get_name_from_path(ft.data.arrayset.path);

	int temp = nbdataarray;
	for(int i=0;i<ft.data.arrayset.nb_dims;i++)
	{
		if((ft.data.arrayset.nb_dims-i-1) != meshentitydim)
			if((ft.data.arrayset.nb_dims-i-1) != componentdim)
				if((ft.data.arrayset.nb_dims-i-1) != timedim)
				{
					temp = (int)temp/ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].nb_values;
					int j = 0 ;
					if(temp==0) j=nbdataarray;
					while(j<nbdataarray)
					{
						if(ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].type_class==H5T_COMPOUND)
							for (int l=0;l<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].nb_values;l++)
							{
								std::ostringstream buf;
								buf<<"_"<<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].values.c[l].re
										<<"_j"<<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].values.c[l].im;
								for (int k=0;k<temp;k++)
								{
									vtkstd::string label="";
									for(int ii=0;i<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.nb_instances;ii++)
										if(strcmp(ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.instances[i].name,"label")==0)
											label=label+ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.instances[i].value.s;
									dataname[j+k]=dataname[j+k]+"_"+label+buf.str();
								}
								j=j+temp;

							}
						else if(ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].type_class==H5T_FLOAT)
							for (int l=0;l<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].nb_values;l++)
							{
								std::ostringstream buf;
								buf<<"_"<<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].values.f[l];
								for (int k=0;k<temp;k++)
								{
									vtkstd::string label="";
									for(int ii=0;i<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.nb_instances;ii++)
										if(strcmp(ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.instances[i].name,"label")==0)
											label=label+ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.instances[i].value.s;
									dataname[j+k]=dataname[j+k]+"_"+label+buf.str();
								}
								j=j+temp;
							}
						else if(ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].type_class==H5T_INTEGER)
							for (int l=0;l<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].nb_values;l++)
							{
								std::ostringstream buf;
								buf<<"_"<<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].values.i[l];
								for (int k=0;k<temp;k++)
								{
									vtkstd::string label="";
									for(int ii=0;i<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.nb_instances;ii++)
										if(strcmp(ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.instances[i].name,"label")==0)
											label=label+ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.instances[i].value.s;
									dataname[j+k]=dataname[j+k]+"_"+label+buf.str();
								}
								j=j+temp;
							}
						else if(ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].type_class==H5T_STRING)
							for (int l=0;l<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].nb_values;l++)
							{
								for (int k=0;k<temp;k++)
								{
									vtkstd::string label="";
									for(int ii=0;i<ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.nb_instances;ii++)
										if(strcmp(ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.instances[i].name,"label")==0)
											label=label+ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].opt_attrs.instances[i].value.s;
									dataname[j+k]=dataname[j+k]+"_"+label+ft.data.arrayset.dims[ft.data.arrayset.nb_dims-i-1].values.s[l];
								}
								j=j+temp;
							}
					}
				}
	}
	int nbelt = 0;
	for (int i=0;i<nbdataarray;i++)
			std::cout<<"dataname ["<<i<<"] = "<<dataname[i]<<std::endl;
	nbelt = ahdfmesh.readMeshGroup(file_id,ft.data.arrayset.dims[meshentitydim].values.s[0],grid);

	int offset = 0;
	int nbtotaldata=1;
	if(ft.data.arrayset.nb_dims>1)
		for(int i=0;i<ft.data.arrayset.nb_dims;i++)
			nbtotaldata = nbtotaldata*ft.data.arrayset.dims[i].nb_values;
	for(int i=0;i<nbdataarray;i++)
	{
		vtkFloatArray *floatscalar = vtkFloatArray::New();
		floatscalar->SetName(dataname[i].c_str());
		if(ft.data.arrayset.nb_dims>1)
		{
			if(timedim>-1)
			{
				int actualtimestep = this->ActualTimeStep;
				int offsetcomp=1;
				for(int ioffsetdim=0;ioffsetdim<componentdim;ioffsetdim++)
				{
					if(ioffsetdim==meshentitydim)
                        offsetcomp=offsetcomp*nbelt;
                     else
                        offsetcomp=offsetcomp*ft.data.arrayset.dims[ioffsetdim].nb_values;
				}
				int offsetmesh=1;
				for(int ioffsetdim=0;ioffsetdim<meshentitydim;ioffsetdim++)
					offsetmesh=offsetmesh*ft.data.arrayset.dims[ioffsetdim].nb_values;
				int offsettime=1;
				for(int ioffsetdim=0;ioffsetdim<timedim;ioffsetdim++)
				{
					if(ioffsetdim==meshentitydim)
						offsettime=offsettime*nbelt;
					else
						offsettime=offsettime*ft.data.arrayset.dims[ioffsetdim].nb_values;
				}
				this->TimeStepMode = true;
				int offsetc=0;
				for(int j=0;j<ft.data.arrayset.dims[timedim].nb_values;j++)
				{
					if(componentdim>-1)
					{
						if(ft.data.arrayset.dims[componentdim].nb_values<3)
							floatscalar->SetNumberOfComponents(ft.data.arrayset.dims[componentdim].nb_values);
						else
							floatscalar->SetNumberOfComponents(3);
						for(int j2=0;j2<ft.data.arrayset.dims[componentdim].nb_values;j2++)
						{
							int offsetm=offset;
							for(int k=0;k<nbelt;k++)
							{
								if (j2<3)
								{
									if(ft.data.arrayset.data.type_class == H5T_FLOAT)
									{
										if(actualtimestep==j)
											floatscalar->InsertComponent(k,j2,ft.
													data.arrayset.data.values.f[j2*offsetcomp+k*offsetmesh+(offsettime)*j]);
									}
									else if(ft.data.arrayset.data.type_class == H5T_COMPOUND)
									{
										if(actualtimestep==j)
										{
											float module;
											int offsetcompt;
											if (offsetcomp==0) offsetcompt=1;
											int offsetmesht;
											if (offsetmesh==0) offsetmesh=1;

											module = abs_complex(ft.data.arrayset.data.values.c[j2*offsetcomp+k*offsetmesh+(offsettime)*j]);
											floatscalar->InsertComponent(k,j2,module);
										}
									}
								}
								if(offsetmesh==0) offsetm=offsetm+offsetmesh+1;
								else offsetm=offsetm+offsetmesh;
							}
						}
						offset=offset+ft.data.arrayset.dims[componentdim].nb_values*nbelt;
					}
					else
					{
						for(int k=0;k<nbelt;k++)
						{
							if(ft.data.arrayset.data.type_class == H5T_FLOAT)
							{
								if(actualtimestep==j)
									floatscalar->InsertTuple1(k,ft.data.arrayset.data.values.f[k+offset]);
							}
							else if(ft.data.arrayset.data.type_class == H5T_COMPOUND)
							{
								if(actualtimestep==j)
									floatscalar->InsertTuple1(k,abs_complex(ft.data.arrayset.data.values.c[k+offset]));
							}
						}
						offset=offset+nbelt;
					}
					if(actualtimestep==j)
					{
						if(grid->GetCell(0)->GetCellType()==VTK_VERTEX)
						{
							grid->GetPointData()->AddArray(floatscalar);
						}
						else
						{
							grid->GetCellData()->AddArray(floatscalar);
						}
						double timevalue = (double)ft.data.arrayset.dims[timedim].values.f[j];
						floatscalar->Delete();
						output->GetInformation()->Set(vtkDataObject::DATA_TIME_STEP(),timevalue);
					}
				}
			}
			else if(componentdim>-1)
			{
				int offsetcomp=0;
				for(int ioffsetdim=0;ioffsetdim<componentdim;ioffsetdim++)
                    offsetcomp=offsetcomp+ft.data.arrayset.dims[ioffsetdim].nb_values;
				int offsetmesh=0;
				for(int ioffsetdim=0;ioffsetdim<meshentitydim;ioffsetdim++)
				    offsetmesh=offsetmesh+ft.data.arrayset.dims[ioffsetdim].nb_values;
				if(ft.data.arrayset.dims[componentdim].nb_values<3)
				    floatscalar->SetNumberOfComponents(ft.data.arrayset.dims[componentdim].nb_values);
				else
				    floatscalar->SetNumberOfComponents(3);
				int offsetm=offset;
				for(int j=0;j<ft.data.arrayset.dims[componentdim].nb_values;j++)
				{
					if(offsetcomp==0) offsetm=offset+j;
					for(int k=0;k<nbelt;k++)
					{
						if(j<3)
						{
							if(ft.data.arrayset.data.type_class==H5T_FLOAT)
							{
								floatscalar->InsertComponent(k,j,ft.data.arrayset.data.values.f[offsetm]);
							}
							else if(ft.data.arrayset.data.type_class==H5T_COMPOUND)
							{
								float module;
								module = abs_complex(ft.data.arrayset.data.values.c[offsetm]);
								floatscalar->InsertComponent(k,j,module);
							}
						}
						if(offsetmesh==0) offsetm=offsetm+1;
						else offsetm=offsetm+offsetmesh;
					}
				}
				offset=offset+ft.data.arrayset.dims[componentdim].nb_values*nbelt;
				if(grid->GetCell(0)->GetCellType()==VTK_VERTEX)
				{
					grid->GetPointData()->AddArray(floatscalar);
				}
				else
				{
					grid->GetCellData()->AddArray(floatscalar);
				}
				floatscalar->Delete();
			}
			else
			{
				for(int k=0;k<nbelt;k++)
				{
					if(ft.data.arrayset.data.type_class==H5T_FLOAT)
					{
						floatscalar->InsertTuple1(k,ft.data.arrayset.data.values.f[k+offset]);
					}
					else if(ft.data.arrayset.data.type_class==H5T_COMPOUND)
					{
						float module;
					    module = abs_complex(ft.data.arrayset.data.values.c[k+offset]);
					    floatscalar->InsertTuple1(k,module);
					}
				}
				offset=offset+nbelt;
				if(grid->GetCell(0)->GetCellType()==VTK_VERTEX)
				{
					grid->GetPointData()->AddArray(floatscalar);
				}
				else
				{
					grid->GetCellData()->AddArray(floatscalar);
				}
				floatscalar->Delete();
			}

		}
		else
		{
			for(int k=0;k<nbelt;k++)
			{
				if(ft.data.arrayset.data.type_class==H5T_FLOAT)
				{
					floatscalar->InsertTuple1(k,ft.data.arrayset.data.values.f[k+offset]);
				}
				else if(ft.data.arrayset.data.type_class==H5T_COMPOUND)
				{
					float module;
					module = abs_complex(ft.data.arrayset.data.values.c[k+offset]);
					floatscalar->InsertTuple1(k,module);
				}
			}
			offset=offset+nbelt;
			if(grid->GetCell(0)->GetCellType()==VTK_VERTEX)
			{
				grid->GetPointData()->AddArray(floatscalar);
			}
			else
			{
				grid->GetCellData()->AddArray(floatscalar);
			}
			floatscalar->Delete();
		}
	}


	AH5_free_floatingtype(&ft);
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

  hid_t    file_id;
  int ret_val;
  cout<<"[vtkAmeletHDFReader::CanReadFile] BEGIN"<<endl;
  char *amelet;
  const char *path=".";
  ret_val = 0;
  if ( is_readable(filename))
  {
     
      file_id = H5Fopen (filename, H5F_ACC_RDONLY, H5P_DEFAULT);
      // Check FORMAT and AMELETHDF_FORMAT_VERSION attributes values
      AH5_read_str_attr(file_id, path, AH5_FILE_A_FORMAT, &amelet);
      if(strcmp(amelet,"AMELETHDF")==0)
    	  ret_val=1;
      H5Fclose(file_id);
      cout<<"[vtkAmeletHDFReader::CanReadFile] END "<<endl;
      return ret_val;
  }
  else 
  {
    std::cout<<"file doesn't exist !!!"<<std::endl;
    return 0;
  }


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
  char* entryPoint = NULL;
  std::cout<<"[vtkAmeletHDFReader::RequestData] BEGIN"<<std::endl;
  // get the info objects
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkMultiBlockDataSet *output = vtkMultiBlockDataSet::GetData(outInfo);
  this->UpdateProgress(0.);

  if (!this->FileName)
     {
     vtkErrorMacro("No filename specified.");
     return 0;
     }
  if( !is_readable(this->FileName))
  {
      vtkErrorMacro("The file does not exist.");
      return 0;
  }
  // Test data type (unstructured mesh, structured mesh, data on mesh, or data ?)
  dataType = getAmeletHDFDataType(this->FileName);
  if (dataType==0)
      {
	  vtkErrorMacro("This is not a mesh, data or data on mesh ameletHDF file.");
	  return 0;
	  }

  file_id = H5Fopen (this->FileName, H5F_ACC_RDONLY, H5P_DEFAULT);
  AH5_read_str_attr(file_id, ".", AH5_A_ENTRY_POINT, &entryPoint);

  if(dataType==1)
  {
	  //cout<<"data on mesh conversion"<<endl;
      int tsLength = outInfo->Length(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
      double* steps = outInfo->Get(vtkStreamingDemandDrivenPipeline::TIME_STEPS());

	  if(outInfo->Has(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP()) && this->TimeStepMode )
	  {
		  double requestedTimeValue = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP());
		  int nSteps = outInfo->Length(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
		  double* steps = outInfo->Get(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
          int cnt = 0;
          while(cnt < tsLength-1 && steps[cnt] < requestedTimeValue)
          {
        	  cnt++;
          }
          this->ActualTimeStep = cnt;
		  output->GetInformation()->Set(vtkDataObject::DATA_TIME_STEP(), TimeStepValues[this->ActualTimeStep]);
	  }
	  this->ReadDataOnMesh(file_id,output);
      this->UpdateProgress(1);
   	  //loc_id = H5Gopen(file_id,"/floatingType",H5P_DEFAULT);


  }
  else if(dataType==2)
  {
      //cout<<"data conversion"<<endl;
      vtkTable *table = vtkTable::New();
      output->SetBlock(0,table);
      vtkAmeletHDFDataReader ahdfdata;
      ahdfdata.readData(file_id,table);
      
  }
  else if(dataType==3) //mesh
  {
	  cout<<"mesh conversion"<<endl;
	  vtkAmeletHDFMeshReader ahdfmesh;
	  AH5_mesh_t mesh;
	  //read mesh "/mesh"
	  AH5_read_mesh(file_id, &mesh);
      cout<<mesh.nb_groups<<endl;
      int block_id=0;
      for(int i=0;i < mesh.nb_groups ; i++)
      {
    	  for (int j=0; j<mesh.groups[i].nb_msh_instances;j++)
    	  {
    		  cout<<mesh.groups[i].msh_instances[j].type<<" unstructured="<<MSH_UNSTRUCTURED<<" structured="<<MSH_STRUCTURED<<endl;
    		  if(mesh.groups[i].msh_instances[j].type == MSH_UNSTRUCTURED)
    		  {
    			  cout<<"umesh conversion"<<endl;
    			  vtkUnstructuredGrid *ugrid = AllocateGetBlock(output, block_id, IS_UMESH());
    		      ahdfmesh.readUmesh(mesh.groups[i].msh_instances[j].data.unstructured,ugrid);
    		  }
    		  else if(mesh.groups[i].msh_instances[j].type == MSH_STRUCTURED)
    		  {
    			  cout<<"smesh conversion"<<endl;
    		      vtkUnstructuredGrid *sgrid = AllocateGetBlock(output, block_id ,IS_SMESH());
    		      ahdfmesh.readSmesh(mesh.groups[i].msh_instances[j].data.structured,sgrid);

    		  }
    		  double prog = this->GetProgress()+(i+1)*j/(mesh.nb_groups);
    		  this->UpdateProgress(prog);
    		  block_id=block_id+1;
    	  }

      }
      cout<<"end read mesh"<<endl;
      AH5_free_mesh(&mesh);

  }
  else if(dataType>3 || dataType<1)
  {
	  vtkErrorMacro("This is not an ameletHDF data or mesh file .");
	  return 0;
  }

  cout<<"[vtkAmeletHDFReader::RequestData] END"<<endl;
  H5Fclose(file_id);
  return 1;
}
// -----------------------------------------------------------------------------
//
int vtkAmeletHDFReader::RequestInformation(vtkInformation *vtkNotUsed(request),
                                           vtkInformationVector **vtkNotUsed(inputVector),
                                           vtkInformationVector *outputVector)
{
	std::cout<<"[vtkAmeletHDFReader::RequestInformation] BEGIN"<<std::endl;
	vtkInformation* outInfo = outputVector->GetInformationObject(0);
	int dataType=0;
	char* entryPoint = NULL;
	AH5_ft_t floatingtype;
	vtkDebugMacro("RequestInformation");
	if (!this->FileName)
	    {
	    vtkErrorMacro("No filename specified.");
	    return 0;
	    }
	// Test data type (unstructured mesh, structured mesh, data on mesh, or data ?)
        if( !is_readable(this->FileName))
        {
          vtkErrorMacro("The file does not exist.");
          return 0;
        }
    dataType = getAmeletHDFDataType(this->FileName);
    if(dataType==1)
    {
    	//time series ?
    	hid_t file_id;
    	file_id=H5Fopen(this->FileName,H5F_ACC_RDONLY, H5P_DEFAULT);
        //loop on dims
    	TimeStepMode = false;
    	const char * path=".";
    	AH5_read_str_attr(file_id, path, AH5_A_ENTRY_POINT, &entryPoint);
        std::cout<<"entrypoint="<<entryPoint<<std::endl;
    	AH5_read_floatingtype(file_id,entryPoint, &floatingtype);
    	std::cout<<"End read ft"<<std::endl;
        for(int i=0;i<floatingtype.data.arrayset.nb_dims;i++)
        {

            for(int j=0;j<floatingtype.data.arrayset.dims[i].opt_attrs.nb_instances;j++)
            	if(floatingtype.data.arrayset.dims[i].opt_attrs.instances[j].type==H5T_STRING)
            	{

            		if((strcmp(floatingtype.data.arrayset.dims[i].opt_attrs.instances[j].value.s,"time")==0) ||
            				(strcmp(floatingtype.data.arrayset.dims[i].opt_attrs.instances[j].value.s,"frequency")==0))
            		{
            			outInfo->Remove(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
            			outInfo->Remove(vtkStreamingDemandDrivenPipeline::TIME_RANGE());
                    	this->TimeStepMode = true;
                        this->TimeStepValues = new double[floatingtype.data.arrayset.dims[i].nb_values];
                    	for(int k=0;k<floatingtype.data.arrayset.dims[i].nb_values;k++)
                    		this->TimeStepValues[k] = floatingtype.data.arrayset.dims[i].values.f[k];
                    	this->TimeStepRange[0] = 0;
                    	this->TimeStepRange[1] = floatingtype.data.arrayset.dims[i].nb_values - 1;
    		            outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_STEPS(),
    		            		&this->TimeStepValues[0], floatingtype.data.arrayset.dims[i].nb_values);
    		            double timeRange[2];
    		            timeRange[0]=this->TimeStepValues[0];
    		            timeRange[1]=this->TimeStepValues[floatingtype.data.arrayset.dims[i].nb_values-1];
    		            outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_RANGE(), timeRange,2);
            		}
            	}
        }

        H5Fclose(file_id);
        AH5_free_floatingtype(&floatingtype);
    }


    if (dataType==0) return 0;
    std::cout<<"[vtkAmeletHDFReader::RequestInformation] END"<<std::endl;
	return 1;
}


