#ifndef GmVGenerDist_HH
#define GmVGenerDist_HH

#include "globals.hh"
#include <vector>
class GmParticleSource;

class GmVGenerDist
{
public:
  GmVGenerDist(){};
  virtual ~GmVGenerDist(){};

  virtual void SetParams( const std::vector<G4String>& ){}; //it is not mandatory to implement this method

  virtual void ResetParams(); // if not implemented it will call method above

  void CopyParams( std::vector<G4String> params ) {
    theParams = params; }

  G4String GetName() const { 
    return theName; }

  GmParticleSource* GetParticleSource() const {
    return theParticleSource; }
  void SetParticleSource( GmParticleSource* src ) {
    theParticleSource = src; }


protected:
  G4String theName;

  std::vector<G4String> theParams;

  GmParticleSource* theParticleSource;
};

#endif
