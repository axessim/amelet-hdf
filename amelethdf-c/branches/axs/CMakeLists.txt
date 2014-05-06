# Top level CMakeList.txt file

cmake_minimum_required (VERSION 2.6 FATAL_ERROR)
ENABLE_TESTING()

PROJECT(AMELETHDF)

MESSAGE( "--------------------------------------------------")
MESSAGE( STATUS "Configuring project AmeketHDF HL")
MESSAGE( STATUS "Host system is ${CMAKE_SYSTEM_NAME}")
MESSAGE( STATUS "Configuration is ${CMAKE_BUILD_TYPE}")
SITE_NAME(HOST_NAME)

OPTION(AMELETHDF_ENABLE_MPI "Enable MPI support" OFF)
OPTION(AMELETHDF_BUILD_DOCS "Build Amelet-HDF docs" ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/resources/cmake") # For custom cmake modules
include(DoxygenTarget)

#-------------------------------------------------------------
# Configu install-tree
#-------------------------------------------------------------
# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
IF (WIN32)
  set(INSTALL_ARCHIVE_DIR lib/static CACHE PATH "Installation directory for libraries")
ELSE ()
  set(INSTALL_ARCHIVE_DIR lib CACHE PATH "Installation directory for libraries")
ENDIF ()
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
set(INSTALL_DATA_DIR share CACHE PATH
  "Installation directory for data files")

#-------------------------------------------------------------
# Found all dependaces.
#-------------------------------------------------------------
# This set of var define the Amelet-HDF library and hight level library
# dependance. Used acumulat style "set(VAR NEW_VALUE ${VAR})"
SET(AMELETHDF_DEP_INCLUDE_DIRS "")
SET(AMELETHDF_DEP_LINK_LIBS "")
SET(AMELETHDF_DEP_LINK_LIBS_DIRS "")
SET(AMELETHDF_HL_DEP_INCLUDE_DIRS "")
SET(AMELETHDF_HL_DEP_LINK_LIBS "")
SET(AMELETHDF_HL_DEP_LINK_LIBS_DIRS "")

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

#-------------------------------------------------------------
# Configure compilateur
#-------------------------------------------------------------
set(GCC_COMPILE_FLAGS          "-Wall -W -Wextra -std=gnu90")
set(GCC_DEBUG_COMPILE_FLAGS    "-g3 -D_DEBUG")
set(GCC_RELEASE_COMPILE_FLAGS  "-O3 -g0 -DNDEBUG")

set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
set(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

set(CMAKE_C_FLAGS         "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_DEBUG_COMPILE_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${GCC_RELEASE_COMPILE_FLAGS}")
# SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_LINK_FLAGS}" )

#-------------------------------------------------------------
# Generat configure file
#-------------------------------------------------------------
string(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "DEBUG" AMELETHDF_DEBUG)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/src/amelethdf/ah5_config.h.cmake 
  ${CMAKE_CURRENT_BINARY_DIR}/src/amelethdf/ah5_config.h)

# Define the Amelet-HDF tools and test dependance.
SET(AMELETHDF_TOOLS_LINK_LIBS amelethdfc ${AMELETHDF_DEP_LINK_LIBS})
SET(AMELETHDF_TESTS_LINK_LIBS amelethdfc ${AMELETHDF_DEP_LINK_LIBS})
# 
SET(AMELETHDF_INCLUDE_DIRS 
  ${CMAKE_CURRENT_BINARY_DIR}/src/amelethdf
  ${CMAKE_CURRENT_SOURCE_DIR}/src/amelethdf
  ${AMELETHDF_DEP_INCLUDE_DIRS})

# finalised hight level library depenance
SET(AMELETHDF_HL_DEP_INCLUDE_DIRS 
  ${AMELETHDF_INCLUDE_DIRS}
  ${AMELETHDF_HL_DEP_INCLUDE_DIRS})
SET(AMELETHDF_HL_DEP_LINK_LIBS 
  amelethdfc 
  ${AMELETHDF_DEP_LINK_LIBS} 
  ${AMELETHDF_HL_DEP_LINK_LIBS})
SET(AMELETHDF_HL_DEP_LINK_LIBS_DIRS 
  ${AMELETHDF_DEP_LINK_LIBS_DIRS} 
  ${AMELETHDF_HL_DEP_LINK_LIBS_DIRS})
# 
SET(AMELETHDF_HL_INCLUDE_DIRS 
  ${CMAKE_CURRENT_SOURCE_DIR}/hl/src
  ${AMELETHDF_HL_DEP_INCLUDE_DIRS})

SET(AMELETHDF_HL_TOOLS_LINK_LIBS amelethdfc_hl ${AMELETHDF_HL_DEP_LINK_LIBS})
SET(AMELETHDF_HL_TESTS_LINK_LIBS amelethdfc_hl ${AMELETHDF_HL_DEP_LINK_LIBS})

#-------------------------------------------------------------
# 
#-------------------------------------------------------------
IF (AMELETHDF_BUILD_DOCS)
  SET(DOXY_SOURCES_DIRS 
    "${PROJECT_SOURCE_DIR}/src/amelethdf ${PROJECT_SOURCE_DIR}/hl/src")
  SET(DOXY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/docs)
  add_doxygen(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
  INSTALL(DIRECTORY ${PROJECT_BINARY_DIR}/docs/html 
    DESTINATION ${INSTALL_DATA_DIR}/amelethdf)
  INSTALL(DIRECTORY ${PROJECT_BINARY_DIR}/docs/latex 
    DESTINATION ${INSTALL_DATA_DIR}/amelethdf)
ENDIF (AMELETHDF_BUILD_DOCS)

#-------------------------------------------------------------
# Walk sub dir
#-------------------------------------------------------------
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(hl)

#-------------------------------------------------------------
# create a AmeletHDFConfig.cmake file and co.
#-------------------------------------------------------------
## CMAKE CONFIGURE FILE into build-tree
set(AMELETHDF_CMAKE_DIR ${PROJECT_BINARY_DIR})
set(AMELETHDF_CMAKE_DIR_ABS ${AMELETHDF_CMAKE_DIR})                # The absolute path
set(AMELETHDF_EXPORT_INCLUDE_DIRS ${AMELETHDF_HL_INCLUDE_DIRS})
set(AMELETHDF_EXPORT_LIBRARY_DIRS 
  ${PROJECT_BINARY_DIR}/src/amelethdf
  ${PROJECT_BINARY_DIR}/hl/src)
set(AMELETHDF_EXPORT_LIBRARIES amelethdfc_hl amelethdfc hdf5_hl hdf5)

# Add all targets to the build-tree export set
export(TARGETS amelethdfc amelethdfc_hl 
  FILE "${AMELETHDF_CMAKE_DIR}/AmeletHDFLibraryDepends.cmake")
# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE AmeletHDF)
# Create a AxesSignConfig.cmake file for the use from the build tree
configure_file(resources/cmake/AmeletHDFConfig.cmake.in
  "${AMELETHDF_CMAKE_DIR}/AmeletHDFConfig.cmake" @ONLY)
configure_file(resources/cmake/AmeletHDFConfigVersion.cmake.in
  "${AMELETHDF_CMAKE_DIR}/AmeletHDFConfigVersion.cmake" @ONLY)

## CMAKE CONFIGURE FILE into install-tree
set(AMELETHDF_INCLUDE_DIRS "${INSTALL_INCLUDE_DIR}")
set(AMELETHDF_CMAKE_DIR "${INSTALL_INCLUDE_DIR}/cmake/amelethdf")
set(AMELETHDF_CMAKE_DIR_ABS "${CMAKE_INSTALL_PREFIX}/${AMELETHDF_CMAKE_DIR}")
set(AMELETHDF_EXPORT_LIBRARY_DIRS "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}")
# Install the export set for use with the install-tree
install(EXPORT AmeletHDFLibraryDepends
  DESTINATION "${AMELETHDF_CMAKE_DIR}"
  COMPONENT dev)
install(EXPORT AmeletHDFHLLibraryDepends
  DESTINATION "${AMELETHDF_CMAKE_DIR}"
  COMPONENT dev)
# Create a AxesSignConfig.cmake file for the use from the install tree
# and install it
configure_file(resources/cmake/AmeletHDFConfig.cmake.in
  "${PROJECT_BINARY_DIR}/InstallFiles/AmeletHDFConfig.cmake" @ONLY)
configure_file(resources/cmake/AmeletHDFConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/InstallFiles/AmeletHDFConfigVersion.cmake" @ONLY)
install(FILES
  "${PROJECT_BINARY_DIR}/InstallFiles/AmeletHDFConfig.cmake"
  "${PROJECT_BINARY_DIR}/InstallFiles/AmeletHDFConfigVersion.cmake"
  DESTINATION "${AMELETHDF_CMAKE_DIR}" 
  COMPONENT dev)
