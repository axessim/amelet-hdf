# tools CMakeList.txt file

SET(exec_ameletreader ameletreader)
ADD_EXECUTABLE(${exec_ameletreader} ${exec_ameletreader}.c)
TARGET_LINK_LIBRARIES(${exec_ameletreader} ${tools_lib_dep})

INSTALL(TARGETS ${exec_ameletreader} DESTINATION bin)
