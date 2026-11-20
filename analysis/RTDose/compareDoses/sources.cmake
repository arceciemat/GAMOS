# - compareDoses module build definition

gamos_add_module(compareDoses
  PUBLIC_HEADERS
    DoseData.hh
    DoseSet.hh
    DoserData.hh
    FileData.hh
    PlotDataSVG.icc
    ROOTDoseData.hh
    ROOTDoserData.hh
    ROOTFileData.hh
    ReadDoses.icc
    TextDoseData.hh
    TextDoserData.hh
    TextFileData.hh
  SOURCES
    DoseData.cc
    DoseSet.cc
    DoserData.cc
    FileData.cc
    ROOTDoseData.cc
    ROOTDoserData.cc
    ROOTFileData.cc
    TextDoseData.cc
    TextDoserData.cc
    TextFileData.cc
)

gamos_module_link_libraries(compareDoses
  PUBLIC
    GamosCore_GamosUtils
)

# Program compareDoses
gamos_add_executable(compareDoses
  SOURCES
    compareDoses.cc
  LINK_LIBRARIES
    compareDoses
    GamosCore_GamosBase_Base
    GamosCore_GamosRunManager
    GamosCore_GamosUtils
    compareScores
)
