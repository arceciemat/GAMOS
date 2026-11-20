# - SOE_ImageRecon module build definition

gamos_add_module(SOE_ImageRecon
  PUBLIC_HEADERS
    C3Matrix.h
    C3Vector.h
    CCone.h
    CConeExtension.h
    CCurrentState.h
    CDataSetManager.h
    CDensityMatrix.h
    CEvent.h
    CEventManager.h
    CHit.h
    CPETLine.h
    CUserParameters.h
    constants.h
  SOURCES
    C3Matrix.cc
    C3Vector.cc
    CCone.cc
    CConeExtension.cc
    CCurrentState.cc
    CDataSetManager.cc
    CDensityMatrix.cc
    CEvent.cc
    CEventManager.cc
    CHit.cc
    CPETLine.cc
    CUserParameters.cc
    constants.cc
)

# Program SOE_ImageRecon
gamos_add_executable(SOE_ImageRecon
  SOURCES
    SOE_ImageRecon.cc
  LINK_LIBRARIES
    SOE_ImageRecon
)
