#include "GmMicroyzG4UA.hh"
#include "GmDNAVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

#include "Randomize.hh"
#include "G4SDManager.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmMicroyzG4UA::GmMicroyzG4UA()
  : GmVMicroyzUA()
{
  theName = "GmMicroyzG4UA";
  theSphereRadius = theParamMgr->GetNumericValue(GetName()+":SphereRadius",5*nm);
  theVolumeChord = 4.*theSphereRadius/3;
  G4double volumeDensity = 1 * g/cm3; // G4_WATER
  theVolumeMass = (4./3)*CLHEP::pi*theSphereRadius*theSphereRadius*theSphereRadius*volumeDensity;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmMicroyzG4UA::BookHistos()
{
  if(bHistos) {
    GmVMicroyzUA::BookHistos();
    G4cout << "  GmVMicroyzUA::BookHistos() " << G4endl; //GDEB   
    theNHistosV = theAnaMgr->GetHistos1D().size();
    G4cout << "1  GmVMicroyzUA::BookHistos() " << theNHistosV << G4endl; //GDEB   
    G4double Eincident = 10*keV;
    theAnaMgr->CreateHisto1D("N_hits",140,80*Eincident/(1*keV),150*Eincident/(1*keV),theHistoNumber+theNHistosV+1);
    theAnaMgr->CreateHisto1D("N_scored_hits",100,0.,100,theHistoNumber+theNHistosV+2);
    //    theAnaMgr->CreateHisto1D("E_incident",100,Eincident*0.9,Eincident*1.1,theHistoNumber+theNHistos+3);

  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmMicroyzG4UA::UserSteppingAction(const G4Step* aStep)
{
  G4double edep = aStep->GetTotalEnergyDeposit();

  if (edep==0.) return;

  GmMicroyzHit* newHit = new GmMicroyzHit();
  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetEdep(edep);
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());

  if (aStep->GetTrack()->GetTrackID()==1 && aStep->GetTrack()->GetParentID()==0)
    newHit->SetIncidentEnergy(aStep->GetTrack()->GetVertexKineticEnergy());

  theHits.push_back( newHit );

  //newHit->Print();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmMicroyzG4UA::EndOfEventAction(const G4Event* evt)
{

  G4int nofHits = theHits.size();

  // PROCESSING OF MICRODOSIMETRY Y & Z SPECTRA
  //
 
  //***************
  // y and z
  //***************
  
  // select random hit
  G4int randHit = 0; // Runs from 0 to number of hits - 1
  randHit = static_cast<G4int>( G4UniformRand()*nofHits );
    
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) G4cout << "======> random selection of hit number randHit ="  << randHit << G4endl;
#endif
  
  // get selected random hit position
  G4ThreeVector hitPos =  theHits[randHit]->GetPos();
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) G4cout << "======> random hit position /nm =" << hitPos/nm << G4endl; 
#endif
  
  // random placement of sphere: method 1
  /*  
  G4ThreeVector randDir = G4RandomDirection();
  G4double randRadius = G4UniformRand()*theSphereRadius;
  G4ThreeVector randCenterPos = randRadius*randDir + hitPos;
  */  

  // random placement of sphere: method 2
  G4double xRand = 1.01*theSphereRadius;
  G4double yRand = 1.01*theSphereRadius;
  G4double zRand = 1.01*theSphereRadius;
  G4double randRad = 1.01*theSphereRadius;
  do
  {
    xRand = (2*G4UniformRand()-1)*theSphereRadius;
    yRand = (2*G4UniformRand()-1)*theSphereRadius;
    zRand = (2*G4UniformRand()-1)*theSphereRadius;
    randRad = std::sqrt( xRand*xRand+yRand*yRand+zRand*zRand );
  }
  while (randRad>theSphereRadius);

  G4ThreeVector 
    randCenterPos(xRand+hitPos.x(),yRand+hitPos.y(),zRand+hitPos.z());
 
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) G4cout << " GmMicroG4UA randCenterPos " << randCenterPos << " nofHits " << nofHits << G4endl;
#endif

  // search for neighbouring hits in the sphere and cumulate deposited energy 
  //  in epsilon
  G4double epsilon = 0;
  G4int nbEdep = 0;
  
  for ( G4int ii=0; ii<nofHits; ii++ ) 
  { 
    GmMicroyzHit* hit = theHits[ii];

    G4ThreeVector localPos = hit->GetPos();
    
    if ( 
        (localPos.x()-randCenterPos.x()) * (localPos.x()-randCenterPos.x()) +
        (localPos.y()-randCenterPos.y()) * (localPos.y()-randCenterPos.y()) +
        (localPos.z()-randCenterPos.z()) * (localPos.z()-randCenterPos.z()) 
         <= theSphereRadius*theSphereRadius
       ) 
       
    { 
      epsilon = epsilon + hit->GetEdep() ;
      nbEdep = nbEdep+1;

#ifndef GAMOS_NO_VERBOSE
      if( DNAVerb(testVerb) ) G4cout << ii << " GmMicroG4UA near hist " << epsilon << " n=" << nbEdep << G4endl; 
      //  if( DNAVerb(-debugVerb) ) G4cout << " GmMicroG4UA near hist " << localPos/nm << " " << randCenterPos/nm << G4endl;  //GDEB
#endif
    }
       
  }

  G4double y = (epsilon/eV)/(theVolumeChord/nm);
  G4double z = (epsilon/theVolumeMass)/gray;
  /*?? // normalize to number of hits (to ensure a spatially uniform sampling)
    G4cout << " Y=" << y << " Z="<< z << G4endl; //GDEB
  G4cout << "WEIGHTD Y=" << y << " Z="<< z << " nbEdep " << nbEdep << "/"<<fHitsCollection->GetSize()<< G4endl; //GDEB*/
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) G4cout << " GmMicroG4UA y= " << y << " z= " << z << G4endl;
#endif

  if( bTextFile ) {
    *theFileOut << nofHits << "," << nbEdep << ","
		<< y << "," << z << ","
		<< G4endl;
  } else if( bBinFile ) {
    *theFileOut << nofHits << nbEdep 
		<< y << z ;
  }

  double wei = (float(nbEdep)/theHits.size());
  if(bHistos) {
    theAnaMgr->GetHisto1(theHistoNumber+theNHistosV+1)->Fill(nofHits,wei);
    theAnaMgr->GetHisto1(theHistoNumber+theNHistosV+2)->Fill(nbEdep,wei);
    //    theAnaMgr->GetHisto1(theHistoNumber+theNHistosV+3)->Fill(Einc,wei);
    if( theAnaMgr->GetHisto1(theHistoNumber+1,false) ) 
	theAnaMgr->GetHisto1(theHistoNumber+1)->Fill(y,wei);
    if( theAnaMgr->GetHisto1(theHistoNumber+2,false) )
      theAnaMgr->GetHisto1(theHistoNumber+2)->Fill(z,wei);
    if( theAnaMgr->GetHisto1(theHistoNumber+3,false) )
      theAnaMgr->GetHisto1(theHistoNumber+3)->Fill(y*y,wei);
    if( theAnaMgr->GetHisto1(theHistoNumber+4,false) )
      theAnaMgr->GetHisto1(theHistoNumber+4)->Fill(z*z,wei);
    if( theAnaMgr->GetHisto1(theHistoNumber+5,false) )
      theAnaMgr->GetHisto1(theHistoNumber+5)->Fill(log10(y),wei);
    if( theAnaMgr->GetHisto1(theHistoNumber+6,false) )
      theAnaMgr->GetHisto1(theHistoNumber+6)->Fill(log10(z),wei);
    if( theAnaMgr->GetHisto1(theHistoNumber+7,false) )
      theAnaMgr->GetHisto1(theHistoNumber+7)->Fill(log10(y*y),wei);
    if( theAnaMgr->GetHisto1(theHistoNumber+8,false) )
      theAnaMgr->GetHisto1(theHistoNumber+8)->Fill(log10(z*z),wei);
  }
  theSumY += y*wei;
  theSumY2 += y*y*wei;
  theSumY4 += pow(y,4)*wei;
  theSumZ += z*wei;
  theSumZ2 += z*z*wei;
  theSumZ4 += pow(z,4)*wei;
  theSumWei += wei;
  theSumWei2 += wei*wei;

  //clear theHits
  GmVMicroyzUA::EndOfEventAction( evt );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GmMicroyzG4UA::~GmMicroyzG4UA()
{
}

