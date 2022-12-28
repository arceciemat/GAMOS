//--- POSITION DATA
#include "GmDataInitialPosX.hh"
#include "GmDataInitialPosY.hh"
#include "GmDataInitialPosZ.hh"
#include "GmDataInitialPosMag.hh"
#include "GmDataInitialPosPerp.hh"
#include "GmDataInitialPosPhi.hh"
#include "GmDataInitialPosTheta.hh"
#include "GmDataInitialPosMaxXY.hh"
#include "GmDataFinalPosX.hh"
#include "GmDataFinalPosY.hh"
#include "GmDataFinalPosZ.hh"
#include "GmDataFinalPosMag.hh"
#include "GmDataFinalPosPerp.hh"
#include "GmDataFinalPosPhi.hh"
#include "GmDataFinalPosTheta.hh"
#include "GmDataFinalPosMaxXY.hh"
#include "GmDataVertexPosX.hh"
#include "GmDataVertexPosY.hh"
#include "GmDataVertexPosZ.hh"
#include "GmDataVertexPosMag.hh"
#include "GmDataVertexPosPerp.hh"
#include "GmDataVertexPosPhi.hh"
#include "GmDataVertexPosTheta.hh"
#include "GmDataRandomPosX.hh"
#include "GmDataRandomPosY.hh"
#include "GmDataRandomPosZ.hh"
#include "GmDataRandomPosMag.hh"
#include "GmDataRandomPosPerp.hh"
#include "GmDataRandomPosPhi.hh"
#include "GmDataRandomPosTheta.hh"

//-- Local position
#include "GmDataInitialLocalPosX.hh"
#include "GmDataInitialLocalPosY.hh"
#include "GmDataInitialLocalPosZ.hh"
#include "GmDataInitialLocalPosMag.hh"
#include "GmDataInitialLocalPosPerp.hh"
#include "GmDataInitialLocalPosPhi.hh"
#include "GmDataInitialLocalPosTheta.hh"
#include "GmDataInitialLocalPos1X.hh"
#include "GmDataInitialLocalPos1Y.hh"
#include "GmDataInitialLocalPos1Z.hh"
#include "GmDataFinalLocalPos1X.hh"
#include "GmDataFinalLocalPos1Y.hh"
#include "GmDataFinalLocalPos1Z.hh"
#include "GmDataVertexLocalPos1X.hh"
#include "GmDataVertexLocalPos1Y.hh"
#include "GmDataVertexLocalPos1Z.hh"

#include "GmDataFinalLocalPosX.hh"
#include "GmDataFinalLocalPosY.hh"
#include "GmDataFinalLocalPosZ.hh"
#include "GmDataFinalLocalPosMag.hh"
#include "GmDataFinalLocalPosPerp.hh"
#include "GmDataFinalLocalPosPhi.hh"
#include "GmDataFinalLocalPosTheta.hh"

#include "GmDataFinalLocalInPrePosX.hh"
#include "GmDataFinalLocalInPrePosY.hh"
#include "GmDataFinalLocalInPrePosZ.hh"
#include "GmDataFinalLocalInPrePosMag.hh"
#include "GmDataFinalLocalInPrePosPerp.hh"
#include "GmDataFinalLocalInPrePosPhi.hh"
#include "GmDataFinalLocalInPrePosTheta.hh"

//-- Position change
#include "GmDataPosChangeX.hh"
#include "GmDataPosChangeY.hh"
#include "GmDataPosChangeZ.hh"
#include "GmDataPosChangeMag.hh"
#include "GmDataPosChangePerp.hh"
#include "GmDataPosChangePhi.hh"
#include "GmDataPosChangeTheta.hh"
#include "GmDataTrackPosChangeMag.hh"

//--- MOMENTUM DATA
#include "GmDataInitialMomX.hh"
#include "GmDataInitialMomY.hh"
#include "GmDataInitialMomZ.hh"
#include "GmDataInitialMomMag.hh"
#include "GmDataInitialMomPerp.hh"
#include "GmDataInitialMomPhi.hh"
#include "GmDataInitialMomTheta.hh"
#include "GmDataFinalMomX.hh"
#include "GmDataFinalMomY.hh"
#include "GmDataFinalMomZ.hh"
#include "GmDataFinalMomMag.hh"
#include "GmDataFinalMomPerp.hh"
#include "GmDataFinalMomPhi.hh"
#include "GmDataFinalMomTheta.hh"
#include "GmDataVertexMomX.hh"
#include "GmDataVertexMomY.hh"
#include "GmDataVertexMomZ.hh"
#include "GmDataVertexMomMag.hh"
#include "GmDataVertexMomPerp.hh"
#include "GmDataVertexMomPhi.hh"
#include "GmDataVertexMomTheta.hh"

#include "GmDataMomChangeX.hh"
#include "GmDataMomChangeY.hh"
#include "GmDataMomChangeZ.hh"
#include "GmDataMomChangeMag.hh"
#include "GmDataMomChangePerp.hh"
#include "GmDataMomChangePhi.hh"
#include "GmDataMomChangeTheta.hh"

