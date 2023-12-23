#include "GmMicroyzVolUA.hh"
#include "GmDNAVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4VSolid.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmMicroyzVolUA::GmMicroyzVolUA()
  : GmVMicroyzUA()
{
  theName = "GmMicroyzVolUA";
  theVolumeName = theParamMgr->GetStringValue(GetName()+":Volume","");
  if( theVolumeName == "" ) {
    G4Exception("GmMicroyzVolUA::GmMicroyzVolUA",
		"",
		FatalException,
		("You must define a volume name, use /P "+GetName()+":Volume <NAME>").c_str());
  }

  std::vector<G4LogicalVolume*> logVols = GmGeometryUtils::GetInstance()->GetLogicalVolumes( theVolumeName, true );
  if( logVols.size() != 1 ) {
    G4Exception("GmMicroyzVolUA::GmMicroyzVolUA",
		  "",
		  FatalException,
		  ("There can only be one logical volume with name "+ theVolumeName).c_str());
  }
  G4LogicalVolume* logVol = logVols[0];
  
  // There cannot be children volumes
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();  
  G4PhysicalVolumeStore::const_iterator ite;
  for(ite = pvs->begin(); ite != pvs->end(); ite++ ){
    if( (*ite)->GetMotherLogical() == logVol ) {
      G4Exception("GmMicroyzVolUA::GmMicroyzVolUA",
		  "",
		  FatalException,
		  ("You cannot select a volume with children : "+(*ite)->GetMotherLogical()->GetName()+" is a child of "+logVol->GetName()).c_str());
    }
  }                                                     

  G4VSolid* solid = logVol->GetSolid();
  G4double vol = solid->GetCubicVolume();
  G4double volDensity = logVol->GetMaterial()->GetDensity(); // G4_WATER
  theVolumeMass = vol*volDensity;
  G4double surf = solid->GetSurfaceArea();
  theVolumeChord = 4*vol/surf;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmMicroyzVolUA::BookHistos()
{
  if(bHistos) {
    GmVMicroyzUA::BookHistos();
    theNHistosV = theAnaMgr->GetHistos1D().size();
    G4double Eincident = 10*keV;
    theAnaMgr->CreateHisto1D("N_hits",140,80*Eincident/(1*keV),150*Eincident/(1*keV),theHistoNumber+theNHistosV+1);
    //    theAnaMgr->CreateHisto1D("E_incident",100,Eincident*0.9,Eincident*1.1,theHistoNumber+theNHistos+3);

  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmMicroyzVolUA::UserSteppingAction(const G4Step* aStep)
{
  G4String volName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();
  if( volName != theVolumeName ) return;
  
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep==0.) return;

  GmMicroyzHit* newHit = new GmMicroyzHit();
  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetEdep(edep);
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
  theHits.push_back( newHit );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmMicroyzVolUA::EndOfEventAction(const G4Event* evt)
{
  G4int nofHits = theHits.size();

  // PROCESSING OF MICRODOSIMETRY Y & Z SPECTRA
  //
 
  //***************
  // y and z
  //***************
  
  // search for neighbouring hits in the sphere and cumulate deposited energy 
  //  in epsilon
  G4double epsilon = 0;

  for ( G4int ii=0; ii<nofHits; ii++ ) 
  { 
    GmMicroyzHit* hit = theHits[ii];

    epsilon = epsilon + hit->GetEdep() ;

#ifndef GAMOS_NO_VERBOSE
      if( DNAVerb(testVerb) ) G4cout << ii << " GmMicroVolUA near hist " << epsilon << G4endl; 
      //  if( DNAVerb(-debugVerb) ) G4cout << " GmMicroVolUA near hist " << localPos/nm << " " << randCenterPos/nm << G4endl;  //GDEB
#endif
  }


  G4double y = (epsilon/eV)/(theVolumeChord/nm);
  G4double z = (epsilon/theVolumeMass)/gray;
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) G4cout << "nHits= " << nofHits << " GmMicroVolUA y= " << y << " z= " << z << G4endl;
#endif

  if( bTextFile ) {
    *theFileOut << nofHits << ","
		<< y << "," << z << ","
		<< G4endl;
  } else if( bBinFile ) {
    *theFileOut << nofHits  
		<< y << z ;
  }

  double wei = 1.;
  if(bHistos) {
    theAnaMgr->GetHisto1(theHistoNumber+theNHistosV+1)->Fill(nofHits,wei);
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
  theSumY2 += y*wei*y*wei;
  theSumY4 += pow(y*wei,4);
  theSumZ += z*wei;
  theSumZ2 += z*wei*z*wei;
  theSumZ4 += pow(z*wei,4);
  theSumWei += wei;
  theSumWei2 += wei*wei;

  //clear theHits
  GmVMicroyzUA::EndOfEventAction( evt );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GmMicroyzVolUA::~GmMicroyzVolUA()
{
}

