#.rst:
# GamosMacros
# -----------
#
# Functions to define the GAMOS libraries and executables, following the
# structure of the Geant4 build (geant4_add_module/geant4_module_link_libraries).
#
# Each GAMOS module (a directory with include/ and src/) has a CMakeLists.txt
# that includes its sources.cmake, where the module is defined with:
#
#   gamos_add_module(<name>
#     PUBLIC_HEADERS  <hdr1> ...   # relative to the HEADER_DIR (default: include)
#     PRIVATE_HEADERS <hdr1> ...   # relative to src
#     SOURCES         <src1> ...   # relative to src
#     [HEADER_DIR <dir>]           # directory of the public headers (default: include)
#     [HEADER_INSTALL_DIR <dir>]   # where they are installed, under include/GAMOS
#                                  # (default: <module path>/<HEADER_DIR>, so that
#                                  #  #include "GamosCore/GamosUtils/include/X.hh" works)
#     [NO_GEANT4]                  # do not link with Geant4 (SEAL libraries)
#     [NO_PLUGIN])                 # do not register the library as a SEAL plugin
#
#   gamos_module_link_libraries(<name> [PUBLIC|PRIVATE|INTERFACE] <lib1> ...)
#
#   gamos_add_executable(<name>
#     SOURCES <main.cc> ...        # relative to the module directory
#     [LINK_LIBRARIES <lib1> ...]
#     [NO_AS_NEEDED])              # keep all LINK_LIBRARIES as dependencies of the
#                                  # program, even if it does not use them directly
#
# Each module builds the shared library lib<name>.so, with the same name as
# in the GNUmake build, and registers it as a SEAL plugin (file <name>.reg in
# the module directory, see GAMOS_MODULE_DIR), as the GNUmake build does.

include_guard(GLOBAL)

#-----------------------------------------------------------------------------
# Include directories common to all GAMOS targets (as $(GAMOSINSTALL)/source,
# $(GAMOSINSTALL)/analysis and the SEAL directories of the GNUmake build)
set(GAMOS_BUILD_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/source
  ${PROJECT_SOURCE_DIR}/analysis
  ${PROJECT_SOURCE_DIR}/source/SEAL_Foundation/PluginManager
  ${PROJECT_SOURCE_DIR}/source/SEAL_Foundation/SealPlatform
  ${PROJECT_SOURCE_DIR}/source/SEAL_Foundation/SealBase)

#-----------------------------------------------------------------------------
# Target of a GAMOS module: normally its name, but <name>-lib if an imported
# target with that name already exists (e.g. DCMTK exports a program "dcmdump")
function(__gamos_target _name _var)
  get_property(_t GLOBAL PROPERTY GAMOS_TARGET_${_name})
  if(_t)
    set(${_var} ${_t} PARENT_SCOPE)
  else()
    set(${_var} ${_name} PARENT_SCOPE)
  endif()
endfunction()

# Translates a list of module names (and other libraries) into targets
function(__gamos_targets _var)
  set(_out)
  foreach(_n ${ARGN})
    __gamos_target(${_n} _t)
    list(APPEND _out ${_t})
  endforeach()
  set(${_var} ${_out} PARENT_SCOPE)
endfunction()