#include "GmDataInitialVelocity.hh"
#include "GmDataFinalVelocity.hh"

//--- DIRECTION DATA
#include "GmDataInitialDirX.hh"
#include "GmDataInitialDirY.hh"
#include "GmDataInitialDirZ.hh"
#include "GmDataInitialDirPerp.hh"
#include "GmDataInitialDirPhi.hh"
#include "GmDataInitialDirTheta.hh"
#include "GmDataFinalDirX.hh"
#include "GmDataFinalDirY.hh"
#include "GmDataFinalDirZ.hh"
#include "GmDataFinalDirPerp.hh"
#include "GmDataFinalDirPhi.hh"
#include "GmDataFinalDirTheta.hh"
#include "GmDataVertexDirX.hh"
#include "GmDataVertexDirY.hh"
#include "GmDataVertexDirZ.hh"
#include "GmDataVertexDirPerp.hh"
#include "GmDataVertexDirPhi.hh"
#include "GmDataVertexDirTheta.hh"
#include "GmDataInitialNoVertexDirTheta.hh"

#include "GmDataDirChangeX.hh"
#include "GmDataDirChangeY.hh"
#include "GmDataDirChangeZ.hh"
#include "GmDataDirChangeMag.hh"
#include "GmDataDirChangePerp.hh"
#include "GmDataDirChangePhi.hh"
#include "GmDataDirChangeTheta.hh"

//-- Local direction
#include "GmDataInitialLocalDirX.hh"
#include "GmDataInitialLocalDirY.hh"
#include "GmDataInitialLocalDirZ.hh"
#include "GmDataInitialLocalDirPerp.hh"
#include "GmDataInitialLocalDirPhi.hh"
#include "GmDataInitialLocalDirTheta.hh"

#include "GmDataFinalLocalDirX.hh"
#include "GmDataFinalLocalDirY.hh"
#include "GmDataFinalLocalDirZ.hh"
#include "GmDataFinalLocalDirPerp.hh"
#include "GmDataFinalLocalDirPhi.hh"
#include "GmDataFinalLocalDirTheta.hh"

#include "GmDataFinalLocalInPreDirX.hh"
#include "GmDataFinalLocalInPreDirY.hh"
#include "GmDataFinalLocalInPreDirZ.hh"
#include "GmDataFinalLocalInPreDirPerp.hh"
#include "GmDataFinalLocalInPreDirPhi.hh"
#include "GmDataFinalLocalInPreDirTheta.hh"

#include "GmDataAngleChange.hh"

//--- ENERGY DATA
#include "GmDataInitialKineticEnergy.hh"
#include "GmDataFinalKineticEnergy.hh"
#include "GmDataVertexKineticEnergy.hh"
#include "GmDataInitialTotalEnergy.hh"
#include "GmDataFinalTotalEnergy.hh"
#include "GmDataAccumulatedEnergyLost.hh"
#include "GmDataAccumulatedEnergyDeposit.hh"
#include "GmDataKineticEnergyChange.hh"
#include "GmDataAccumulatedDose.hh"
#include "GmDataAccumulatedKerma.hh"
#include "GmDataInitialDEDX.hh"
#include "GmDataFinalDEDX.hh"
#include "GmDataInitialGammaAttLength.hh"
#include "GmDataFinalGammaAttLength.hh"
#include "GmDataInitialNeutronEquivalentDose.hh"
#include "GmDataFinalGammaEquivalentDose.hh"
#include "GmDataInitialGammaEquivalentDose.hh"
#include "GmDataFinalNeutronEquivalentDose.hh"
#include "GmDataInitialNeutronEnergyToEquivalentDose.hh"
#include "GmDataInitialGammaEnergyToEquivalentDose.hh"
#include "GmDataFinalNeutronEnergyToEquivalentDose.hh"
#include "GmDataFinalGammaEnergyToEquivalentDose.hh"

//--- GEOMETRY DATA
#include "GmDataInitialSolid.hh"
#include "GmDataFinalSolid.hh"
#include "GmDataInitialLogicalVolume.hh"
#include "GmDataFinalLogicalVolume.hh"
#include "GmDataInitialPhysicalVolume.hh"
#include "GmDataFinalPhysicalVolume.hh"
#include "GmDataInitialTouchable.hh"
#include "GmDataFinalTouchable.hh"
#include "GmDataInitialRegion.hh"
#include "GmDataFinalRegion.hh"
#include "GmDataInitialSolidType.hh"
#include "GmDataFinalSolidType.hh"
#include "GmDataInitialPVCopyNumber.hh"
#include "GmDataFinalPVCopyNumber.hh"
#include "GmDataInitialPhantomStruct.hh"
#include "GmDataFinalPhantomStruct.hh"

