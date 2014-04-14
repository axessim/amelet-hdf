# amelethdf/src/lib CMakeList.txt file

#-------------------------------------------------------------
# Generat configure file
#-------------------------------------------------------------
string(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "DEBUG" AMELETHDF_DEBUG)
CONFIGURE_FILE(ah5_config.h.cmake ah5_config.h)

SET(AMELETHDF_INCLUDE_DIRS 
  ${CMAKE_CURRENT_BINARY_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${AMELETHDF_DEP_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${AMELETHDF_DEP_INCLUDE_DIRS})

FILE(GLOB_RECURSE amelethdf_src *.c)

IF (AMELETHDF_BUILD_SHARED_LIBS)
  ADD_LIBRARY(shared_amelethdf_lib SHARED ${amelethdf_src})
  SET_TARGET_PROPERTIES(shared_amelethdf_lib PROPERTIES 
    OUTPUT_NAME "amelethdfc")
  
  message("${AMELETHDF_DEP_LINK_LIBS}")
  TARGET_LINK_LIBRARIES(shared_amelethdf_lib ${AMELETHDF_DEP_LINK_LIBS})
  
  INSTALL(TARGETS shared_amelethdf_lib
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin")
ENDIF ()

IF (AMELETHDF_BUILD_STATIC_LIBS)
  ADD_LIBRARY(static_amelethdf_lib STATIC ${amelethdf_src})
  SET_TARGET_PROPERTIES( static_amelethdf_lib  PROPERTIES OUTPUT_NAME "amelethdfc")
  INSTALL(TARGETS static_amelethdf_lib ARCHIVE DESTINATION "lib")
ENDIF ()

