#ifndef GmVDataUser_hh
#define GmVDataUser_hh

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
class GmVFilter;
class GmVClassifier;

class GmVDataUser 
{
public:
  GmVDataUser();
  ~GmVDataUser(){};

  virtual void BuildData();
  virtual GmVData* BuildData2D( const G4String& dataName1, const G4String& dataName2 );
  virtual GmVData* BuildDataProf1D( const G4String& dataName1, const G4String& dataName2 );
  virtual GmVData* BuildDataProf2D( const G4String& dataName1, const G4String& dataName2, const G4String& dataName3 );

  GmDType GetDataType() const
  {
    return theDataType;
  }

  G4String GetDataTypeAsString() const;
  
protected:
  GmVData* Build1Data( const G4String& dataName );
  void SetUserName( G4String name ) {
    theUserName = name;
  }
  
protected:
  std::vector<GmVData*> theData;
  std::vector<GmVData*> theDoubleData;

  std::vector<G4String> theDataList;

  GmDType theDataType;

  G4bool bUseAtInitial;

  G4String theUserName; // same as User Action name
};

#endif
