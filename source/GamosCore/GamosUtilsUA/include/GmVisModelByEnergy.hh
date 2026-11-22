// Class Description:
// Abstract base class for trajectory drawing models. Trajectory drawing
// models are responsible for drawing individual trajectories according 
// to a particular style.
// Class Description - End:

#ifndef GmVisModelByEnergy_HH
#define GmVisModelByEnergy_HH

#include "G4VTrajectoryModel.hh"
#include "G4ModelColourMap.hh"

class GmVisModelByEnergy : public G4VTrajectoryModel
{

public:
  enum Energy {E10eV,E100eV,E1keV,E10keV,EBig};

  // Construct with context object
  GmVisModelByEnergy(const G4String& name, G4VisTrajContext* fpContext=0);

  // Destructor
  virtual ~GmVisModelByEnergy();
  
  // Draw method
  virtual void Draw(const G4VTrajectory& trajectory, 
		    const G4bool& visible = true) const;
  
  // Print configuration
  virtual void Print(std::ostream& ostr) const;
  
  // Accessors
  G4String Name() const ;
  const G4VisTrajContext& GetContext() const;
  
  // Set verbosity
  void SetVerbose(const G4bool&);
  G4bool GetVerbose() const;

private:

  // Private copy constructor and assigment operator - copying and
  // assignment not allowed.  Keeps Coverity happy.
  GmVisModelByEnergy (const GmVisModelByEnergy&);
  GmVisModelByEnergy& operator = (const GmVisModelByEnergy&);

  G4String fName;
  G4bool fVerbose;
  G4VisTrajContext* fpContext;

  G4ModelColourMap<Energy> fMap;

};

#endif

