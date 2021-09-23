#ifndef GmAOData_hh
#define GmAOData_hh

#include <vector>
#include <map>
#include "globals.hh"
class GmAODataSearch;

//------------------------------------------------------------------------
class GmAOData 
{ 
public:
  GmAOData( const std::vector<G4String>& wl, GmAODataSearch* ds, G4String fname );
  G4String GetMessage() const;
  void PrintData() const;

  std::vector<G4String> GetLine() const 
  {
    return theLine;
  }

  G4String GetWord(G4int ) const;

  GmAODataSearch* GetDataSearch() const
  {
    return theDataSearch;
  }

  G4int GetNEvents();

  static std::map<G4String,G4int> theNEvents; // filename - number of events processed

 private:
  std::vector<G4String> theLine;
  GmAODataSearch* theDataSearch;
  G4String theFileName;

};

#endif
