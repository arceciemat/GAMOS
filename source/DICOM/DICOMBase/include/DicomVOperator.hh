#ifndef DicomVOperator__HH
#define DicomVOperator__HH

//#include "DicomVImage.hh"
class DicomMgr;
class DicomVImage;
#include "globals.hh"

class DicomVOperator
{ 
public:
  DicomVOperator( G4int iOrder = INT_MAX, G4String name = "");
  ~DicomVOperator(){};

public:
  virtual void Operate( DicomVImage* ) = 0;

  G4String GetName() const {
    return theName;
  }
  void SetName( G4String name) {
    theName = name;
  }
  G4int GetOrder() const {
    return theOrder;
  } 
  void SetOrder( G4int iord ) {
    theOrder = iord;
  }

  G4bool IsAutomaticOperate() const {
    return bAutomaticOperate;
  }
  void SetAutomaticOperate(G4bool bAO) {
    bAutomaticOperate = bAO;
  }

protected:
  G4String theName;
  G4bool bAutomaticOperate;

private:
  DicomMgr* theDicomMgr;
  G4int theOrder;
};

#endif
