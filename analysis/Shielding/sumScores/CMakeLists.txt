#------------------------------------------------------------------------------
# Module : Shielding
# Package: Shielding_sumScores
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(sumScores)

include(UseGamosAtGeant4)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(sumScores sumScores.cc ${sources} ${headers})
target_link_libraries(sumScores 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
)

install(TARGETS sumScores DESTINATION bin)

