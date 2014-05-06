

# add a target to generate API documentation with Doxygen
function(add_doxygen DOXYFILE_IN)
  find_package(Doxygen)
  
  if(DOXYGEN_FOUND)
    configure_file(${DOXYFILE_IN}
      ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(docs ALL
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM
      )
  endif(DOXYGEN_FOUND)
endfunction()
