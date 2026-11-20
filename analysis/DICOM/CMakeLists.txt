# - analysis/DICOM

add_subdirectory(DICOM2G4)
add_subdirectory(getOrganInfo)
add_subdirectory(drawDICOM)
add_subdirectory(getGammaIndex)
add_subdirectory(changeStructure)
add_subdirectory(printValueAtPoint)
add_subdirectory(printValueAlongLine)
add_subdirectory(GamosToDICOM)
add_subdirectory(resizeImage)
add_subdirectory(dcmdump)
add_subdirectory(getImageValue)
add_subdirectory(sqdoseToRTDose)

# Not built (as in the GNUmake build, where they are not in SUBDIRS):
#add_subdirectory(DmToDw)
#add_subdirectory(changeExtraStructures)
#add_subdirectory(changeStructureHU)
#add_subdirectory(cleanOutsideStruct)
#add_subdirectory(drawDICOM2)
#add_subdirectory(extendImageAir)
#add_subdirectory(fmjpeg2koj)
#add_subdirectory(softenImage)
