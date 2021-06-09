#ifndef GmG4PSData_h
#define GmG4PSData_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GamosCore/GamosData/Users/include/GmVDataUser.hh"

#include "G4THitsMap.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring value from a GmData
// 
///////////////////////////////////////////////////////////////////////////////

class GmG4PSData : public GmVPrimitiveScorer,
		       public GmVDataUser
{
 
 public: // with description
      GmG4PSData(G4String name);

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  public:
      virtual ~GmG4PSData();

  public: 
      virtual void EndOfEvent(G4HCofThisEvent*);

      virtual void DrawAll();
      virtual void PrintAll();
  virtual G4int GetIndex(G4Step*);

  void SetParameters( const std::vector<G4String>& params);

};



#endif
