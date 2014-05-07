# amelethdf/hl/src/CMakeList.txt file

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tools)
IF (UNIX)
  ADD_SUBDIRECTORY(test)
ENDIF (UNIX)
