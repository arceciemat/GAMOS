#include "GmMicroyzHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
#include <set>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmMicroyzHit::GmMicroyzHit()
 : fTrackID(-1),
   fEdep(0.),
   fPos(G4ThreeVector()),
   fIncidentEnergy(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmMicroyzHit::~GmMicroyzHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmMicroyzHit::GmMicroyzHit(const GmMicroyzHit& right)
{
  fTrackID   = right.fTrackID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
  fIncidentEnergy = right.fIncidentEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const GmMicroyzHit& GmMicroyzHit::operator=(const GmMicroyzHit& right)
{
  fTrackID   = right.fTrackID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
  fIncidentEnergy      = right.fIncidentEnergy;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool GmMicroyzHit::operator==(const GmMicroyzHit& right) const
{
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmMicroyzHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmMicroyzHit::Print()
{
  G4cout
     << "  trackID: " << fTrackID 
     << "Edep: "
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " Position: "
     << std::setw(7) << G4BestUnit( fPos,"Length")
     << "IncidentEnergy: "
     << std::setw(7) << G4BestUnit(fIncidentEnergy,"Energy")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmMicroyzHit::Print(std::ostream& os, G4bool bVerbose )
{
  if( bVerbose ) os << " trackID: ";
  os << fTrackID << G4endl;
  if( bVerbose ) os << "Edep: ";
  os << std::setw(7) << G4BestUnit(fEdep,"Energy") << G4endl;
  if( bVerbose ) os << " Position: ";
  os << std::setw(7) << G4BestUnit(fPos,"Length") << G4endl;
  if( bVerbose ) os << "IncidentEnergy: ";
  os << std::setw(7) << G4BestUnit(fIncidentEnergy,"Energy") << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
