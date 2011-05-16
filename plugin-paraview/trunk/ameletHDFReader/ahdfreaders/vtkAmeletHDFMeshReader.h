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



class vtkAmeletHDFMeshReader
{
public:
    virtual int readUmesh(hid_t loc_id, char *name, vtkUnstructuredGrid *ugrid);
    virtual int readSmesh(hid_t loc_id, char *name, vtkUnstructuredGrid *sgrid);
    virtual int readUgrp(hid_t loc_id, char *name, vtkUnstructuredGrid *ugrid, char* groupname);
    virtual int readUgrpgrp(hid_t loc_id, char *name, vtkUnstructuredGrid *ugrid, char* groupname);
    virtual int readSgrp(hid_t loc_id, char *name, vtkUnstructuredGrid *sgrid, char* groupname);
    virtual int readSgrpgrp(hid_t loc_id, char *name, vtkUnstructuredGrid *sgrid, char* groupname);
    virtual int readSSom(hid_t meshId, char *name, vtkUnstructuredGrid *sgrid, char* somname);
    virtual int readUSom(hid_t meshId, char *name, vtkUnstructuredGrid *sgrid, char* somname);

};
#endif /* __VTKvtkAmeletHDFMeshReader_H_  */
