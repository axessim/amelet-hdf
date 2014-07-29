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
    virtual int readUmesh(AH5_umesh_t umesh, vtkUnstructuredGrid *ugrid);
    virtual int readSmesh(AH5_smesh_t smesh, vtkUnstructuredGrid *sgrid);
    virtual int mergeVtkGrid(vtkUnstructuredGrid *gridtemp, vtkUnstructuredGrid *grid);
    virtual int extractUmshGroup(AH5_msh_instance_t *msh_i, const char * path, AH5_umesh_t *ugroup);
    virtual int extractSmshGroup(AH5_msh_instance_t *msh_i, const char * path, AH5_smesh_t *sgroup);
    virtual int extractGroupGroup(AH5_msh_instance_t *msh_i, const char * path, vtkUnstructuredGrid *grid);
    virtual int readUSom( AH5_msh_instance_t *msh_i, const char * path, vtkUnstructuredGrid *grid);
    virtual int readSSom( AH5_msh_instance_t *msh_i, const char * path, vtkUnstructuredGrid *grid);
    virtual int readMeshGroup(hid_t loc_id, const char* path, vtkUnstructuredGrid *grid);
};
#endif /* __VTKvtkAmeletHDFMeshReader_H_  */
