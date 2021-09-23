#include "G4MaterialPropertiesTable.hh"
#include "GmGeometryMessenger.hh"

#include "GmGeomVerbosity.hh"
#include "GmGeometryUtils.hh"
#include "GmTouchable.hh"
#include "GmOpticalPropertiesMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "GmUniformMagField.hh"
#include "GmUniformElectricField.hh"
#include "GmUniformEMField.hh"
#include "G4UniformMagField.hh"

#include "G4UIdirectory.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
    
#include "G4FieldManager.hh"
#include "G4Material.hh"
#include "G4GeometryManager.hh"
#include "G4tgrUtils.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmGeometryMessenger::GmGeometryMessenger()
{ 
  theMagFieldDir = new G4UIdirectory("/gamos/field/");
  theMagFieldDir->SetGuidance("Commands to define uniform magnetic, electric or electromagnetic fields");
  
  theMagFieldCmd = new GmUIcmdWithAString("/gamos/field/setMagField",this);  
  theMagFieldCmd->SetGuidance("Define magnetic field vector: FIELD_X FIELD_Y FIELD_Z");
  theMagFieldCmd->AvailableForStates(G4State_Idle);  

  theElecFieldCmd = new GmUIcmdWithAString("/gamos/field/setElecField",this);  
  theElecFieldCmd->SetGuidance("Define magnetic field vector: FIELD_X FIELD_Y FIELD_Z");
  theElecFieldCmd->AvailableForStates(G4State_Idle);  

  theEMFieldCmd = new GmUIcmdWithAString("/gamos/field/setEMField",this);  
  theEMFieldCmd->SetGuidance("Define magnetic field vector: FIELD_X FIELD_Y FIELD_Z");
  theEMFieldCmd->AvailableForStates(G4State_Idle);  

  theMagFieldLocalCmd = new GmUIcmdWithAString("/gamos/field/setLocalMagField",this);  
  theMagFieldLocalCmd->SetGuidance("Define magnetic field vector: FIELD_X FIELD_Y FIELD_Z");
  theMagFieldLocalCmd->AvailableForStates(G4State_Idle);

  theGeometryDir = new G4UIdirectory("/gamos/geometry/");
  theGeometryDir->SetGuidance("Commands to control geometry");
  
  theRefracIndexCmd = new GmUIcmdWithAString("/gamos/geometry/setRefractionIndex",this);  
  theRefracIndexCmd->SetGuidance("Set refraction index for a material:MATERIAL_NAME ENERGY_1 REFRACTION_INDEX_1 ENERGY_2 REFRACTION_INDEX_2 ... ");
  theRefracIndexCmd->AvailableForStates(G4State_Idle);  

  thePrintTouchablesCmd = new GmUIcmdWithAString("/gamos/geometry/printTouchables",this);  
  thePrintTouchablesCmd->SetGuidance("Print touchable information");
  thePrintTouchablesCmd->AvailableForStates(G4State_Idle);  

  thePrintVolumeTreeCmd = new G4UIcmdWithAnInteger("/gamos/geometry/printVolumeTree",this);  
  thePrintVolumeTreeCmd->SetGuidance("Print touchable information");
  thePrintVolumeTreeCmd->AvailableForStates(G4State_Idle);  

  thePrintMaterialsCmd = new G4UIcmdWithAnInteger("/gamos/geometry/printMaterials",this);
  thePrintMaterialsCmd->SetGuidance("Print materials");
  thePrintMaterialsCmd->AvailableForStates(G4State_Idle);

  theSetWorldMaxCmd = new G4UIcmdWithADouble("/gamos/geometry/SetWorldMaximumExtent",this);
  theSetWorldMaxCmd->SetGuidance("Set the world maximum extent (it will change the precision of the Geant4 navigation");
  theSetWorldMaxCmd->AvailableForStates(G4State_PreInit);

  theCmdMatePropertiesTable = new GmUIcmdWithAString("/gamos/opticalProperties/matePropertiesTable",this);  
  theCmdMatePropertiesTable->SetGuidance("Create a material properties table");
  theCmdMatePropertiesTable->AvailableForStates(G4State_PreInit);
  theCmdMatePTAddEnergies = new GmUIcmdWithAString("/gamos/opticalProperties/matePTAddEnergies",this);  
  theCmdMatePTAddEnergies->SetGuidance("Add energies to a material properties table");
  theCmdMatePTAddEnergies->AvailableForStates(G4State_PreInit);
  theCmdMatePTAddProperty = new GmUIcmdWithAString("/gamos/opticalProperties/matePTAddProperty",this);  
  theCmdMatePTAddProperty->SetGuidance("Add property to a material properties table");
  theCmdMatePTAddProperty->AvailableForStates(G4State_PreInit);
  theCmdMatePTAddConstProperty = new GmUIcmdWithAString("/gamos/opticalProperties/matePTAddConstProperty",this);  
  theCmdMatePTAddConstProperty->SetGuidance("Add const property to a material properties table");
  theCmdMatePTAddConstProperty->AvailableForStates(G4State_PreInit);
  theCmdAttachPT2Material = new GmUIcmdWithAString("/gamos/opticalProperties/attachPT2Material",this);  
  theCmdAttachPT2Material->SetGuidance("Attach a material properties table to a material");
  theCmdAttachPT2Material->AvailableForStates(G4State_PreInit);
  theCmdAttachPT2OpticalSurface = new GmUIcmdWithAString("/gamos/opticalProperties/attachPT2OpticalSurface",this);  
  theCmdAttachPT2OpticalSurface->SetGuidance("Attach a material properties table to an optical surface");
  theCmdAttachPT2OpticalSurface->AvailableForStates(G4State_PreInit);
  theCmdCreateOpticalSurface = new GmUIcmdWithAString("/gamos/opticalProperties/createOpticalSurface",this);  
  theCmdCreateOpticalSurface->SetGuidance("Create an optical surface");
  theCmdCreateOpticalSurface->AvailableForStates(G4State_PreInit);
  theCmdCreateLogicalBorderSurface = new GmUIcmdWithAString("/gamos/opticalProperties/createLogicalBorderSurface",this);  
  theCmdCreateLogicalBorderSurface->SetGuidance("Create a  logical border surface");
  theCmdCreateLogicalBorderSurface->AvailableForStates(G4State_PreInit);
  theCmdCreateLogicalSkinSurface = new GmUIcmdWithAString("/gamos/opticalProperties/createLogicalSkinSurface",this);  
  theCmdCreateLogicalSkinSurface->SetGuidance("Create a  logical skin surface");
  theCmdCreateLogicalSkinSurface->AvailableForStates(G4State_PreInit);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmGeometryMessenger::~GmGeometryMessenger()
{
  delete theMagFieldDir;
  delete theMagFieldCmd;
  delete theElecFieldCmd;
  delete theEMFieldCmd;
  delete theMagFieldLocalCmd;
  delete theRefracIndexCmd;
  delete theGeometryDir;
  delete thePrintTouchablesCmd;
  delete thePrintVolumeTreeCmd;
  delete thePrintMaterialsCmd;
  delete theSetWorldMaxCmd;

  delete theCmdMatePropertiesTable;
  delete theCmdMatePTAddEnergies;
  delete theCmdMatePTAddProperty;
  delete theCmdMatePTAddConstProperty;
  delete theCmdAttachPT2Material;
  delete theCmdAttachPT2OpticalSurface;
  delete theCmdCreateOpticalSurface;
  delete theCmdCreateLogicalBorderSurface;
  delete theCmdCreateLogicalSkinSurface;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmGeometryMessenger::SetNewValue(G4UIcommand* command,G4String newValues)
{ 
  if( command == theMagFieldCmd ) {
    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: FIELD_X FIELD_Y FIELD_Z"); 
    SetMagField(newValues);
  } else  if( command == theElecFieldCmd ) {
    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: FIELD_X FIELD_Y FIELD_Z"); 
    SetElecField(newValues);
  } else  if( command == theEMFieldCmd ) {
    GmGenUtils::CheckNWords(newValues,6,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 6 arguments:  MAG_FIELD_X MAG_FIELD_Y MAG_FIELD_Z ELEC_FIELD_X ELEC_FIELD_Y ELEC_FIELD_Z");
    SetEMField(newValues);
  } else  if( command == theMagFieldLocalCmd ) {
    SetMagFieldLocal(newValues);
  } 
  else if( command == theRefracIndexCmd ) {
    SetRefractionIndex( newValues );
  }
  else if( command == thePrintTouchablesCmd ) {
    PrintTouchables( newValues );
  }
  else if( command == thePrintVolumeTreeCmd ) {
    PrintVolumeTree( newValues );
  }
  else if( command == thePrintMaterialsCmd ) {
    PrintMaterials( newValues );
  }
  else if( command == theSetWorldMaxCmd ) {
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(GmGenUtils::GetValue(newValues));
  }
  
  else if( command == theCmdMatePropertiesTable ) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_EQ, " :MATE_PROPERTIES_TABLE");
    GmOpticalPropertiesMgr::GetInstance()->CreateMaterialPropertiesTable( wl );
  }
  else if( command == theCmdMatePTAddEnergies ) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    G4tgrUtils::CheckWLsize( wl, 3, WLSIZE_GE, " :MATEPT_ADD_ENERGIES");
    GmOpticalPropertiesMgr::GetInstance()->AddEnergiesToTable( wl );
  }
  else if( command == theCmdMatePTAddProperty ) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    G4tgrUtils::CheckWLsize( wl, 4, WLSIZE_GE, " :MATEPT_ADD_PROPERTY");
    GmOpticalPropertiesMgr::GetInstance()->AddPropertyToTable( wl );
  }
  else if( command == theCmdMatePTAddConstProperty ) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    G4tgrUtils::CheckWLsize( wl, 4, WLSIZE_EQ, " :MATEPT_ADD_CONST_PROPERTY");
    GmOpticalPropertiesMgr::GetInstance()->AddConstPropertyToTable( wl );
  }
  else if( command == theCmdAttachPT2Material ) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :MATEPT_ATTACH_TO_MATERIAL");
    //    G4cout << " GmGeometryMessenger:.theCmdAttachPT2Material " << wl[0] << " " << wl[1] << G4endl; //GDEB
    GmOpticalPropertiesMgr::GetInstance()->AttachTableToMaterial( wl );  }
  else if( command == theCmdAttachPT2OpticalSurface ) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :MATEPT_ATTACH_TO_OPTICAL_SURFACE");
    GmOpticalPropertiesMgr::GetInstance()->AttachTableToOpsurface( wl );
  }
  else if( command == theCmdCreateOpticalSurface ) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :OPTICAL_SURFACE");
    GmOpticalPropertiesMgr::GetInstance()->CreateOpticalSurface( wl );
  }
  else if( command == theCmdCreateLogicalBorderSurface ) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :LOGICAL_BORDER_SURFACE");
    GmOpticalPropertiesMgr::GetInstance()->CreateLogicalBorderSurface( wl );
  }
  else if( command == theCmdCreateLogicalSkinSurface ) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :LOGICAL_SKIN_SURFACE");
    GmOpticalPropertiesMgr::GetInstance()->CreateLogicalSkinSurface( wl );
  }
  
}

