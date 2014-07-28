#include <vtkAmeletHDFMeshReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPoints.h>
#include <vtkVoxel.h>
#include <vtkPixel.h>
#include <vtkLine.h>
#include <vtkTriangle.h>
#include <vtkTetra.h>
#include <vtkQuad.h>
#include <vtkVertex.h>
#include <vtkHexahedron.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkCellData.h>
#include <vtkCell.h>
#include "vtkIdList.h"

#include "ah5_attribute.h"
#include "ah5_c_mesh.h"

using namespace vtkstd;

#include <map>



#define TRUE            1
#define FALSE           0

int vtkAmeletHDFMeshReader::readUmesh( AH5_umesh_t umesh, vtkUnstructuredGrid *ugrid)
{
    std::cout<<"Read umesh"<<std::endl;
    //std::cout<<"=========="<<std::endl;

    int idel;
    float xyz[3];
    vtkPoints *points = vtkPoints::New();
    int nb_nodes = umesh.nb_nodes[0];
    for (int i=0 ;i<nb_nodes;i++)
    {
    	for (int j=0;j<umesh.nb_nodes[1];j++)
    	{
    		xyz[j]= umesh.nodes[i*umesh.nb_nodes[1]+j];
    	}
        points->InsertNextPoint(xyz);
    }


    ugrid->SetPoints(points);
    points->Delete();

    int idnode=0;
    for(int i=0; i<umesh.nb_elementtypes;i++)
    {
      	if((umesh.elementtypes[i]==1) ||(umesh.elementtypes[i]==2))
       	{
       		vtkLine * linecell = vtkLine::New();
       		linecell->GetPointIds()->SetId(0,umesh.elementnodes[idnode]);
       		linecell->GetPointIds()->SetId(1,umesh.elementnodes[idnode+1]);
       		ugrid->InsertNextCell(linecell->GetCellType(),linecell->GetPointIds());
            linecell->Delete();
       		idnode=idnode+2;
            if(umesh.elementtypes[i]==2) idnode=idnode+1;
       	}
       	else if((umesh.elementtypes[i]==11) || (umesh.elementtypes[i]==12))
       	{
       		vtkTriangle * tricell = vtkTriangle::New();
       		tricell->GetPointIds()->SetId(0,umesh.elementnodes[idnode]);
       		tricell->GetPointIds()->SetId(1,umesh.elementnodes[idnode+1]);
       		tricell->GetPointIds()->SetId(2,umesh.elementnodes[idnode+2]);
       		ugrid->InsertNextCell(tricell->GetCellType(),tricell->GetPointIds());
                tricell->Delete();
       		idnode=idnode+3;
                if(umesh.elementtypes[i]==12) idnode=idnode+3;
       	}
       	else if((umesh.elementtypes[i]==13) || (umesh.elementtypes[i]==14))
       	{
       		vtkQuad * quadcell = vtkQuad::New();
       		quadcell->GetPointIds()->SetId(0,umesh.elementnodes[idnode]);
       		quadcell->GetPointIds()->SetId(1,umesh.elementnodes[idnode+1]);
       		quadcell->GetPointIds()->SetId(2,umesh.elementnodes[idnode+2]);
       		quadcell->GetPointIds()->SetId(3,umesh.elementnodes[idnode+3]);
       		ugrid->InsertNextCell(quadcell->GetCellType(),quadcell->GetPointIds());
                quadcell->Delete();
       		idnode=idnode+4;
                if(umesh.elementtypes[i]==14) idnode=idnode+4;
       	}
       	else if(umesh.elementtypes[i]==101)
       	{
       		vtkTetra * tetracell = vtkTetra::New();
       		tetracell->GetPointIds()->SetId(0,umesh.elementnodes[idnode]);
       		tetracell->GetPointIds()->SetId(1,umesh.elementnodes[idnode+1]);
       		tetracell->GetPointIds()->SetId(2,umesh.elementnodes[idnode+2]);
       		tetracell->GetPointIds()->SetId(3,umesh.elementnodes[idnode+3]);
       		ugrid->InsertNextCell(tetracell->GetCellType(),tetracell->GetPointIds());
            tetracell->Delete();
       		idnode=idnode+4;
       	}
       	else if(umesh.elementtypes[i]==104)
       	       	{
       	       		vtkHexahedron * hexacell = vtkHexahedron::New();
       	       		hexacell->GetPointIds()->SetId(0,umesh.elementnodes[idnode]);
       	       		hexacell->GetPointIds()->SetId(1,umesh.elementnodes[idnode+1]);
       	       		hexacell->GetPointIds()->SetId(2,umesh.elementnodes[idnode+2]);
       	       		hexacell->GetPointIds()->SetId(3,umesh.elementnodes[idnode+3]);
       	       		hexacell->GetPointIds()->SetId(4,umesh.elementnodes[idnode+4]);
       	       		hexacell->GetPointIds()->SetId(5,umesh.elementnodes[idnode+5]);
       	       		hexacell->GetPointIds()->SetId(6,umesh.elementnodes[idnode+6]);
       	       		hexacell->GetPointIds()->SetId(7,umesh.elementnodes[idnode+7]);
       	       		ugrid->InsertNextCell(hexacell->GetCellType(),hexacell->GetPointIds());
       	            hexacell->Delete();
       	       		idnode=idnode+8;
       	       	}
    }
    


    // Read GroupGroup
    if(umesh.nb_elementnodes==0)
    {
    	//so there is only nodes in mesh
    	for(int i=0;i<ugrid->GetNumberOfPoints();i++)
    	{
    		vtkVertex * vertexcell = vtkVertex::New();
    		vertexcell->GetPointIds()->SetId(0,i);
    		ugrid->InsertNextCell(vertexcell->GetCellType(),vertexcell->GetPointIds());
                vertexcell->Delete();
    	}
    }

    vtkIntArray *grpgroupId = vtkIntArray::New();
    vtkIntArray *groupId = vtkIntArray::New();
    grpgroupId->SetName("groupGroup Id");
    groupId->SetName("group Id");
    // Initialize groupId to 0
    for (int i=0;i<ugrid->GetNumberOfCells();i++)
    {
        grpgroupId->InsertTuple1(i,-1);
        groupId->InsertTuple1(i,-1);
    }

    // set data for each groupGroup
  	for(int i=0;i<umesh.nb_groupgroups;i++)
    {
                
    		for(int j=0;j<umesh.groupgroups[i].nb_groupgroupnames;j++)
    		{
                    for (int k=0; k<umesh.nb_groups;k++)
                    {
                    	if(strcmp(umesh.groupgroups[i].groupgroupnames[j],AH5_get_base_from_path(umesh.groups[k].path))==0)
                    	{
                    		if(strcmp(umesh.groups[k].type,"node")!=0)
    			                for(int k2=0;k2<umesh.groups[k].nb_groupelts;k2++)
                                    grpgroupId->SetTuple1(umesh.groups[k].groupelts[k2],i);
                        }
                    }
    		}
    	    ugrid->GetCellData()->AddArray(grpgroupId);
    	    grpgroupId->Delete();
    }
    // set data for each group
    
    for (int i=0;i<umesh.nb_groups;i++)
    {
    	if(strcmp(umesh.groups[i].type,"node")!=0)
        {
            for (int k=0;k<umesh.groups[i].nb_groupelts;k++)
                groupId->SetTuple1(umesh.groups[i].groupelts[k],i);
        }
        else
        {
            for(int k=0;k<umesh.groups[i].nb_groupelts;k++)
            {
                vtkVertex * vertexcell = vtkVertex::New();
                vertexcell->GetPointIds()->SetId(0,umesh.groups[i].groupelts[k]);
                int nbcells = ugrid->GetNumberOfCells();
                ugrid->InsertNextCell(vertexcell->GetCellType(),vertexcell->GetPointIds());
                vertexcell->Delete();
                grpgroupId->InsertTuple1(nbcells,-1);
                groupId->InsertTuple1(nbcells,i);
            }
        }
    }
    
    ugrid->GetCellData()->AddArray(groupId);
    groupId->Delete();
    std::cout<<"Read umesh end"<<std::endl;
    return 1;
}

