#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: 
#
#------------------------------------------------------------------------------
add_definitions(-D_ALLOC_EXPORT)

add_subdirectory(GamosUtils) 
add_subdirectory(GamosBase/Base) 
add_subdirectory(GamosUserActionMgr) 
add_subdirectory(GamosGeometry)  
add_subdirectory(GamosBase/Classifiers) 
add_subdirectory(GamosAnalysis)  
add_subdirectory(GamosData) 
add_subdirectory(GamosPhysics)  
add_subdirectory(GamosBase/Filters) 
add_subdirectory(GamosGenerator)  
add_subdirectory(GamosMovement)  
add_subdirectory(GamosSD) 
add_subdirectory(GamosUtilsUA)  
add_subdirectory(GamosReadDICOM)  
add_subdirectory(GamosScoring)  
add_subdirectory(GamosRunManager)  
