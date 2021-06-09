#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosScoring_Scorers
#
#------------------------------------------------------------------------------
# List external includes needed.

# List external includes needed.
include(GamosSetup)
include(UseRoot)

#
# Define the GAMOS Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosScoring_Scorers

   HEADERS
       GmG4PSPassageCellFlux.hh
       GmG4PSPopulation.hh
       GmScoringSurfaceOrb.hh
       GmG4PSDoseDeposit_NoSkipEqualMate.hh
       GmG4PSDoseDepositVector.hh
       GmG4PSKerma.hh
       GmPSCylindricalRPhiDoseDeposit.hh
       GmPSCylindricalZPhiDoseDeposit.hh
       GmScoringSurfaceTubs.hh
       GmG4PSDoseDeposit.hh
       GmG4PSNofStep.hh
       GmVScoringSurfaceSolid.hh
       GmG4PSCellCharge.hh
       GmG4PSData.hh
       GmPSSurfaceFlux.hh
       GmG4PSTrackCounter.hh
       GmScoringSurfaceBox.hh
       GmPSSphericalDoseDeposit.hh
       GmG4PSCellFlux.hh
       GmPSWithDirection.hh
       GmG4PSTrackLength.hh
       GmG4PSEnergyLost.hh
       GmScoringSurfaceSphere.hh
       GmG4PSPassageCellCurrent.hh
       GmG4PSDoseDeposit_SkipEqualMateSimple.hh
       GmPSVolumeFlux.hh
       GmPSCylindricalRZDoseDeposit.hh
       GmG4PSPassageTrackLength.hh
       GmG4PSTermination.hh
       GmG4PSDoseDeposit_old.hh
       GmG4PSNofCollision.hh
       GmG4PSNofSecondary.hh
       GmG4PSMinKinEAtGeneration.hh
       GmG4PSEnergyDeposit.hh

   SOURCES
       GmScoringSurfaceSphere.cc
       plugin.cc
       GmG4PSData.cc
       GmG4PSNofSecondary.cc
       GmVScoringSurfaceSolid.cc
       GmPSSphericalDoseDeposit.cc
       GmG4PSMinKinEAtGeneration.cc
       GmG4PSNofStep.cc
       GmPSSurfaceFlux.cc
       GmG4PSPassageTrackLength.cc
       GmG4PSCellCharge.cc
       GmG4PSEnergyDeposit.cc
       GmG4PSPassageCellCurrent.cc
       GmG4PSDoseDeposit_old.cc
       GmG4PSEnergyLost.cc
       GmG4PSTrackCounter.cc
       GmPSCylindricalRZDoseDeposit.cc
       GmG4PSDoseDeposit_SkipEqualMateSimple.cc
       GmScoringSurfaceBox.cc
       GmG4PSNofCollision.cc
       GmScoringSurfaceTubs.cc
       GmG4PSDoseDepositVector.cc
       GmG4PSPopulation.cc
       GmPSVolumeFlux.cc
       GmPSWithDirection.cc
       GmG4PSDoseDeposit_NoSkipEqualMate.cc
       GmG4PSDoseDeposit.cc
       GmPSCylindricalRPhiDoseDeposit.cc
       GmG4PSCellFlux.cc
       GmG4PSKerma.cc
       GmPSCylindricalZPhiDoseDeposit.cc
       GmScoringSurfaceOrb.cc
       GmG4PSPassageCellFlux.cc
       GmG4PSTrackLength.cc
       GmG4PSTermination.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosGeometry
  GamosCore_GamosUtils
  GamosCore_GamosReadDICOM
  GamosCore_GamosScoring_Management
  GamosCore_GamosData_Users
  GamosCore_GamosBase_Base

   LINK_LIBRARIES
)

# List any source specific properties here
