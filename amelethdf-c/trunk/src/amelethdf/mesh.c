#include <stdlib.h>
#include <hdf5.h>
#include <string.h>

#include "mesh.h"
#include "amelethdf.h"

#define FALSE 0
#define TRUE 1


int meshtype(hid_t loc_id, const char * path)
{
	herr_t   status;
	hid_t    attr_id, attr_type, space, memtype, mesh_id;
	size_t sdim;
	hsize_t     dims[1]={1} ;
	int ndims, i, ret_val=-1;
	char **rdata;

	mesh_id=H5Gopen1(loc_id,path);

	ret_val = 0;
	attr_id = H5Aopen(mesh_id,"type",H5P_DEFAULT);

	if (attr_id >= 0)
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
	  if(strcmp(rdata[0],"unstructured")==0) ret_val=UNSTRUCTURED;
	  else if (strcmp(rdata[0],"structured")==0) ret_val=STRUCTURED;
	  H5Aclose(attr_id);
	}
	H5Gclose(mesh_id);
	return ret_val;

}



groupgroup_t readGroupGroup(hid_t grpgrp_id,const char* name)
{
  hid_t space, filetype, memtype;
  int ndims,i;
  hsize_t dims[1];
  groupgroup_t rdata;
  herr_t status;
  size_t sdim;


  rdata.name = (char *)malloc(ELEMENT_NAME_LENGTH * sizeof(char));
  strcpy(rdata.name,name);
  filetype = H5Dget_type(grpgrp_id);
  sdim = H5Tget_size(filetype);
  sdim++;  /*Make room for null terminator */

  space = H5Dget_space (grpgrp_id);
  ndims = H5Sget_simple_extent_dims (space, dims, NULL);

  /* Allocate array of pointers to rows. */
  rdata.groupGroupnames = (char **) malloc (dims[0] * sizeof (char *));
  /* Allocate space for integer data */
  rdata.groupGroupnames[0] = (char *) malloc (dims[0] * sdim * sizeof (char));
  rdata.nbeltgroupGroup = dims[0];
  for (i=1; i<dims[0]; i++)
     rdata.groupGroupnames[i] = rdata.groupGroupnames[0] + i * sdim;

  /* Create the memory datatype */
  memtype = H5Tcopy (H5T_C_S1);
  status = H5Tset_size (memtype, sdim);

  /* read the data */

  status = H5Dread (grpgrp_id, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdata.groupGroupnames[0]);

  return rdata;
}

