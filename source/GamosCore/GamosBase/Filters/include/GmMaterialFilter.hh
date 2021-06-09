#ifndef GmMaterialFilter_h
#define GmMaterialFilter_h 1

class G4VProcess;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by material
// 
///////////////////////////////////////////////////////////////////////////////

class GmMaterialFilter : public GmVFilter 
{
public:
  GmMaterialFilter(G4String name);
  virtual ~GmMaterialFilter();

  virtual G4bool AcceptStep(const G4Step* aStep);

  void show();
  void SetParameters( std::vector<G4String>& params);
  
private:
  std::set<G4Material*> theMaterials;
  
};

#endif
