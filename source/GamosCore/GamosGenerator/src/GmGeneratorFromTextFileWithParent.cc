#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4RunManager.hh"
#include "globals.hh"

#include "GmGeneratorFromTextFileWithParent.hh"
#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

//------------------------------------------------------------------------
GmGeneratorFromTextFileWithParent::GmGeneratorFromTextFileWithParent()
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance(); 
  G4String fname = parmgr->GetStringValue("GmGeneratorFromTextFileWithParent:FileName", "generator.txt");
  theFileIn = GmFileIn::GetInstance(fname);
  theFileIn.SetSeparator(',');

  thePrevData.eventID = -1;
  theNLinesRead = 0;

  theNEventsToSkip = GmParameterMgr::GetInstance()->GetNumericValue("GmGeneratorFromTextFileWithParent:NEventsToSkip",0);

  theGoodParticle = parmgr->GetStringValue("GmGeneratorFromTextFileWithParent:GoodParticle", "");
}


//------------------------------------------------------------------------
GmGeneratorFromTextFileWithParent::~GmGeneratorFromTextFileWithParent()
{
  theFileIn.Close();
}

//------------------------------------------------------------------------
void GmGeneratorFromTextFileWithParent::GeneratePrimaries(G4Event* evt)
{
  if( theFileIn.eof() ) {
    G4RunManager::GetRunManager()->AbortRun();  //  /abortrun
  }

  //P  22 18.7405 -15.5304 -11.4193 0.85396 -0.625037 -0.711131 0  
 
  if( thePrevData.eventID != -1 ) {
    GenerateParticle(thePrevData, evt);
  }
  
  GenerTextFileDataWP evtDat;
  std::vector<G4String> wl;
  for( ;; ){
    //  for( G4int ii = 0; ii < ntrk; ii++ ){
    //read it the fast way
    if( !theFileIn.GetWordsInLine(wl) ) {
      G4RunManager::GetRunManager()->AbortRun();  //  abortrun
      break;
    } 
    if( wl.size() == 0 ) {
      G4RunManager::GetRunManager()->AbortRun();  //  abortrun
      break;
    } 
    theNLinesRead ++;
    if( wl.size() != 11 ) {
      G4Exception("GmGeneratorFromTextFileWithParent::GeneratePrimaries",
		  G4String("Wrong number of words in line "+GmGenUtils::itoa(theNLinesRead)).c_str(),
		  FatalErrorInArgument,
		  (G4String("Number of words is ") + GmGenUtils::itoa(wl.size())).c_str());
    }

    evtDat.eventID = GmGenUtils::GetInteger(wl[0]);
    G4String parentPartName = wl[1];
    if( theGoodParticle != "" && theGoodParticle != parentPartName ) continue;
    evtDat.partName = wl[2];
    evtDat.posx = GmGenUtils::GetValue(wl[3]);
    evtDat.posy = GmGenUtils::GetValue(wl[4]);
    evtDat.posz = GmGenUtils::GetValue(wl[5]);
    evtDat.momx = GmGenUtils::GetValue(wl[6]);
    evtDat.momy = GmGenUtils::GetValue(wl[7]);
    evtDat.momz = GmGenUtils::GetValue(wl[8]); 
    evtDat.time = GmGenUtils::GetValue(wl[9]);
    evtDat.weight = GmGenUtils::GetValue(wl[10]);
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << evtDat.eventID 
				      << " " << evtDat.partName 
				      << " " << evtDat.posx 
				      << " " << evtDat.posy 
				      << " " << evtDat.posz 
				      << " " << evtDat.momx 
				      << " " << evtDat.momy 
				      << " " << evtDat.momz 
				      << " " << evtDat.time 
				      << " " << evtDat.weight 
				      << G4endl;
#endif
    if( thePrevData.eventID == -1 || thePrevData.eventID == evtDat.eventID ) {
      if( evtDat.eventID >= theNEventsToSkip ) {
	GenerateParticle( evtDat, evt ); 
	if( thePrevData.eventID == -1 ) {
	  thePrevData = evtDat;
	}
      }
    } else {
      break;  // generate particle in next event
    }

  }

  thePrevData = evtDat;

}
  
void GmGeneratorFromTextFileWithParent::GenerateParticle( const GenerTextFileDataWP& evtDat, G4Event* evt )
{
  G4PrimaryVertex* vtx = new G4PrimaryVertex( G4ThreeVector(evtDat.posx,evtDat.posy,evtDat.posz), evtDat.time );
  evt->AddPrimaryVertex( vtx );
  G4ParticleDefinition* partDef = GmGetParticleMgr::GetInstance()->GetG4Particle( evtDat.partName, false );
  if( !partDef ) {
    GmGetParticleMgr::GetInstance()->CreateIon( evtDat.partName );
    partDef = GmGetParticleMgr::GetInstance()->GetG4Particle( evtDat.partName, true );
  }

#ifndef GAMOS_NO_VERBOSE
  if( !partDef ) if( GenerVerb(debugVerb) ) G4cout << " partDef " << partDef << " partName " << evtDat.partName << G4endl;
#endif
  G4PrimaryParticle* particle = new G4PrimaryParticle( partDef, evtDat.momx, evtDat.momy, evtDat.momz);
  particle->SetWeight( evtDat.weight );  

  vtx->SetPrimary(particle);

}