#include "G4TransportationManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmGeometryMessenger::SetMagField(const G4String& newValues)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );

  G4ThreeVector fieldVector( GmGenUtils::GetValue(wl[0]), GmGenUtils::GetValue(wl[1]), GmGenUtils::GetValue(wl[2]) );

  new GmUniformMagField( fieldVector );

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(infoVerb) ) G4cout << "GmGeometryMessenger::SetMagField: fieldVector " << fieldVector << G4endl;
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmGeometryMessenger::SetElecField(const G4String& newValues)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );

  G4ThreeVector fieldVector( GmGenUtils::GetValue(wl[0]), GmGenUtils::GetValue(wl[1]), GmGenUtils::GetValue(wl[2]) );

  new GmUniformElectricField( fieldVector );

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(infoVerb) ) G4cout << "GmGeometryMessenger::SetElecField: fieldVector " << fieldVector << G4endl;
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmGeometryMessenger::SetEMField(const G4String& newValues)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );

  G4ThreeVector magFieldVector( GmGenUtils::GetValue(wl[0]), GmGenUtils::GetValue(wl[1]), GmGenUtils::GetValue(wl[2]) );
  G4ThreeVector elecFieldVector( GmGenUtils::GetValue(wl[3]), GmGenUtils::GetValue(wl[4]), GmGenUtils::GetValue(wl[5]) );

  new GmUniformEMField( magFieldVector, elecFieldVector );

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(infoVerb) ) G4cout << "GmGeometryMessenger::SetEMField: magFieldVector " << magFieldVector << " elecFieldVector " << elecFieldVector << G4endl;
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmGeometryMessenger::SetMagFieldLocal(const G4String& newValues)
{
	
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );

  if( wl.size() < 4 ) {
    G4Exception("GmGeometryMessenger::SetMagFieldLocal",
		"Wrong number of arguments",
		FatalErrorInArgument,
		"command /gamos/field/setFieldLocal must have at least four arguments: FIELD_X FIELD_Y FIELD_Z LOGICAL_VOLUME_1 LOGICAL_VOLUME_2 ... LOGICAL_VOLUME_N");
  }

  G4ThreeVector fieldVector( GmGenUtils::GetValue(wl[0]), GmGenUtils::GetValue(wl[1]), GmGenUtils::GetValue(wl[2]) );

  G4UniformMagField* magField = new G4UniformMagField( fieldVector );

  G4FieldManager* fieldMgr = new G4FieldManager( magField );
  fieldMgr->CreateChordFinder( magField );

  // attach field manager to all logical volumes
  for( unsigned int ii = 3; ii < wl.size(); ii++ ) {
    std::vector<G4LogicalVolume*> LVs = GmGeometryUtils::GetInstance()->GetLogicalVolumes( wl[ii] );
    for( unsigned int jj = 0; jj < LVs.size(); jj++ ){
      LVs[jj]->SetFieldManager(fieldMgr, true );
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(infoVerb) ) G4cout << "GmGeometryMessenger::SetMagFieldLocal: fieldVector " << fieldVector << G4endl;
#endif
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmGeometryMessenger::SetRefractionIndex(const G4String& newValues)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );

  if( wl.size() < 3 || wl.size()%2 == 0 ) {
    G4Exception("GmGeometryMessenger::SetRefractionIndex",
		"Wrong number of arguments",
		FatalErrorInArgument,
		"command /gamos/geometry/setRefractionIndex must have an even number of arguments: MATERIAL_NAME ENERGY_1 REFRACTION_INDEX_1 ENERGY_2 REFRACTION_INDEX_2 ... ");
  }

  unsigned int nEntries = (wl.size()-1)/2;
  G4double* energies = new G4double[nEntries];
  for( unsigned int ii = 0; ii < nEntries; ii++ ){
    energies[ii] = GmGenUtils::GetValue(wl[1+ii*2]);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(infoVerb) ) G4cout << " GmGeometryMessenger::SetRefractionIndex:  add energy " << energies[ii] << G4endl;
