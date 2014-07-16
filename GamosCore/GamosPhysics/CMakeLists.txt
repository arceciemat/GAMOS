#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosPhysics
#
#------------------------------------------------------------------------------
add_definitions(-DGamosPhysics_ALLOC_EXPORT)

add_subdirectory(PhysicsList)
add_subdirectory(OtherPhysicsLists)
add_subdirectory(Cuts)
add_subdirectory(VarianceReduction)
