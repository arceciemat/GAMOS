# - analysis/RTDose

add_subdirectory(analyseSqdose)
add_subdirectory(sum3ddose)
add_subdirectory(sumSqdose)
add_subdirectory(sqdoseTo3ddose)
add_subdirectory(compareDoses)
add_subdirectory(multSqdose)
add_subdirectory(operateSqdose)

# Not built (as in the GNUmake build, where they are not in SUBDIRS):
#add_subdirectory(3ddoseToSqdose)
#add_subdirectory(sumSqdoseDisp)
