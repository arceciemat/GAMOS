from enum import Enum

deoxyribose1 = 1
phosphate1 = 2
deoxyribose2 = 3  
phosphate2 = 4

base_cytosine = 5
base_guanine = 6
base_thymine = 7
base_adenine = 8

deoxyribose1_water = 9
phosphate1_water = 10
deoxyribose2_water = 11 
phosphate2_water = 12

base_adenine_water = 13 
base_guanine_water = 14
base_cytosine_water = 15
base_thymine_water = 16

histone = 17
physWorld = 18 
VoxelStraight = 19

DNAVolumeType = Enum('DNAVolumeType',[
    'deoxyribose1', 
    'phosphate1', 
    'deoxyribose2', 
    'phosphate2',
    'base_cytosine', 
    'base_guanine', 
    'base_thymine', 
    'base_adenine',
    'deoxyribose1_water', 
    'phosphate1_water', 
    'deoxyribose2_water', 
    'phosphate2_water', 
    'base_adenine_water', 
    'base_guanine_water',
    'base_cytosine_water', 
    'base_thymine_water', 
    'histone', 
    'physWorld', 
    'VoxelStraight'])

def printDNAVolumeType( voltyp ) :

    return DNAVolumeType(voltyp).name

def isBase(volumeType) :
    isB = volumeType == DNAVolumeType.base_adenine.value or  \
       volumeType == DNAVolumeType.base_cytosine.value or \
       volumeType == DNAVolumeType.base_guanine.value or \
       volumeType == DNAVolumeType.base_thymine.value or \
       volumeType == DNAVolumeType.base_adenine_water.value or  \
       volumeType == DNAVolumeType.base_cytosine_water.value or \
       volumeType == DNAVolumeType.base_guanine_water.value or \
       volumeType == DNAVolumeType.base_thymine_water.value
    return isB

def isBaseByName(volumeName) :
    isB = volumeName == "base_adenine" or \
       volumeName == "base_cytosine" or \
       volumeName == "base_guanine" or \
       volumeName == "base_thymine" 
    return isB

def isLeftStrand(volumeType) :
    isLS = volumeType == DNAVolumeType.deoxyribose1.value or \
        volumeType == DNAVolumeType.phosphate1.value or \
        volumeType == DNAVolumeType.deoxyribose1_water.value or \
        volumeType == DNAVolumeType.phosphate1_water.value 
    return isLS

def isRightStrand(volumeType) :
    isRS = volumeType == DNAVolumeType.deoxyribose2.value or \
        volumeType == DNAVolumeType.phosphate2.value or \
        volumeType == DNAVolumeType.deoxyribose2_water.value or \
        volumeType == DNAVolumeType.phosphate2_water.value 
    return isRS

def isDeoxyribose(volumeType) :
    isD = volumeType == DNAVolumeType.deoxyribose1.value or \
        volumeType == DNAVolumeType.deoxyribose2.value or \
        volumeType == DNAVolumeType.deoxyribose1_water.value or \
        volumeType == DNAVolumeType.deoxyribose2_water.value
    return isD

def isPhosphate(volumeType) :
    isP =  volumeType == DNAVolumeType.phosphate1.value or \
        volumeType == DNAVolumeType.phosphate2.value or \
        volumeType == DNAVolumeType.phosphate1_water.value or \
        volumeType == DNAVolumeType.phosphate2_water.value
    return isP

    
