# Script run at build time (cmake -P): writes the list of SEAL plugin names
# (plugin.lis, read by the GAMOS version of Geant4, GmEquivalentCommandMgr)
# from the output of SealPluginDump
#   -DDUMP=<SealPluginDump> -DMODULEDIR=<dir of .reg files> -DOUTPUT=<plugin.lis>
set(ENV{SEAL_PLUGINS} "${MODULEDIR}")
execute_process(COMMAND "${DUMP}"
  OUTPUT_VARIABLE _out ERROR_VARIABLE _err RESULT_VARIABLE _rc)
if(NOT _rc EQUAL 0)
  message(FATAL_ERROR "SealPluginDump failed (${_rc}):\n${_err}")
endif()
string(REPLACE "\n" ";" _lines "${_out}")
set(_list "")
foreach(_l ${_lines})
  if(_l MATCHES "^  [^ *]")
    string(APPEND _list "${_l}\n")
  endif()
endforeach()
if(_list STREQUAL "")
  message(FATAL_ERROR "SealPluginDump did not list any plugin:\n${_out}\n${_err}")
endif()
file(WRITE "${OUTPUT}" "${_list}")
