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

/// \file MUTTrackerSD.cc
/// \brief Implementation of the MUTTrackerSD class

#include "MUTTrackerSD.hh"

#include "G4Exception.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessType.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "Randomize.hh"
#include "G4tgrParameterMgr.hh"
#include "G4Box.hh"

#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MUTTrackerSD::MUTTrackerSD(G4String sdName)
  : G4VSensitiveDetector(sdName)
{
  collectionName.insert("TrackerHitColl");

  InitialiseHistoNames();
  bDetectorDataInitialised = false;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MUTTrackerSD::~MUTTrackerSD() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MUTTrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection =
    new MUTTrackerHitColl(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int collID =
    G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

  hce->AddHitsCollection(collID, fHitsCollection);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MUTTrackerSD::InitialiseHistoNames()
{
  std::vector<GmVFilter*> filters;
  GmVClassifier* classifier = 0;
  SetHistoNameAndNumber("gener", filters, classifier);
  BookHistos();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MUTTrackerSD::BookHistos()
{
  // Define binning for each sort of histogram
  //
  //  Kinetic energy at the entrance and exit histograms
  const G4double kinEmin = 0.;
  const G4double kinEmax = 300.;
  const G4double kinEbinWidth = 0.1;
  const G4int kinEbins = 
    static_cast<G4int>((kinEmax - kinEmin) / kinEbinWidth);

  // Logarithmic binning for energy histograms
  const G4int minLog10E = -4.;
  const G4int maxLog10E = +4.;
  const G4int nBinsLog10E = (maxLog10E - minLog10E) * 50;

  // Logarithmic binning for weighted numbers histograms
  const G4int minLog10W = 0;
  const G4int maxLog10W = +6;
  const G4int nBinsLog10W = (maxLog10W - minLog10W) * 50;

  // Linear binning for counter histograms
  G4int minCount = 0;
  G4int maxCount = 20000;
  G4int nBinsCount = (maxCount - minCount) / 100;

  // Create histograms
  //
  theAnaMgr = GmAnalysisMgr::GetInstance("MUTRunAction");

  // Logarithmic binning  Energy histograms
  theAnaMgr->CreateHisto1D("fe Energy imparted per event [keV] (log binning)",nBinsLog10E,minLog10E,maxLog10E,theHistoNumber+11); // fe
  theAnaMgr->CreateHisto1D("efe Weighted energy imparted per event [keV] (log binning)",nBinsLog10E,minLog10E,maxLog10E,theHistoNumber+12); // efe
  theAnaMgr->CreateHisto1D("e2fe Squared-weighted energy imparted per event [keV] (log binning)",nBinsLog10E,minLog10E,maxLog10E,theHistoNumber+13); // e2fe

  // Logarithmic binning for lineal energy histograms
  theAnaMgr->CreateHisto1D("fy Lineal energy [keV/um] (log binning)",nBinsLog10E,minLog10E,maxLog10E,theHistoNumber+21); // fy
  theAnaMgr->CreateHisto1D("yfy Dose-weighted lineal energy [keV/um] (log binning)",nBinsLog10E,minLog10E,maxLog10E,theHistoNumber+22); // "yfy",
  theAnaMgr->CreateHisto1D("y2fy Squared-weighted lineal energy [keV/um] (log binning)",nBinsLog10E,minLog10E,maxLog10E,theHistoNumber+23); // "y2fy"

  // Logarithmic binning for specific energy histograms
  theAnaMgr->CreateHisto1D("fz Single-event specific energy [Gy] (log binning)",nBinsLog10E,minLog10E,maxLog10E,theHistoNumber+31); // "fz"
  theAnaMgr->CreateHisto1D("zfz Dose-weighted single-event specific energy [Gy] (log binning)",nBinsLog10E,minLog10E,maxLog10E,theHistoNumber+32); // "zfz"
  theAnaMgr->CreateHisto1D("z2fz Squared-weighted single-event specific energy [Gy] (log binning)",nBinsLog10E,minLog10E,maxLog10E,theHistoNumber+33); // "z2fz"

  // Counters histograms
  theAnaMgr->CreateHisto1D("Nsel Number of selectable hits per event", nBinsCount, minCount, maxCount,theHistoNumber+41); // "Nsel"
  theAnaMgr->CreateHisto1D("Nsite Number of hits in site", nBinsCount, minCount, maxCount,theHistoNumber+42); // "Nsite"
  theAnaMgr->CreateHisto1D("Nint Number of selectable hits in site", nBinsCount, minCount, maxCount,theHistoNumber+43); //"Nint"
  
  // Kinetic energy at the entrance and exit histograms
  theAnaMgr->CreateHisto1D("KinE_in Kinetic energy at the entrance [MeV]", kinEbins, kinEmin, kinEmax,theHistoNumber+51); // "KinE_in"
  theAnaMgr->CreateHisto1D("KinE_out Kinetic energy at the exit [MeV]", kinEbins, kinEmin, kinEmax,theHistoNumber+52); // "KinE_out"
  
  // 2D histogram for Nsite vs weighted Edep
  theAnaMgr->CreateHisto2D("Nsite_vs_e Number of hits in site vs energy imparted [keV] (log-log)",nBinsLog10E,minLog10E,maxLog10E,nBinsLog10W,minLog10W,maxLog10W,theHistoNumber+61); // "Nsite_vs_e"
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MUTTrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  // Get the track
  G4Track* aTrack = aStep->GetTrack();

  // Get Pre and Post step points
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

  // Get ParentID
  G4int parentID = aTrack->GetParentID();

  // Only for primary particles
  if (parentID == 0) {
    // Check if the particle is entering or exiting
    if (preStepPoint->GetStepStatus() == fGeomBoundary) {
      // Get the pre-step energy
      G4double preEnergy = preStepPoint->GetKineticEnergy();

      // Fill the histogram
      theAnaMgr->GetHisto1(theHistoNumber+51)->Fill(preEnergy);
    }

    if (postStepPoint->GetStepStatus() == fGeomBoundary) {
      // Get the post-step energy
      G4double postEnergy = postStepPoint->GetKineticEnergy();

      // Fill the histogram
      theAnaMgr->GetHisto1(theHistoNumber+52)->Fill( postEnergy);
    }
  }

  // Get the energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.) return false;

  // Declare a new hit
  auto newHit = new MUTTrackerHit();

  newHit->SetEdep(edep);

  // Get the position of the hit
  G4ThreeVector Pos = postStepPoint->GetPosition();
  newHit->SetPosition(Pos);

  // Add the hit to the collection
  fHitsCollection->insert(newHit);

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MUTTrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  const auto nofHits = fHitsCollection->entries();

  if (verboseLevel > 0) {
    G4cout << "  Hits Collection: in this event, we have " << nofHits
           << " hits in the tracker." << G4endl;
  }

  if( ! bDetectorDataInitialised ) {
    G4String paramSiteRadius = G4tgrParameterMgr::GetInstance()->FindParameter("SITE_RADIUS",true);
    theSiteRadius = GmGenUtils::GetValue(paramSiteRadius);

    GmGeometryUtils* theGeomUtils = GmGeometryUtils::GetInstance();
    // std::vector<G4LogicalVolume*> worldVols = theGeomUtils->GetLogicalVolumes("World", true);
    G4LogicalVolume* worldVol = theGeomUtils->GetTopLV();
    // Declare variables for the dimensions of the detector
    G4Box* worldBox = dynamic_cast<G4Box*>(worldVol->GetSolid());
    G4double worldXY = worldBox->GetXHalfLength();
    G4double worldZ = worldBox->GetZHalfLength();
    // :P WORLD_XY $HITSELREG_XY + 4 * $SITERADIUS
    // :P WORLD_Z $HITSELREG_Z + 4 * $SITERADIUS + 2 * $MAXRANGE
    //--- hit selection region from the geometry parameters, as in the Geant4 example microtrack
    //    (before it was derived from the world HALF lengths with the formulas of the full lengths:
    //     for HITSELREG_XY=100 um, HITSELREG_Z=4 um it gave 48 um and 50 um)
    (void)worldXY; (void)worldZ;
    theHitSelRegXY = GmGenUtils::GetValue(G4tgrParameterMgr::GetInstance()->FindParameter("HITSELREG_XY",true));
    theHitSelRegZ = GmGenUtils::GetValue(G4tgrParameterMgr::GetInstance()->FindParameter("HITSELREG_Z",true));
    G4Material* mateWorldAndSD = worldVol->GetMaterial();
    /* std::vector<G4LogicalVolume*> SDVols = theGeomUtils->GetLogicalVolumes("SDbox", true);
    // Declare variables for the dimensions of the detector
    G4Box* SDBox = dynamic_cast<G4Box*>(SDVols[0]->GetSolid());
    G4double SDXY = SDBox->GetXHalfLength();
    G4double SDZ = SDBox->GetZHalfLength();
    //:P SENSDET_XY $HITSELREG_XY + 4 * $SITERADIUS
    //:P SENSDET_Z $HITSELREG_Z + 4 * $SITERADIUS
    G4double maxRange = (SDZ - SDXY ) / 2.;
    theHitSelRegXY = SDXY - 4* theSiteRadius - 2*maxRange;
    theHitSelRegZ = SDZ - 4* theSiteRadius;
    G4Material* mateWorldAndSD = SDVols[0]->GetMaterial();
    */
    
    theDetDensity = mateWorldAndSD->GetDensity();
  
    bDetectorDataInitialised = true;
  }
  if (verboseLevel > 0) {
    G4cout << " nofHits " << nofHits <<  " TrackerSD  HitSelRegZ " << theHitSelRegZ << " HitSelRegXY " << theHitSelRegXY << " site_radius " << theSiteRadius << " DetDensity " << theDetDensity << " =g/cm3 " << theDetDensity/(g/cm3) <<  G4endl; }

  G4ThreeVector hitPos;
  G4ThreeVector randCenterPos;
    
  G4int nHsel = 0;  // number of hits in the selection region
  G4int nHsite = 0;  // number of hits in the site
  G4int nHint = 0;  // number of hits both in site and selection region
  G4double evtEdep = 0.;  // energy deposit in the event

  if (nofHits > 0) {
    const G4int maxTries = 1000;
    G4int tries = 0;
    G4bool found = false;

    while (tries < maxTries && !found) {
      std::size_t randHit = static_cast<std::size_t>(G4UniformRand() * nofHits);
      auto hit = (*fHitsCollection)[randHit];
      G4ThreeVector hitPosition = hit->GetPosition();

      if (hitPosition.x() > -theHitSelRegXY / 2
          && hitPosition.x() < theHitSelRegXY / 2
          && hitPosition.y() > -theHitSelRegXY / 2
          && hitPosition.y() < theHitSelRegXY / 2
          && hitPosition.z() > -theHitSelRegZ / 2
          && hitPosition.z() < theHitSelRegZ / 2)
      {
        hitPos = hitPosition;  // store valid hit position

        found = true;
	if (verboseLevel > 0) {
	  G4cout << tries << " TrackerSD HIT VALID (IN SELREG) " << hit->GetPosition() << "  " << hitPos << G4endl; 
	}
      }
      ++tries;
    }

    if (found) {
      G4double theSiteRadius2 = theSiteRadius * theSiteRadius;

      // Random placement of a sphere center containing the hit
      G4double xRand, yRand, zRand, randRad2;
      do {
        xRand = (2 * G4UniformRand() - 1) * theSiteRadius;
        yRand = (2 * G4UniformRand() - 1) * theSiteRadius;
        zRand = (2 * G4UniformRand() - 1) * theSiteRadius;
        randRad2 = xRand * xRand + yRand * yRand + zRand * zRand;
      } while (randRad2 > theSiteRadius2);

      randCenterPos = G4ThreeVector(xRand + hitPos.x(), yRand + hitPos.y(),
                                    zRand + hitPos.z());
    }

    else {
      G4cout
        << "In this event, no hits were found in the hit selection "
        << "region after trying " << tries << " times." << G4endl
        << "Please consider increasing the size of the hit selection region."
        << G4endl;
      return;  // Skip analysis if no valid hit was found
    }
  }

  // Loop over hits
  for (std::size_t jj = 0; jj < nofHits; jj++) {
    auto hit2 = (*fHitsCollection)[jj];
    G4ThreeVector hit2Position = hit2->GetPosition();

    G4bool inSlab =
      (hit2Position.x() > -theHitSelRegXY / 2 && hit2Position.x() < theHitSelRegXY / 2
       && hit2Position.y() > -theHitSelRegXY / 2
       && hit2Position.y() < theHitSelRegXY / 2
       && hit2Position.z() > -theHitSelRegZ / 2
       && hit2Position.z() < theHitSelRegZ / 2);

    // Check if the hit is within the site
    G4double dist = (hit2Position - randCenterPos).mag();
    if (verboseLevel > 0) {
      G4cout << jj << " DIST " << dist << "theSiteRadius " << theSiteRadius << " " << hit2Position << G4endl;
    }
    if (dist < theSiteRadius) {
      nHsite++;
      evtEdep += hit2->GetEdep();
    // Check if the hit is within the selection region and the site
      if ( inSlab) nHint++;    
      if (verboseLevel > 0) {
	G4cout << jj << " " << inSlab << " nHsite " << nHsite << " nHint " << nHint << " evtEdep " << evtEdep << " TrackerSD HIT FOUND dist " << dist << "  " << hit2Position << "  " << randCenterPos << G4endl;
      }
    }

    // Check if the hit is within the selection region
    if (inSlab) nHsel++;

  }

  if (verboseLevel > 0) {
    G4cout <<  log10(evtEdep) << " FINAL nHsite " << nHsite << " nHint " << nHint << " evtEdep " << evtEdep << G4endl;
  }
  // Calculate lineal energy
  G4double y = (evtEdep) / ((4. / 3.) * theSiteRadius);  // in keV/um

  // Calculate specific energy
  G4double mass = ((4. / 3.) * CLHEP::pi * theSiteRadius * theSiteRadius
                   * theSiteRadius * theDetDensity);
  G4double z = (evtEdep / mass);

  // Fill histograms
  if (nHint > 0) {
    // Define the weight
    G4double wght = G4double(nHsel) / G4double(nHint);
    G4double lee = log10(evtEdep/keV);
    // Histogram 11: Single-event energy imparted in keV
    theAnaMgr->GetHisto1(theHistoNumber+11)->Fill( lee, wght);

    // Histogram 12: Weighted single-event energy imparted in keV
   theAnaMgr->GetHisto1(theHistoNumber+12)->Fill( lee, (evtEdep * wght / keV));

    // Histogram 13: Squared weighted energy imparted per event in keV^2
   theAnaMgr->GetHisto1(theHistoNumber+13)->Fill( lee, 
                            ((evtEdep * evtEdep * wght) / (keV * keV)));

    // Histogram 21: Lineal energy in keV/um
   G4double ly = log10(y / (keV / um));
   theAnaMgr->GetHisto1(theHistoNumber+21)->Fill(ly, wght);

    // Histogram 22: Dose-weighted lineal energy in keV/um
   theAnaMgr->GetHisto1(theHistoNumber+22)->Fill(ly, (y * wght / (keV / um)));

    // Histogram 23: Squared weighted lineal energy in (keV/um)^2
   theAnaMgr->GetHisto1(theHistoNumber+23)->Fill(ly,
                            (y * y * wght / ((keV / um) * (keV / um))));

    // Histogram 31: Specific energy in keV/g
   G4double lz = log10(z / gray); // (was y: the specific energy histograms were filled with the lineal energy)
   theAnaMgr->GetHisto1(theHistoNumber+31)->Fill( lz, wght);

    // Histogram 32: Weighted specific energy in keV/g
   theAnaMgr->GetHisto1(theHistoNumber+32)->Fill( lz, (z * wght / gray));

    // Histogram 33: Squared-weighted specific energy in (keV/g)^2
   theAnaMgr->GetHisto1(theHistoNumber+33)->Fill( lz, (z * z * wght / (gray * gray)));

    // Histogram 41: Number of hits in the selection region
   theAnaMgr->GetHisto1(theHistoNumber+41)->Fill( nHsel, 1.);

    // Histogram 42: Number of hits in the site
   theAnaMgr->GetHisto1(theHistoNumber+42)->Fill( nHsite, 1.);

    // Histogram 43: Number of hits both in site and selection region
   theAnaMgr->GetHisto1(theHistoNumber+43)->Fill( nHint, 1.);

    // Histogram 61: Number of hits in site vs Energy imparted per event
    theAnaMgr->GetHisto2(theHistoNumber+61)->Fill( evtEdep / keV, nHsite, 1.);
  }
  else {
    G4ExceptionDescription msg;
    msg << "In this event, we had nHint == 0. "
        << "Something unexpected has occurred." << G4endl;
    G4Exception("MUTTrackerSD::EndOfEvent()", "MUTTrackerSD003", JustWarning, msg);
  }
}

