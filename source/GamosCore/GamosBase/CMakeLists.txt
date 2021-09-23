#------------------------------------------------------------------------------
# Module : GamosBase
# Package: GamosCore_GamosBase
#
#------------------------------------------------------------------------------
# 
add_subdirectory(Base)
add_subdirectory(Classifiers)
add_subdirectory(Filters)
if(NOT GEANT4_BUILD_GRANULAR_LIBS)
	GEANT4_GLOBAL_LIBRARY_TARGET(NAME GmBase
		COMPONENTS
			Base/sources.cmake
			Classifiers/sources.cmake
			Filters/sources.cmake
)
endif()
