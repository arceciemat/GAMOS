#ifndef RTExtraInfoMgr_h
#define RTExtraInfoMgr_h

/*---------------------------------------------------------------------------   
ClassName:   RTExtraInfoMgr    
Author:      P. Arce
Changes:     Created. 02/2009 
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include "globals.hh"
#include <vector>
#include <iostream>
#include "iaea_record.hh"
#include "RTVExtraInfoProviderLong.hh"
class RTVExtraInfoProviderFloat;

//------------------------------------------------------------------------
class RTExtraInfoMgr 
{ 
 private:
  RTExtraInfoMgr();

 public:
  ~RTExtraInfoMgr();
  //! Get the only instance 
  static RTExtraInfoMgr* GetInstance();

  G4int AddExtraInfoProviderLong( RTVExtraInfoProviderLong* eip );
  G4int AddExtraInfoProviderFloat( RTVExtraInfoProviderFloat* eip );

  std::vector<RTVExtraInfoProviderLong*> GetProvidersLong() {
    return theProvidersLong;
  }
  std::vector<RTVExtraInfoProviderFloat*> GetProvidersFloat() {
    return theProvidersFloat;
  }

  std::vector<bs32*> GetWordsLong();
  std::vector<float*> GetWordsFloat();

  void DumpProviders( std::ostream& out = G4cout );
  void DumpProvidersLong( std::ostream& out = G4cout );
  void DumpProvidersFloat( std::ostream& out = G4cout );

  void InitExtraInfo( iaea_record_type *p_iaea_read );
  void FillExtraInfo( iaea_record_type *p_iaea_read );

  long GetExtraInfoLong(int nei) const {
    return theExtraInfoLongs[nei];
  }
  float GetExtraInfoFloat(int nei) const {
    return theExtraInfoFloats[nei];
  }

  G4int GetNumberOfLongs() {
    return theExtraInfoLongs.size();
  }
  G4int GetNumberOfFloats() {
    return theExtraInfoFloats.size();
  }

private:
  static RTExtraInfoMgr* theInstance;

  std::vector<RTVExtraInfoProviderLong*> theProvidersLong;
  std::vector<RTVExtraInfoProviderFloat*> theProvidersFloat;

  std::vector<long> theExtraInfoLongs;
  std::vector<float> theExtraInfoFloats;

};

#endif