//--- PARTICLE AND PROCESS DATA
#include "GmDataParticle.hh"
#include "GmDataPrimaryParticle.hh"
#include "GmDataInitialProcess.hh"
#include "GmDataFinalProcess.hh"
#include "GmDataCreatorProcess.hh"
#include "GmDataParticleType.hh"
#include "GmDataParticleSubType.hh"
#include "GmDataParticleLifeTime.hh"
#include "GmDataParticleStable.hh"
#include "GmDataParticleMass.hh"
#include "GmDataParticleWidth.hh"
#include "GmDataParticleCharge.hh"
#include "GmDataParticlePDGEncoding.hh"
#include "GmDataNProcesses.hh"

//--- MATERIAL DATA
#include "GmDataInitialMaterial.hh"
#include "GmDataFinalMaterial.hh"

#include "GmDataInitialMaterial.hh"
#include "GmDataFinalMaterial.hh"
#include "GmDataInitialPressure.hh"
#include "GmDataInitialTemperature.hh"
#include "GmDataInitialDensity.hh"
#include "GmDataInitialElectronDensity.hh"
#include "GmDataInitialRadLength.hh"
#include "GmDataInitialNuclearInterLength.hh"
#include "GmDataFinalPressure.hh"
#include "GmDataFinalTemperature.hh"
#include "GmDataFinalDensity.hh"
#include "GmDataFinalElectronDensity.hh"
#include "GmDataFinalRadLength.hh"
#include "GmDataFinalNuclearInterLength.hh"

//--- OTHERS
#include "GmDataTrackLength.hh"
#include "GmDataTrackID.hh"
#include "GmDataParentTrackID.hh"
#include "GmDataEventID.hh"
#include "GmDataRunID.hh"
#include "GmDataStepNumber.hh"
#include "GmDataInitialWeight.hh"
#include "GmDataFinalWeight.hh"
#include "GmDataAccumulatedLength.hh"
#include "GmDataAccumulatedTrackLength.hh"
#include "GmDataInitialTime.hh"
#include "GmDataFinalTime.hh"
#include "GmDataInitialLocalTime.hh"
#include "GmDataFinalLocalTime.hh"
#include "GmDataInitialProperTime.hh"
#include "GmDataFinalProperTime.hh"
#include "GmDataInitialCSDARange.hh"
#include "GmDataFinalCSDARange.hh"

//---- SECONDARY DATA
#include "GmDataNofSecondaries.hh"
#include "GmDataSumSecoKineticEnergy.hh"
#include "GmDataInitialPrimMinusSecoKineticEnergy.hh"
#include "GmDataFinalPrimMinusSecoKineticEnergy.hh"
#include "GmDataSecoDividedInitialPrimKineticEnergy.hh"  
#include "GmDataSecoKineticEnergy.hh"
#include "GmDataSecoDirTheta.hh" 
#include "GmDataInitialPrimSecoAngle.hh"        
#include "GmDataFinalPrimSecoAngle.hh"        
#include "GmDataInitialPrimKineticEnergy.hh"
#include "GmDataFinalPrimKineticEnergy.hh"
#include "GmDataVertexPrimKineticEnergy.hh"
#include "GmDataPrimParticle.hh"

#include "GmDataSecoInitialDirTheta.hh"

#include "GmDataFinalDirNormalAngle.hh"

#include "GmDataNofHits.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

//--- POSITION DATA
PLUGINSVC_FACTORY(GmDataInitialPosX,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosY,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosTheta,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosMaxXY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosTheta,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosMaxXY,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexPosX,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexPosY,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexPosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexPosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexPosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexPosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexPosTheta,GmVData*())
PLUGINSVC_FACTORY(GmDataRandomPosX,GmVData*())
PLUGINSVC_FACTORY(GmDataRandomPosY,GmVData*())
PLUGINSVC_FACTORY(GmDataRandomPosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataRandomPosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataRandomPosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataRandomPosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataRandomPosTheta,GmVData*())

//-- Local position
PLUGINSVC_FACTORY(GmDataInitialLocalPosX,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosY,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosTheta,GmVData*())

PLUGINSVC_FACTORY(GmDataFinalLocalPosX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosTheta,GmVData*())

PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosTheta,GmVData*())

//-- Position change
PLUGINSVC_FACTORY(GmDataPosChangeX,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangeY,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangeZ,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangeMag,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangePerp,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangePhi,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangeTheta,GmVData*())
PLUGINSVC_FACTORY(GmDataTrackPosChangeMag,GmVData*())

//--- MOMENTUM DATA
PLUGINSVC_FACTORY(GmDataInitialMomX,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialMomY,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialMomZ,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialMomMag,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialMomPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialMomPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialMomTheta,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalMomX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalMomY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalMomZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalMomMag,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalMomPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalMomPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalMomTheta,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexMomX,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexMomY,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexMomZ,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexMomMag,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexMomPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexMomPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexMomTheta,GmVData*())

PLUGINSVC_FACTORY(GmDataMomChangeX,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangeY,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangeZ,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangeMag,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangePerp,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangePhi,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangeTheta,GmVData*())

PLUGINSVC_FACTORY(GmDataInitialVelocity,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalVelocity,GmVData*())
 
