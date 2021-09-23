//
#include "GmVisModelByEnergy.hh"
#include "G4VisTrajContext.hh"
#include "G4TrajectoryDrawerUtils.hh"
#include "G4Trajectory.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include <assert.h>

GmVisModelByEnergy::GmVisModelByEnergy(const G4String& name, 
				       G4VisTrajContext* context)
  : G4VTrajectoryModel( name, context )
  ,fName(name)
  ,fVerbose(false)
  ,fpContext(context) 
{
  
  // Create context object if none is provided. Model will
  // then use default G4VisTrajContext configuration
  if (0 == fpContext) fpContext = new G4VisTrajContext();
  G4cout << " GmVisModelByEnergy::GmVisModelByEnergy( " << G4endl; //GDEB
  //E10eV,E100eV,E1keV,E10keV,EBig}
  fMap[E10eV] = G4Colour(1.,0.,0.,1.);  // red
  fMap[E100eV] = G4Colour(0.9,0.6,0.,1.); // orange 
  //  fMap[E1keV] = G4Colour(1.,1.,0.,1.);   //yellow 
  fMap[E1keV] = G4Colour(0.,1.,0.,1.);  // green 
  fMap[E10keV] = G4Colour(0.,0.5,1.,1.);  // cyan
  fMap[EBig] = G4Colour(0.,0.,1.,1.);    // blue

}

// Draw method
void GmVisModelByEnergy::Draw(const G4VTrajectory& traj, 
		    const G4bool& visible) const
{
  G4cout << " GmVisModelByEnergy::Draw( " << G4endl; //GDEB
  G4Colour colour;

  //--- classify energies
  const G4Trajectory* g4traj = dynamic_cast<const G4Trajectory*>(&traj);
  G4double energy = g4traj->GetInitialKineticEnergy();
  G4cout << " PARTICLE " << g4traj->GetParticleName() << " E= " << energy << G4endl; //GDEB
  if( energy < 10*CLHEP::eV ) {
    fMap.GetColour(E10eV, colour);
  } else if( energy < 100*CLHEP::eV ) {
    fMap.GetColour(E100eV, colour);
  } else if( energy < 1*CLHEP::keV ) {
    fMap.GetColour(E1keV, colour);
  } else if( energy < 10*CLHEP::keV ) {
    fMap.GetColour(E10keV, colour);
  } else {
    fMap.GetColour(EBig, colour);
  }
     /*  if(charge>0.)      fMap.GetColour(Positive, colour);
  else if(charge<0.) fMap.GetColour(Negative, colour);
  else               fMap.GetColour(Neutral, colour);
     */
  G4VisTrajContext myContext(GetContext());
  myContext.SetLineColour(colour);

  if (GetVerbose()) {
    G4cout<<"G4TrajectoryDrawByCharge drawer named "<<Name();
    G4cout<<", drawing trajectory with energy, "<<energy<<G4endl;
    G4cout<<", with configuration:"<<G4endl;
    myContext.Print(G4cout);
  }
  myContext.Print(G4cout);

  G4TrajectoryDrawerUtils::DrawLineAndPoints(traj, myContext);
}
  
// Print configuration
void GmVisModelByEnergy::Print(std::ostream& ) const
{
  G4cout << " GmVisModelByEnergy::Print( " << G4endl; //GDEB
}


GmVisModelByEnergy::~GmVisModelByEnergy()
{
  delete fpContext;
}


const G4VisTrajContext&
GmVisModelByEnergy::GetContext() const 
{
  // Expect context to exist
  assert (0 != fpContext);
  return *fpContext;
}

G4String 
GmVisModelByEnergy::Name() const 
{
  return fName;
}

void
GmVisModelByEnergy::SetVerbose(const G4bool& verbose)
{
  fVerbose = verbose;
}

G4bool
GmVisModelByEnergy::GetVerbose() const
{
  return fVerbose;
}
