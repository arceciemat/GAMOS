#ifndef GmDataMgr_hh
#define GmDataMgr_hh

#include "G4RunManager.hh"
#include <map>
class GmVData;

class GmDataMgr
{
private:
  GmDataMgr();

public:
  ~GmDataMgr();

  static GmDataMgr* GetInstance();

  GmVData* BuildData( G4String dataName );

  GmVData* CreateData( G4String dataName );

private:
  static GmDataMgr* theInstance;

  G4bool IsSeparator( const G4String word );
  std::string::size_type GetNextSeparator( G4int iSeparator, G4String dataName );

};

#endif