int vtkAmeletHDFMeshReader::readSmesh(AH5_smesh_t smesh, vtkUnstructuredGrid *sgrid)
{
    //std::cout<<"Read smesh"<<std::endl;
    //std::cout<<"=========="<<std::endl;
    vector<bool> ptexist;
    int idptexist;

    for(int k=0;k<smesh.z.nb_nodes;k++)
    { 
        for(int j=0;j<smesh.y.nb_nodes;j++)
        {
            for(int i=0;i<smesh.x.nb_nodes;i++)
            {
		idptexist = (k*(smesh.x.nb_nodes)*(smesh.y.nb_nodes))+(j*smesh.x.nb_nodes)+i;
                ptexist.push_back(false);
            }
        }
    }

    vtkIntArray *grpgroupId = vtkIntArray::New();
    vtkIntArray *groupId = vtkIntArray::New();
    grpgroupId->SetName("groupGroup Id");
    groupId->SetName("group Id");

    map<const char*,int> grpgroupId_value;

    int cellId=0;
    int groupValue=-1;
    for(unsigned int i = 0;i<smesh.nb_groups;i++)
        for(unsigned int j=0;j<smesh.groups[i].dims[0];j++)
        {
            if(strcmp(smesh.groups[i].entitytype,"face")==0)
            {
                int ijk[4][3];
                int imin = smesh.groups[i].elements[j*6+0];
                int jmin = smesh.groups[i].elements[j*6+1];
                int kmin = smesh.groups[i].elements[j*6+2];
                int imax = smesh.groups[i].elements[j*6+3];
                int jmax = smesh.groups[i].elements[j*6+4];
                int kmax = smesh.groups[i].elements[j*6+5];

                if(imin==imax)
                {
                    // point 1
                    ijk[0][0]=imin;
                    ijk[0][1]=jmin;
                    ijk[0][2]=kmin;
                    // point 2
                    ijk[1][0]=imin;
                    ijk[1][1]=jmax;
                    ijk[1][2]=kmin;
                    // point 3
                    ijk[2][0]=imin;
                    ijk[2][1]=jmin;
                    ijk[2][2]=kmax;
                    // point 4
                    ijk[3][0]=imin;
                    ijk[3][1]=jmax;
                    ijk[3][2]=kmax;
                    double point[4][3];
                    for (int ii=0; ii<4; ii++)
                    {
                        unsigned int id = (ijk[ii][2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes))+
                                (ijk[ii][1]*(smesh.x.nb_nodes))+ijk[ii][0];
                         
                        ptexist[id]=true;
                    }
                }
                if(jmin==jmax)
                {
                    // point 1
                    ijk[0][0]=imin;
                    ijk[0][1]=jmin;
                    ijk[0][2]=kmin;
                    // point 2
                    ijk[1][0]=imax;
                    ijk[1][1]=jmin;
                    ijk[1][2]=kmin;
                    // point 3
                    ijk[2][0]=imin;
                    ijk[2][1]=jmin;
                    ijk[2][2]=kmax;
                    // point 4
                    ijk[3][0]=imax;
                    ijk[3][1]=jmin;
                    ijk[3][2]=kmax;
                    double point[4][3];
                    for (int ii=0; ii<4; ii++)
                    {
                        unsigned int id = (ijk[ii][2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes))+
                                 (ijk[ii][1]*(smesh.x.nb_nodes))+ijk[ii][0];
                        ptexist[id]=true;
                    }
                }
                if(kmin==kmax)
                {
                    // point 1
                    ijk[0][0]=imin;
                    ijk[0][1]=jmin;
                    ijk[0][2]=kmin;
                    // point 2
                    ijk[1][0]=imax;
                    ijk[1][1]=jmin;
                    ijk[1][2]=kmin;
                    // point 3
                    ijk[2][0]=imin;
                    ijk[2][1]=jmax;
                    ijk[2][2]=kmin;
                    // point 4
                    ijk[3][0]=imax;
                    ijk[3][1]=jmax;
                    ijk[3][2]=kmin;
                    double point[4][3];
                    for (int ii=0; ii<4; ii++)
                    {
                        unsigned int id = (ijk[ii][2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes))+
                                 (ijk[ii][1]*(smesh.x.nb_nodes))+ijk[ii][0];
                        ptexist[id]=true;
                    }
                }
                
            }
            else if(strcmp(smesh.groups[i].entitytype,"volume")==0)
            {
                int ijk[8][3];
                int imin = smesh.groups[i].elements[j*6+0];
                int jmin = smesh.groups[i].elements[j*6+1];
                int kmin = smesh.groups[i].elements[j*6+2];
                int imax = smesh.groups[i].elements[j*6+3];
                int jmax = smesh.groups[i].elements[j*6+4];
                int kmax = smesh.groups[i].elements[j*6+5];
                // point 1
                ijk[0][0]=imin;
                ijk[0][1]=jmin;
                ijk[0][2]=kmin;
                // point 2
                ijk[1][0]=imax;
                ijk[1][1]=jmin;
                ijk[1][2]=kmin;
                // point 3
                ijk[2][0]=imin;
                ijk[2][1]=jmax;
                ijk[2][2]=kmin;
                // point 4
                ijk[3][0]=imax;
                ijk[3][1]=jmax;
                ijk[3][2]=kmin;
                // point 5
                ijk[4][0]=imin;
                ijk[4][1]=jmin;
                ijk[4][2]=kmax;
                // point 6
                ijk[5][0]=imax;
                ijk[5][1]=jmin;
                ijk[5][2]=kmax;
                // point 7
                ijk[6][0]=imin;
                ijk[6][1]=jmax;
                ijk[6][2]=kmax;
                // point 8
                ijk[7][0]=imax;
                ijk[7][1]=jmax;
                ijk[7][2]=kmax;
                double point[8][3];
                for (int ii=0; ii<8; ii++)
                {
                    unsigned int id = (ijk[ii][2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes))+
                             (ijk[ii][1]*(smesh.x.nb_nodes))+ijk[ii][0];
                    ptexist[id]=true;
                }
                
            }
            else if((strcmp(smesh.groups[i].entitytype,"edge")==0) ||
                    (strcmp(smesh.groups[i].entitytype,"slot")==0))
            {
                int ijk[2][3];
                int imin = smesh.groups[i].elements[j*6+0];
                int jmin = smesh.groups[i].elements[j*6+1];
                int kmin = smesh.groups[i].elements[j*6+2];
                int imax = smesh.groups[i].elements[j*6+3];
                int jmax = smesh.groups[i].elements[j*6+4];
                int kmax = smesh.groups[i].elements[j*6+5];
                // point 1
                ijk[0][0]=imin;
                ijk[0][1]=jmin;
                ijk[0][2]=kmin;
                // point 2
                ijk[1][0]=imax;
                ijk[1][1]=jmax;
                ijk[1][2]=kmax;
                double point[2][3];
                for (int ii=0; ii<2; ii++)
                {
                    unsigned int id = (ijk[ii][2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes))+
                             (ijk[ii][1]*(smesh.x.nb_nodes))+ijk[ii][0];
                    ptexist[id]=true;
                }
                
            }
            else if(strcmp(smesh.groups[i].type,"node")==0)
            {
                int ijk[3];
                int imin = smesh.groups[i].elements[j*3+0];
                int jmin = smesh.groups[i].elements[j*3+1];
                int kmin = smesh.groups[i].elements[j*3+2];

                ijk[0]=imin;
                ijk[1]=jmin;
                ijk[2]=kmin;
                unsigned int id = (ijk[2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes))+
                         (ijk[1]*(smesh.x.nb_nodes))+ijk[0];
                ptexist[id]=true;
            }
        }


    unsigned int nbptexistreal=0;
    unsigned int nbexist=0;
    vtkPoints *xyzpointsreal = vtkPoints::New();
    map <unsigned int,int> ptugridreal;
    for (int k=0; k<smesh.z.nb_nodes;k++)
    {
    	for(int j=0;j<smesh.y.nb_nodes;j++)
	    {
    		for(int i=0;i<smesh.x.nb_nodes;i++)
		    {
    			idptexist = (k*(smesh.x.nb_nodes)*(smesh.y.nb_nodes))+(j*(smesh.x.nb_nodes))+i;
    			if(ptexist[idptexist])
    			{
    				xyzpointsreal->InsertNextPoint(smesh.x.nodes[i],
						                       smesh.y.nodes[j],
						                       smesh.z.nodes[k]);
    				ptugridreal[idptexist]=nbptexistreal;
    				nbptexistreal=nbptexistreal+1;
    			}
    			nbexist=nbexist+1;
		    }
	    }
    }
    sgrid->SetPoints(xyzpointsreal);
    xyzpointsreal->Delete();
    ptexist.clear();
    for(int i=0;i<smesh.nb_groups;i++)
    {
    	for(unsigned int j=0;j<smesh.groups[i].dims[0];j++)
        {
            if(strcmp(smesh.groups[i].entitytype,"face")==0)
            {
                int ijk[4][3];
                int imin = smesh.groups[i].elements[j*6+0];
                int jmin = smesh.groups[i].elements[j*6+1];
                int kmin = smesh.groups[i].elements[j*6+2];
                int imax = smesh.groups[i].elements[j*6+3];
                int jmax = smesh.groups[i].elements[j*6+4];
                int kmax = smesh.groups[i].elements[j*6+5];
                if(imin==imax)
                {
                    // point 1
                    ijk[0][0]=imin;
                    ijk[0][1]=jmin;
                    ijk[0][2]=kmin;
                    // point 2
                    ijk[1][0]=imin;
                    ijk[1][1]=jmax;
                    ijk[1][2]=kmin;
                    // point 3
                    ijk[2][0]=imin;
                    ijk[2][1]=jmin;
                    ijk[2][2]=kmax;
                    // point 4
                    ijk[3][0]=imin;
                    ijk[3][1]=jmax;
                    ijk[3][2]=kmax;
                    double point[4][3];
                    vtkPixel *pixelcell = vtkPixel::New();
                    for (int ii=0; ii<4; ii++)
                    {
                        unsigned int idtemp = (ijk[ii][2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes))+
                                 (ijk[ii][1]*(smesh.x.nb_nodes))+ijk[ii][0];
		                unsigned int id = ptugridreal[idtemp];
                        sgrid->GetPoint(id,point[ii]);
                        pixelcell->GetPointIds()->SetId(ii,id);
                    }
                    sgrid->InsertNextCell(pixelcell->GetCellType(),
                                          pixelcell->GetPointIds());
                    pixelcell->Delete();
                    //grpgroupId->InsertTuple1(cellId,grpgroupId_value[grpchild.childnames[i]]);
                    
                    cellId++;
                }
                if(jmin==jmax)
                {
                    // point 1
                    ijk[0][0]=imin;
                    ijk[0][1]=jmin;
                    ijk[0][2]=kmin;
                    // point 2
                    ijk[1][0]=imax;
                    ijk[1][1]=jmin;
                    ijk[1][2]=kmin;
                    // point 3
                    ijk[2][0]=imin;
                    ijk[2][1]=jmin;
                    ijk[2][2]=kmax;
                    // point 4
                    ijk[3][0]=imax;
                    ijk[3][1]=jmin;
                    ijk[3][2]=kmax;
                    double point[4][3];
                    vtkPixel *pixelcell = vtkPixel::New();
                    for (int ii=0; ii<4; ii++)
                    {
                        unsigned int idtemp = (ijk[ii][2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes))+
                                 (ijk[ii][1]*(smesh.x.nb_nodes))+ijk[ii][0];
		        unsigned int id = ptugridreal[idtemp];
                        sgrid->GetPoint(id,point[ii]);
                        pixelcell->GetPointIds()->SetId(ii,id);
                    }
                    sgrid->InsertNextCell(pixelcell->GetCellType(),
                                          pixelcell->GetPointIds());
                    pixelcell->Delete();
                    //grpgroupId->InsertTuple1(cellId,grpgroupId_value[grpchild.childnames[i]]);
                    cellId++;
                }
                if(kmin==kmax)
                {
                    // point 1
                    ijk[0][0]=imin;
                    ijk[0][1]=jmin;
                    ijk[0][2]=kmin;
                    // point 2
                    ijk[1][0]=imax;
                    ijk[1][1]=jmin;
                    ijk[1][2]=kmin;
                    // point 3
                    ijk[2][0]=imin;
                    ijk[2][1]=jmax;
                    ijk[2][2]=kmin;
                    // point 4
                    ijk[3][0]=imax;
                    ijk[3][1]=jmax;
                    ijk[3][2]=kmin;
                    double point[4][3];
                    vtkPixel *pixelcell = vtkPixel::New();
                    for (int ii=0; ii<4; ii++)
                    {
                        unsigned int idtemp = (ijk[ii][2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes))+
                                 (ijk[ii][1]*(smesh.x.nb_nodes))+ijk[ii][0];
                        unsigned int id = ptugridreal[idtemp];
                        sgrid->GetPoint(id,point[ii]);
                        pixelcell->GetPointIds()->SetId(ii,id);
                    }
                    sgrid->InsertNextCell(pixelcell->GetCellType(),
                                          pixelcell->GetPointIds());
                    pixelcell->Delete();
                    //grpgroupId->InsertTuple1(cellId,grpgroupId_value[grpchild.childnames[i]]);
                    cellId++;
                }
                groupId->InsertTuple1(cellId-1,i);
                
            }
            else if(strcmp(smesh.groups[i].entitytype,"volume")==0)
            {

                int ijk[8][3];
                int imin = smesh.groups[i].elements[j*6+0];
                int jmin = smesh.groups[i].elements[j*6+1];
                int kmin = smesh.groups[i].elements[j*6+2];
                int imax = smesh.groups[i].elements[j*6+3];
                int jmax = smesh.groups[i].elements[j*6+4];
                int kmax = smesh.groups[i].elements[j*6+5];
                // point 1
                ijk[0][0]=imin;
                ijk[0][1]=jmin;
                ijk[0][2]=kmin;
                // point 2
                ijk[1][0]=imax;
                ijk[1][1]=jmin;
                ijk[1][2]=kmin;
                // point 3
                ijk[2][0]=imin;
                ijk[2][1]=jmax;
                ijk[2][2]=kmin;
                // point 4
                ijk[3][0]=imax;
                ijk[3][1]=jmax;
                ijk[3][2]=kmin;
                // point 5
                ijk[4][0]=imin;
                ijk[4][1]=jmin;
                ijk[4][2]=kmax;
                // point 6
                ijk[5][0]=imax;
                ijk[5][1]=jmin;
                ijk[5][2]=kmax;
                // point 7
                ijk[6][0]=imin;
                ijk[6][1]=jmax;
                ijk[6][2]=kmax;
                // point 8
                ijk[7][0]=imax;
                ijk[7][1]=jmax;
                ijk[7][2]=kmax;
                double point[8][3];
                vtkVoxel *voxelcell = vtkVoxel::New();
                for (int ii=0; ii<8; ii++)
                {
                    unsigned int idtemp = ijk[ii][2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes)+
                             ijk[ii][1]*(smesh.x.nb_nodes)+ijk[ii][0];
		    unsigned int id = ptugridreal[idtemp];
                    sgrid->GetPoint(id,point[ii]);
                    voxelcell->GetPointIds()->SetId(ii,id);
                }
                sgrid->InsertNextCell(voxelcell->GetCellType(),voxelcell->GetPointIds());
                voxelcell->Delete();
                //grpgroupId->InsertTuple1(cellId,grpgroupId_value[grpchild.childnames[i]]);
                groupId->InsertTuple1(cellId,i);
                
                cellId++;
                
            }
            else if((strcmp(smesh.groups[i].entitytype,"edge")==0) ||
                    (strcmp(smesh.groups[i].entitytype,"slot")==0))
            {
                int ijk[2][3];
                int imin = smesh.groups[i].elements[j*6+0];
                int jmin = smesh.groups[i].elements[j*6+1];
                int kmin = smesh.groups[i].elements[j*6+2];
                int imax = smesh.groups[i].elements[j*6+3];
                int jmax = smesh.groups[i].elements[j*6+4];
                int kmax = smesh.groups[i].elements[j*6+5];

                // point 1
                ijk[0][0]=imin;
                ijk[0][1]=jmin;
                ijk[0][2]=kmin;
                // point 2
                ijk[1][0]=imax;
                ijk[1][1]=jmax;
                ijk[1][2]=kmax;
                vtkLine *linecell = vtkLine::New();
                double point[2][3];
                for (int ii=0; ii<2; ii++)
                {
                    unsigned int idtemp = ijk[ii][2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes)+
                             ijk[ii][1]*(smesh.x.nb_nodes)+ijk[ii][0];
		            unsigned int id = ptugridreal[idtemp];
                    sgrid->GetPoint(id,point[ii]);
                    linecell->GetPointIds()->SetId(ii,id);
                }
                sgrid->InsertNextCell(linecell->GetCellType(),linecell->GetPointIds());
                linecell->Delete();
                //grpgroupId->InsertTuple1(cellId,grpgroupId_value[grpchild.childnames[i]]);
                groupId->InsertTuple1(cellId,i);
                
                cellId++;
                
            }
            else if(strcmp(smesh.groups[i].type,"node")==0)
            {
                int ijk[3];
                int imin = smesh.groups[i].elements[j*3+0];
                int jmin = smesh.groups[i].elements[j*3+1];
                int kmin = smesh.groups[i].elements[j*3+2];
                ijk[0]=imin;
                ijk[1]=jmin;
                ijk[2]=kmin;
                vtkVertex *vertexcell = vtkVertex::New();
                double point[3];
                unsigned int idtemp = ijk[2]*(smesh.x.nb_nodes)*(smesh.y.nb_nodes)+
                         ijk[1]*(smesh.x.nb_nodes)+ijk[0];
		        unsigned int id = ptugridreal[idtemp];
                sgrid->GetPoint(id,point);
                vertexcell->GetPointIds()->SetId(0,id);
                sgrid->InsertNextCell(vertexcell->GetCellType(),vertexcell->GetPointIds());
                vertexcell->Delete();
                //grpgroupId->InsertTuple1(cellId,grpgroupId_value[grpchild.childnames[i]]);
                groupId->InsertTuple1(cellId,i);
                cellId++;
                
            }
        }
    }

    ptugridreal.clear();
    //sgrid->GetCellData()->AddArray(grpgroupId);
    sgrid->GetCellData()->AddArray(groupId);
    grpgroupId->Delete();
    groupId->Delete();
    return 1;
}
int vtkAmeletHDFMeshReader::mergeVtkGrid(vtkUnstructuredGrid *ugridtemp , vtkUnstructuredGrid *ugrid)
{
	// Not Tested
	int offset = ugrid->GetNumberOfPoints();
	vtkPoints *points = vtkPoints::New();
	vtkCell *cell;
	int nb_point_cell;

	for(int i=0;i<ugrid->GetNumberOfPoints();i++)
	{
		points->InsertNextPoint(ugrid->GetPoint(i));
	}
	for(int i=0;i<ugridtemp->GetNumberOfPoints();i++)
	{
		points->InsertNextPoint(ugridtemp->GetPoint(i));
	}
	ugrid->SetPoints(points);
	points->Delete();

	for(int i=0;i<ugridtemp->GetNumberOfCells();i++)
	{
		vtkIdList *idpoint;
		cell = ugridtemp->GetCell(i);
		nb_point_cell = cell->GetNumberOfPoints();
		idpoint = (vtkIdList *) malloc(nb_point_cell * sizeof(vtkIdList));
		idpoint = cell->GetPointIds();
		for (int j=0;j<nb_point_cell;j++)
		{
		    idpoint->SetId(j,idpoint->GetId(j)+offset);
		}
		ugrid->InsertNextCell(cell->GetCellType(),idpoint);
        free(idpoint);
	}

	return 1;
}

