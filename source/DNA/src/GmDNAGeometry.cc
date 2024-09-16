#include "GmDNAGeometry.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4MoleculeGun.hh"
#include "G4tgbVolumeMgr.hh"
//#include "G4tgbParallelGeomMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeomTextDetectorBuilder.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
//#include "GamosCore/GamosGeometry/include/GmParallelWorldCreator.hh"
#include "G4tgrMessenger.hh"
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmDNAGeometry::GmDNAGeometry() 
  : GmGeometryFromText()
{
  new G4tgrMessenger;

  fpGmDNAFabricParser.reset(new GmDNAFabricParser());

  G4String fileName = GmParameterMgr::GetInstance()->GetStringValue("GmDNAGeometry:fba2g4dnaFileName","VoxelStraight.fab2g4dna");
  if( fileName == "" ) {
    G4Exception("GmDNAGeometry::GmDNAGeometry",
		"",
		FatalException,
		"No fab2g4 file, please use parameter '/P GmDNAGeometry:fba2g4dnaFileName <FILE_NAME>");
  }
  fpGmDNAFabricParser->ParseFile(fileName);
  fpGun = fpGmDNAFabricParser->ReleaseMoleculeGun();

  //  this->RegisterParallelWorld(new DNADamParallelWorld("ChemistryWorld"));

  theClassName = "GmDNAGeometry";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmDNAGeometry::~GmDNAGeometry()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VPhysicalVolume* GmDNAGeometry::Construct()
{
  GmGeometryFromText::Construct();

  //---- DNA parser
  G4LogicalVolume* pLogicStraightVoxel = fpGmDNAFabricParser->CreateLogicVolume();
  fGeometryMap = fpGmDNAFabricParser->ReleaseGeoData();
 
  G4LogicalVolume* logiWorld = thePhysiWorld->GetLogicalVolume();
  G4VPhysicalVolume* pPhyVoxel = new G4PVPlacement(0, 
                                                    G4ThreeVector(),
                                                    pLogicStraightVoxel, 
                                                    "VoxelStraight",
                                                    logiWorld,
                                                    false,
                                                    0);

  fGeometryMap.insert(std::make_pair(thePhysiWorld,
                                       GmDNAFabricVolumeType::physWorld));
  fGeometryMap.insert(std::make_pair(pPhyVoxel,
                                       GmDNAFabricVolumeType::VoxelStraight));

  return thePhysiWorld;
}
