#ifndef GmVFilter_h
#define GmVFilter_h 1

class G4Track;
#include "G4Step.hh"
#include "globals.hh"
#include <vector>

// class description:
//
//  This is the abstract base class of a filter

class GmVFilter
{
  
public: // with description
  GmVFilter(G4String name);
  
public:
  virtual ~GmVFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*){ return TRUE; };
  virtual G4bool AcceptStackedTrack(const G4Track* aTrack){
    return AcceptTrack(aTrack); };
  virtual G4bool AcceptStep(const G4Step* aStep){ 
    return AcceptTrack(aStep->GetTrack()); }
  
  virtual void show(){}
  virtual void SetParameters( std::vector<G4String>& ){}

public:
  inline G4String GetName() const {
    return theFilterName; }
  inline G4String GetClass() const {
    return theClass; }
  inline void SetClass( G4String cl ) {
    theClass = cl;  }
  G4bool IsFutureFilter() const {
    return bFutureFilter;   
  }

protected:
  G4String theFilterName;
  G4String theClass;
  G4bool bFutureFilter;  
};

#endif
