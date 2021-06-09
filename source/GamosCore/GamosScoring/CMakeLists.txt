#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosScoring
#
#------------------------------------------------------------------------------
add_definitions(-DGamosScoring_ALLOC_EXPORT)

add_subdirectory(Management)
add_subdirectory(Scorers)
add_subdirectory(Printers)
add_subdirectory(PointDetector)
