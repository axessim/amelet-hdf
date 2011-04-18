#include <vtkAmeletHDFReader.h>
#include <sstream>


extern "C" {
    #include "amelethdf.h"
    #include "mesh.h"
    #include "arrayset.h"
    #include "hdfpath.h"
    #include "unstructuredmesh.h"
    #include "structuredmesh.h"
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
    char * attr_value;
    char * path;
    attr_value = (char *) malloc(ELEMENT_NAME_LENGTH * sizeof(char));
    path = (char *) malloc(ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
    arrayset_t ars;

    child = read_children_name(file_id,"/floatingType");
    if(child.nbchild>1)
    {
        vtkErrorMacro("This is more than one arrayset in the ameletHDF file.");
        return 0;
    }
//    strcpy(attr,"floatingType");
    strcpy(path,"/floatingType");
    strcat(path,"/");
    strcat(path,child.childnames[0]);
    strcpy(attr_value,read_string_attribute(file_id,path,"floatingType"));
    if(strcmp(attr_value,"arraySet")!=0)
    {
    	vtkErrorMacro("This is not an arrayset in the ameletHDF file.");
    	return 0;
    }
    free(attr_value);
    int meshentitydim=-1;
    int componentdim=-1;
    int timedim=-1;

    ars = read_arrayset(file_id,path); 
    free(path);
    //char **dataname;
    int nbdataarray=1;
    int max=ars.nbdims-1;
    for (int i=0;i<ars.nbdims;i++){
      //std::cout<<"physicalnature ="<<ars.dims[i].single.physical_nature<<std::endl;
      if(strcmp(ars.dims[i].single.physical_nature,"meshEntity")==0)
        	meshentitydim=i;
      if(strcmp(ars.dims[i].single.physical_nature,"component")==0)
           componentdim=i;
      if(strcmp(ars.dims[i].single.physical_nature,"time")==0)
           timedim=i;
      if(strcmp(ars.dims[i].single.physical_nature,"frequency")==0)
           timedim=i;
      if(strcmp(ars.dims[i].single.physical_nature,"Frequency")==0)
           timedim=i;
    }


    if(ars.nbdims>1)
    {
        //if(componentdim>-1) max=ars.nbdims-2;
        //else if(strcmp(ars.dims[1].single.physical_nature,"time")==0) max=ars.nbdims-2;
        for(int i=0;i<ars.nbdims;i++){
            if(i!=meshentitydim){
               if(i!=componentdim){
                 if(i!=timedim)
                   nbdataarray = nbdataarray*ars.dims[i].nbvalues;
               }
            }
        }
    }
    

    //set data name
    vtkstd::string dataname[nbdataarray];
    for(int i=0;i<nbdataarray;i++)
    {
        dataname[i]= child.childnames[0];
        
    }
    int temp =  nbdataarray;
    for(int i=0;i<ars.nbdims;i++)
    {
        if((ars.nbdims-i-1)!=meshentitydim)
        {
          if((ars.nbdims-i-1)!=componentdim) 
          {
            if((ars.nbdims-i-1)!=timedim) 
            {
            temp = (int)temp/ars.dims[ars.nbdims-i-1].nbvalues;
            int j=0;
            if(temp==0)j=nbdataarray; 
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
           }
        }
    }
//      for(int i=0;i<nbdataarray;i++)
//      {
//          std::cout<<"dataname["<<i<<"] = "<<dataname[i]<<std::endl;
//      }

        char *meshEntity;
	meshEntity = (char *) malloc (ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
        int meshdim = meshentitydim+1;
      
        strcpy(meshEntity,ars.dims[meshentitydim].svalue[0]);
        char * grp2;
        char * grpname;
        char *meshname;
        char *meshgrp;
	grp2 = (char *)malloc (ELEMENT_NAME_LENGTH * sizeof(char));
        grpname = (char *)malloc (ELEMENT_NAME_LENGTH * sizeof(char));
        meshname = (char *) malloc (ELEMENT_NAME_LENGTH * sizeof (char));
        meshgrp = (char *) malloc (ELEMENT_NAME_LENGTH * sizeof (char));
        strcpy(grp2,path_element(meshEntity,1,1));
        strcpy(grpname,path_element(meshEntity,0,1));
        strcpy(meshname,path_element(meshEntity,2,1));
        strcpy(meshgrp,path_element(meshEntity,3,1));
        
        
        loc_id = H5Gopen(file_id, "/mesh",H5P_DEFAULT);
        mesh_id =  H5Gopen(loc_id, meshgrp,H5P_DEFAULT);
        meshType = meshtype(mesh_id,meshname);
        int nbelt = 0;
        if(meshType==UNSTRUCTURED)
        {
             //ahdfmesh.readUmesh(mesh_id,meshChild.childnames[0],grid);
             if(strcmp(grp2,"group")==0)
               nbelt = ahdfmesh.readUgrp(mesh_id,meshname,grid,grpname);
             else
               nbelt = ahdfmesh.readUgrpgrp(mesh_id,meshname,grid,grpname);
        }
        else if(meshType==STRUCTURED)
        {
             ahdfmesh.readSmesh(mesh_id,meshname,grid);
        }
        free(grpname);
        free(meshname);
        free(meshgrp);
        H5Gclose(mesh_id);
        H5Gclose(loc_id);
        
        if(strcmp(grp2,"group")==0 || strcmp(grp2,"groupGroup")==0)
        {
            hid_t grp_id = H5Dopen1(file_id,meshEntity);
            ugroup_t grp;
            sgroup_t sgrp;
            //char pathmesh[ABSOLUTE_PATH_NAME_LENGTH];
            char *pathmesh;
            pathmesh = (char *) malloc (ABSOLUTE_PATH_NAME_LENGTH * sizeof (char));
            char *grpname;
	    grpname =  (char *) malloc (ELEMENT_NAME_LENGTH * sizeof (char));
            strcpy(grpname,path_element(meshEntity,0,1));
            
            if(meshType != 1)
                sgrp = readSGroup(grp_id,grpname);
            free(grpname); 
            int offset=0;
            int nbeltgrp;

            if(meshType==1)
            {
              nbeltgrp=nbelt;
            }
            else {
              nbeltgrp=sgrp.nbelt;
	      free(sgrp.name);
              free(sgrp.imin);
              free(sgrp.jmin);
              free(sgrp.kmin);
              free(sgrp.imax);
              free(sgrp.jmax);
              free(sgrp.kmax);
            }
            
            int nbtotaldata=1;
            if(ars.nbdims>1)
            {
            for(int i=0;i<ars.nbdims;i++)
                   nbtotaldata = nbtotaldata*ars.dims[i].nbvalues;
            }
            
            for(int i=0;i<nbdataarray;i++)
            {
              
                vtkFloatArray *floatscalar = vtkFloatArray::New();
                floatscalar->SetName(dataname[i].c_str());
                if(ars.nbdims>1)
                {
                  if(timedim>-1)
                    {
                      int actualtimestep = this->ActualTimeStep;
                      /*if(ars.dims[timedim].nbvalues==1)
                                actualtimestep=actualtimestep-1;*/
                      int offsetcomp=1;
                      for(int ioffsetdim=0;ioffsetdim<componentdim;ioffsetdim++){
                             if(ioffsetdim==meshentitydim)
                                offsetcomp=offsetcomp*nbeltgrp;
                             else
                                offsetcomp=offsetcomp*ars.dims[ioffsetdim].nbvalues;}
                      //cout<<"offsetcomp="<<offsetcomp<<endl;
                      int offsetmesh=1;
                      for(int ioffsetdim=0;ioffsetdim<meshentitydim;ioffsetdim++)
                        offsetmesh=offsetmesh*ars.dims[ioffsetdim].nbvalues;
                      //cout<<"offsetmesh="<<offsetmesh<<endl;
                      int offsettime=1;
                      for(int ioffsetdim=0;ioffsetdim<timedim;ioffsetdim++){
                          if(ioffsetdim==meshentitydim)
                              offsettime=offsettime*nbeltgrp;
                          else
                              offsettime=offsettime*ars.dims[ioffsetdim].nbvalues;}
                      //cout<<"offsettime="<<offsettime<<endl;

                      
                        this->TimeStepMode = true;
                        int offsetc=0;
                        for(int j=0;j<ars.dims[timedim].nbvalues;j++)
                        {
                          if(componentdim>-1)
                          {
                            if(ars.dims[componentdim].nbvalues<3)
                              floatscalar->SetNumberOfComponents(ars.dims[componentdim].nbvalues);
                            else
                              floatscalar->SetNumberOfComponents(3);
                            
                            
                          for(int j2=0;j2<ars.dims[componentdim].nbvalues;j2++)
                          {
                            int offsetm=offset;
                            for(int k=0;k<nbeltgrp;k++)
                            {
                              
                                if(j2<3)
                                {
                                    if(ars.data.rvalue!=NULL)
                                    {
                                      if(actualtimestep==j)
                                      {
                                        floatscalar->InsertComponent(k,j2,ars.data.rvalue[j2*offsetcomp+k*offsetmesh+(offsettime)*j]);
                                      }
                                    }
                                    else if(ars.data.cvalue!=NULL)
                                    {
                                      if(actualtimestep==j)
                                      {
                                        float module;
                                        //module=cabs(ars.data.cvalue[offsetm+offset]);
                                        int offsetcompt;
                                        if (offsetcomp==0) offsetcompt=1;
                                        //else offsetcompt=offsetcomp*j2+offsetc;
                                        int offsetmesht;
                                        if (offsetmesh==0) offsetmesh=1;
                                        //else offsetmesht=offsetm;
                                        module=abs_complex(ars.data.cvalue[j2*offsetcomp+k*offsetmesh+(offsettime)*j]);
                                        //if(meshType==1) floatscalar->InsertComponent(grp.eltgroup[k],j2,module);
                                        //else 
                                        floatscalar->InsertComponent(k,j2,module);
                                      }
                                    }
                                 
                                  }
                                  
                            if(offsetmesh==0) offsetm=offsetm+offsetmesh+1;
                                  else offsetm=offsetm+offsetmesh;
                                 
                            }
                            
                            //if(offsetcomp==0) offsetm=offsetm;
                            //offsetm=offsetm+offsetcomp;
                            
                            
                           }
                           offset=offset+ars.dims[componentdim].nbvalues*nbeltgrp;
                           
                           }
                           else{
                             
                            for(int k=0;k<nbeltgrp;k++)
                            {
                                if(ars.data.rvalue!=NULL)
                                {
                                    if(actualtimestep==j)
                                    {
                                       floatscalar->InsertTuple1(k,ars.data.rvalue[k+offset]);
                                    }
                                }
                                else if(ars.data.cvalue!=NULL)
                                {
                                    if(actualtimestep==j)
                                    {
                                        float module;
                                        module=abs_complex(ars.data.cvalue[k+offset]);
                                        floatscalar->InsertTuple1(k,module);
                                    }
                                }
                           }
                           offset=offset+nbeltgrp;
                           }
                           if(actualtimestep==j)
                           {
                             if(meshType == 1 && (strcmp(grp2,"group")==0) && (strcmp(read_string_attribute(file_id,meshEntity,"type"),"node")==0))
                             {
                                 grid->GetPointData()->AddArray(floatscalar);
                             }
                             else
                             {
                                 grid->GetCellData()->AddArray(floatscalar);
                             }
                             double timevalue = (double)ars.dims[timedim].rvalue[j];
                             floatscalar->Delete();
                             output->GetInformation()->Set(vtkDataObject::DATA_TIME_STEPS(),&timevalue,1);
                           }
                        }
                 }
                  else if(componentdim>-1)
                    {
                      
                      int offsetcomp=0;
                      for(int ioffsetdim=0;ioffsetdim<componentdim;ioffsetdim++)
                        offsetcomp=offsetcomp+ars.dims[ioffsetdim].nbvalues;
                      //cout<<"offsetcomp="<<offsetcomp<<endl;
                      int offsetmesh=0;
                      for(int ioffsetdim=0;ioffsetdim<meshentitydim;ioffsetdim++)
                        offsetmesh=offsetmesh+ars.dims[ioffsetdim].nbvalues;
                      //cout<<"offsetmesh="<<offsetmesh<<endl;
                      if(ars.dims[componentdim].nbvalues<3)
                        floatscalar->SetNumberOfComponents(ars.dims[componentdim].nbvalues);
                      else
                        floatscalar->SetNumberOfComponents(3);
                      int offsetm=offset;
                      for(int j=0;j<ars.dims[componentdim].nbvalues;j++)
                      {
                            
                            if(offsetcomp==0) offsetm=offset+j;
                            for(int k=0;k<nbeltgrp;k++)
                            {
                                if(j<3)
                                {
                                    if(ars.data.rvalue!=NULL)
                                    {
/*                                        if(meshType==1)
                                            floatscalar->InsertComponent(grp.eltgroup[k],
                                                                         j,ars.
                                                                         data.rvalue[offsetm]);
                                        else*/
                                       floatscalar->InsertComponent(k,j,ars.data.rvalue[offsetm]);
                                    }
                                    else if(ars.data.cvalue!=NULL)
                                    {
                                        float module;
                                        module=abs_complex(ars.data.cvalue[offsetm]);
                                       /* if(meshType==1) floatscalar->InsertComponent(grp.eltgroup[k],j,module);
                                        else*/
                                       floatscalar->InsertComponent(k,j,module);
                                    }
                                        
                                  }
                                  if(offsetmesh==0) offsetm=offsetm+1;
                                  else offsetm=offsetm+offsetmesh;
                               }
                              
                        }
                        offset=offset+ars.dims[componentdim].nbvalues*nbeltgrp;
                        //cout<<"offset="<<offset<<endl;
                        if(meshType == 1 && (strcmp(read_string_attribute(file_id,meshEntity,"type"),"node")==0))
                        {
                          grid->GetPointData()->AddArray(floatscalar);
                        }
                        else {
                          grid->GetCellData()->AddArray(floatscalar);
                        }
                        floatscalar->Delete();
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
                             module=abs_complex(ars.data.cvalue[k+offset]);
                             if(meshType==1) floatscalar->InsertTuple1(grp.eltgroup[k],module);
                             else floatscalar->InsertTuple1(k,module);
                         }
                     }
                     offset=offset+nbeltgrp;
                     //std::cout<<"name = "<<floatscalar->GetName()<<std::endl;
                     if(meshType == 1 && (strcmp(read_string_attribute(file_id,meshEntity,"type"),"node")==0))
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
                         module=abs_complex(ars.data.cvalue[k+offset]);
                         if(meshType==1) floatscalar->InsertTuple1(grp.eltgroup[k],module);
                         else floatscalar->InsertTuple1(k,module);
                     }
                 }
                 offset=offset+nbeltgrp;
                 //std::cout<<"name = "<<floatscalar->GetName()<<std::endl;
                 if(meshType == 1 && (strcmp(read_string_attribute(file_id,meshEntity,"type"),"node")==0))
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

    free(meshEntity);
    /*if(meshType==1){
    free(grp.eltgroup);
    free(grp.name);}*/
    H5Dclose(grp_id);
    }
    //for (int idel=0;idel<child.nbchild; idel++)
    //    free(*(child.childnames + idel)); 
    if(child.nbchild>1) free(child.childnames[0]);
    free(child.childnames);
    for(int i=0;i<nbdataarray;i++)
    {
        dataname[i].erase();
    }
    free(grp2);

    free(ars.data.single.label);
    free(ars.data.single.comment);
    free(ars.data.single.physical_nature);
    free(ars.data.single.unit);
    free(ars.data.dims);
    if(ars.data.ivalue!=NULL)
        free(ars.data.ivalue);
    if(ars.data.rvalue!=NULL)
        free(ars.data.rvalue);
    if(ars.data.cvalue!=NULL)
        free(ars.data.cvalue);
    free(ars.single.label);
    free(ars.single.comment);
    free(ars.single.physical_nature);
    free(ars.single.unit);
    for (int idel=0;idel<ars.nbdims;idel++)
    {
        if(ars.dims[idel].ivalue!=NULL)
            free(ars.dims[idel].ivalue);
	else if(ars.dims[idel].rvalue!=NULL)
	    free(ars.dims[idel].rvalue);
	else if(ars.dims[idel].cvalue!=NULL)
	    free(ars.dims[idel].cvalue);
	else if(ars.dims[idel].svalue!=NULL)
            free(ars.dims[idel].svalue);
	free(ars.dims[idel].single.label);
	free(ars.dims[idel].single.comment);
	free(ars.dims[idel].single.physical_nature);
	free(ars.dims[idel].single.unit);
    }
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
  //ret_val = getAmeletHDFAttribut(group_id, "AMELETHDF_FORMAT_VERSION", "1.0.0");

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
	  //cout<<"data on mesh conversion"<<endl;
	  //read mesh under "/mesh"
	  loc_id = H5Gopen(file_id, "/mesh",H5P_DEFAULT);
	  child = read_children_name(file_id,"/mesh");
	  if(child.nbchild>1)
	  {
	   	   vtkErrorMacro("This is more than one mesh in the ameletHDF file.");
	   	   return 0;
	  }
	  //for (int idel=0;idel<child.nbchild; idel++)
          //   free(*(child.childnames + idel));
          if(child.nbchild>1) free(child.childnames[0]);
          free(child.childnames);
          int tsLength = outInfo->Length(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
          double* steps = outInfo->Get(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
	  //vtkUnstructuredGrid *grid = AllocateGetBlock(output, 0,IS_DATAONMESH());
	  if(outInfo->Has(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS()) && this->TimeStepMode )
	  {
		  double requestedTimeValue = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS())[0];
		  int nSteps =
		        outInfo->Length(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
		  double* steps =
		        outInfo->Get(vtkStreamingDemandDrivenPipeline::TIME_STEPS());
                  int cnt = 0;
                  while(cnt < tsLength-1 && steps[cnt] < requestedTimeValue)
                  {
                    cnt++;
                  }
                  this->ActualTimeStep = cnt;
		  /*this->ActualTimeStep =
		        vtkstd::find_if(this->TimeStepValues.begin(), this->TimeStepValues.end(),
		                        vtkstd::bind2nd( WithinTolerance(1e-9), requestedTimeValue ))
		        - this->TimeStepValues.begin();*/
                  
		 output->GetInformation()->Set(vtkDataObject::DATA_TIME_STEPS(),
		                                    &TimeStepValues[this->ActualTimeStep], 1);
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
	  //cout<<"mesh conversion"<<endl;
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
      //for (int idel=0;idel<child.nbchild; idel++)
      //  free(*(child.childnames + idel));
      if(child.nbchild>1) free(child.childnames[0]);
      free(child.childnames);
      if(meshChild.nbchild>1) free(meshChild.childnames[0]); 
      free(meshChild.childnames);
  }
  else if(dataType>3 || dataType<1)
  {
	  vtkErrorMacro("This is not an ameletHDF data or mesh file .");
	  return 0;
  }

  size_t nbdata=H5Fget_obj_count(file_id,H5F_OBJ_DATASET);
  hid_t data_id_list[nbdata];
  H5Fget_obj_count(file_id,H5F_OBJ_DATASET);
  H5Fget_obj_ids(file_id,H5F_OBJ_DATASET,nbdata,data_id_list);
  for(int i=0;i<nbdata;i++)
  {
	  
	  H5Dclose(data_id_list[nbdata-1-i]);
  }

  size_t nbobjects=H5Fget_obj_count(file_id,H5F_OBJ_GROUP);
  hid_t grp_id_list[nbobjects];
  H5Fget_obj_ids(file_id,H5F_OBJ_GROUP,nbobjects,grp_id_list);
  for(int i=0;i<nbobjects;i++)
  {
     
     H5Gclose(grp_id_list[nbobjects-1-i]);
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
    	char *path;
        char *path2;
	path = (char *) malloc (ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
	path2 = (char *) malloc (ABSOLUTE_PATH_NAME_LENGTH * sizeof(char));
	strcpy(path,"/floatingType");
    	children_t child;
    	child=read_children_name(file_id,path);
    	strcat(path,"/");
    	strcat(path,child.childnames[0]);
        strcat(path,"/ds");
        child=read_children_name(file_id,path);
        
    	this->TimeStepMode = false;
        strcpy(path2,path);
        for(int i=0;i<child.nbchild;i++)
        {
          strcat(path,"/");
          strcat(path,child.childnames[i]);
          
    	  if(H5Lexists(file_id,path,H5P_DEFAULT)!=false)
    	  {
            
    	    vector_t vec=read_vector(file_id,path);
            
    	    if((strcmp(vec.single.physical_nature,"time")==0) || (strcmp(vec.single.physical_nature,"frequency")==0))
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
	    if(vec.ivalue!=NULL)
                free(vec.ivalue);
	    else if(vec.cvalue!=NULL)
                free(vec.cvalue);
	    else if(vec.rvalue!=NULL)
                free(vec.rvalue);
	    else if(vec.svalue!=NULL)
                free(vec.svalue);
	    free(vec.single.label);
	    free(vec.single.comment);
	    free(vec.single.physical_nature);
	    free(vec.single.unit);

    	   }
    	   
    	   strcpy(path,path2);
           
        }
	free(path);
	free(path2);
	//for (int idel=0;idel<child.nbchild; idel++)
        if(child.nbchild>1) free(child.childnames[0]);
        free(child.childnames);
//	free(child.childnames);
    	H5Fclose(file_id);

    }
    if (dataType==0) return 0;

	return 1;
}


