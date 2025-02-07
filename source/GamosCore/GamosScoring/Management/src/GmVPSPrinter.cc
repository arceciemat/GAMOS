#include "GmVPSPrinter.hh"
#include "GmScoringVerbosity.hh"
#include "GmVPrimitiveScorer.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-------------------------------------------------------------------
GmVPSPrinter::GmVPSPrinter(G4String name) : theName( name )
{
  theNewUnit = -1;
}

//-------------------------------------------------------------------
void GmVPSPrinter::SetParameters( const std::vector<G4String>& params )
{
  if( params.size() != 2 && params.size() != 0 ){ //if 0 parameters, scorer unit/unitname is taken
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception((theName+"::SetParameters").c_str(),
		"There should be two parameters: UNIT UNIT_NAME",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }

  if( params.size() == 2 ) {
    theNewUnit = GmGenUtils::GetValue( params[0] );
    theUnitName = params[1];
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) )
      G4cout << this << theName + "::SetParameters " << theUnit << " " << theUnitName << G4endl;
#endif
  }

}

//-------------------------------------------------------------------
void GmVPSPrinter::CheckSpectrum(GmVPrimitiveScorer* scorer, G4bool bMustBeSpectrum )
{
  G4bool bSpectrum = scorer->IsSpectrum();
  if( bSpectrum != bMustBeSpectrum ) {
    G4String msg = "";
    if( bMustBeSpectrum ) {
      msg = "You cannot assign a printer which is of type PrinterSpectrum to a scorer which is not of type ScorerSpectrum \n";
    } else {
      msg = "You cannot assign a printer which is not of type PrinterSpectrum to a scorer which is of type ScorerSpectrum \n";
    }
    G4Exception("GmVPSPrinter::DumpAll",
		"",
		FatalException,
		(msg+ G4String("Trying to assign printer ")+GetName()+G4String(" to scorer ")+scorer->GetName()).c_str());
  }
}

//-------------------------------------------------------------------
void GmVPSPrinter::SetUnit(GmVPrimitiveScorer* scorer )
{
  theUnit = scorer->GetUnit();
  if( theNewUnit == -1 ) {
    theNewUnit = scorer->GetUnit();
    theUnitName = scorer->GetUnitName();
    // G4cout << "SetUnit " <<  theName << " " << theNewUnit << " Name " << theUnitName << G4endl; //GDEB
  }
  theUnitRatio = theUnit/theNewUnit;
  theUnitRatio2 = theUnitRatio*theUnitRatio;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(-debugVerb) ) 
    G4cout << " GmVPSPrinter::SetUnit " << theName << " " << theUnit << " New " << theNewUnit << " Ratio " << theUnitRatio << G4endl;
#endif
}
