Instruction to build the fortran library AmeletHDf with ifort v11.1

# Introduction #

To build amelethdf fortran with ifort v11.1, you need:
  * Microsoft Visual Studio 2008.,
  * amelethdf-fortran sources
  * HDF5 version 1.8.6 library (http://www.hdfgroup.org/ftp/HDF5/current/bin/windows/hdf5-1.8.6-win64-cmake-shared.zip)

# Build library amelethdf-fortran #

Create a static library project in Visual Studio 2008:
  * File/New/Project
  * select static library
  * give the name amelethdf

Import fortran amelethdf library sources files. There are in src/amelethdf:
  * Do a right clic on the Source Files present in the Solution Explorer and choose the menu Add Existing Item, and select all f90 files present in src/amelethdf
  * Open the configuration manager and select x64 for Active Solution platfom

Add hdf5 include directories:
  * Go to the menu Project/Properties
  * Add these two includes path in Fortra/General/"Additionnal include directories":
    * C:\.....\HDF5\_1.8.6\include
    * C:\.....\HDF5\_1.8.6\include\fortran

Now you can make build. The amelethdf.lib library file is created.

You can use this file amelehdf.lib to build your fortran application. When you build your wrapper don't forget to add :
  * these include directories :
    * C:\....\HDF5\_1.8.6\include
    * C:\....\HDF5\_1.8.6\include\fortran
    * C:\....\amelethdf\x64\Debug
  * these libraries to the linker options (configuration properties/linker/input):
    * zlib1.lib
    * szip.lib
    * hdf5dll.lib
    * hdf5\_hldll.lib
    * hdf5\_fortrandll.lib
    * hdf5\_hl\_fortrandll.lib
    * amelethdf.lib
  * and paths until these lib in configuration properties/linker/general/Additionnal Library directories

