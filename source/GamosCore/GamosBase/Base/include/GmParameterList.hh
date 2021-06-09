#ifndef GmParameterList_hh
#define GmParameterList_hh
//
//
#include<map>
#include "globals.hh"
#include "GmParameter.hh"

template<class TYP>
class GmParameterList: public std::map<G4String, GmParameter<TYP> >
{
public:
  GmParameterList(){}
  virtual ~GmParameterList(){}

  G4bool AddParam( const G4String& name, const TYP& val );
  TYP GetValue( const G4String& parname, G4bool& parExists );

  void DumpList( const G4String& type, std::ostream& out = G4cout );
  void DumpListNoValue( const G4String& type, std::ostream& out = G4cout );

};

#include "GmParameterList.icc"

#endif // GmParameterList_hh
