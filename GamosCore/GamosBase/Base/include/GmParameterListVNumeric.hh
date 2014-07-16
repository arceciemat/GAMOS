#ifndef GmParameterListVNumeric_hh
#define GmParameterListVNumeric_hh
//
//
#include<map>
#include<vector>
#include "globals.hh"
#include "GmParameter.hh"

class GmParameterListVNumeric: public std::map<G4String, GmParameter<std::vector<G4double> > >
{
public:
  GmParameterListVNumeric(){}
  virtual ~GmParameterListVNumeric(){}

  G4bool AddParam( const G4String& name, const std::vector<G4double>& val );
  std::vector<G4double> GmGenUtils::GetValue( const G4String& parname, G4bool& parExists );

  void DumpList( const G4String& type, std::ostream& out = G4cout );
  void DumpListNoValue( const G4String& type, std::ostream& out = G4cout );

};

#include "GmParameterListVNumeric.icc"

#endif // GmParameterListVNumeric_hh
