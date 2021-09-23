#------------------------------------------------------------------------------
# Module : AnalyseOutput
# Package: AnalyseOutput
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(AnalyseOutput)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
#include_directories(G:/GAMOSwin/external/geant4/geant4.10.05.p01.gamos/source/GAMOS610.seal/analysis/AnalyseOutput/include)
include_directories(${PROJECT_SOURCE_DIR}/include)
 
add_executable(AnalyseOutput AnalyseOutput.cc ${sources} ${headers})

target_link_libraries(AnalyseOutput 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
)

install(TARGETS AnalyseOutput DESTINATION bin)
