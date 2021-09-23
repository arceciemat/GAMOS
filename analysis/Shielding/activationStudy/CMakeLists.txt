#------------------------------------------------------------------------------
# Module : Shielding
# Package: Shielding_activationStudy
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(activationStudy)

include(UseGamosAtGeant4)

include_directories(${PROJECT_SOURCE_DIR}/include/)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(activationStudy activationStudy.cc ${sources} ${headers})
target_link_libraries(activationStudy 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
Shielding
)

install(TARGETS activationStudy DESTINATION bin)