//--- DIRECTION DATA
PLUGINSVC_FACTORY(GmDataInitialDirX,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialDirY,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialDirZ,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialDirPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialDirPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialDirTheta,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalDirX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalDirY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalDirZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalDirPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalDirPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalDirTheta,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexDirX,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexDirY,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexDirZ,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexDirPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexDirPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexDirTheta,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialNoVertexDirTheta,GmVData*())

PLUGINSVC_FACTORY(GmDataDirChangeX,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangeY,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangeZ,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangeMag,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangePerp,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangePhi,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangeTheta,GmVData*())

//-- Local direction
PLUGINSVC_FACTORY(GmDataInitialLocalDirX,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalDirY,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalDirZ,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalDirPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalDirPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalDirTheta,GmVData*())

PLUGINSVC_FACTORY(GmDataFinalLocalDirX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalDirY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalDirZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalDirPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalDirPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalDirTheta,GmVData*())

PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirTheta,GmVData*())

PLUGINSVC_FACTORY(GmDataAngleChange,GmVData*())

//--- ENERGY DATA
PLUGINSVC_FACTORY(GmDataInitialKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialTotalEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalTotalEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataAccumulatedEnergyLost,GmVData*())
PLUGINSVC_FACTORY(GmDataAccumulatedEnergyDeposit,GmVData*())
PLUGINSVC_FACTORY(GmDataKineticEnergyChange,GmVData*())
PLUGINSVC_FACTORY(GmDataAccumulatedDose,GmVData*())
PLUGINSVC_FACTORY(GmDataAccumulatedKerma,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialDEDX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalDEDX,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialGammaAttLength,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalGammaAttLength,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialNeutronEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalGammaEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialGammaEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalNeutronEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialNeutronEnergyToEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialGammaEnergyToEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalNeutronEnergyToEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalGammaEnergyToEquivalentDose,GmVData*())

//--- GEOMETRY DATA
PLUGINSVC_FACTORY(GmDataInitialSolid,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalSolid,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLogicalVolume,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLogicalVolume,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPhysicalVolume,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPhysicalVolume,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialTouchable,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalTouchable,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialRegion,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalRegion,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialSolidType,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalSolidType,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPVCopyNumber,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPVCopyNumber,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialCopyNo,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalCopyNo,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPhantomStruct,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPhantomStruct,GmVData*())

//--- PARTICLE AND PROCESS DATA
PLUGINSVC_FACTORY(GmDataParticle,GmVData*())
PLUGINSVC_FACTORY(GmDataPrimaryParticle,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialProcess,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalProcess,GmVData*())
PLUGINSVC_FACTORY(GmDataCreatorProcess,GmVData*())
PLUGINSVC_FACTORY(GmDataParticleType,GmVData*())
PLUGINSVC_FACTORY(GmDataParticleSubType,GmVData*())
PLUGINSVC_FACTORY(GmDataParticleLifeTime,GmVData*())
PLUGINSVC_FACTORY(GmDataParticleStable,GmVData*())
PLUGINSVC_FACTORY(GmDataParticleMass,GmVData*())
PLUGINSVC_FACTORY(GmDataParticleWidth,GmVData*())
PLUGINSVC_FACTORY(GmDataParticleCharge,GmVData*())
PLUGINSVC_FACTORY(GmDataParticlePDGEncoding,GmVData*())
PLUGINSVC_FACTORY(GmDataNProcesses,GmVData*())


//--- MATERIAL DATA
PLUGINSVC_FACTORY(GmDataInitialMaterial,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalMaterial,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPressure,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialTemperature,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialDensity,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialElectronDensity,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialRadLength,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialNuclearInterLength,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPressure,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalTemperature,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalDensity,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalElectronDensity,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalRadLength,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalNuclearInterLength,GmVData*())

//--- OTHERS
PLUGINSVC_FACTORY(GmDataTrackLength,GmVData*())
PLUGINSVC_FACTORY(GmDataTrackID,GmVData*())
PLUGINSVC_FACTORY(GmDataParentTrackID,GmVData*())
PLUGINSVC_FACTORY(GmDataEventID,GmVData*())
PLUGINSVC_FACTORY(GmDataRunID,GmVData*())
PLUGINSVC_FACTORY(GmDataStepNumber,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialWeight,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalWeight,GmVData*())
PLUGINSVC_FACTORY(GmDataAccumulatedLength,GmVData*())
PLUGINSVC_FACTORY(GmDataAccumulatedTrackLength,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialTime,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalTime,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalTime,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalTime,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialProperTime,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalProperTime,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialCSDARange,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalCSDARange,GmVData*())

//---- SECONDARY DATA
PLUGINSVC_FACTORY(GmDataSumSecoKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPrimMinusSecoKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPrimMinusSecoKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataSecoDividedInitialPrimKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataSecoKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataSecoDirTheta,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPrimSecoAngle,GmVData*()) 
PLUGINSVC_FACTORY(GmDataFinalPrimSecoAngle,GmVData*()) 
PLUGINSVC_FACTORY(GmDataInitialPrimKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPrimKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexPrimKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataPrimParticle,GmVData*())

