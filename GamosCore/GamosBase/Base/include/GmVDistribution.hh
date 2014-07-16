#ifndef GmVDistribution_h
#define GmVDistribution_h 1

class G4Track;
class G4Step;
#include "globals.hh"
#include <vector>
#include <map>

class GmVData;
// class description:
//
//  This is the abstract base class of a distribution

class GmVDistribution
{
  
public: 
  GmVDistribution(G4String name);
  virtual ~GmVDistribution(){};

  virtual void BuildData() = 0;
  virtual void ReadFile() = 0;

  virtual void SetParameters( std::vector<G4String>& ){}
  
  virtual G4double GetValueFromStep(const G4Step* aStep) = 0;
  virtual G4double GetValueFromTrack(const G4Track* aTrack) = 0;

  virtual void Normalize() = 0;

  inline G4String GetName() const {
    return theName; }
  inline G4String GetClass() const {
    return theClass; }
  inline void SetClass( G4String cl ) {
    theClass = cl;  }

protected:
  G4String theName;
  G4String theClass;
  
  GmVData* theData;

};

#endif
