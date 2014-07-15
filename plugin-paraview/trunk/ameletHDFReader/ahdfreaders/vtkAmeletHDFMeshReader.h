#ifndef __VTKvtkAmeletHDFMeshReader_H_
#define __VTKvtkAmeletHDFMeshReader_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <hdf5.h>
#ifdef __cplusplus
}
#endif
#include "vtkUnstructuredGrid.h"
#include "ah5.h"



class vtkAmeletHDFMeshReader
{
public:
    virtual int readUmesh(hid_t loc_id, AH5_umesh_t umesh, vtkUnstructuredGrid *ugrid);
    virtual int readSmesh(hid_t loc_id, AH5_smesh_t smesh, vtkUnstructuredGrid *sgrid);

};
#endif /* __VTKvtkAmeletHDFMeshReader_H_  */
