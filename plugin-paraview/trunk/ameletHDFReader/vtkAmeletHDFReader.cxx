#include <vtkAmeletHDFReader.h>
#include <sstream>


#include <ah5.h>



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

  char *amelet;
  const char *path=".";
  ret_val = 0;
  if ( is_readable(filename))
  {
     
      file_id = H5Fopen (filename, H5F_ACC_RDONLY, H5P_DEFAULT);
      // Check FORMAT and AMELETHDF_FORMAT_VERSION attributes values
      AH5_read_str_attr(file_id, path, AH5_A_FORMAT, &amelet);
      if(strcmp(amelet,"AMELETHDF")==0)
    	  ret_val=1;
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
  //cout<<"Request DATA"<<endl;
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
  AH5_read_str_attr(file_id, ".", "entryPoint", &entryPoint);

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
    		      ahdfmesh.readUmesh(file_id,mesh. groups[i].msh_instances[j].data.unstructured,ugrid);
    		  }
    		  else if(mesh.groups[i].msh_instances[j].type == MSH_STRUCTURED)
    		  {
    			  cout<<"smesh conversion"<<endl;
    		      vtkUnstructuredGrid *sgrid = AllocateGetBlock(output, block_id ,IS_SMESH());
    		      ahdfmesh.readSmesh(file_id,mesh.groups[i].msh_instances[j].data.structured,sgrid);

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
	char* entryPoint = NULL;
	AH5_ft_t *floatingtype;
	
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
    	hid_t file_id=H5Fopen(this->FileName,H5F_ACC_RDONLY, H5P_DEFAULT);
        //loop on dims
    	this->TimeStepMode = false;
    	AH5_read_str_attr(file_id, "/", "entryPoint", &entryPoint);
    	AH5_read_floatingtype(file_id,entryPoint, floatingtype);
        for(int i=0;i<floatingtype->data.arrayset.nb_dims;i++)
        {
            for(int j=0;j<floatingtype->data.arrayset.dims[i].opt_attrs.nb_instances;j++)
            	if(floatingtype->data.arrayset.dims[i].opt_attrs.instances[j].type==H5T_STRING)
            		if((strcmp(floatingtype->data.arrayset.dims[i].opt_attrs.instances[j].value.s,"time")==0) ||
            				(strcmp(floatingtype->data.arrayset.dims[i].opt_attrs.instances[j].value.s,"frequency")==0))
            		{
            			outInfo->Remove(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
                    	this->TimeStepMode = true;
                    	for(int k=0;k<floatingtype->data.arrayset.dims[i].nb_values;k++)
                    	{
                    		this->TimeStepValues.push_back(floatingtype->data.arrayset.dims[i].values.f[k]);
                    	}
                    	this->TimeStepRange[0] = 0;
                    	this->TimeStepRange[1] = floatingtype->data.arrayset.dims[i].nb_values - 1;
    		            outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_STEPS(),
    		                        &this->TimeStepValues[0], this->TimeStepValues.size());
    		            double timeRange[2];
    		            timeRange[0]=this->TimeStepValues.front();
    		            timeRange[1]=this->TimeStepValues.back();
    		            outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_RANGE(), timeRange, 2);
            		}
        }
    }


    if (dataType==0) return 0;

	return 1;
}


