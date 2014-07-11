#ifndef __VTKvtkAmeletHDFReader_H_
#define __VTKvtkAmeletHDFReader_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <hdf5.h>
//VTK Includes
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationObjectBaseKey.h"
#include "vtkInformationVector.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkSmartPointer.h"
#include "vtkInformationIntegerKey.h"
#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridAlgorithm.h"
#include "vtkIdList.h"
#include "vtkFloatArray.h"
#include "vtkCellData.h"
#include "vtkMultiBlockDataSetAlgorithm.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "ahdfreaders/vtkAmeletHDFMeshReader.h"
#include "ahdfreaders/vtkAmeletHDFDataReader.h"
#include <sys/stat.h>
#include <vtkstd/vector>
#include <algorithm>
#include <vtkTable.h>

/**
* @class vtkAmeletHDFReader vtkAmeletHDFReader.h PathToHeader/vtkAmeletHDFReader.h
* @brief
* @author
* @date
* @version
*/
class vtkInformationIntegerKey;
class vtkInformationObjectBaseKey;

class VTK_EXPORT vtkAmeletHDFReader : public vtkMultiBlockDataSetAlgorithm
{
  public:
      static vtkAmeletHDFReader *New();
      vtkTypeRevisionMacro(vtkAmeletHDFReader, vtkMultiBlockDataSetAlgorithm);
      void PrintSelf(ostream& os, vtkIndent indent);

      vtkSetStringMacro( FileName );
      vtkGetStringMacro( FileName );

      int CanReadFile(const char *filename);

      // Description:
      // All the data sets stored in the multiblock output share the same point
      // data.  For convenience, the point coordinates (vtkPoints) and point data
      // (vtkPointData) are saved under these keys in the vtkInformation of the
      // output data set.
      static vtkInformationObjectBaseKey *POINTS();
      static vtkInformationObjectBaseKey *POINT_DATA();
      static vtkInformationIntegerKey *IS_UMESH();
      static vtkInformationIntegerKey *IS_SMESH();
      static vtkInformationIntegerKey *IS_DATAONMESH();
      static vtkInformationIntegerKey *IS_DATA();

      int ReadDataOnMesh(hid_t file_id, vtkMultiBlockDataSet *output);
      // Description:
      // Which TimeStep to read.
      vtkSetMacro(TimeStep, int);
      vtkGetMacro(TimeStep, int);

      // Description:
      // Returns the number of timesteps.
      vtkGetMacro(NumberOfTimeSteps, int);

      // Description:
      // Which TimeStepRange to read
      vtkGetVector2Macro(TimeStepRange, int);
      vtkSetVector2Macro(TimeStepRange, int);

  protected:
    vtkAmeletHDFReader();
    ~vtkAmeletHDFReader();

    char* FileName;
    bool TimeStepMode;


    int TimeStep;
    int ActualTimeStep;
    int NumberOfTimeSteps;
    int TimeStepRange[2];


    virtual int FillInputPortInformation(int, vtkInformation *info);

    /**
     * @brief Standard vtk 5.x pipeline method. This method is used to get information about the data set.
     */
    int RequestInformation( vtkInformation*, vtkInformationVector**, vtkInformationVector* );

    /**
     * @brief Standard Method to over ride in the VTK 5.x pipeline
     */
    int RequestData( vtkInformation*, vtkInformationVector**, vtkInformationVector* );



  private:
    vtkAmeletHDFReader(const vtkAmeletHDFReader&); // Not implemented.
    void operator=(const vtkAmeletHDFReader&); // Not implemented.
//BTX
      // Hide this variable declaration from bin/vtkWrapClientServer. If omitted, we'd get a
      // *** SYNTAX ERROR found in parsing the header file vtkFooReader.h ***
    vtkstd::vector<double> TimeStepValues;
//ETX
};
#endif /* __VTKvtkAmeletHDFReader_H_  */

