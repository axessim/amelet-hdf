# amelethdf/src/tools CMakeList.txt file

INCLUDE_DIRECTORIES(${AMELETHDF_INCLUDE_DIRS})

# build fortran tools
FILE(GLOB tools_src *.c)
FOREACH(tool_file ${tools_src})
    FILE(RELATIVE_PATH rel_tool_file ${CMAKE_CURRENT_SOURCE_DIR} ${tool_file})
    STRING(REPLACE ".c"  "" tool_exec ${rel_tool_file})
    ADD_EXECUTABLE(${tool_exec} ${tool_file})
    
    TARGET_LINK_LIBRARIES(${tool_exec} ${AMELETHDF_HL_TOOLS_LINK_LIBS})
    INSTALL(TARGETS ${tool_exec} DESTINATION bin)
ENDFOREACH(tool_file)
