# Top level CMakeList.txt file
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

PROJECT(amelet)
MESSAGE( "--------------------------------------------------" )
MESSAGE( STATUS "Configuring project Amelet-HDF"         )
MESSAGE( STATUS "Host system is ${CMAKE_SYSTEM_NAME}"   )
MESSAGE( STATUS "Configuration is ${CMAKE_BUILD_TYPE}"  )

SET(LIBHDF5_DIR /usr CACHE PATH "Path to HDF5 library")

INCLUDE_DIRECTORIES(${LIBHDF5_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/amelethdf)


find_library(M_LIB m)

FIND_PACKAGE(ZLIB REQUIRED)

IF (${CMAKE_HOST_WIN32})
    SET(amelethdf_lib_dep ${LIBHDF5_DIR}/lib/hdf5_hldll.lib
                          ${LIBHDF5_DIR}/lib/hdf5dll.lib
                          ${ZLIB_LIBRARY})
ELSE (${CMAKE_HOST_UNIX})
    SET(amelethdf_lib_dep ${LIBHDF5_DIR}/lib/libhdf5_hl.a
                          ${LIBHDF5_DIR}/lib/libhdf5.a
                          ${ZLIB_LIBRARY}
                          ${M_LIB})

ENDIF (${CMAKE_HOST_WIN32})

SET(tools_lib_dep amelethdf ${amelethdf_lib_dep})


ADD_SUBDIRECTORY(src)


