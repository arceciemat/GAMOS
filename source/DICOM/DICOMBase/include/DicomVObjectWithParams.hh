#ifndef DicomVObjectWithParams__HH
#define DicomVObjectWithParams__HH

#include <vector>
#include <iostream>
#include <map>
#include "globals.hh"
#include "dcmtk/dcmrt/seq/drtcos.h"

class DicomVObjectWithParams 
{ 
public:
  DicomVObjectWithParams();
  ~DicomVObjectWithParams(){};

public:
  void SetParam( G4String str, Float64 flt );
  Float64 GetParam( G4String str, bool bMustExist = FALSE );
  bool ParamExists( G4String str );

  std::map<G4String,Float64> GetParams() const {
    return theParams;
  }

  void SetParamStr( G4String str,  OFString fst );
  Float64 GetParamStr( G4String str );
  std::map<G4String,OFString> GetParamStrs() const {
    return theParamStrs;
  }

  Float64 CorrectByDirection( Float64 angle, OFString dir );

  virtual void Print( std::ostream& out, G4String name );

  void DicomObjectException(std::string className, std::string paramName);

protected:
  OFString fstr;
  Sint32 fint;
  Float64 ffloat;
  Float32 ffloat32;
  OFVector<Float64> fvfloat;
  OFCondition cond;
  
  std::map<G4String,Float64> theParams;
  std::map<G4String,OFString> theParamStrs;

};

#endif  

