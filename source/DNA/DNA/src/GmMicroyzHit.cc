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
/// \file GmMicroyzHit.cc
/// \brief Implementation of the GmMicroyzHit class

#include "GmMicroyzHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
#include <set>

G4ThreadLocal G4Allocator<GmMicroyzHit>* GmMicroyzHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmMicroyzHit::GmMicroyzHit()
 : G4VHit(),
   fTrackID(-1),
   fEdep(0.),
   fPos(G4ThreeVector()),
   fIncidentEnergy(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmMicroyzHit::~GmMicroyzHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmMicroyzHit::GmMicroyzHit(const GmMicroyzHit& right)
  : G4VHit()
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
