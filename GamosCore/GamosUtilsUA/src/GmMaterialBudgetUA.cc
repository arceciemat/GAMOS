#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Track.hh"

#include "GmMaterialBudgetUA.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"


//----------------------------------------------------------------
GmMaterialBudgetUA::GmMaterialBudgetUA()
{
}

void GmMaterialBudgetUA::InitialiseHistoNames()
{
  SetHistoNameAndNumber("matbud",theFilters,theClassifier);
}

//----------------------------------------------------------------
void GmMaterialBudgetUA::BeginOfRunAction( const G4Run* )
{
}

 
//----------------------------------------------------------------
void GmMaterialBudgetUA::BookHistos(G4int index)
{
  theHistos.insert(index);

  G4String hname;
  if( theClassifier ) hname += theClassifier->GetIndexName(index);
  G4String histoName = theHistoName + "_" + hname;
  G4int histoNumber = theHistoNumber + index*100;
  // Create histograms
  theAnaMgr->CreateHistoProfile1D(histoName+": Position X",100,-1000.,1000.,histoNumber+11);
  theAnaMgr->CreateHistoProfile1D(histoName+": Position Y",100,-1000.,1000.,histoNumber+12);
  theAnaMgr->CreateHistoProfile1D(histoName+": Position Z",100,-1000.,1000.,histoNumber+13);
  //  theAnaMgr->CreateHistoProfile2D(histoName+": Position XY",theHPosNbins,theHPosMin,theHPosMax,theHPosNbins,theHPosMin,theHPosMax,histoNumber+21);
  // theAnaMgr->CreateHistoProfile2D(histoName+": Position XZ",theHPosNbins,theHPosMin,theHPosMax,theHPosNbins,theHPosMin,theHPosMax,histoNumber+22);
  //  theAnaMgr->CreateHistoProfile2D(histoName+": Position YZ",theHPosNbins,theHPosMin,theHPosMax,theHPosNbins,theHPosMin,theHPosMax,histoNumber+23);
}


//----------------------------------------------------------------
void GmMaterialBudgetUA::UserSteppingAction(const G4Step* aStep )
{
  G4int index;
  if (theClassifier ) {
    index = theClassifier->GetIndexFromStep(aStep);
  } else {
    index = 0;
  }
  if( theHistos.find(index) == theHistos.end() ) BookHistos(index);

  //  G4cout << " COPY " << aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(0) << G4endl;
  G4double matBud = 1./aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetMaterial()->GetRadlen() * 
    aStep->GetStepLength();
  //  G4cout << " MATBUD " << matBud  << " " << theMatBud[index] << " " << aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetMaterial()->GetRadlen()  << " " << aStep->GetStepLength() << " " << aStep->GetTrack()->GetPosition().z() << G4endl;
  //  G4cout << " mate " << aStep->GetTrack()->GetVolume()->GetName() << " " << aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName() << G4endl;
  if(matBud > 1.e30) matBud = 0.;

  theMatBud[index] += matBud;
}

//----------------------------------------------------------------
void GmMaterialBudgetUA::PostUserTrackingAction(const G4Track* aTrack )
{

  std::map<G4int,G4double>::const_iterator ite;
  for( ite = theMatBud.begin(); ite != theMatBud.end(); ite++) {
    G4int index = (*ite).first;
    G4int histoNumber = theHistoNumber + index*100;
    G4ThreeVector pos = aTrack->GetPosition();

    //    G4cout << "MATBUDTRK " << pos << " " << (*ite).second << G4endl;
    theAnaMgr->GetHistoProf1(histoNumber+11)->Fill(pos.x(),(*ite).second);
    theAnaMgr->GetHistoProf1(histoNumber+12)->Fill(pos.y(),(*ite).second);
    theAnaMgr->GetHistoProf1(histoNumber+13)->Fill(pos.z(),(*ite).second);
    //    theAnaMgr->GetHistoProf2(histoNumber+21)->Fill(pos.x(),pos.y(),(*ite).second);
    // theAnaMgr->GetHistoProf2(histoNumber+22)->Fill(pos.x(),pos.z(),(*ite).second);
    // theAnaMgr->GetHistoProf2(histoNumber+23)->Fill(pos.y(),pos.z(),(*ite).second);
  }

  theMatBud.clear();

}


//----------------------------------------------------------------
void GmMaterialBudgetUA::EndOfRunAction( const G4Run* )
{
}
