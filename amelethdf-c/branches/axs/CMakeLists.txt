# Top level CMakeList.txt file

cmake_minimum_required (VERSION 2.6 FATAL_ERROR)
ENABLE_TESTING()

PROJECT(AMELETHDF)

MESSAGE( "--------------------------------------------------")
MESSAGE( STATUS "Configuring project AmeketHDF HL")
MESSAGE( STATUS "Host system is ${CMAKE_SYSTEM_NAME}")
MESSAGE( STATUS "Configuration is ${CMAKE_BUILD_TYPE}")
SITE_NAME(HOST_NAME)

OPTION(AMELETHDF_BUILD_SHARED_LIBS "Build shared library" ON)
OPTION(AMELETHDF_BUILD_STATIC_LIBS "Build static library" ON)
OPTION(AMELETHDF_ENABLE_MPI "Enable MPI support" OFF)

#-------------------------------------------------------------
# Found all dependaces.
#-------------------------------------------------------------
SET(AMELETHDF_DEP_INCLUDE_DIRS "")
SET(AMELETHDF_DEP_LINK_LIBS "")
SET(AMELETHDF_DEP_LINK_LIBS_DIRS "")

# HDF5 
FIND_PACKAGE(HDF5 REQUIRED NO_DEFAULT_PATH)
IF (NOT HDF5_FOUND)
  MESSAGE( FATAL_ERROR "HDF5 not found.")
ENDIF ()
# Check that HDF5 has parallel support
IF (HDF5_FOUND)
  IF (AMELETHDF_ENABLE_MPI)
    IF (NOT (HDF5_IS_PARALLEL OR HDF5_ENABLE_PARALLEL))
      MESSAGE(STATUS 
        "HDF5 has been found, but is missing parallel support. It will not be enabled.")
      SET(HDF5_FOUND false)
    ENDIF()
  ENDIF()
ENDIF()
SET(AMELETHDF_DEP_INCLUDE_DIRS ${HDF5_INCLUDE_DIR} ${AMELETHDF_DEP_INCLUDE_DIRS})
SET(AMELETHDF_DEP_LINK_LIBS ${HDF5_LIBRARIES} ${AMELETHDF_DEP_LINK_LIBS})

IF (HDF5_ENABLE_Z_LIB_SUPPORT)
  FIND_PACKAGE(ZLIB)
  SET(AMELETHDF_DEP_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR} ${AMELETHDF_DEP_INCLUDE_DIRS})
  SET(AMELETHDF_DEP_LINK_LIBS ${ZLIB_LIBRARY} ${AMELETHDF_DEP_LINK_LIBS})
ENDIF()

IF (HDF5_ENABLE_SZIP_SUPPORT)
#  FIND_PACKAGE(SZIP)
ENDIF()

# 
SET(AMELETHDF_TOOLS_LINK_LIBS shared_amelethdf_lib ${AMELETHDF_DEP_LINK_LIBS})
SET(AMELETHDF_TESTS_LINK_LIBS shared_amelethdf_lib ${AMELETHDF_DEP_LINK_LIBS})

#-------------------------------------------------------------
# Walk sub dir
#-------------------------------------------------------------
ADD_SUBDIRECTORY(src)
