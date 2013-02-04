# tests CMakeList.txt file

#
# A function for build/add test by extenstion.
#
FUNCTION (ADD_TESTS ext)
  FILE (GLOB tests_src "*${ext}")
  FOREACH (test_file ${tests_src})
    FILE (RELATIVE_PATH rel_test_file ${CMAKE_CURRENT_SOURCE_DIR} ${test_file})
    STRING (REPLACE "${ext}"  "" test_exec_name ${rel_test_file})
    SET(test_exec "ah5_test_${test_exec_name}")
    
    ADD_EXECUTABLE (${test_exec} ${test_file})
    TARGET_LINK_LIBRARIES (${test_exec} ${tests_dep})
    INSTALL(TARGETS ${test_exec} DESTINATION test)
    ADD_TEST ("${test_exec}" "${EXECUTABLE_OUTPUT_PATH}/${test_exec}")
  ENDFOREACH (test_file)
ENDFUNCTION ()
#
# A all file with ext ".cpp" as tests.
#
IF (AH5_BUILD_TESTING)
  ADD_TESTS(".c")
ENDIF ()
