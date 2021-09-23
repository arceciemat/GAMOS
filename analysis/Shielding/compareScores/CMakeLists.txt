#------------------------------------------------------------------------------
# Module : Shielding
# Package: Shielding_compareScores
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(compareScores)

include(UseGamosAtGeant4)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(compareScores compareScores.cc ${sources} ${headers})
target_link_libraries(compareScores 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
)

install(TARGETS compareScores DESTINATION bin)

