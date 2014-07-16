#include "Reflex/PluginService.h"

//--- POSITION DATA
#include "GmDataInitialPosX.hh"
#include "GmDataInitialPosY.hh"
#include "GmDataInitialPosZ.hh"
#include "GmDataInitialPosMag.hh"
#include "GmDataInitialPosPerp.hh"
#include "GmDataInitialPosPhi.hh"
#include "GmDataInitialPosTheta.hh"
#include "GmDataFinalPosX.hh"
#include "GmDataFinalPosY.hh"
#include "GmDataFinalPosZ.hh"
#include "GmDataFinalPosMag.hh"
#include "GmDataFinalPosPerp.hh"
#include "GmDataFinalPosPhi.hh"
#include "GmDataFinalPosTheta.hh"
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

PLUGINSVC_FACTORY(GmDataInitialPosX,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosY,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPosTheta,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPosTheta,GmVData*())
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
#include "GmDataInitialLocalPosX.hh"
#include "GmDataInitialLocalPosY.hh"
#include "GmDataInitialLocalPosZ.hh"
#include "GmDataInitialLocalPosMag.hh"
#include "GmDataInitialLocalPosPerp.hh"
#include "GmDataInitialLocalPosPhi.hh"
#include "GmDataInitialLocalPosTheta.hh"
PLUGINSVC_FACTORY(GmDataInitialLocalPosX,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosY,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalPosTheta,GmVData*())

#include "GmDataFinalLocalPosX.hh"
#include "GmDataFinalLocalPosY.hh"
#include "GmDataFinalLocalPosZ.hh"
#include "GmDataFinalLocalPosMag.hh"
#include "GmDataFinalLocalPosPerp.hh"
#include "GmDataFinalLocalPosPhi.hh"
#include "GmDataFinalLocalPosTheta.hh"
PLUGINSVC_FACTORY(GmDataFinalLocalPosX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalPosTheta,GmVData*())

#include "GmDataFinalLocalInPrePosX.hh"
#include "GmDataFinalLocalInPrePosY.hh"
#include "GmDataFinalLocalInPrePosZ.hh"
#include "GmDataFinalLocalInPrePosMag.hh"
#include "GmDataFinalLocalInPrePosPerp.hh"
#include "GmDataFinalLocalInPrePosPhi.hh"
#include "GmDataFinalLocalInPrePosTheta.hh"
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosMag,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPrePosTheta,GmVData*())

//-- Position change
#include "GmDataPosChangeX.hh"
#include "GmDataPosChangeY.hh"
#include "GmDataPosChangeZ.hh"
#include "GmDataPosChangeMag.hh"
#include "GmDataPosChangePerp.hh"
#include "GmDataPosChangePhi.hh"
#include "GmDataPosChangeTheta.hh"

PLUGINSVC_FACTORY(GmDataPosChangeX,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangeY,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangeZ,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangeMag,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangePerp,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangePhi,GmVData*())
PLUGINSVC_FACTORY(GmDataPosChangeTheta,GmVData*())

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

#include "GmDataMomChangeX.hh"
#include "GmDataMomChangeY.hh"
#include "GmDataMomChangeZ.hh"
#include "GmDataMomChangeMag.hh"
#include "GmDataMomChangePerp.hh"
#include "GmDataMomChangePhi.hh"
#include "GmDataMomChangeTheta.hh"

PLUGINSVC_FACTORY(GmDataMomChangeX,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangeY,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangeZ,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangeMag,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangePerp,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangePhi,GmVData*())
PLUGINSVC_FACTORY(GmDataMomChangeTheta,GmVData*())

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

#include "GmDataDirChangeX.hh"
#include "GmDataDirChangeY.hh"
#include "GmDataDirChangeZ.hh"
#include "GmDataDirChangeMag.hh"
#include "GmDataDirChangePerp.hh"
#include "GmDataDirChangePhi.hh"
#include "GmDataDirChangeTheta.hh"

PLUGINSVC_FACTORY(GmDataDirChangeX,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangeY,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangeZ,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangeMag,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangePerp,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangePhi,GmVData*())
PLUGINSVC_FACTORY(GmDataDirChangeTheta,GmVData*())

//-- Local direction
#include "GmDataInitialLocalDirX.hh"
#include "GmDataInitialLocalDirY.hh"
#include "GmDataInitialLocalDirZ.hh"
#include "GmDataInitialLocalDirPerp.hh"
#include "GmDataInitialLocalDirPhi.hh"
#include "GmDataInitialLocalDirTheta.hh"
PLUGINSVC_FACTORY(GmDataInitialLocalDirX,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalDirY,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalDirZ,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalDirPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalDirPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalDirTheta,GmVData*())

#include "GmDataFinalLocalDirX.hh"
#include "GmDataFinalLocalDirY.hh"
#include "GmDataFinalLocalDirZ.hh"
#include "GmDataFinalLocalDirPerp.hh"
#include "GmDataFinalLocalDirPhi.hh"
#include "GmDataFinalLocalDirTheta.hh"
PLUGINSVC_FACTORY(GmDataFinalLocalDirX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalDirY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalDirZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalDirPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalDirPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalDirTheta,GmVData*())