PLUGINSVC_FACTORY(GmDataSecoInitialDirTheta,GmVData*())

PLUGINSVC_FACTORY(GmDataFinalDirNormalAngle,GmVData*())

PLUGINSVC_FACTORY(GmDataNofHits,GmVData*())

#else

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
DEFINE_SEAL_MODULE ();

#include "GamosCore/GamosData/Management/include/GmDataFactory.hh"

//--- POSITION DATA
DEFINE_GAMOS_DATA(GmDataInitialPosX);
DEFINE_GAMOS_DATA(GmDataInitialPosY);
DEFINE_GAMOS_DATA(GmDataInitialPosZ);
DEFINE_GAMOS_DATA(GmDataInitialPosMag);
DEFINE_GAMOS_DATA(GmDataInitialPosPerp);
DEFINE_GAMOS_DATA(GmDataInitialPosPhi);
DEFINE_GAMOS_DATA(GmDataInitialPosTheta);
DEFINE_GAMOS_DATA(GmDataInitialPosMaxXY);
DEFINE_GAMOS_DATA(GmDataFinalPosX);
DEFINE_GAMOS_DATA(GmDataFinalPosY);
DEFINE_GAMOS_DATA(GmDataFinalPosZ);
DEFINE_GAMOS_DATA(GmDataFinalPosMag);
DEFINE_GAMOS_DATA(GmDataFinalPosPerp);
DEFINE_GAMOS_DATA(GmDataFinalPosPhi);
DEFINE_GAMOS_DATA(GmDataFinalPosTheta);
DEFINE_GAMOS_DATA(GmDataFinalPosMaxXY);
DEFINE_GAMOS_DATA(GmDataVertexPosX);
DEFINE_GAMOS_DATA(GmDataVertexPosY);
DEFINE_GAMOS_DATA(GmDataVertexPosZ);
DEFINE_GAMOS_DATA(GmDataVertexPosMag);
DEFINE_GAMOS_DATA(GmDataVertexPosPerp);
DEFINE_GAMOS_DATA(GmDataVertexPosPhi);
DEFINE_GAMOS_DATA(GmDataVertexPosTheta);
DEFINE_GAMOS_DATA(GmDataRandomPosX);
DEFINE_GAMOS_DATA(GmDataRandomPosY);
DEFINE_GAMOS_DATA(GmDataRandomPosZ);
DEFINE_GAMOS_DATA(GmDataRandomPosMag);
DEFINE_GAMOS_DATA(GmDataRandomPosPerp);
DEFINE_GAMOS_DATA(GmDataRandomPosPhi);
DEFINE_GAMOS_DATA(GmDataRandomPosTheta);

//-- Local position
DEFINE_GAMOS_DATA(GmDataInitialLocalPosX);
DEFINE_GAMOS_DATA(GmDataInitialLocalPosY);
DEFINE_GAMOS_DATA(GmDataInitialLocalPosZ);
DEFINE_GAMOS_DATA(GmDataInitialLocalPosMag);
DEFINE_GAMOS_DATA(GmDataInitialLocalPosPerp);
DEFINE_GAMOS_DATA(GmDataInitialLocalPosPhi);
DEFINE_GAMOS_DATA(GmDataInitialLocalPosTheta);
DEFINE_GAMOS_DATA(GmDataInitialLocalPos1X);
DEFINE_GAMOS_DATA(GmDataInitialLocalPos1Y);
DEFINE_GAMOS_DATA(GmDataInitialLocalPos1Z);
DEFINE_GAMOS_DATA(GmDataFinalLocalPos1X);
DEFINE_GAMOS_DATA(GmDataFinalLocalPos1Y);
DEFINE_GAMOS_DATA(GmDataFinalLocalPos1Z);
DEFINE_GAMOS_DATA(GmDataVertexLocalPos1X);
DEFINE_GAMOS_DATA(GmDataVertexLocalPos1Y);
DEFINE_GAMOS_DATA(GmDataVertexLocalPos1Z);

DEFINE_GAMOS_DATA(GmDataFinalLocalPosX);
DEFINE_GAMOS_DATA(GmDataFinalLocalPosY);
DEFINE_GAMOS_DATA(GmDataFinalLocalPosZ);
DEFINE_GAMOS_DATA(GmDataFinalLocalPosMag);
DEFINE_GAMOS_DATA(GmDataFinalLocalPosPerp);
DEFINE_GAMOS_DATA(GmDataFinalLocalPosPhi);
DEFINE_GAMOS_DATA(GmDataFinalLocalPosTheta);

DEFINE_GAMOS_DATA(GmDataFinalLocalInPrePosX);
DEFINE_GAMOS_DATA(GmDataFinalLocalInPrePosY);
DEFINE_GAMOS_DATA(GmDataFinalLocalInPrePosZ);
DEFINE_GAMOS_DATA(GmDataFinalLocalInPrePosMag);
DEFINE_GAMOS_DATA(GmDataFinalLocalInPrePosPerp);
DEFINE_GAMOS_DATA(GmDataFinalLocalInPrePosPhi);
DEFINE_GAMOS_DATA(GmDataFinalLocalInPrePosTheta);

