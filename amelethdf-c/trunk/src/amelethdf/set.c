#include <hdf5.h>
#include <hdf5_hl.h>
#include "set.h"
#include <string.h>
#include <stdlib.h>
#include "amelethdf.h"

#define FALSE 0
#define TRUE 1

// Return TRUE if aelement is in aset
int in(set_t aset, char* aelement)
{
	int i;
	int present = FALSE;
	for(i=0;i<aset.number_of_element;i++)
	{
		if(strcmp(aset.array[i],aelement)==0) present=TRUE;
	}
	return present;
}

// Add aelement to aset
set_t add(set_t aset, char* aelement)
{
	if(!(in(aset,aelement)))
	{
		aset.number_of_element = aset.number_of_element + 1;
		aset.array[aset.number_of_element]=strdup(aelement);
	}
	return aset;
}

// Get ith from aset
char* get(set_t aset, int id)
{
	char* result;
	result = (char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
	strcpy(result,aset.array[id]);
	return result;
}

// Return the index of an element
int get_index(set_t aset, char* aelement)
{
	int i, index=-1;
	for(i=0;i<aset.number_of_element;i++)
		if(strcmp(aelement,aset.array[i])==0) index=i;
	return index;
}

set_t clean(set_t aset)
{
	char* buf;
	buf = (char *)malloc(ABSOLUTE_PATH_NAME_LENGTH*sizeof(char));
	strcpy(buf,"");
	if(aset.array != NULL) strcpy(aset.array[0],buf);
	aset.number_of_element=0;
	return aset;
}
