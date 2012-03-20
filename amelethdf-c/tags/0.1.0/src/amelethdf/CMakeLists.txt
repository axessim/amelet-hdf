# Amelet-hdf CMakeList.txt file
	
file(GLOB amelethdf_src *.c)

ADD_LIBRARY(amelethdf_so SHARED ${amelethdf_src})
SET_TARGET_PROPERTIES( amelethdf_so  PROPERTIES OUTPUT_NAME "amelethdfc")
TARGET_LINK_LIBRARIES(amelethdf_so ${amelethdf_lib_dep})
INSTALL(TARGETS amelethdf_so LIBRARY DESTINATION "lib"
                             ARCHIVE DESTINATION "lib"
                             RUNTIME DESTINATION "bin")

ADD_LIBRARY(amelethdf STATIC ${amelethdf_src})
SET_TARGET_PROPERTIES( amelethdf  PROPERTIES OUTPUT_NAME "amelethdfc")
INSTALL(TARGETS amelethdf ARCHIVE DESTINATION "lib")

file(GLOB amelethdf_include ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
INSTALL(FILES ${amelethdf_include} DESTINATION include)