#-----------------------------------------------------------------------------
# Path of the current module relative to source/ or analysis/
function(__gamos_module_path _var)
  file(RELATIVE_PATH _rel "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
  string(REGEX REPLACE "^(source|analysis)/?" "" _rel "${_rel}")
  set(${_var} "${_rel}" PARENT_SCOPE)
endfunction()

#-----------------------------------------------------------------------------
# Common settings of all GAMOS targets
function(__gamos_target_settings _target)
  target_include_directories(${_target}
    PUBLIC
      "$<BUILD_INTERFACE:${GAMOS_BUILD_INCLUDE_DIRS}>"
      "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/GAMOS>")
  # Headers of the module included without path (-I./include in GNUmake)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(${_target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
  endif()
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
    target_include_directories(${_target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
  endif()
  target_compile_definitions(${_target} PUBLIC ${GAMOS_COMPILE_DEFINITIONS})
  target_compile_options(${_target} PRIVATE ${GAMOS_CXX_WARNING_FLAGS})
  target_compile_features(${_target} PUBLIC cxx_std_${GAMOS_CXX_STANDARD})
endfunction()

#-----------------------------------------------------------------------------
# gamos_install_headers(DIRECTORY <dir> DESTINATION <dir under include/GAMOS> FILES <f1> ...)
# Installs headers that keep their subdirectories (e.g. sysapi/X.h)
function(gamos_install_headers)
  cmake_parse_arguments(_GIH "" "DIRECTORY;DESTINATION" "FILES" ${ARGN})
  foreach(_hdr ${_GIH_FILES})
    get_filename_component(_sub "${_hdr}" DIRECTORY)
    install(FILES "${_GIH_DIRECTORY}/${_hdr}"
      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/GAMOS/${_GIH_DESTINATION}/${_sub}"
      COMPONENT Development)
  endforeach()
endfunction()

#-----------------------------------------------------------------------------
# Registration of a library as a SEAL plugin: <name>.reg with
#   module <full path of the library>
# in the module directory of the build tree and of the install tree
function(__gamos_register_plugin _name _tgt)
  file(GENERATE
    OUTPUT "${GAMOS_BUILD_MODULE_DIR}/${_name}.reg"
    CONTENT "module $<TARGET_FILE:${_tgt}>\n")
  install(CODE "
    set(_moddir \"\$ENV{DESTDIR}${GAMOS_INSTALL_FULL_MODULEDIR}\")
    file(MAKE_DIRECTORY \"\${_moddir}\")
    file(WRITE \"\${_moddir}/${_name}.reg\"
      \"module $<IF:$<PLATFORM_ID:Windows>,${CMAKE_INSTALL_FULL_BINDIR},${CMAKE_INSTALL_FULL_LIBDIR}>/$<TARGET_FILE_NAME:${_tgt}>\\n\")
    message(STATUS \"Registered SEAL plugin: \${_moddir}/${_name}.reg\")"
    COMPONENT Runtime)
endfunction()

#-----------------------------------------------------------------------------
function(gamos_add_module _name)
  cmake_parse_arguments(_GAM
    "NO_GEANT4;NO_PLUGIN"
    "HEADER_DIR;HEADER_INSTALL_DIR"
    "PUBLIC_HEADERS;PRIVATE_HEADERS;SOURCES"
    ${ARGN})
  if(_GAM_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "gamos_add_module(${_name}): unknown arguments '${_GAM_UNPARSED_ARGUMENTS}'")
  endif()
  if(NOT _GAM_HEADER_DIR)
    set(_GAM_HEADER_DIR include)
  endif()
  if(NOT _GAM_HEADER_INSTALL_DIR)
    __gamos_module_path(_modpath)
    if(_modpath)
      set(_GAM_HEADER_INSTALL_DIR "${_modpath}/${_GAM_HEADER_DIR}")
    else()
      set(_GAM_HEADER_INSTALL_DIR "${_GAM_HEADER_DIR}")
    endif()
  endif()

  set(_pub)
  foreach(_h ${_GAM_PUBLIC_HEADERS})
    list(APPEND _pub "${CMAKE_CURRENT_SOURCE_DIR}/${_GAM_HEADER_DIR}/${_h}")
  endforeach()
  set(_priv)
  foreach(_h ${_GAM_PRIVATE_HEADERS})
    list(APPEND _priv "${CMAKE_CURRENT_SOURCE_DIR}/src/${_h}")
  endforeach()
  set(_src)
  foreach(_s ${_GAM_SOURCES})
    list(APPEND _src "${CMAKE_CURRENT_SOURCE_DIR}/src/${_s}")
  endforeach()
  foreach(_f ${_pub} ${_priv} ${_src})
    if(NOT EXISTS "${_f}")
      message(FATAL_ERROR "gamos_add_module(${_name}): file '${_f}' does not exist")
    endif()
  endforeach()

  set(_tgt ${_name})
  if(TARGET ${_name})
    get_target_property(_imported ${_name} IMPORTED)
    if(NOT _imported)
      message(FATAL_ERROR "gamos_add_module(${_name}): module defined twice")
    endif()
    set(_tgt ${_name}-lib)
    set_property(GLOBAL PROPERTY GAMOS_TARGET_${_name} ${_tgt})
  endif()
  add_library(${_tgt} SHARED ${_src} ${_pub} ${_priv})
  add_library(GAMOS::${_name} ALIAS ${_tgt})
  __gamos_target_settings(${_tgt})
  if(NOT _GAM_NO_GEANT4)
    target_link_libraries(${_tgt} PUBLIC ${Geant4_LIBRARIES})
    if(GAMOS_ROOT)
      target_link_libraries(${_tgt} PUBLIC ${GAMOS_ROOT_LIBRARIES})
    endif()
  endif()
  # Windows: the DLL (RUNTIME) goes to bin, with the programs, and the import library (ARCHIVE) to lib;
  # "$<0:>" avoids the per-configuration subdirectories of the multi-configuration generators (Visual Studio)
  set_target_properties(${_tgt} PROPERTIES
    OUTPUT_NAME ${_name}
    EXPORT_NAME ${_name}
    LIBRARY_OUTPUT_DIRECTORY "${GAMOS_BUILD_LIBDIR}$<0:>"
    RUNTIME_OUTPUT_DIRECTORY "${GAMOS_BUILD_BINDIR}$<0:>"
    ARCHIVE_OUTPUT_DIRECTORY "${GAMOS_BUILD_LIBDIR}$<0:>")

  install(TARGETS ${_tgt}
    EXPORT GAMOSLibraryDepends
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)
  gamos_install_headers(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${_GAM_HEADER_DIR}"
    DESTINATION "${_GAM_HEADER_INSTALL_DIR}"
    FILES ${_GAM_PUBLIC_HEADERS})

  if(NOT _GAM_NO_PLUGIN)
    __gamos_register_plugin(${_name} ${_tgt})
  endif()
  set_property(GLOBAL APPEND PROPERTY GAMOS_LIBRARIES ${_name})
endfunction()

#-----------------------------------------------------------------------------
function(gamos_module_link_libraries _name)
  __gamos_target(${_name} _tgt)
  __gamos_targets(_libs ${ARGN})
  target_link_libraries(${_tgt} ${_libs})
endfunction()

#-----------------------------------------------------------------------------
# The executable target is <name>, or <name>-exe if there is a library <name>
# (as in GNUmake, where lib<name>.so is built from src/ and the program <name>
# from <name>.cc); the program file is always <name>.
function(gamos_add_executable _name)
  cmake_parse_arguments(_GAE "NO_AS_NEEDED" "" "SOURCES;LINK_LIBRARIES" ${ARGN})
  set(_target ${_name})
  if(TARGET ${_name})
    set(_target ${_name}-exe)
  endif()
  set(_src)
  foreach(_s ${_GAE_SOURCES})
    list(APPEND _src "${CMAKE_CURRENT_SOURCE_DIR}/${_s}")
  endforeach()
  add_executable(${_target} ${_src})
  __gamos_target_settings(${_target})
  __gamos_targets(_libs ${_GAE_LINK_LIBRARIES})
  target_link_libraries(${_target} PRIVATE ${_libs} ${Geant4_LIBRARIES})
  if(_GAE_NO_AS_NEEDED AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_options(${_target} PRIVATE "LINKER:--no-as-needed")
  endif()
  if(GAMOS_ROOT)
    target_link_libraries(${_target} PRIVATE ${GAMOS_ROOT_LIBRARIES})
  endif()
  set_target_properties(${_target} PROPERTIES
    OUTPUT_NAME ${_name}
    RUNTIME_OUTPUT_DIRECTORY "${GAMOS_BUILD_BINDIR}$<0:>")
  install(TARGETS ${_target}
    EXPORT GAMOSLibraryDepends
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime)
  set_property(GLOBAL APPEND PROPERTY GAMOS_EXECUTABLES ${_target})
endfunction()
