# Top level CMakeList.txt file
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

PROJECT(amelet)
MESSAGE( "--------------------------------------------------" )
MESSAGE( STATUS "Configuring project Amelet-HDF"         )
MESSAGE( STATUS "Host system is ${CMAKE_SYSTEM_NAME}"   )
MESSAGE( STATUS "Configuration is ${CMAKE_BUILD_TYPE}"  )

#SET(LIBHDF5_DIR /usr CACHE PATH "Path to HDF5 library")

#INCLUDE_DIRECTORIES(
#${LIBHDF5_DIR}/include
#                    ${CMAKE_CURRENT_SOURCE_DIR}/src/amelethdf)


find_library(M_LIB m)

FIND_PACKAGE(ZLIB REQUIRED)
FIND_PACKAGE(HDF5 REQUIRED)
INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/amelethdf)
get_filename_component (C_COMPILER_NAME ${CMAKE_C_COMPILER} NAME)
MESSAGE(C COMPILER = "${C_COMPILER_NAME}")
IF (${CMAKE_HOST_WIN32})
    IF (C_COMPILER_NAME STREQUAL "gcc.exe")
        MESSAGE(GCC)
        SET(amelethdf_lib_dep ${LIBHDF5_DIR}/lib/libhdf5_hl.a
                              ${LIBHDF5_DIR}/lib/libhdf5.a
                              ${ZLIB_LIBRARY})
    ELSE (C_COMPILER_NAME STREQUAL "gcc.exe")
        MESSAGE(NOT GCC)
        SET(amelethdf_lib_dep ${LIBHDF5_DIR}/lib/hdf5_hl.lib
                              ${LIBHDF5_DIR}/lib/hdf5.lib
                              ${ZLIB_LIBRARY})
    ENDIF (C_COMPILER_NAME STREQUAL "gcc.exe")
   
ELSE (${CMAKE_HOST_UNIX})
    SET(amelethdf_lib_dep 
#${LIBHDF5_DIR}/lib/libhdf5_hl.a
#${LIBHDF5_DIR}/lib/libhdf5.a
                          ${HDF5_LIBRARY}
                          ${ZLIB_LIBRARY}
                          ${M_LIB}
                          hdf5_hl
                          hdf5)

ENDIF (${CMAKE_HOST_WIN32})

SET(tools_lib_dep amelethdf ${amelethdf_lib_dep})


ADD_SUBDIRECTORY(src)