//-- Position change
DEFINE_GAMOS_DATA(GmDataPosChangeX);
DEFINE_GAMOS_DATA(GmDataPosChangeY);
DEFINE_GAMOS_DATA(GmDataPosChangeZ);
DEFINE_GAMOS_DATA(GmDataPosChangeMag);
DEFINE_GAMOS_DATA(GmDataPosChangePerp);
DEFINE_GAMOS_DATA(GmDataPosChangePhi);
DEFINE_GAMOS_DATA(GmDataPosChangeTheta);
DEFINE_GAMOS_DATA(GmDataTrackPosChangeMag);

//--- MOMENTUM DATA
DEFINE_GAMOS_DATA(GmDataInitialMomX);
DEFINE_GAMOS_DATA(GmDataInitialMomY);
DEFINE_GAMOS_DATA(GmDataInitialMomZ);
DEFINE_GAMOS_DATA(GmDataInitialMomMag);
DEFINE_GAMOS_DATA(GmDataInitialMomPerp);
DEFINE_GAMOS_DATA(GmDataInitialMomPhi);
DEFINE_GAMOS_DATA(GmDataInitialMomTheta);
DEFINE_GAMOS_DATA(GmDataFinalMomX);
DEFINE_GAMOS_DATA(GmDataFinalMomY);
DEFINE_GAMOS_DATA(GmDataFinalMomZ);
DEFINE_GAMOS_DATA(GmDataFinalMomMag);
DEFINE_GAMOS_DATA(GmDataFinalMomPerp);
DEFINE_GAMOS_DATA(GmDataFinalMomPhi);
DEFINE_GAMOS_DATA(GmDataFinalMomTheta);
DEFINE_GAMOS_DATA(GmDataVertexMomX);
DEFINE_GAMOS_DATA(GmDataVertexMomY);
DEFINE_GAMOS_DATA(GmDataVertexMomZ);
DEFINE_GAMOS_DATA(GmDataVertexMomMag);
DEFINE_GAMOS_DATA(GmDataVertexMomPerp);
DEFINE_GAMOS_DATA(GmDataVertexMomPhi);
DEFINE_GAMOS_DATA(GmDataVertexMomTheta);

DEFINE_GAMOS_DATA(GmDataMomChangeX);
DEFINE_GAMOS_DATA(GmDataMomChangeY);
DEFINE_GAMOS_DATA(GmDataMomChangeZ);
DEFINE_GAMOS_DATA(GmDataMomChangeMag);
DEFINE_GAMOS_DATA(GmDataMomChangePerp);
DEFINE_GAMOS_DATA(GmDataMomChangePhi);
DEFINE_GAMOS_DATA(GmDataMomChangeTheta);

DEFINE_GAMOS_DATA(GmDataInitialVelocity);
DEFINE_GAMOS_DATA(GmDataFinalVelocity);

//--- DIRECTION DATA
DEFINE_GAMOS_DATA(GmDataInitialDirX);
DEFINE_GAMOS_DATA(GmDataInitialDirY);
DEFINE_GAMOS_DATA(GmDataInitialDirZ);
DEFINE_GAMOS_DATA(GmDataInitialDirPerp);
DEFINE_GAMOS_DATA(GmDataInitialDirPhi);
DEFINE_GAMOS_DATA(GmDataInitialDirTheta);
DEFINE_GAMOS_DATA(GmDataFinalDirX);
DEFINE_GAMOS_DATA(GmDataFinalDirY);
DEFINE_GAMOS_DATA(GmDataFinalDirZ);
DEFINE_GAMOS_DATA(GmDataFinalDirPerp);
DEFINE_GAMOS_DATA(GmDataFinalDirPhi);
DEFINE_GAMOS_DATA(GmDataFinalDirTheta);
DEFINE_GAMOS_DATA(GmDataVertexDirX);
DEFINE_GAMOS_DATA(GmDataVertexDirY);
DEFINE_GAMOS_DATA(GmDataVertexDirZ);
DEFINE_GAMOS_DATA(GmDataVertexDirPerp);
DEFINE_GAMOS_DATA(GmDataVertexDirPhi);
DEFINE_GAMOS_DATA(GmDataVertexDirTheta);
DEFINE_GAMOS_DATA(GmDataInitialNoVertexDirTheta);

DEFINE_GAMOS_DATA(GmDataDirChangeX);
DEFINE_GAMOS_DATA(GmDataDirChangeY);
DEFINE_GAMOS_DATA(GmDataDirChangeZ);
DEFINE_GAMOS_DATA(GmDataDirChangeMag);
DEFINE_GAMOS_DATA(GmDataDirChangePerp);
DEFINE_GAMOS_DATA(GmDataDirChangePhi);
DEFINE_GAMOS_DATA(GmDataDirChangeTheta);

