#ifndef RTPlanParamReader_H
#define RTPlanParamReader_H
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include <map>

class RTPlanParamReader
{
public:
  RTPlanParamReader();
  ~RTPlanParamReader(){}

  void ReadParams( GmFileIn& fin);
  void CheckIfNewParameter( const std::vector<G4String>& wl,
			    G4bool bMustBeNew );
 
public:
  void SetParam(G4String parName, G4double val, G4bool bMustExist = false );
  void SetParamStr(G4String parName, G4String val, G4bool bMustExist = true );
  G4double GetParam(G4String parName, G4bool bMustExist = true) const;
  G4bool ParamExists(G4String parName) const;
  std::map<G4String,G4double> GetParams() const {
    return theParams;
  }
  std::map<G4String,G4String> GetParamStrs() const {
    return theParamStrs;
  }

protected:
  std::map<G4String,G4double> theParams;
  std::map<G4String,G4String> theParamStrs;

};

#endif // RTPlanParamReader_H
