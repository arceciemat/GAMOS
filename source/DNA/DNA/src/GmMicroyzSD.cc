//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software 
// shall cite the following Geant4-DNA collaboration publications:
// Phys. Med. 31 (2015) 861-874
// Med. Phys. 37 (2010) 4692-4708
// The Geant4-DNA web site is available at http://geant4-dna.org
//
/// \file GmMicroyzSD.cc
/// \brief Implementation of the GmMicroyzSD class

#include "GmMicroyzSD.hh"
#include "GmMicroyzUA.hh"
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

GmMicroyzSD::GmMicroyzSD(const G4String& name )  
  :G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  G4String hitsCollectionName = "GmMicroyzSDHC";
  collectionName.insert(hitsCollectionName);
  
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  theSphereRadius = parMgr->GetNumericValue("GmMicroyzSD:SphereRadius",5*nm);
  theVolumeChord = 4.*theSphereRadius/3;
  G4double volumeDensity = 1 * g/cm3; // G4_WATER
  theVolumeMass = (4./3)*CLHEP::pi*theSphereRadius*theSphereRadius*theSphereRadius*volumeDensity;
  
  G4String fileNameOut = parMgr->GetStringValue("GmMicroyzSD:OutputFileName","GmMicroyz.out");
  G4String suffix = parMgr->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  if( suffix != "" ) {
    fileNameOut += "."+suffix;
  }
  bTextFile = G4bool(parMgr->GetNumericValue("GmMicroyzSD:OutputFormatText",1));
  if( bTextFile ) {
    theFileOut= new std::ofstream(fileNameOut);
  } else {
    theFileOut= new std::ofstream(fileNameOut, std::ios::out | std::ios::binary);
  }
  bHistos = G4bool(parMgr->GetNumericValue("GmMicroyzSD:HistostputFormatText",1));
  if( bHistos ) bHistosInitialised = false;

  theSumY = 0;
  theSumY2 = 0;
  theSumZ = 0;
  theSumZ2 = 0;
  theSumWei = 0;
  new GmMicroyzUA( this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmMicroyzSD::BookHistos( G4double Eincident )
{
  if(bHistos) {
    theAnaMgr = GmAnalysisMgr::GetInstance("GmMicroyzSD");
    theHistoNumber = GmVHistoBuilder::BuildHistoNumber();
    theAnaMgr->CreateHisto1D("N_hits",140,80*Eincident/(1*keV),150*Eincident/(1*keV),theHistoNumber+1);
    theAnaMgr->CreateHisto1D("N_scored_hits",100,0.,100,theHistoNumber+2);
    theAnaMgr->CreateHisto1D("E_incident",100,Eincident*0.9,Eincident*1.1,theHistoNumber+3);

    G4double ylim = 120.;
    G4double zlim = ylim*0.204/pow(2*theSphereRadius/micrometer,2);
    G4int ozlim = log10(zlim);  zlim = pow(10.,ozlim-1)*(int(zlim/pow(10.,ozlim-1))); // round 2 numbers
    //   G4cout <<  ylim*0.204/pow(2*theSphereRadius/micrometer,2) << " zlim " << zlim << " oz " << ozlim << " " <<  pow(10.,ozlim-1) << "*" << int(zlim/pow(10.,ozlim-1)) << G4endl; //GDEB
    theAnaMgr->CreateHisto1D("yf(y)",120,0.,ylim,theHistoNumber+11);
    theAnaMgr->CreateHisto1D("zf(z)",125,0.,zlim,theHistoNumber+12);
    theAnaMgr->CreateHisto1D("yd(y)",120,0.,ylim*100,theHistoNumber+13);
    theAnaMgr->CreateHisto1D("zd(z)",125,0.,zlim*10000,theHistoNumber+14);
    
    theAnaMgr->CreateHisto1D("log10(yf(y))",120,-1.,log10(ylim)+1,theHistoNumber+21);
    theAnaMgr->CreateHisto1D("log10(zf(z))",125,-1,log10(zlim)+1,theHistoNumber+22);
    theAnaMgr->CreateHisto1D("log10(yd(y))",120,-1.,log10(ylim)+2+1,theHistoNumber+23);
    theAnaMgr->CreateHisto1D("log10(zd(z))",125,-1.,log10(zlim)+4+1,theHistoNumber+24);

  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmMicroyzSD::~GmMicroyzSD()
{
  theFileOut->close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmMicroyzSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new GmMicroyzHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce

  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  
  hce->AddHitsCollection( hcID, fHitsCollection );

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool GmMicroyzSD::ProcessHits(G4Step* aStep, 
                                     G4TouchableHistory*)
{

  if( bHistos && ! bHistosInitialised ) {
    BookHistos(aStep->GetTrack()->GetVertexKineticEnergy());   
    
    bHistosInitialised = true;
  }
  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();

  if (edep==0.) return false;

  GmMicroyzHit* newHit = new GmMicroyzHit();

  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetEdep(edep);
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());

  if (aStep->GetTrack()->GetTrackID()==1&&aStep->GetTrack()->GetParentID()==0)
    newHit->SetIncidentEnergy(aStep->GetTrack()->GetVertexKineticEnergy());

  fHitsCollection->insert( newHit );

  //newHit->Print();

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmMicroyzSD::EndOfEvent(G4HCofThisEvent*)
{
  G4int nofHits = fHitsCollection->entries();

  G4double Einc=0;
  
  // PROCESSING OF MICRODOSIMETRY Y & Z SPECTRA

  //
 
  //***************
  // y and z
  //***************
  
  // select random hit
  G4int randHit=0; // Runs from 0 to number of hits - 1
  randHit = static_cast<G4int>( G4UniformRand()*nofHits );
    
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) G4cout << "======> random selection of hit number randHit ="  << randHit << G4endl;
#endif
  
  // get selected random hit position
  G4ThreeVector hitPos =  (*fHitsCollection)[randHit]->GetPos();
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
  if( DNAVerb(debugVerb) ) G4cout << " TRACKERSD randCenterPos " << randCenterPos << G4endl;
#endif

  // search for neighbouring hits in the sphere and cumulate deposited energy 
  //  in epsilon
  G4double epsilon = 0;
  G4int nbEdep = 0;
  
  for ( G4int ii=0; ii<nofHits; ii++ ) 
  { 
    GmMicroyzHit* hit = dynamic_cast<GmMicroyzHit*>((*fHitsCollection)[ii]);
    if (hit->GetIncidentEnergy()>0)
      Einc = hit->GetIncidentEnergy();
    
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
      if( DNAVerb(testVerb) ) G4cout << ii << " TRACKERSD near hist " << epsilon << " n=" << nbEdep << G4endl; 
      //  if( DNAVerb(-debugVerb) ) G4cout << " TRACKERSD near hist " << localPos/nm << " " << randCenterPos/nm << G4endl;  //GDEB
#endif
    }
       
  }

  G4double y = (epsilon/eV)/(theVolumeChord/nm);
  G4double z = (epsilon/theVolumeMass)/gray;
  /*?? // normalize to number of hits (to ensure a spatially uniform sampling)
    G4cout << " Y=" << y << " Z="<< z << G4endl; //GDEB
  G4cout << "WEIGHTD Y=" << y << " Z="<< z << " nbEdep " << nbEdep << "/"<<fHitsCollection->GetSize()<< G4endl; //GDEB*/
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) G4cout << " TRACKERSD y= " << y << " z= " << z << G4endl;
#endif

  
  if( bTextFile ) {
    *theFileOut << nofHits << "," << nbEdep << ","
		<< y << "," << z << ","
		<< Einc/eV << G4endl;
  } else {
    *theFileOut << nofHits << nbEdep 
		<< y << z 
	       << Einc/eV;
  }

  double wei = (float(nbEdep)/fHitsCollection->GetSize());
  if(bHistos) {
    theAnaMgr->GetHisto1(theHistoNumber+1)->Fill(nofHits,wei);
    theAnaMgr->GetHisto1(theHistoNumber+2)->Fill(nbEdep,wei);
    theAnaMgr->GetHisto1(theHistoNumber+3)->Fill(Einc,wei);
    theAnaMgr->GetHisto1(theHistoNumber+11)->Fill(y,wei);
    theAnaMgr->GetHisto1(theHistoNumber+12)->Fill(z,wei);
    theAnaMgr->GetHisto1(theHistoNumber+13)->Fill(y*y,wei);
    theAnaMgr->GetHisto1(theHistoNumber+14)->Fill(z*z,wei);
    theAnaMgr->GetHisto1(theHistoNumber+21)->Fill(log10(y),wei);
    theAnaMgr->GetHisto1(theHistoNumber+22)->Fill(log10(z),wei);
    theAnaMgr->GetHisto1(theHistoNumber+23)->Fill(log10(y*y),wei);
    theAnaMgr->GetHisto1(theHistoNumber+24)->Fill(log10(z*z),wei);
  }

  theSumY += y*wei;
  theSumY2 += y*wei*y*wei;
  theSumY4 += pow(y*wei,4);
  theSumZ += z*wei;
  theSumZ2 += z*wei*z*wei;
  theSumZ4 += pow(z*wei,4);
  theSumWei += wei;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
