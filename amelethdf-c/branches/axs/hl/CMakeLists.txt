# amelethdf/hl/src/CMakeList.txt file

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tools)
IF (UNIX)
  ADD_SUBDIRECTORY(test)
ELSE (UNIX)
  MESSAGE("FIXME: HL tests not build for this platform!")
ENDIF (UNIX)