//-- Local direction
DEFINE_GAMOS_DATA(GmDataInitialLocalDirX);
DEFINE_GAMOS_DATA(GmDataInitialLocalDirY);
DEFINE_GAMOS_DATA(GmDataInitialLocalDirZ);
DEFINE_GAMOS_DATA(GmDataInitialLocalDirPerp);
DEFINE_GAMOS_DATA(GmDataInitialLocalDirPhi);
DEFINE_GAMOS_DATA(GmDataInitialLocalDirTheta);

DEFINE_GAMOS_DATA(GmDataFinalLocalDirX);
DEFINE_GAMOS_DATA(GmDataFinalLocalDirY);
DEFINE_GAMOS_DATA(GmDataFinalLocalDirZ);
DEFINE_GAMOS_DATA(GmDataFinalLocalDirPerp);
DEFINE_GAMOS_DATA(GmDataFinalLocalDirPhi);
DEFINE_GAMOS_DATA(GmDataFinalLocalDirTheta);

DEFINE_GAMOS_DATA(GmDataFinalLocalInPreDirX);
DEFINE_GAMOS_DATA(GmDataFinalLocalInPreDirY);
DEFINE_GAMOS_DATA(GmDataFinalLocalInPreDirZ);
DEFINE_GAMOS_DATA(GmDataFinalLocalInPreDirPerp);
DEFINE_GAMOS_DATA(GmDataFinalLocalInPreDirPhi);
DEFINE_GAMOS_DATA(GmDataFinalLocalInPreDirTheta);

DEFINE_GAMOS_DATA(GmDataAngleChange);

//--- ENERGY DATA
DEFINE_GAMOS_DATA(GmDataInitialKineticEnergy);
DEFINE_GAMOS_DATA(GmDataFinalKineticEnergy);
DEFINE_GAMOS_DATA(GmDataVertexKineticEnergy);
DEFINE_GAMOS_DATA(GmDataInitialTotalEnergy);
DEFINE_GAMOS_DATA(GmDataFinalTotalEnergy);
DEFINE_GAMOS_DATA(GmDataAccumulatedEnergyLost);
DEFINE_GAMOS_DATA(GmDataAccumulatedEnergyDeposit);
DEFINE_SEAL_PLUGIN(GmDataFactory, GmDataAccumulatedEnergyDeposit, "GmDataAccumulatedEnergyDeposited");
DEFINE_GAMOS_DATA(GmDataKineticEnergyChange);
DEFINE_GAMOS_DATA(GmDataAccumulatedDose);
DEFINE_GAMOS_DATA(GmDataAccumulatedKerma);
DEFINE_GAMOS_DATA(GmDataInitialDEDX);
DEFINE_GAMOS_DATA(GmDataFinalDEDX);
DEFINE_GAMOS_DATA(GmDataInitialGammaAttLength);
DEFINE_GAMOS_DATA(GmDataFinalGammaAttLength);
DEFINE_GAMOS_DATA(GmDataInitialNeutronEquivalentDose);
DEFINE_GAMOS_DATA(GmDataFinalGammaEquivalentDose);
DEFINE_GAMOS_DATA(GmDataInitialGammaEquivalentDose);
DEFINE_GAMOS_DATA(GmDataFinalNeutronEquivalentDose);
DEFINE_GAMOS_DATA(GmDataInitialNeutronEnergyToEquivalentDose);
DEFINE_GAMOS_DATA(GmDataInitialGammaEnergyToEquivalentDose);
DEFINE_GAMOS_DATA(GmDataFinalNeutronEnergyToEquivalentDose);
DEFINE_GAMOS_DATA(GmDataFinalGammaEnergyToEquivalentDose);

//--- GEOMETRY DATA
DEFINE_GAMOS_DATA(GmDataInitialSolid);
DEFINE_GAMOS_DATA(GmDataFinalSolid);
DEFINE_GAMOS_DATA(GmDataInitialLogicalVolume);
DEFINE_GAMOS_DATA(GmDataFinalLogicalVolume);
DEFINE_GAMOS_DATA(GmDataInitialPhysicalVolume);
DEFINE_GAMOS_DATA(GmDataFinalPhysicalVolume);
DEFINE_GAMOS_DATA(GmDataInitialTouchable);
DEFINE_GAMOS_DATA(GmDataFinalTouchable);
DEFINE_GAMOS_DATA(GmDataInitialRegion);
DEFINE_GAMOS_DATA(GmDataFinalRegion);
DEFINE_GAMOS_DATA(GmDataInitialSolidType);
DEFINE_GAMOS_DATA(GmDataFinalSolidType);
DEFINE_GAMOS_DATA(GmDataInitialPVCopyNumber);
DEFINE_GAMOS_DATA(GmDataFinalPVCopyNumber);
DEFINE_SEAL_PLUGIN(GmDataFactory, GmDataInitialPVCopyNumber, "GmDataInitialCopyNo");
DEFINE_SEAL_PLUGIN(GmDataFactory, GmDataFinalPVCopyNumber, "GmDataFinalCopyNo");
DEFINE_GAMOS_DATA(GmDataInitialPhantomStruct);
DEFINE_GAMOS_DATA(GmDataFinalPhantomStruct);

