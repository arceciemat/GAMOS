#ifndef GmTouchablePhantomStructure_h
#define GmTouchablePhantomStructure_h 1

class G4VPhysicalVolume;
class G4VTouchable;
#include "GamosCore/GamosBase/Filters/include/GmVTouchableFilterVolume.hh"
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters touchables by G4VPhysicalVolume
// 
///////////////////////////////////////////////////////////////////////////////
class GmRegularParamUtils;
class GmReadPhantomStMgr;

class GmTouchablePhantomStructure : public GmVTouchableFilterVolume 
{

public: // with description
  GmTouchablePhantomStructure();
  
  virtual ~GmTouchablePhantomStructure();
  
public: // with description
  virtual G4bool AcceptTouchable( const G4VTouchable* );

  void show();

  virtual void SetParameters( std::vector<G4String>& params);
  G4bool CheckIndex( G4int idx );

protected:
  std::set<G4int> theIndices;

  GmRegularParamUtils* theRegularUtils;
  GmReadPhantomStMgr* theStructMgr;

};

#endif
