#include "RTPlanParamReader.hh"
#include "G4tgrUtils.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

//-----------------------------------------------------------------------
RTPlanParamReader::RTPlanParamReader()
{
}

//-----------------------------------------------------------------------
void RTPlanParamReader::ReadParams( GmFileIn& fin)
{
  std::vector<G4String> wl;
  for(;;) {
    if( !fin.GetWordsInLine(wl) ) break;
    //------------------------------- parameter number
    if( wl[0]== ":P" )  {
      CheckIfNewParameter( wl, 0 );
      theParams[ wl[1] ] = G4tgrUtils::GetDouble( wl[2] );
      //      G4cout << this << " ReadParams " << wl[1] << " = " << wl[2] << G4endl; //GDEB
  //------------------------------- parameter string
    } else if( wl[0] == ":PS" ) {
      CheckIfNewParameter( wl, 0 );
      theParamStrs[ wl[1] ] = wl[2];
    }
    //    G4cout << " ReadParams NPARAM " << theParams.size() << G4endl; //GDEB
  }

}

//-------------------------------------------------------------
void RTPlanParamReader::CheckIfNewParameter( const std::vector<G4String>& wl,
                                                   G4bool bMustBeNew  )
{
  //---------- Find first if it exists already
  std::map<G4String,G4double>::const_iterator itep = theParams.find( wl[1] );
  std::map<G4String,G4String>::const_iterator iteps = theParamStrs.find( wl[1] );
  if( itep != theParams.end() 
      || iteps != theParamStrs.end() ) {
    if( bMustBeNew ) {
      G4Exception("RTPlanParamReader::CheckParameter()",
		  "",
		  FatalException,
		  ("Parameter already exists: "+wl[1]).c_str());
    } else {
      G4Exception("RTPlanParamReader::CheckParameter()",
		  "",
		  JustWarning,
		  ("Parameter already exists: "+wl[1]).c_str());
    }
  }
  
  //---------- Check for miminum number of words read 
  G4tgrUtils::CheckWLsize( wl, 3, WLSIZE_EQ, "RTPlanParamReader::AddParameter");
}


//-------------------------------------------------------------
G4double RTPlanParamReader::GetParam(G4String parName, G4bool bMustExist ) const
{
  //  G4cout << " parName, " << parName << " " <<bMustExist << G4endl; //GDEB
  std::map<G4String,G4double>::const_iterator ite = theParams.find( parName );

  if( ite == theParams.end() && bMustExist ) {
    G4cerr << this << " NUMBER OF PARAMETERS " << theParams.size() << G4endl; 
    for( ite = theParams.begin(); ite != theParams.end(); ite++ ) {
      G4cerr << "PARAM= " << (*ite).first << " : " << (*ite).second << G4endl;
    }
    G4Exception("RTPlanParamReader::GetParam",
		"",
		FatalException,
		("Parameter not found : " + parName).c_str());
  } else {
    return (*ite).second;
  }

  return 0.;
}

//-------------------------------------------------------------
G4bool RTPlanParamReader::ParamExists(G4String parName) const
{
  std::map<G4String,G4double>::const_iterator ite = theParams.find( parName );

  if( ite == theParams.end() ) {
    return 0; 
  } else {
    return 1;
  }
}

//-------------------------------------------------------------
void RTPlanParamReader::SetParam(G4String parName, G4double val, G4bool bMustExist )
{
  if( bMustExist && theParams.find(parName) == theParams.end() ) {
    std::map<G4String,G4double>::const_iterator ite;
    for( ite = theParams.begin(); ite != theParams.end(); ite++ ) {
      G4cerr << " PARAM: " << (*ite).first << " = " << (*ite).second << G4endl;
    }
    G4Exception("RTPlanParamReader::SetParam",
		"",
		FatalException,
		("Parameter not found : " + parName).c_str());
  }
  theParams[parName] = val;
}

//-------------------------------------------------------------
void RTPlanParamReader::SetParamStr(G4String parName, G4String val, G4bool bMustExist )
{
  if( bMustExist && theParamStrs.find(parName) == theParamStrs.end() ) {
    std::map<G4String,G4String>::const_iterator ite;
    for( ite = theParamStrs.begin(); ite != theParamStrs.end(); ite++ ) {
      G4cerr << " PARAM: " << (*ite).first << " = " << (*ite).second << G4endl;
  }
    G4Exception("RTPlanParamReader::SetParamStr",
		"",
		FatalException,
		("Parameter not found : " + parName).c_str());
  }
  theParamStrs[parName] = val;
}