//--- PARTICLE AND PROCESS DATA
DEFINE_GAMOS_DATA(GmDataParticle);
DEFINE_GAMOS_DATA(GmDataPrimaryParticle);
DEFINE_GAMOS_DATA(GmDataInitialProcess);
DEFINE_GAMOS_DATA(GmDataFinalProcess);
DEFINE_GAMOS_DATA(GmDataCreatorProcess);
DEFINE_GAMOS_DATA(GmDataParticleType);
DEFINE_GAMOS_DATA(GmDataParticleSubType);
DEFINE_GAMOS_DATA(GmDataParticleLifeTime);
DEFINE_GAMOS_DATA(GmDataParticleStable);
DEFINE_GAMOS_DATA(GmDataParticleMass);
DEFINE_GAMOS_DATA(GmDataParticleWidth);
DEFINE_GAMOS_DATA(GmDataParticleCharge);
DEFINE_GAMOS_DATA(GmDataParticlePDGEncoding);
DEFINE_GAMOS_DATA(GmDataNProcesses);

//--- MATERIAL DATA
DEFINE_GAMOS_DATA(GmDataInitialMaterial);
DEFINE_GAMOS_DATA(GmDataFinalMaterial);
DEFINE_GAMOS_DATA(GmDataInitialPressure);
DEFINE_GAMOS_DATA(GmDataInitialTemperature);
DEFINE_GAMOS_DATA(GmDataInitialDensity);
DEFINE_GAMOS_DATA(GmDataInitialElectronDensity);
DEFINE_GAMOS_DATA(GmDataInitialRadLength);
DEFINE_GAMOS_DATA(GmDataInitialNuclearInterLength);
DEFINE_GAMOS_DATA(GmDataFinalPressure);
DEFINE_GAMOS_DATA(GmDataFinalTemperature);
DEFINE_GAMOS_DATA(GmDataFinalDensity);
DEFINE_GAMOS_DATA(GmDataFinalElectronDensity);
DEFINE_GAMOS_DATA(GmDataFinalRadLength);
DEFINE_GAMOS_DATA(GmDataFinalNuclearInterLength);

//--- OTHERS
DEFINE_GAMOS_DATA(GmDataTrackLength);
DEFINE_GAMOS_DATA(GmDataTrackID);
DEFINE_GAMOS_DATA(GmDataParentTrackID);
DEFINE_GAMOS_DATA(GmDataEventID);
DEFINE_GAMOS_DATA(GmDataRunID);
DEFINE_GAMOS_DATA(GmDataStepNumber);
DEFINE_GAMOS_DATA(GmDataInitialWeight);
DEFINE_GAMOS_DATA(GmDataFinalWeight);
DEFINE_GAMOS_DATA(GmDataAccumulatedLength);
DEFINE_GAMOS_DATA(GmDataAccumulatedTrackLength);
DEFINE_GAMOS_DATA(GmDataInitialTime);
DEFINE_GAMOS_DATA(GmDataFinalTime);
DEFINE_GAMOS_DATA(GmDataInitialLocalTime);
DEFINE_GAMOS_DATA(GmDataFinalLocalTime);
DEFINE_GAMOS_DATA(GmDataInitialProperTime);
DEFINE_GAMOS_DATA(GmDataFinalProperTime);
DEFINE_GAMOS_DATA(GmDataInitialCSDARange);
DEFINE_GAMOS_DATA(GmDataFinalCSDARange);

//---- SECONDARY DATA
DEFINE_GAMOS_DATA(GmDataNofSecondaries);
DEFINE_GAMOS_DATA(GmDataSumSecoKineticEnergy);
DEFINE_GAMOS_DATA(GmDataInitialPrimMinusSecoKineticEnergy);
DEFINE_GAMOS_DATA(GmDataFinalPrimMinusSecoKineticEnergy);
DEFINE_GAMOS_DATA(GmDataSecoDividedInitialPrimKineticEnergy);
DEFINE_GAMOS_DATA(GmDataSecoKineticEnergy);
DEFINE_GAMOS_DATA(GmDataSecoDirTheta);
DEFINE_GAMOS_DATA(GmDataInitialPrimSecoAngle); 
DEFINE_GAMOS_DATA(GmDataFinalPrimSecoAngle); 
DEFINE_GAMOS_DATA(GmDataInitialPrimKineticEnergy);
DEFINE_GAMOS_DATA(GmDataFinalPrimKineticEnergy);
DEFINE_GAMOS_DATA(GmDataVertexPrimKineticEnergy);
DEFINE_GAMOS_DATA(GmDataPrimParticle);

DEFINE_GAMOS_DATA(GmDataSecoInitialDirTheta);

DEFINE_GAMOS_DATA(GmDataFinalDirNormalAngle);

DEFINE_GAMOS_DATA(GmDataNofHits);

#endif