int vtkAmeletHDFMeshReader::extractUmshGroup(AH5_msh_instance_t *msh_i, const char * path,AH5_umesh_t *ugroup)
{
	int idnode=0;
	int step;
	int elt_nodes_start[msh_i->data.unstructured.nb_elementtypes];
	int elttype;
	ugroup->elementnodes = NULL;
    ugroup->elementtypes = NULL;
    ugroup->nodes = NULL;
    ugroup->groups = NULL;
    ugroup->groupgroups = NULL;
    ugroup->som_tables = NULL;

	for (int i=0;i<msh_i->data.unstructured.nb_elementtypes;i++)
	{
		elttype=msh_i->data.unstructured.elementtypes[i];
		elt_nodes_start[i]=idnode;
		if((elttype==1) || (elttype==2)){
			step=2;
			if(elttype==2)step=3;
		}
		else if((elttype==11) || (elttype==12)){
			step=3;
			if(elttype==12)step=6;
		}
		else if((elttype==13) || (elttype==14)){
			step=4;
			if(elttype==14)step=8;
		}
		else if((elttype==101) || (elttype==104)){
			step=4;
			if(elttype==104)step=8;
		}
		idnode=idnode+step;
	}
	for (int i=0;i<msh_i->data.unstructured.nb_groups;i++)
		if(strcmp(msh_i->data.unstructured.groups[i].path,path)==0)
		{

			if(strcmp(msh_i->data.unstructured.groups[i].type,"element")==0)
			{
				elttype=msh_i->data.unstructured.elementtypes[msh_i->data.unstructured.groups[i].groupelts[0]];
				step=-1;
				if((elttype==1) || (elttype==2)){
					step=2;
					if(elttype==2) step=3;
				}
				else if((elttype==11) || (elttype==12)){
					step=3;
					if(elttype==12) step=6;
				}
				else if((elttype==13) || (elttype==14)){
					step=4;
					if(elttype==14) step=8;
				}
				else if((elttype==101) || (elttype==104)){
					step=4;
					if(elttype==104) step=8;
				}
				ugroup->nb_elementtypes = msh_i->data.unstructured.groups[i].nb_groupelts;
				ugroup->elementtypes = (char *) malloc((size_t) ugroup->nb_elementtypes * sizeof(char));
				ugroup->nb_elementnodes = step* ugroup->nb_elementtypes;
				ugroup->elementnodes = (int *) malloc((size_t) ugroup->nb_elementnodes * sizeof(int));
				int id = 0;
				for (int ielt=0;ielt<msh_i->data.unstructured.groups[i].nb_groupelts;ielt++)
				{
					int grp_elt = msh_i->data.unstructured.groups[i].groupelts[ielt];
					ugroup->elementtypes[ielt]=msh_i->data.unstructured.elementtypes[msh_i->data.unstructured.groups[i].groupelts[0]];
					for (int istep=0; istep<step; istep++)
						ugroup->elementnodes[id+istep]=msh_i->data.unstructured.elementnodes[elt_nodes_start[grp_elt]+istep];
                    id = id + step;
				}
				ugroup->nb_nodes[0] = msh_i->data.unstructured.nb_nodes[0];
				ugroup->nb_nodes[1] = msh_i->data.unstructured.nb_nodes[1];
				ugroup->nodes = (float *) malloc((size_t) ugroup->nb_nodes[0]*ugroup->nb_nodes[1] * sizeof(float));
				int nbnodestotal =ugroup->nb_nodes[0]*ugroup->nb_nodes[1];

				for (int inode=0;inode<nbnodestotal;inode++)
					ugroup->nodes[inode] = msh_i->data.unstructured.nodes[inode];


				ugroup->nb_groupgroups = 0;
				ugroup->nb_som_tables = 0;
				ugroup->nb_groups = 1;
				ugroup->groups = (AH5_ugroup_t *) malloc ((size_t) sizeof(AH5_ugroup_t));
				ugroup->groups[0].groupelts = (int *) malloc((size_t) msh_i->data.unstructured.groups[i].nb_groupelts* sizeof(int));
				for (int ielt=0;ielt<msh_i->data.unstructured.groups[i].nb_groupelts;ielt++)
					ugroup->groups[0].groupelts[ielt]=ielt;
				ugroup->groups[0].entitytype = strdup(msh_i->data.unstructured.groups[i].entitytype);
				ugroup->groups[0].type = strdup(msh_i->data.unstructured.groups[i].type);
				ugroup->groups[0].nb_groupelts = msh_i->data.unstructured.groups[i].nb_groupelts;
				ugroup->groups[0].path = strdup(msh_i->data.unstructured.groups[i].path);

			}

		}
	return 1;
}

