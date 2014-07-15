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

int vtkAmeletHDFMeshReader::readUmesh(hid_t meshId, AH5_umesh_t umesh, vtkUnstructuredGrid *ugrid)
{
    //std::cout<<"Read umesh"<<std::endl;
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
    
    return 1;
}

int vtkAmeletHDFMeshReader::readSmesh(hid_t meshId, AH5_smesh_t smesh, vtkUnstructuredGrid *sgrid)
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

