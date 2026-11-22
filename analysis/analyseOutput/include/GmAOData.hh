#ifndef GmAOData_hh
#define GmAOData_hh

// Windows: static data used from other DLLs/programs must be dllexport/dllimport
#ifdef _WIN32
# ifdef analyseOutput_EXPORTS
#  define ANALYSEOUTPUT_DATA_API __declspec(dllexport)
# else
#  define ANALYSEOUTPUT_DATA_API __declspec(dllimport)
# endif
#else
# define ANALYSEOUTPUT_DATA_API
#endif

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

  ANALYSEOUTPUT_DATA_API static std::map<G4String,G4int> theNEvents; // filename - number of events processed

 private:
  std::vector<G4String> theLine;
  GmAODataSearch* theDataSearch;
  G4String theFileName;

};

#endif
