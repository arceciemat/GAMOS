#ifndef RTFilterByEILong_h
#define RTFilterByEILong_h 1

class G4Region;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters particles by Extra Info Long 
// 
///////////////////////////////////////////////////////////////////////////////
class RTExtraInfoMgr;

class RTFilterByEILong : public GmVFilter 
{

  public: // with description
      RTFilterByEILong(G4String name);

      virtual ~RTFilterByEILong();

  public: // with description
      virtual G4bool AcceptStep(const G4Step* aStep);
      virtual G4bool AcceptTrack(const G4Track* aTrack);

      //
  void show();
  void SetParameters( std::vector<G4String>& params);

private:
  std::set<G4int> theIndices;

  RTExtraInfoMgr* theExtraInfoMgr;
};

#endif
