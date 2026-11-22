#include "GmVModule.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GmGeomVerbosity.hh"

//------------------------------------------------------------------------
GmVModule::GmVModule( const std::vector<G4String>& par )
{
  std::vector<G4String>::const_iterator ite = par.begin()+1;
  ite++;
  for( ; ite != par.end(); ite++ ){
    theParams.push_back( *ite );
  }

}

//------------------------------------------------------------------------
void GmVModule::SetWord( G4String nameW, G4int idW )
{
  if( idW >= int(theParams.size()) ) {
    G4Exception("GmVModule::SetWord",
		"",
		FatalException,
		("Too few words building module "+theName+
		", check GAMOS documentation").c_str());
  }
  //  G4cout << " GmVModule::SetWord " << nameW << " " << theParams[idW] << G4endl;//GDEB
  if( idW >= 0 ) {
    theWords[nameW] = theParams[idW];
  } else {
    theWords[nameW] = "";
  }
  theWordIDs[nameW] = idW;
  
}


//------------------------------------------------------------------------
G4String GmVModule::GetParam( G4int ii )
{
  if( ii < 0 || ii >= G4int(theParams.size()) ) {
    G4Exception("GmVModule::GetParam",
		"Wrong argument index",
		FatalException,
		G4String("There are " + GmGenUtils::itoa(theParams.size()) + ", and you are asking for parameter number: " + GmGenUtils::itoa(ii)).c_str());
  }

  return theParams[ii];

}

//------------------------------------------------------------------------
G4String GmVModule::GetParam( G4String name )
{
  std::map<G4String,G4int>::const_iterator ite;
  for( ite = theWordIDs.begin(); ite != theWordIDs.end(); ite++ ) {
    if( (*ite).first.find(name) != std::string::npos ) {
      if( (*ite).second == -1 ) {
	 G4Exception("GmVModule::GetParams",
		"",
		FatalException,
		G4String("Cannot ask for a parameter that was not explicitly defined " + name ).c_str());
      }
      //      G4cout << "  GmVModule::GetParam( " << name << " = " <<(*ite).first << G4endl; //GDEB
      return theParams[(*ite).second];
    }
  }
  if( ite == theWordIDs.end() ) {
    for( ite = theWordIDs.begin(); ite != theWordIDs.end(); ite++ ) {
      G4cerr << " PARAM: " << (*ite).first << " = " << theParams[(*ite).second] << " : " << (*ite).second << G4endl;
    }
    G4Exception("GmVModule::GetParams",
		"",
		FatalException,
		G4String("Param name not found " + name ).c_str());
  }
  
  return theParams[0];

}

//------------------------------------------------------------------------
void GmVModule::ReplaceParam( G4int ii, G4String val )
{
  if( ii < 0 || ii >= G4int(theParams.size()) ) {
    G4Exception("GmVModule::ReplaceParam",
		"Wrong argument index",
		FatalException,
		G4String("There are " + GmGenUtils::itoa(theParams.size()) + ", and you are asking for parameter number: " + GmGenUtils::itoa(ii)).c_str());
  }
  theParams[ii] = val;
//  G4cout << "  GmVModule::ReplaceParam( " << ii << " = " << val << " NP " << theParams.size() << G4endl; //GDEB
}


//------------------------------------------------------------------------
void GmVModule::ReplaceParam( G4String name, G4String val )
{
  std::map<G4String,G4int>::const_iterator ite;
  for( ite = theWordIDs.begin(); ite != theWordIDs.end(); ite++ ) {
    if( (*ite).first.find(name) != std::string::npos ) {
      if( (*ite).second == -1 ) {
	G4Exception("GmVModule::GetParams",
		    "",
		    FatalException,
		    G4String("Cannot ask for a parameter that was not explicitly defined " + name ).c_str());
      }
      //      G4cout << "  GmVModule::ReplaceParam( " << name << " : " <<(*ite).first << " = " << val << G4endl; //GDEB
      theParams[(*ite).second] = val;
      break;
    }
  }
  // G4cout << "  GmVModule::ReplaceParam( " << ii << " = " << val << " NP " << theParams.size() << G4endl; //GDEB
  
  if( ite == theWordIDs.end() ) {
    for( ite = theWordIDs.begin(); ite != theWordIDs.end(); ite++ ) {
      G4cerr << " PARAM: " << (*ite).first << " = " << theParams[(*ite).second] << " : " << (*ite).second << G4endl;
    }
    G4Exception("GmVModule::ReplaceParam",
		"",
		FatalException,
		G4String("Param name not found " + name ).c_str());
  }

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
  if( GeomVerb(debugVerb) )  G4cout << "@@GmVModule::BuildObject:  " << foutStr << G4endl;
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


//------------------------------------------------------------------------
G4String GmVModule::PrintW(G4String name )
{
  return name + "= " + theWords[name] + " ";
}

//------------------------------------------------------------------------
G4String GmVModule::PrintWVal(G4String name )
{
  return name + "= " + GmGenUtils::ftoa(GmGenUtils::GetValue(theWords[name])) + " ";
}

//------------------------------------------------------------------------
G4double GmVModule::GetPosRound( G4double posCircleZ, G4double slope, G4double profR, G4double hvlX, G4int sign )
{
  G4double pos ;
  G4double slope2 = slope*slope;
#ifndef BEAMZPOS
  pos = posCircleZ*slope+(sign)*sqrt(profR*profR*(1.+slope2)-hvlX*hvlX);
#else
  G4double posCircleZ2 = posCircleZ*posCircleZ;
  //	G4double CCN = posCircleZ*slopeN-sqrt(profR*profR*(1.+slopeN2)-hvlXN*hvlXN);
  G4double CC = slope2*posCircleZ2-(1.+slope2)*(posCircleZ2-profR*profR)-hvlX*hvlX/4.*(1.+slope2)*(1.+slope2);
  pos = (posCircleZ+sqrt(posCircleZ2+CC))/slope;
#endif
  //  G4cout << " GetPosRound " << pos << " : " << posCircleZ << " " << slope << " " << profR << " " << hvlX << " " << sign << G4endl; //GDEB
  return pos;
}