#endif
  }
  
  G4double* rindices = new G4double[nEntries];
  for( unsigned int ii = 0; ii < nEntries; ii++ ){
    rindices[ii] = GmGenUtils::GetValue(wl[2+ii*2]);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(infoVerb) ) G4cout << " GmGeometryMessenger::SetRefractionIndex:  add rindex " << rindices[ii] <<G4endl;
#endif
  }
 
  std::vector<G4Material*> mates = GmG4Utils::GetG4MaterialList( wl[0], 1 );

  
  for( unsigned int ii = 0; ii < mates.size(); ii++ ){
    G4Material* mate = mates[ii];
    G4MaterialPropertiesTable* aMaterialPropertiesTable;
    if(!mate->GetMaterialPropertiesTable() ) {
      aMaterialPropertiesTable = new G4MaterialPropertiesTable();
      mate->SetMaterialPropertiesTable( aMaterialPropertiesTable );
    }
 #ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(infoVerb) ) G4cout << " GeometryMessenger::SetRefractionIndex: add rindex for material " << mate->GetName() << G4endl;
#endif 
    aMaterialPropertiesTable->AddProperty("RINDEX",energies,rindices,nEntries);
  }
}

void GmGeometryMessenger::PrintTouchables(const G4String& newValues)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
 
  for( unsigned int ii = 0; ii < wl.size(); ii++ ) {
    std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( wl[ii], true );
    for( unsigned int jj = 0; jj < touchs.size(); jj++ ) {
      G4cout << " TOUCHABLE " << *(touchs[jj]) << G4endl;
    }
  }

}


void GmGeometryMessenger::PrintVolumeTree(const G4String& newValues)
{
  G4int val = thePrintVolumeTreeCmd->GetNewIntValue(newValues);
  
  GmGeometryUtils::GetInstance()->DumpG4PVLVTree( G4cout, val );

}

void GmGeometryMessenger::PrintMaterials(const G4String& newValues)
{
  G4int val = thePrintMaterialsCmd->GetNewIntValue(newValues);
  
  GmGeometryUtils::GetInstance()->DumpG4PVLVTree( G4cout, val );

}
