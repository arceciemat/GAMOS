#include "GmVModule.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GmGeomVerbosity.hh"

//------------------------------------------------------------------------
GmVModule::GmVModule( const std::vector<G4String>& par )
{
  std::vector<G4String>::const_iterator ite = par.begin()+1;
  theName = *ite++;
  for( ; ite != par.end(); ite++ ){
    theParams.push_back( *ite );
  }

}


//------------------------------------------------------------------------
G4String GmVModule::GetParams( G4int ii )
{
  if( ii < 0 || ii >= G4int(theParams.size()) ) {
    G4Exception("GmVModule::GetParams",
		"Wrong argument index",
		FatalException,
		G4String("There are " + GmGenUtils::itoa(theParams.size()) + ", and you are asking for parameter number: " + GmGenUtils::itoa(ii)).c_str());
  }

  return theParams[ii];

}


//------------------------------------------------------------------------
GmVModule::~GmVModule()
{
}


//------------------------------------------------------------------------
void GmVModule::BuildObject( std::ostringstream& fout )
{
  G4String foutStr = fout.str();
  std::vector<G4String> lineWords = GmGenUtils::GetWordsInString(foutStr);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) )  G4cout << " GmVModule::BuildObject FOUT" << fout.str() << "FF" << G4endl;
#endif
  theLineProcessor->ProcessLine( lineWords );

  fout.str("");

}


//------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const GmVModule& mod)
{
  out << " GEOMETRY_MODULE: " << mod.GetName() << G4endl;
  std::vector<G4String>::const_iterator ite;
  std::vector<G4String> par = mod.GetParams();
  out << " PARAMS:";
  for( ite = par.begin(); ite != par.end(); ite++ ){
    out << " " << *ite;
  }
  out << G4endl;

  return out;
}