unstructured_mesh_t read_unstructured_mesh(hid_t file_id, const char* path)
{
	unstructured_mesh_t umesh;
   	hid_t nodes_id, eltype_id, eltnode_id, loc_id;
   	char *bufpath;
   	char *bufpath2;
   	hid_t groupGroup_id, group_id, grpGrp_id;
   	children_t children;
    int i;

   	// read nodes

   	bufpath=(char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
   	strcpy(bufpath,path);
   	strcat(bufpath,NODES);
   	nodes_id = H5Dopen1(file_id,bufpath);
   	umesh.nodes = readNodes(nodes_id);
   	H5Dclose(nodes_id);
   	free(bufpath);


   	//read element nodes
   	bufpath=(char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
   	strcpy(bufpath,path);
   	strcat(bufpath,ELEMENT_NODES);
    eltnode_id = H5Dopen1(file_id,bufpath);
    umesh.element_nodes = readElementNodes(eltnode_id);
    H5Dclose(eltnode_id);
    free(bufpath);


   	//read element types;
   	bufpath=(char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
   	strcpy(bufpath,path);
   	strcat(bufpath,ELEMENTS);
    eltype_id = H5Dopen1(file_id,bufpath);
    umesh.elements = readElementTypes(eltype_id);
    H5Dclose(eltype_id);
    free(bufpath);



    //read groupGroup if exists
   	bufpath=(char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
   	strcpy(bufpath,path);
   	strcat(bufpath,GROUPGROUP);

   	umesh.groupgroups.nbgroupGroup = 0;
    if (H5Lexists(file_id,bufpath,H5P_DEFAULT)!=FALSE)
    {
    	printf("Groupgroup exists\n");
    	children = read_children_name(file_id,bufpath);
    	umesh.groupgroups.groupgroups = (groupgroup_t *)malloc(children.nbchild*sizeof(groupgroup_t));
    	umesh.groupgroups.nbgroupGroup = children.nbchild;
    	for(i=0;i<children.nbchild;i++)
    	{
    		bufpath2 = (char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
    		strcpy(bufpath2,bufpath);
    		strcat(bufpath2,"/");
    		strcat(bufpath2,children.childnames[i]);
    		grpGrp_id = H5Dopen1(file_id , bufpath2);
    		umesh.groupgroups.groupgroups[i] = readGroupGroup(grpGrp_id,children.childnames[i]);
    		H5Dclose(grpGrp_id);
    		free(bufpath2);
    	}
    	free(children.childnames);
    }
    free(bufpath);


    // read group
   	bufpath=(char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
   	strcpy(bufpath,path);
   	strcat(bufpath,GROUP);
    children = read_children_name(file_id,bufpath);
    umesh.groups.nbgroup = children.nbchild;
    umesh.groups.groups = (ugroup_t *)malloc(children.nbchild * sizeof(ugroup_t));
    for(i=0;i<children.nbchild;i++)
    {
    	bufpath2 = (char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
    	strcpy(bufpath2,bufpath);
    	strcat(bufpath2,"/");
    	strcat(bufpath2,children.childnames[i]);
    	group_id = H5Dopen1(file_id,bufpath2);
    	umesh.groups.groups[i] = readUGroup(group_id,children.childnames[i]);
    	H5Dclose(group_id);
    	free(bufpath2);
    }
    free(bufpath);


    // read selectorOnMesh/elements
   	bufpath=(char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
   	strcpy(bufpath,path);
   	strcat(bufpath,SELECTOR_ON_MESH);
   	umesh.som_elements.nbsomelt=0;
   	umesh.som_nodes.nbsomnodes=0;
   	if(H5Lexists(file_id,bufpath,H5P_DEFAULT)!=FALSE)
   	{
   		strcat(bufpath,L_ELEMENTS);
   		if (H5Lexists(file_id,bufpath,H5P_DEFAULT)!=FALSE)
   		{
   			umesh.som_elements = readSemElt(file_id,bufpath);
   		}
   		free(bufpath);


   		// read selectorOnMesh/nodes
   		bufpath=(char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
   		strcpy(bufpath,path);
   		strcat(bufpath,SELECTOR_ON_MESH);
   		strcat(bufpath,NODES);
   		if (H5Lexists(file_id,bufpath,H5P_DEFAULT)!=FALSE)
   		{
   			umesh.som_nodes = readSemNodes(file_id,bufpath);
   		}
   		free(bufpath);
   	}
   	else
   	{
   		free(bufpath);
   	}


	return umesh;
}

void print_unstructured_mesh(unstructured_mesh_t umesh)
{
	int i,j, offset;
	int element_type;

	offset = 0;
	printf("Name : %s\n",umesh.name);
	printf("Number of nodes : %i\n",umesh.nodes.nbnodes);
	for(i=0;i<umesh.nodes.nbnodes;i++)
	{
		printf("Node n°%i : ",i);
	    for(j=0;j<3;j++)
	    	printf("%f ",umesh.nodes.nodes[i][j]);
	    printf("\n");
	}

	printf("\nNumber of elements : %i\n",umesh.elements.nbelttypes);
	for(i=0;i<umesh.elements.nbelttypes;i++)
	{
		printf("Element n°%i , type : %i , ",i,umesh.elements.elttypes[i]);
		element_type = umesh.elements.elttypes[i];
		if(element_type == 1)
			for(j=0;j<2;j++)
			{
				printf( "Node n°%i : %i ",j,umesh.element_nodes.eltnodes[offset]);
				offset++;
			}
		else if(element_type == 2)
			for(j=0;j<3;j++)
			{
				printf( "Node n°%i : %i ",j,umesh.element_nodes.eltnodes[offset]);
				offset++;
			}
		else if(element_type == 11)
			for(j=0;j<3;j++)
			{
				printf( "Node n°%i : %i ",j,umesh.element_nodes.eltnodes[offset]);
				offset++;
			}
		else if(element_type == 12)
			for(j=0;j<6;j++)
			{
				printf( "Node n°%i : %i ",j,umesh.element_nodes.eltnodes[offset]);
				offset++;
			}
		else if(element_type == 13)
			for(j=0;j<4;j++)
			{
				printf( "Node n°%i : %i ",j,umesh.element_nodes.eltnodes[offset]);
				offset++;
			}
		else if(element_type == 14)
			for(j=0;j<8;j++)
			{
				printf( "Node n°%i : %i ",j,umesh.element_nodes.eltnodes[offset]);
				offset++;
			}
		else if(element_type == 101)
			for(j=0;j<4;j++)
			{
				printf( "Node n°%i : %i ",j,umesh.element_nodes.eltnodes[offset]);
				offset++;
			}
		else if(element_type == 102)
			for(j=0;j<5;j++)
			{
				printf( "Node n°%i : %i ",j,umesh.element_nodes.eltnodes[offset]);
				offset++;
			}
		else if(element_type == 103)
			for(j=0;j<6;j++)
			{
				printf( "Node n°%i : %i ",j,umesh.element_nodes.eltnodes[offset]);
				offset++;
			}
		else if(element_type == 104)
			for(j=0;j<8;j++)
			{
				printf( "Node n°%i : %i ",j,umesh.element_nodes.eltnodes[offset]);
				offset++;
			}
		printf("\n");
	}
	printf("\nNumber of groups : %i\n",umesh.groups.nbgroup);
	for(i=0;i<umesh.groups.nbgroup;i++)
		printf(" Name : %s\n",umesh.groups.groups[i].name);

	printf("\nNumber of groupGroups : %i\n",umesh.groupgroups.nbgroupGroup);
	for(i=0;i<umesh.groupgroups.nbgroupGroup;i++)
		printf(" Name : %s\n",umesh.groupgroups.groupgroups[i].name);

	printf("\nSelector on mesh / nodes ...\n");
	for(i=0;i<umesh.som_nodes.nbsomnodes;i++)
		printf(" shortname = %s, index = %i\n",umesh.som_nodes.somnodes[i].shortname,
				                                umesh.som_nodes.somnodes[i].index);

	printf("\nSelector on mesh / elements ...\n");
	for(i=0;i<umesh.som_elements.nbsomelt;i++)
		printf(" shortname = %s, index = %i, v1 = %f, v2 = %f, v3 = %f\n",
				  umesh.som_elements.somelt[i].shortname,
				  umesh.som_elements.somelt[i].index,
				  umesh.som_elements.somelt[i].v1,
				  umesh.som_elements.somelt[i].v2,
				  umesh.som_elements.somelt[i].v3);

}
structured_mesh_t read_structured_mesh(hid_t file_id, const char* path)
{
	structured_mesh_t smesh;
	int i;
	children_t children;
	hid_t group_id, grpGrp_id;

	char *bufpath;
	char *bufpath2;

	bufpath = (char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
	// X Axis
	strcpy(bufpath,path);
	strcat(bufpath,G_CARTESIAN_GRID);
	strcat(bufpath,"/x");
	smesh.x = readAxis(file_id,bufpath);
	free(bufpath);

	// Y Axis
	bufpath = (char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
	strcpy(bufpath,path);
	strcat(bufpath,G_CARTESIAN_GRID);
	strcat(bufpath,"/y");
	smesh.y = readAxis(file_id,bufpath);
	free(bufpath);

	// Z Axis
	bufpath = (char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
	strcpy(bufpath,path);
	strcat(bufpath,G_CARTESIAN_GRID);
	strcat(bufpath,"/z");
	smesh.z = readAxis(file_id,bufpath);
	free(bufpath);

	// groups
	bufpath = (char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
	strcpy(bufpath,path);
	strcat(bufpath,GROUP);
    children = read_children_name(file_id,bufpath);
    smesh.groups.nbgroup = children.nbchild;
    smesh.groups.groups = (sgroup_t *)malloc(children.nbchild * sizeof(sgroup_t));
    for(i=0;i<children.nbchild;i++)
    {
    	bufpath2 = (char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
    	strcpy(bufpath2,bufpath);
    	strcat(bufpath2,"/");
    	strcat(bufpath2,children.childnames[i]);
    	group_id = H5Dopen1(file_id,bufpath2);
    	smesh.groups.groups[i] = readSGroup(group_id,children.childnames[i]);
    	H5Dclose(group_id);
    	free(bufpath2);
    }
    free(children.childnames);
    free(bufpath);

    // groupGroups
   	bufpath=(char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
   	strcpy(bufpath,path);
   	strcat(bufpath,GROUPGROUP);

   	smesh.groupgroups.nbgroupGroup = 0;
    if (H5Lexists(file_id,bufpath,H5P_DEFAULT)!=FALSE)
    {
    	children = read_children_name(file_id,bufpath);
    	smesh.groupgroups.groupgroups = (groupgroup_t *)malloc(children.nbchild*sizeof(groupgroup_t));
    	smesh.groupgroups.nbgroupGroup = children.nbchild;
    	for(i=0;i<children.nbchild;i++)
    	{
    		bufpath2 = (char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
    		strcpy(bufpath2,bufpath);
    		strcat(bufpath2,"/");
    		strcat(bufpath2,children.childnames[i]);
    		grpGrp_id = H5Dopen1(file_id , bufpath2);
    		smesh.groupgroups.groupgroups[i] = readGroupGroup(grpGrp_id,children.childnames[i]);
    		H5Dclose(grpGrp_id);
    		free(bufpath2);
    	}
    	free(children.childnames);
    }
    free(bufpath);

	return smesh;
}

void print_structured_mesh(structured_mesh_t smesh)
{
	int i;
	printf("Structured mesh\n");
	printf("Name : %s\n",smesh.name);
	printf("Groups : \n");
	printf("Number of groups : %i\n",smesh.groups.nbgroup);
	for(i=0;i<smesh.groups.nbgroup;i++)
	{
		printf("Name : %s, type : %s\n",smesh.groups.groups[i].name,
				smesh.groups.groups[i].type);
	}
	printf("GroupGRoups :\n");
	printf("Number of groupgroups : %i\n",smesh.groupgroups.nbgroupGroup);
	for(i=0;i<smesh.groupgroups.nbgroupGroup;i++)
	{
		printf("Name : %s\n",smesh.groupgroups.groupgroups[i].name);
	}
}
