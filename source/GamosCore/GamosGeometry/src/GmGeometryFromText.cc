#include "GmGeometryFromText.hh"
#include <vector>
#include <map>
#include "GmGeomTextDetectorBuilder.hh"
#include "GmGeometryUtils.hh"
#include "GmGeometryFactory.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4tgbVolumeMgr.hh"
#include "G4tgbParallelGeomMgr.hh"
#include "GmParallelWorldCreator.hh"
#include "G4tgrMessenger.hh"
#include "G4VUserParallelWorld.hh"

#ifdef ROOT5
#include "TROOT.h"
#include "TPluginManager.h"
#endif

GmGeometryFromText::GmGeometryFromText()
{
  new G4tgrMessenger;
#ifdef ROOT5
  gROOT->GetPluginManager()->AddHandler("G4VUserDetectorConstruction", "^geomtext:", "GmGeometryFromText",	
			 "GamosCore_GamosGeometry", "GmGeometryFromText()");
#endif
  theClassName = "GmGeometryFromText";
}


//---------------------------------------------------------------------
G4VPhysicalVolume* GmGeometryFromText::Construct()
{
  //------------------------------------------------ 
  // Get the file names 
  //------------------------------------------------ 
  G4String filename = GmParameterMgr::GetInstance()->GetStringValue(theClassName+":FileName", "");
  
  if( filename == "" ) {
    G4Exception("GeometryFromText::Construct",
		"Wrong argument",
		FatalErrorInArgument,
		("filename is not defined, please use '/gamos/setParam "+theClassName+":FileName MU_FILE_NAME'").c_str());
  }

  filename = GmGenUtils::FileInPath( filename );

  //------------------------------------------------ 
  // Read the text files 
  //------------------------------------------------ 
  GmGeomTextDetectorBuilder* gtb = new GmGeomTextDetectorBuilder;
  G4tgbVolumeMgr* volmgr = G4tgbVolumeMgr::GetInstance();
  volmgr->SetDetectorBuilder( gtb );
  volmgr->AddTextFile(filename);

  std::vector<G4String> fnamepar;
  fnamepar = GmParameterMgr::GetInstance()->GetVStringValue(theClassName+":FileNameParallel", fnamepar);
  if( fnamepar.size() != 0 ){
    if( fnamepar.size() == 1 ){
      volmgr->AddTextFileParallel(fnamepar[0],1);
    } else {
      if( fnamepar.size()%2 != 0 ){
	G4Exception("GmGeometryFromText::Construct",
		    "Error in number of arguments of parameter 'GmGeometryFromText:FileNameParallel'",
		    FatalException,
		    (G4String("There should be a multiple of two: FILE_NAME_1 PARALLEL_WORLD_NUMBER_1 FILE_NAME_2 PARALLEL_WORLD_NUMBER_2 ..., there are ")
		     +GmGenUtils::itoa(fnamepar.size())).c_str());
      } 
      for( unsigned int ii = 0; ii < fnamepar.size(); ii+=2 ) {
	volmgr->AddTextFileParallel(fnamepar[ii],GmGenUtils::GetInteger(fnamepar[ii+1]));
      }
    }

  }

  //-  const G4tgrVolume* tgrVoltop = volmgr->ReadDetector();
  //-  G4VPhysicalVolume* physiWorld = volmgr->ConstructDetector(tgrVoltop);
  const G4tgrVolume* tgrVoltop = gtb->ReadDetector();
 //------------------------------------------------ 
  // Construct the Geant4 parallel worlds
  //------------------------------------------------ 
  G4tgbParallelGeomMgr* parallelMgr = G4tgbParallelGeomMgr::GetInstance();
  parallelMgr->SetWorldCreator( new GmParallelWorldCreator );
  std::vector<G4VUserParallelWorld*> parallelWorlds = parallelMgr->CreateParalleWorlds();
  for( unsigned int ii = 0; ii < parallelWorlds.size(); ii++ ) {
    G4cout << " RegisterParallelWorld " << parallelWorlds[ii]->GetName() << G4endl;
    RegisterParallelWorld( parallelWorlds[ii] );  
  }

  //------------------------------------------------ 
  // Build the Geant4 mass geometry
  //------------------------------------------------ 

  thePhysiWorld = gtb->ConstructDetectorGAMOS(tgrVoltop, -1, 1 );

  //  G4VPhysicalVolume* physiWorld = volmgr->ReadAndConstructDetector();

  return thePhysiWorld;
}
