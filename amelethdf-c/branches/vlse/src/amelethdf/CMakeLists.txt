# Amelet-hdf CMakeList.txt file
	
FILE(GLOB amelethdf_src *.c)

IF (AH5_BUILD_SHARED_LIBS)
  ADD_LIBRARY(amelethdf SHARED ${amelethdf_src})
  SET_TARGET_PROPERTIES( amelethdf  PROPERTIES OUTPUT_NAME "amelethdfc")
  INSTALL(TARGETS amelethdf LIBRARY DESTINATION "lib"
                            ARCHIVE DESTINATION "lib"
                            RUNTIME DESTINATION "bin")

ELSE (AH5_BUILD_SHARED_LIBS)
  ADD_LIBRARY(amelethdf STATIC ${amelethdf_src})
  SET_TARGET_PROPERTIES( amelethdf  PROPERTIES OUTPUT_NAME "amelethdfc")
  INSTALL(TARGETS amelethdf ARCHIVE DESTINATION "lib")
ENDIF (AH5_BUILD_SHARED_LIBS)  

FILE(GLOB amelethdf_include ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
INSTALL(FILES ${amelethdf_include} DESTINATION include)

