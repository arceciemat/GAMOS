#include "GmGenerDistEnergyBetaDecay.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmIsotopeSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/SystemOfUnits.h"

//-----------------------------------------------------------------------
GmGenerDistEnergyBetaDecay::GmGenerDistEnergyBetaDecay()
{
  bFileRead = FALSE;
  SetCalculationType("interpolate");
}


//-----------------------------------------------------------------------
G4double GmGenerDistEnergyBetaDecay::GenerateEnergy( const GmParticleSource* source )
{
  if( source->GetType() != "IsotopeSource" ) {
    G4Exception(" GmGenerDistEnergyBetaDecay::GenerateEnergy",
		"Wrong argument",
		FatalErrorInArgument,
		"Source is not isotope");
  }

  G4double energy = 0.;
  GmIsotopeSource* isoSource = dynamic_cast<GmIsotopeSource*>(const_cast<GmParticleSource*>(source));

  if( isoSource->GetCurrentDecay()->GetProductName() == "gamma" ){
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyBetaDecay decays to gamma, using constant energy " << isoSource->GetName() << G4endl;
#endif
    energy = isoSource->GetCurrentDecay()->GetEnergy();
  } else {
    G4String filename;
    if( isoSource->GetCurrentDecay()->GetProductName() == "e+" ){
      filename = "EnergyDist."+source->GetParticleName()+".BetaPlus.dat";
    } else if( isoSource->GetCurrentDecay()->GetProductName() == "e-" ){
      filename = "EnergyDist."+source->GetParticleName()+".BetaMinus.dat";
    }
    if( !bFileRead ) {
      ReadFile( filename );
      bFileRead = TRUE;
    }
    energy = GmGenerDistEnergyFromFile::GenerateEnergy( source );
  
  }

  return energy;
}


//-----------------------------------------------------------------------
void GmGenerDistEnergyBetaDecay::ReadFile( const G4String fname )
{

  // read input file 
  G4String filename = fname;
  filename = GmGenUtils::FileInPath( filename, false );
  G4bool bOldFormat = true;
  std::ifstream* fintest = new std::ifstream(filename);
  if ( ! fintest->is_open() ) {
    size_t pos = filename.find(".dat");    
    filename.replace(pos,4,".csv");
    filename = GmGenUtils::FileInPath( filename, false );
    bOldFormat = false;
  }
  delete fintest;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyBetaDecay reading file " << filename << G4endl;
#endif


  GmFileIn fin = GmFileIn::GetInstance( filename );
  std::map<G4double,G4double> enerProb;
  
  if( bOldFormat ) {
    fin.SetSuppressQuotes(0);

    std::vector<G4String> wl;
    G4bool readingData = false;
    for( ;; ){
      if(! fin.GetWordsInLine( wl ) ) break;
      if( readingData == true ) {
	//      if( wl.size() == 1 && wl[0] == "\">" ) break;
	if( wl[0].find(";") == std::string::npos ) break;
	//--- separate line in words by ";"
	unsigned int i1 = 0;
	for(unsigned int ii = 0; ii < wl[0].length(); ii++ ) {
	  if( wl[0][ii] == ';' ) {
	    G4String val = wl[0].substr(i1,ii-i1);
	    enerProb[(ii+1)*CLHEP::keV] = GmGenUtils::GetValue( val );
	    i1 = ii+1;
#ifndef GAMOS_NO_VERBOSE
	    if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyBetaDecay  ener " << (ii+1)*CLHEP::keV << " prob " << val <<  G4endl;
#endif
	  }
    }
      }
      for( unsigned int iw = 0; iw < wl.size(); iw++ ){
	//      if( wl[iw] == "name=\"y[]\"" ) {
	if( wl[iw] == "name=y[]" ) {
	  readingData = true;
	  break;
	}
      }
    }
    unsigned int siz = enerProb.size();
    if( siz == 0 ) G4Exception(" GmGenerDistEnergyBetaDecay::ReadFile",	
			       "Wrong argument",
			       FatalErrorInArgument,
			       G4String("Data file is wrong: " + fname + " Please contact the GAMOS experts").c_str());

  } else {
    fin.SetSeparator(',');
    std::vector<G4String> wl;
    fin.GetWordsInLine( wl ); // Skip first line
    for(;;) {
      if(! fin.GetWordsInLine( wl ) ) break;
      enerProb[GmGenUtils::GetValue(wl[1])] = GmGenUtils::GetValue(wl[2]);
    }
  }

  SetEnerProb(enerProb);

  fin.Close();

}