int vtkAmeletHDFMeshReader::extractSmshGroup(AH5_msh_instance_t *msh_i, const char * path,AH5_smesh_t *sgroup)
{
    sgroup->groupgroups = NULL;
    sgroup->groups = NULL;
    sgroup->nb_groupgroups = 0;
    sgroup->nb_groups = 0;
    sgroup->nb_som_tables = 0;
    sgroup->som_tables = NULL;
    sgroup->x.nodes = NULL;
    sgroup->x.nb_nodes = 0;
    sgroup->y.nodes = NULL;
    sgroup->y.nb_nodes = 0;
    sgroup->z.nodes = NULL;
    sgroup->z.nb_nodes = 0;

    // set cartesian grid
    sgroup->x.nb_nodes = msh_i->data.structured.x.nb_nodes;
    sgroup->y.nb_nodes = msh_i->data.structured.y.nb_nodes;
    sgroup->z.nb_nodes = msh_i->data.structured.z.nb_nodes;
    sgroup->x.nodes = (float *) malloc( (size_t) sgroup->x.nb_nodes*sizeof(float));
    for (int k=0;k<sgroup->x.nb_nodes;k++)
    	sgroup->x.nodes[k] = msh_i->data.structured.x.nodes[k];
    sgroup->y.nodes = (float *) malloc( (size_t) sgroup->y.nb_nodes*sizeof(float));
    for (int k=0;k<sgroup->y.nb_nodes;k++)
    	sgroup->y.nodes[k] = msh_i->data.structured.y.nodes[k];
    sgroup->z.nodes = (float *) malloc( (size_t) sgroup->z.nb_nodes*sizeof(float));
    for (int k=0;k<sgroup->z.nb_nodes;k++)
        	sgroup->z.nodes[k] = msh_i->data.structured.z.nodes[k];

    for (int i=0;i<msh_i->data.structured.nb_groups;i++)
    {
    	if(strcmp(msh_i->data.structured.groups[i].path,path)==0)
    	{
    		sgroup->nb_groups = 1;
    		sgroup->groups = (AH5_sgroup_t *) malloc ((size_t) sizeof(AH5_sgroup_t));
    		sgroup->groups[0].dims[0]=msh_i->data.structured.groups[i].dims[0];
    		sgroup->groups[0].dims[1]=msh_i->data.structured.groups[i].dims[1];
    		std::cout<<sgroup->groups[0].dims[0]<<" , "<<sgroup->groups[0].dims[1]<<std::endl;
    		sgroup->groups[0].elements = (int *) malloc((size_t) sgroup->groups[0].dims[0]*sgroup->groups[0].dims[1]*sizeof(int));
    		for(int j=0;j<sgroup->groups[0].dims[0]*sgroup->groups[0].dims[1];j++)
    			sgroup->groups[0].elements[j]=msh_i->data.structured.groups[i].elements[j];
    		sgroup->groups[0].entitytype = strdup(msh_i->data.structured.groups[i].entitytype);
    		sgroup->groups[0].type = strdup(msh_i->data.structured.groups[i].type);
    		sgroup->groups[0].path = strdup(msh_i->data.structured.groups[i].path);
    		sgroup->groups[0].normals = NULL;


    	}
    }
	return 1;
}
int vtkAmeletHDFMeshReader::extractGroupGroup(AH5_msh_instance_t *msh_i, const char * path, vtkUnstructuredGrid *grid)
{
	// Beware : Not Tested
	int nbelt = -1;
	int nbelt_temp = 0;
	int err;
	char *grpname;
	int ktemp=-1;
    AH5_umesh_t ugroup;
    AH5_smesh_t sgroup;

	vtkUnstructuredGrid *gridtemp = vtkUnstructuredGrid::New();

    if (msh_i->type == MSH_UNSTRUCTURED)
    {
    	for(int i=0;i<msh_i->data.unstructured.nb_groupgroups;i++)
    	{
    		if(strcmp(msh_i->data.unstructured.groupgroups[i].path,path)==0)
    		{
    			for(int j=0;j<msh_i->data.unstructured.groupgroups[i].nb_groupgroupnames;j++)
    			{
    				for(int k=0;k<msh_i->data.unstructured.nb_groups;k++)
    				{
    					grpname = AH5_get_base_from_path(msh_i->data.unstructured.groups[k].path);
    					if(strcmp(grpname,msh_i->data.unstructured.groupgroups[i].groupgroupnames[j])==0)
    						ktemp=k;
    				}
    				err = extractUmshGroup(msh_i, msh_i->data.unstructured.groups[ktemp].path , &ugroup);
    				readUmesh( ugroup, gridtemp);
    			    mergeVtkGrid(gridtemp,grid);
    			}

    		}
    	}
    }
    else if (msh_i->type == MSH_STRUCTURED)
    {
    	for(int i=0;i<msh_i->data.structured.nb_groupgroups;i++)
    	{
    		if(strcmp(msh_i->data.structured.groupgroups[i].path,path)==0)
    		{
    			for(int j=0;j<msh_i->data.unstructured.groupgroups[i].nb_groupgroupnames;j++)
    			{
    				for(int k=0;k<msh_i->data.unstructured.nb_groups;k++)
    				{
    					grpname = AH5_get_base_from_path(msh_i->data.unstructured.groups[k].path);
    					if(strcmp(grpname,msh_i->data.unstructured.groupgroups[i].groupgroupnames[j])==0)
    							ktemp=k;
    				}
    				err = extractSmshGroup(msh_i, msh_i->data.unstructured.groups[ktemp].path , &sgroup);
    				readSmesh( sgroup, gridtemp);
    				mergeVtkGrid(gridtemp,grid);
    			}

    		}
    	}
    }
    gridtemp->Delete();
	return nbelt;
}
int vtkAmeletHDFMeshReader::readMeshGroup(hid_t loc_id, const char* path, vtkUnstructuredGrid *grid)
{

	int nbelt=-1;
    char *base;
    char *grp;
    char *mesh_instance;
    int err;
    AH5_msh_instance_t msh_i;
    AH5_umesh_t ugroup;
    AH5_smesh_t sgroup;

    std::cout<<"[vtkAmeletHDFMeshReader::readMeshGroup] BEGIN"<<std::endl;



    base = AH5_get_base_from_path(path);
    grp = AH5_get_name_from_path(base);
    mesh_instance = AH5_get_base_from_path(base);
    AH5_read_msh_instance(loc_id,mesh_instance,&msh_i);

    if(strcmp(grp,"group")==0)
    {
    	if(msh_i.type==MSH_UNSTRUCTURED)
    	{
            err = extractUmshGroup(&msh_i,path,&ugroup);
            readUmesh( ugroup, grid);
            nbelt = ugroup.nb_elementtypes;
            AH5_free_umesh(&ugroup);
    	}
    	else if(msh_i.type==MSH_STRUCTURED)
    	{
    	  	err = extractSmshGroup(&msh_i,path,&sgroup);
    	  	readSmesh( sgroup, grid);
    	  	nbelt = sgroup.groups[0].dims[0];
    	  	AH5_free_smesh(&sgroup);
    	}
    }
    else if(strcmp(grp,"groupGroup")==0)
    	nbelt = extractGroupGroup(&msh_i,path,grid);
    AH5_free_msh_instance(&msh_i);
    std::cout<<"[vtkAmeletHDFMeshReader::readMeshGroup] END"<<std::endl;
	return nbelt;
}
