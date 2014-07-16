#ifndef GmParameterListNumeric_hh
#define GmParameterListNumeric_hh
//
//
#include<map>
#include "globals.hh"
#include "GmParameter.hh"

class GmParameterListNumeric: public std::map<G4String, GmParameter<G4double> >
{
public:
  GmParameterListNumeric(){}
  virtual ~GmParameterListNumeric(){}

  G4bool AddParam( const G4String& name, const G4double& val );
  G4double GmGenUtils::GetValue( const G4String& parname, G4bool& parExists );

  void DumpList( const G4String& type, std::ostream& out = G4cout );
  void DumpListNoValue( const G4String& type, std::ostream& out = G4cout );

};

#include "GmParameterListNumeric.icc"

#endif // GmParameterListNumeric_hh
