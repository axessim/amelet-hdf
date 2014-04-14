# amelethdf/src/tests CMakeList.txt file

INCLUDE_DIRECTORIES(${AMELETHDF_INCLUDE_DIRS})

FILE (GLOB tests_src "*.c")
FOREACH (test_file ${tests_src})
  FILE (RELATIVE_PATH rel_test_file ${CMAKE_CURRENT_SOURCE_DIR} ${test_file})
  STRING (REPLACE ".c"  "" test_exec ${rel_test_file})
  SET(test_exec "c_${test_exec}")

  ADD_EXECUTABLE (${test_exec} ${test_file})
  TARGET_LINK_LIBRARIES (${test_exec} ${AMELETHDF_TESTS_LINK_LIBS})

  ADD_TEST (${test_exec} "${EXECUTABLE_OUTPUT_PATH}/${test_exec}")
ENDFOREACH (test_file)

