#ifndef __VTKvtkAmeletHDFMeshReader_H_
#define __VTKvtkAmeletHDFMeshReader_H_

#include <hdf5.h>
#include "vtkUnstructuredGrid.h"



class vtkAmeletHDFMeshReader
{
public:
    virtual int readUmesh(hid_t loc_id, char *name, vtkUnstructuredGrid *ugrid);
    virtual int readSmesh(hid_t loc_id, char *name, vtkUnstructuredGrid *sgrid);

};
#endif /* __VTKvtkAmeletHDFMeshReader_H_  */
