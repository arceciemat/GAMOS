#ifndef GmStringDataFilter_h
#define GmStringDataFilter_h 1

#include "GmVDataUser.hh"

#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

#include "globals.hh"

#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles if data value is in a defined user range (including limits)
//
///////////////////////////////////////////////////////////////////////////////

class GmStringDataFilter : public GmVFilter, 
		     public GmVDataUser
{
  
  //-------
public: 
  GmStringDataFilter(G4String name);
  
  virtual ~GmStringDataFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  virtual G4bool AcceptStep(const G4Step*);
  virtual G4bool AcceptStackedTrack(const G4Track* aTrack);
  
  void show();
  void SetParameters( std::vector<G4String>& params);

private:
  G4double theLowerLimit;
  G4double theUpperLimit;

  std::vector<GmVDataString*> theDataS;
  std::set<G4String> theStrings;
};

#endif
