#include "G4Run.hh"
#include "G4Track.hh"
#include "G4Event.hh"

#include "GmGenerHistosUA.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4EventManager.hh"

//#define DEBUG 

//----------------------------------------------------------------
GmGenerHistosUA::GmGenerHistosUA()
{
}

//----------------------------------------------------------------
void GmGenerHistosUA::InitialiseHistoNames()
{
  SetHistoNameAndNumber("gener", theFilters, theClassifier);
  BookHistos();

  theTimeOld = 0.;
}

//----------------------------------------------------------------
void GmGenerHistosUA::BookHistos()
{
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  // Create histogram for energy
  theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" KinEnergy",100,0.,1.,theHistoNumber+1);
  // Create histogram for position
  theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Position X",100,-1000,1000,theHistoNumber+11);
  theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Position Y",100,-1000,1000,theHistoNumber+12);
  theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Position Z",100,-1000,1000,theHistoNumber+13);
  theAnaMgr->CreateHisto2D(theHistoName+""+sepa+" Position XY",100,-1000,1000,100,-1000,1000,theHistoNumber+21);
  theAnaMgr->CreateHisto2D(theHistoName+""+sepa+" Position XZ",100,-1000,1000,100,-1000,1000,theHistoNumber+22);
  theAnaMgr->CreateHisto2D(theHistoName+""+sepa+" Position YZ",100,-1000,1000,100,-1000,1000,theHistoNumber+23);
  
  // Create histogram for direction angles
theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Angle theta",100,0.,180.,theHistoNumber+111);
theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Angle phi",100,-180.,180.,theHistoNumber+112);
  
  // Create histogram for time between events (source decays)
theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Time between source decays (ns)",100,0.,1000.,theHistoNumber+211);
  
}

//----------------------------------------------------------------
GmGenerHistosUA::~GmGenerHistosUA() 
{}

//----------------------------------------------------------------
void GmGenerHistosUA::BeginOfEventAction( const G4Event* anEvent )
{

  //---- Loop to all primary particles 
  G4int nvtx = anEvent->GetNumberOfPrimaryVertex();
  for( G4int ii = 0; ii < nvtx; ii++ ){
    G4PrimaryVertex* pvtx = anEvent->GetPrimaryVertex(ii);
    if( !pvtx ) break;
    G4int npart = pvtx->GetNumberOfParticle();
    for( G4int jj = 0; jj < npart; jj++ ){
      G4PrimaryParticle* ppart = pvtx->GetPrimary(jj);
      G4ThreeVector momv = ppart->GetMomentum();
      G4double mom = momv.mag();
      G4double mass = ppart->GetMass();
      G4double kinE = sqrt(mom*mom+mass*mass)-mass;
      G4double weight = ppart->GetWeight();
      //      G4cout << " GmGenerHistosUA::BeginOfEventAction( " << kinE << " " << mom << " " << mass << G4endl;
      theAnaMgr->GetHisto1(theHistoNumber+1)->Fill( kinE );
      G4ThreeVector pos = pvtx->GetPosition();
      theAnaMgr->GetHisto1(theHistoNumber+11)->Fill( pos.x(), weight );
      theAnaMgr->GetHisto1(theHistoNumber+12)->Fill( pos.y(), weight  );
      theAnaMgr->GetHisto1(theHistoNumber+13)->Fill( pos.z(), weight  );
      theAnaMgr->GetHisto2(theHistoNumber+21)->Fill( pos.x(),pos.y(), weight  );
      theAnaMgr->GetHisto2(theHistoNumber+22)->Fill( pos.x(),pos.z(), weight  );
      theAnaMgr->GetHisto2(theHistoNumber+23)->Fill( pos.y(),pos.z(), weight  );
      theAnaMgr->GetHisto1(theHistoNumber+111)->Fill( momv.theta()/CLHEP::deg, weight  );
      theAnaMgr->GetHisto1(theHistoNumber+112)->Fill( momv.phi()/CLHEP::deg, weight  );
    }
    if( ii == 0 )  { //take event time as that of the first particle
      G4double timeDiff = pvtx->GetT0() - theTimeOld;
      //      G4cout << "GmGenerHistosUA timenew " << pvtx->GetT0() << " old " << theTimeOld << " diff " << timeDiff << G4endl;
      theTimeOld = pvtx->GetT0();
      theAnaMgr->GetHisto1(theHistoNumber+211 )->Fill( timeDiff ); 
    }
  } 

}
