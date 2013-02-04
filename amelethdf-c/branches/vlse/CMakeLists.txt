# Top level CMakeList.txt file
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

PROJECT(amelet)
MESSAGE("--------------------------------------------------")
MESSAGE(STATUS "Configuring project Amelet-HDF")
MESSAGE(STATUS "Host system is ${CMAKE_SYSTEM_NAME}")
MESSAGE(STATUS "Configuration is ${CMAKE_BUILD_TYPE}")

#-------------------------------------------------------------
# Some options of MATHMESH libs and tools.
#-------------------------------------------------------------
OPTION(AH5_BUILD_SHARED_LIBS
  "Build shared libraries" OFF)
OPTION(AH5_BUILD_TESTING
  "Build AH5 unit testing (boost utest needed!)" OFF)

#-------------------------------------------------------------
# Manager tests
#-------------------------------------------------------------
IF (AH5_BUILD_TESTING)
  ENABLE_TESTING()
ENDIF ()

#-------------------------------------------------------------
# Found all dependaces.
#-------------------------------------------------------------
# HDF5
FIND_PACKAGE(HDF5 REQUIRED NO_DEFAULT_PATH) # HDF5 managed these deps
IF (NOT HDF5_FOUND)
  MESSAGE(FATAL_ERROR "HDF5 not found.")
ELSE ()
  IF (HDF5_IS_PARALLEL)
    FIND_PACKAGE(MPI REQUIRED)
    LINK_DIRECTORIES(${MPI_C_LIBRARIES})
    INCLUDE_DIRECTORIES(${MPI_C_INCLUDE_PATH})
  ENDIF ()
ENDIF ()
INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIR})

# Set include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/amelethdf)

# Define dep for libs, tools and tests.
SET(amelethdf_lib_dep 
  hdf5 hdf5_hl         # HDF5 lib
  )
SET(tools_lib_dep 
  amelethdf            # AH5 lib
  ${amelethdf_lib_dep} # AH5 deps
  )
SET(tests_dep 
  ${tools_lib_dep}     # AH5 lib and deps
  ${Boost_LIBRARIES}   # Boost test framework
  )

#-------------------------------------------------------------
# Walk sub dir
#-------------------------------------------------------------
ADD_SUBDIRECTORY(src)