#include "GmDataFinalLocalInPreDirX.hh"
#include "GmDataFinalLocalInPreDirY.hh"
#include "GmDataFinalLocalInPreDirZ.hh"
#include "GmDataFinalLocalInPreDirPerp.hh"
#include "GmDataFinalLocalInPreDirPhi.hh"
#include "GmDataFinalLocalInPreDirTheta.hh"
PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirY,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirZ,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirPerp,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirPhi,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalInPreDirTheta,GmVData*())

#include "GmDataAngleChange.hh"
PLUGINSVC_FACTORY(GmDataAngleChange,GmVData*())

//--- ENERGY DATA
#include "GmDataInitialKineticEnergy.hh"
#include "GmDataFinalKineticEnergy.hh"
#include "GmDataVertexKineticEnergy.hh"
#include "GmDataInitialTotalEnergy.hh"
#include "GmDataFinalTotalEnergy.hh"
#include "GmDataAccumulatedEnergyLost.hh"
#include "GmDataAccumulatedEnergyDeposited.hh"
#include "GmDataKineticEnergyChange.hh"
#include "GmDataAccumulatedDose.hh"
#include "GmDataAccumulatedKerma.hh"
#include "GmDataInitialDEDX.hh"
#include "GmDataFinalDEDX.hh"
#include "GmDataInitialNeutronEquivalentDose.hh"
#include "GmDataFinalGammaEquivalentDose.hh"
#include "GmDataInitialGammaEquivalentDose.hh"
#include "GmDataFinalNeutronEquivalentDose.hh"
#include "GmDataInitialNeutronEnergyToEquivalentDose.hh"
#include "GmDataInitialGammaEnergyToEquivalentDose.hh"
#include "GmDataFinalNeutronEnergyToEquivalentDose.hh"
#include "GmDataFinalGammaEnergyToEquivalentDose.hh"

PLUGINSVC_FACTORY(GmDataInitialKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataVertexKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialTotalEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalTotalEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataAccumulatedEnergyLost,GmVData*())
PLUGINSVC_FACTORY(GmDataAccumulatedEnergyDeposited,GmVData*())
PLUGINSVC_FACTORY(GmDataKineticEnergyChange,GmVData*())
PLUGINSVC_FACTORY(GmDataAccumulatedDose,GmVData*())
PLUGINSVC_FACTORY(GmDataAccumulatedKerma,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialDEDX,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalDEDX,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialNeutronEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalGammaEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialGammaEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalNeutronEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialNeutronEnergyToEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialGammaEnergyToEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalNeutronEnergyToEquivalentDose,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalGammaEnergyToEquivalentDose,GmVData*())

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

//--- PARTICLE AND PROCESS DATA
#include "GmDataParticle.hh"
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

PLUGINSVC_FACTORY(GmDataParticle,GmVData*())
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
#include "GmDataTrackLength.hh"
#include "GmDataTrackID.hh"
#include "GmDataParentTrackID.hh"
#include "GmDataEventID.hh"
#include "GmDataRunID.hh"
#include "GmDataStepNumber.hh"
#include "GmDataInitialWeight.hh"
#include "GmDataFinalWeight.hh"
#include "GmDataAccumulatedLength.hh"
#include "GmDataInitialTime.hh"
#include "GmDataFinalTime.hh"
#include "GmDataInitialLocalTime.hh"
#include "GmDataFinalLocalTime.hh"
#include "GmDataInitialProperTime.hh"
#include "GmDataFinalProperTime.hh"

PLUGINSVC_FACTORY(GmDataTrackLength,GmVData*())
PLUGINSVC_FACTORY(GmDataTrackID,GmVData*())
PLUGINSVC_FACTORY(GmDataParentTrackID,GmVData*())
PLUGINSVC_FACTORY(GmDataEventID,GmVData*())
PLUGINSVC_FACTORY(GmDataRunID,GmVData*())
PLUGINSVC_FACTORY(GmDataStepNumber,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialWeight,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalWeight,GmVData*())
PLUGINSVC_FACTORY(GmDataAccumulatedLength,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialTime,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalTime,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialLocalTime,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalLocalTime,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialProperTime,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalProperTime,GmVData*())

//---- SECONDARY DATA
#include "GmDataNofSecondaries.hh"
#include "GmDataSumSecoKineticEnergy.hh"
#include "GmDataInitialPrimMinusSecoKineticEnergy.hh"
#include "GmDataFinalPrimMinusSecoKineticEnergy.hh"
#include "GmDataSecoDividedInitialPrimKineticEnergy.hh"  
#include "GmDataSecoKineticEnergy.hh"
#include "GmDataInitialPrimSecoAngle.hh"        
#include "GmDataFinalPrimSecoAngle.hh"        
#include "GmDataInitialPrimKineticEnergy.hh"
#include "GmDataFinalPrimKineticEnergy.hh"
#include "GmDataPrimParticle.hh"

PLUGINSVC_FACTORY(GmDataNofSecondaries,GmVData*())
PLUGINSVC_FACTORY(GmDataSumSecoKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPrimMinusSecoKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPrimMinusSecoKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataSecoDividedInitialPrimKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataSecoKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataInitialPrimSecoAngle,GmVData*()) 
PLUGINSVC_FACTORY(GmDataFinalPrimSecoAngle,GmVData*()) 
PLUGINSVC_FACTORY(GmDataInitialPrimKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataFinalPrimKineticEnergy,GmVData*())
PLUGINSVC_FACTORY(GmDataPrimParticle,GmVData*())

