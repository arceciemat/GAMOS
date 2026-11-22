#ifndef RTVExtraInfoProvider_HH
#define RTVExtraInfoProvider_HH

#include <iostream>
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

enum RTEIPos { PosLong, PosFloat1, PosFloat2 };

class RTVExtraInfoProvider: public GmUserRunAction
{
public: // With description

  // Construct with filter name
  RTVExtraInfoProvider();

  virtual ~RTVExtraInfoProvider(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void DumpIndices(std::ostream& out) = 0;

  virtual void InitVariables(){};

  G4int GetOrder() const{
    return theOrder;
  }
  void SetOrder( G4int ord )  {
    theOrder = ord;
  }

protected:
  G4String theFileName;
  G4int theOrder; // the order in which providers are defined in the input script file

};

#endif
