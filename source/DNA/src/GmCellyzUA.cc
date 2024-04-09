#include "GmCellyzUA.hh"
#include "GmDNAVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
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

GmCellyzUA::GmCellyzUA()
  : GmVMicroyzUA()
{
  theName = "GmCellyzUA";
  theSphereRadius = theParamMgr->GetNumericValue(GetName()+":SphereRadius",5*nm);
  theVolumeChord = 4.*theSphereRadius/3;
  G4double volumeDensity = 1 * g/cm3; // G4_WATER
  theVolumeMass = (4./3)*CLHEP::pi*theSphereRadius*theSphereRadius*theSphereRadius*volumeDensity;
  std::vector<G4String> params;
  params = theParamMgr->GetVStringValue(GetName()+":CellPositions",params);
  if( params.size() != 10 ) {
    G4Exception("GmCellyzUA::GmCellyzUA","",
		FatalException,
		("Number of parameters in"+GetName()+":CellPositions"+" must be 10, they are "+GmGenUtils::itoa(params.size())).c_str());
  }
  if( params[0] == "SQUARE_XY") {
    theNSpheresX = GmGenUtils::GetValue(params[1]);
    theNSpheresY = GmGenUtils::GetValue(params[2]);
    theNSpheresZ = 1;
    theNSpheresXY = theNSpheresX*theNSpheresY;
    theStepX = GmGenUtils::GetValue(params[3]);
    theStepY = GmGenUtils::GetValue(params[4]);
    theOffsetX = GmGenUtils::GetValue(params[5]);
    theOffsetY = GmGenUtils::GetValue(params[6]);    
    theCentreX = GmGenUtils::GetValue(params[7]);
    theCentreY = GmGenUtils::GetValue(params[8]);
    theCentreZ = GmGenUtils::GetValue(params[9]);
  }
    
  /*   std::vector<G4LogicalVolume*> theLogicalVolumes;
aa  std::vector<G4double> theVolumeChords;
  std::vector<G4String> lvs;
  lvs = theParamMgr->GetVStringValue("GmCellyzUA:Volumes",lvs);
  for( size_t ii = 0; ii < lvs.size(); ii++ ) {
    std::vector<G4LogicalVolume*> lvsg4 = GmGeometryUtils::GetInstance()->GetLogicalVolumes(lvs[ii],true);
    for( size_t jj = 0; jj < lvsg4.size(); jj++ ) {
      G4LogicalVolume logvol = lvsg4[jj];
      theLogicalVolumes.push_back(logvol);
      G4VSolid* solid = logvol->GetSolid();
      volChord = 4*solid->GetCubicVolume()/solid->GetSurfaceArea();
      //      theVolumeChord = 4.*theSphereRadius/3;
      theVolumeChords.push_back(volChord);
    }
  }
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmCellyzUA::BookHistos()
{
  if(bHistos) {
    GmVMicroyzUA::BookHistos();
    theNHistosV = theAnaMgr->GetHistos1D().size();
    //    G4cout << "1  GmVMicroyzUA::BookHistos() " << theNHistosV << G4endl; //GDEB   
    G4double Eincident = 10*keV;
    theAnaMgr->CreateHisto1D("N_hits",140,80*Eincident/(1*keV),150*Eincident/(1*keV),theHistoNumber+theNHistosV+1);
    theAnaMgr->CreateHisto1D("N_scored_hits",100,0.,100,theHistoNumber+theNHistosV+2);
    //    theAnaMgr->CreateHisto1D("E_incident",100,Eincident*0.9,Eincident*1.1,theHistoNumber+theNHistos+3);

  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmCellyzUA::UserSteppingAction(const G4Step* aStep)
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
void GmCellyzUA::EndOfEventAction(const G4Event* evt)
{

  // check hits in a sphere
  G4int nofHits = theHits.size();
  // PROCESSING OF MICRODOSIMETRY Y & Z SPECTRA
  //
  //***************
  // y and z
  //***************

  if( theHits.size() == 0 ) return; 
  
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) G4cout << " GmCellUA nofHits " << nofHits << G4endl;
#endif

  std::set<G4int> theHitsDone;
  typedef std::multimap<G4int,GmMicroyzHit*> mmih;
  mmih theHitsPerCrystal;
  for ( G4int ii=0; ii<nofHits; ii++ ) 
  {
    if( theHitsDone.find(ii) != theHitsDone.end() ) continue;
    GmMicroyzHit* hit = theHits[ii];
    G4ThreeVector pos = hit->GetPos();
    // Get position close to a sphere 
    G4double localPosX = fmod(pos.x()-theCentreX,theStepX);
    G4double localPosY = fmod(pos.y()-theCentreY,theStepY);
    G4double localPosZ = pos.z()-theCentreZ;
    G4double localPosR = sqrt(localPosX*localPosX+localPosY*localPosY+localPosZ*localPosZ);
#ifndef GAMOS_NO_VERBOSE
    if( DNAVerb(debugVerb) ) G4cout << " GmCellUA pos " << pos << " localPos " << localPosX<<","<<localPosY<<","<<localPosZ<< " R:" << localPosR << " <? " << theSphereRadius << G4endl;
#endif
    if( localPosR < theSphereRadius ) { // HIT IN SPHERE
      // get sphere id
      G4int idX = (pos.x()-theCentreX-theOffsetX-theStepX/2.)/theStepX;
      if( idX < 0 || idX > theNSpheresX ) continue;
      G4int idY = (pos.y()-theCentreY-theOffsetY-theStepY/2.)/theStepY;
      if( idY < 0 || idY > theNSpheresY ) continue;
      G4int idZ = 0; 
      G4int sphereID = idX + idY*theNSpheresX + idZ*theNSpheresXY;
      theHitsPerCrystal.insert(mmih::value_type(sphereID,hit));
#ifndef GAMOS_NO_VERBOSE
      if( DNAVerb(debugVerb) ) G4cout << ii << " GmCellUA sphereID " << sphereID << " " << idX<<","<<idY<<","<<idZ << G4endl;
#endif
    }
  }

  // LOOK FOR HITS IN EACH CRYSTAL
  for( mmih::iterator ite = theHitsPerCrystal.begin(); ite != theHitsPerCrystal.end(); ite++ ) {
    auto hitsInID = theHitsPerCrystal.equal_range(ite->first);
    //    G4cout <<  std::distance(theHitsPerCrystal.begin(),ite) << " " << ite->first << " GmCELL n hitsInID " << std::distance(hitsInID.first,hitsInID.second) << G4endl; //GDEB
    G4double epsilon = 0.;
    for( auto ite2 = hitsInID.first; ite2 != hitsInID.second; ite2++ ) {
      epsilon = epsilon + ite2->second->GetEdep() ;
#ifndef GAMOS_NO_VERBOSE
      if( DNAVerb(debugVerb) ) G4cout << ite->first << " : " << std::distance(hitsInID.first,ite2) << " GmCellUA hitEner " << ite2->second->GetEdep() << " : " << epsilon << G4endl;
#endif
    }
    G4double y = (epsilon/eV)/(theVolumeChord/nm);
    G4double z = (epsilon/theVolumeMass)/gray;
#ifndef GAMOS_NO_VERBOSE
    if( DNAVerb(-debugVerb) ) G4cout << " GmCellUA FINAL y= " << y << " z= " << z << G4endl;
#endif

    if( bTextFile ) {
      *theFileOut << nofHits << "," 
		  << y << "," << z << ","
		  << G4endl;
    } else if( bBinFile ) {
      *theFileOut << nofHits 
		  << y << z ;
    }
    
    if(bHistos) {
      theAnaMgr->GetHisto1(theHistoNumber+theNHistosV+1)->Fill(nofHits);
      theAnaMgr->GetHisto1(theHistoNumber+theNHistosV+2)->Fill(1);
      //    theAnaMgr->GetHisto1(theHistoNumber+theNHistosV+3)->Fill(Einc);
      if( theAnaMgr->GetHisto1(theHistoNumber+1,false) ) 
	theAnaMgr->GetHisto1(theHistoNumber+1)->Fill(y);
      if( theAnaMgr->GetHisto1(theHistoNumber+2,false) )
	theAnaMgr->GetHisto1(theHistoNumber+2)->Fill(z);
      if( theAnaMgr->GetHisto1(theHistoNumber+3,false) )
	theAnaMgr->GetHisto1(theHistoNumber+3)->Fill(y*y);
      if( theAnaMgr->GetHisto1(theHistoNumber+4,false) )
	theAnaMgr->GetHisto1(theHistoNumber+4)->Fill(z*z);
      if( theAnaMgr->GetHisto1(theHistoNumber+5,false) )
	theAnaMgr->GetHisto1(theHistoNumber+5)->Fill(log10(y));
      if( theAnaMgr->GetHisto1(theHistoNumber+6,false) )
	theAnaMgr->GetHisto1(theHistoNumber+6)->Fill(log10(z));
      if( theAnaMgr->GetHisto1(theHistoNumber+7,false) )
	theAnaMgr->GetHisto1(theHistoNumber+7)->Fill(log10(y*y));
      if( theAnaMgr->GetHisto1(theHistoNumber+8,false) )
	theAnaMgr->GetHisto1(theHistoNumber+8)->Fill(log10(z*z));
    }
    theSumY += y;
    theSumY2 += y*y;
    theSumY4 += pow(y,4);
    theSumZ += z;
    theSumZ2 += z*z;
    theSumZ4 += pow(z,4);
    theSumWei += 1;
    theSumWei2 += 1;

    ite = hitsInID.second;
    // G4cout << "END LOOP " <<  std::distance(theHitsPerCrystal.begin(),ite) << G4endl; //GDEB
    if( theHitsPerCrystal.end() == ite) break;
  }
  //clear theHits
  GmVMicroyzUA::EndOfEventAction( evt );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GmCellyzUA::~GmCellyzUA()
{
}

