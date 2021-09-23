#------------------------------------------------------------------------------
# Module : Detector
# Package: Detector_PET_ssrb_fbp
#
#------------------------------------------------------------------------------
# 
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(${CMAKE_SOURCE_DIR}/source)
# Define the GAMOS Module.
include(UseGamosAtGeant4)
include(UseFFTW)
#
GEANT4_DEFINE_MODULE(NAME ssrb_fbp
	 HEADERS
		VIP_lm2pd.hh
		VIP_ssrb_fbp.hh
		ssrb_fbp.hh
	 SOURCES
		VIP_lm2pd.cxx
		dummy.cc
		VIP_ssrb_fbp.cxx
		extra_TXT2FBP_outfile.cxx
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 
	fftw3
	m

	LINK_LIBRARIES
)
