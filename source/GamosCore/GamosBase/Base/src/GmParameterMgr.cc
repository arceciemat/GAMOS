
#include "GmParameterMgr.hh"
#include "GmParameterMessenger.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
GmParameterMgr* GmParameterMgr::theInstance = 0;

//----------------------------------------------------------------------
GmParameterMgr* GmParameterMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmParameterMgr;
    new GmParameterMessenger( theInstance );
  }

  return theInstance;

}


//-----------------------------------------------------------------------
G4bool GmParameterMgr::AddParam( const G4String& parstr, paramType typ )
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmParameterMgr::AddParam",
		"Wrong argument",
		FatalErrorInArgument,
		"Parameter not valid, must have at least two-words: 'NAME=  VALUE', it has none");
  } else if( wl.size() < 2 ) { 
    G4Exception("GmParameterMgr::AddParam",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("!!! Parameter not valid, must have at least two-words: 'NAME=  VALUE', it has only one: "+wl[0]).c_str());
  }

  G4String name = wl[0];
  //  G4String valstr = wl[1];
  wl.erase( wl.begin() );

  if( theParametersInScript.find( name ) == theParametersInScript.end() ) {
    theParametersInScript[name] = 0;
  }

  return AddParamOfDefinedType( name, wl, typ );

}


//----------------------------------------------------------------------
G4bool GmParameterMgr::AddParamOfDefinedType( const G4String& name, const std::vector<G4String>& wl, paramType typ )
{
  G4bool bExists = TRUE;
  std::vector<G4String>::const_iterator ite;
  std::vector<G4int> values;
  std::vector<G4double> valuesd;

  switch ( typ ){
  case PTdouble:
    bExists = theParametersDouble.AddParam( name, GmGenUtils::GetValue(wl[0]));
    //  G4cout <<  theParametersDouble.size() << " theParametersDouble.AddParam  name " << name << " GetVal " << GetVal(wl[0]) << G4endl;
    break;
  case PTstring:
    bExists = theParametersString.AddParam( name, wl[0]);
    break;
  case PTVdouble:
    for( ite = wl.begin(); ite != wl.end(); ite++ ){
      valuesd.push_back( GmGenUtils::GetValue(*ite) );
   //   G4cout <<  theParametersDouble.size() << " theParametersVDouble.AddParam  name " << name << " GetVal " << valuesd.size()-1 << " = " << valuesd[valuesd.size()-1]  << G4endl;
    }
    bExists = theParametersVDouble.AddParam( name, valuesd);
    break;
  case PTVstring:
    bExists = theParametersVString.AddParam( name, wl);
    break;
  case PTundef:
  //--- Set parameter type
    G4bool noNumber = FALSE;
    for( ite = wl.begin(); ite != wl.end(); ite++ ){
      if( !GmGenUtils::IsNumberWithUnit( *ite ) ){
	noNumber = TRUE;
      }	
    }
    typ = SetParamType( noNumber, wl.size() );
    AddParamOfDefinedType( name, wl, typ );
    break;
  }

  //  std::vector<G4String> fileNames;
  //  GetVStringValue(name,fileNames);

  return bExists;
}

//----------------------------------------------------------------------
paramType GmParameterMgr::SetParamType( G4bool noNumber, G4int nwords )
{
  paramType typ;
  if( nwords == 1 ){
    if( noNumber ) {
      typ = PTstring;
    } else {
      typ = PTdouble;
    }
  } else {
    if( noNumber ) {
      typ = PTVstring;
    } else {
      typ = PTVdouble;
    }
  }

  //p  G4cout << " SetParamType " << " noNumber " << noNumber << " nwords " << nwords << G4endl;
  return typ;
}


//----------------------------------------------------------------------
G4double GmParameterMgr::GetNumericValue( const G4String& parstr, const G4double def, G4bool mustExist )
{
  if( theParametersInScript.find( parstr ) != theParametersInScript.end() ) {
    theParametersInScript[parstr] ++;
  }
  theParametersInCode.insert(parstr);

  G4bool parExists;
  G4double val = theParametersDouble.GetValue( parstr, parExists );
  //  G4cout << theParametersDouble.size() << " GmParameterMgr::GetNumericValue " << parstr << " exists= " << parExists << " default " << def << " value " << val << G4endl;
  if( parExists ) {
    return val;
  } else {
    if( mustExist ) {
      theParametersDouble.DumpList("DOUBLE");
      G4Exception("GmParameterMgr::GetNumericValue",
		  "Wrong argument",
		  FatalErrorInArgument,
		  ("Parameter is not set in command file "+parstr).c_str());
      return def;
    } else {
      return def;
    }
  }
 
}

//----------------------------------------------------------------------
G4int GmParameterMgr::GetIntegerValue( const G4String& parstr, const G4int def, G4bool mustExist )
{
  return G4int(GetNumericValue( parstr, def, mustExist ));
}

//----------------------------------------------------------------------
G4bool GmParameterMgr::GetBooleanValue( const G4String& parstr, const G4bool def, G4bool mustExist )
{
  return G4bool(GetNumericValue( parstr, def, mustExist ));
}

//----------------------------------------------------------------------
G4String GmParameterMgr::GetStringValue( const G4String& parstr, const G4String& def, G4bool mustExist )
{
  if( theParametersInScript.find( parstr ) != theParametersInScript.end() ) {
    theParametersInScript[parstr] ++;
  }
  for( GmParameterList<G4String>::iterator ites = theParametersString.begin(); ites != theParametersString.end(); ites++ ) {
    GmParameter<G4String> param = ites->second;
  }

  theParametersInCode.insert(parstr);

  G4bool parExists;
  G4String val = theParametersString.GetValue( parstr, parExists );
  if( parExists ) {
    return val;
  } else {
    // check if it is filled as a numeric value
    G4double vald = theParametersDouble.GetValue( parstr, parExists );
    if( parExists ) {
      return GmGenUtils::ftoa(vald);
    } else {
      if( mustExist ) {
	theParametersString.DumpList("STRING");
	G4Exception("GmParameterMgr::GetStringValue",
		    "Wrong argument",
		    FatalErrorInArgument,
		    ("Parameter is not set in command file "+parstr).c_str());
	return def;
      } else {
	return def;
      }
    }
  }

}


//----------------------------------------------------------------------
std::vector<G4double> GmParameterMgr::GetVNumericValue( const G4String& parstr, const std::vector<G4double> def, G4bool )
{
  if( theParametersInScript.find( parstr ) != theParametersInScript.end() ) {
    theParametersInScript[parstr] ++;
    //    G4cout << " ADD theParametersInScriptV NUM " << parstr << " N=" << theParametersInScript[parstr] << G4endl;//GDEB
  }
      
  theParametersInCode.insert(parstr);

  /*  std::vector<G4double> def;
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( defs );
  std::vector<G4String>::const_iterator ite;
  for( ite = wl.begin(); ite != wl.end(); ite++ ){
    def.push_back( GetVal(*ite) );
    }*/

  G4bool parExists;
  std::vector<G4double> val = theParametersVDouble.GetValue( parstr, parExists );
  /*  G4cout << theParametersDouble.size() << " GmParameterMgr::GetVNumericValue " << parstr << " exists= " << parExists << " default ";
  for( unsigned int ii = 0 ; ii < def.size(); ii ++ ) {
    G4cout << def[ii] << " ";
  }
  G4cout << " value ";
  for( unsigned int ii = 0 ; ii < val.size(); ii ++ ) {
    G4cout << val[ii] << " ";
  } 
  G4cout << G4endl;
  */

  if( parExists ) {
    return val;
  } else {
    //---look if it is a single double value
    G4double val1 = theParametersDouble.GetValue( parstr, parExists );
    if( parExists ) {
      std::vector<G4double> valvd;
      valvd.push_back( val1 );
      return valvd;
    } else {
      return def;
    } 

    //    G4double val1 = GetNumericValue( parstr, def[0], 0 );
    //   val.push_back( val1 );
    /*    if( mustExist ) {
      theParametersVDouble.DumpListNoValue("DOUBLE VECTOR"); //NOTE: DumpList cannot print a vector<double>
      G4Exception("GmParameterMgr::GetVNumericValue","ERROR",FatalErrorInArgument,("Parameter is not set in command file "+parstr).c_str());
      return def;
      } else { */
    //    return val;
    //    }
  }

}

//----------------------------------------------------------------------
std::vector<G4int> GmParameterMgr::GetVIntegerValue( const G4String& parstr, const std::vector<G4int> def, G4bool mustExist )
{
  
  std::vector<G4double> vval;
  for( size_t ii = 0; ii < def.size(); ii++ ) {
    vval.push_back( def[ii] );
  }
  vval = GetVNumericValue( parstr, vval, mustExist );
  
  std::vector<G4int> ival;
  for( size_t ii = 0; ii < vval.size(); ii++ ) {
    ival.push_back( G4int(vval[ii]) );
  }

  return ival;
}

//----------------------------------------------------------------------
std::vector<G4String> GmParameterMgr::GetVStringValue( const G4String& parstr, const std::vector<G4String>& def, G4bool )
{
  if( theParametersInScript.find( parstr ) != theParametersInScript.end() ) {
    theParametersInScript[parstr] ++;
    //    G4cout << " ADD theParametersInScript VSTRING " << parstr << " N=" << theParametersInScript[parstr] << G4endl;//GDEB
  }
  theParametersInCode.insert(parstr);

  G4bool parExists;
  std::vector<G4String> val = theParametersVString.GetValue( parstr, parExists );

  /*  for( GmParameterList<std::vector<G4String> >::const_iterator ite = theParametersVString.begin(); ite != theParametersVString.end(); ite++ ) {
    G4cout << parstr << " " << parExists << " PARAMETER VSTRING " << (*ite).first << " parExists " << parExists << " : " << val.size() << G4endl; //GDEB
    } */

  if( parExists ) {
    return val;
  } else {
    //---look if it is a single string value
    G4String val1 = theParametersString.GetValue( parstr, parExists );
    if( parExists ) {
      std::vector<G4String> valvs;
      valvs.push_back( val1 );
      return valvs;
    } else {
      //---- l
      std::vector<G4double> valdv = theParametersVDouble.GetValue( parstr, parExists );
      if( parExists ) {
	std::vector<G4String> valvs;
	for( unsigned int ii = 0; ii < valdv.size(); ii++ ) {
	  valvs.push_back( GmGenUtils::ftoa(valdv[ii]) );
	}
	return valvs;
      } else {
	G4double vald = theParametersDouble.GetValue( parstr, parExists );
	if( parExists ) {
	  std::vector<G4String> valvs;
	  valvs.push_back( GmGenUtils::ftoa(vald) );
	  return valvs;
	}else {
	  return def;
	}
      }
    }
  }

}

//----------------------------------------------------------------------
G4bool GmParameterMgr::IsParameterInScript( const G4String& param )
{ 
  if( theParametersInScript.find(param) != theParametersInScript.end() ){
    return TRUE;
  } else {
    return FALSE;
  }
}

//----------------------------------------------------------------------
void GmParameterMgr::CheckParametersUsage()
{
  PrintParametersUsage( 0 );
}

//----------------------------------------------------------------------
void GmParameterMgr::PrintParametersUsage( G4int iPrint, std::ostream& out, std::ostream& err )
{

  if( iPrint >= 0 ) {
    //---- print parameters not used (in script but not in C++ code)
    std::map<G4String,G4int>::const_iterator ites;
    G4bool bFound = false;
    for( ites = theParametersInScript.begin(); ites != theParametersInScript.end(); ites++ ) {
      if( (*ites).second == 0 ) {
	bFound = true;
      }
    }
    if( bFound ){
      err << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << G4endl;
      err << "%%%%% PARAMETERS NOT USED (DEFINED IN SCRIPT BUT NOT USED BY C++ CODE) " << G4endl;
      err << "%%%    MAYBE YOU HAVE MISSPELLED THEM? " << G4endl;
      err << "%%%    OR YOU HAVE SET THEM AFTER THE CODE THAT USES THEM? " << G4endl;
      err << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << G4endl;
      for( ites = theParametersInScript.begin(); ites != theParametersInScript.end(); ites++ ) {
	if( (*ites).second == 0 ) {
	  err << "PARAMETER: " << (*ites).first << std::endl;
	}
      }
      err << "%%%   PLEASE PUT THIS COMMAND AT THE END OF YOUR SCRIPT: " << G4endl;
      err << "%%%   /gamos/base/printParametersUsage 2 " << G4endl;
      err << "%%%   AND LOOK FOR A SIMILAR PARAMETER " << G4endl;
      err << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << G4endl;
    }
  }

  if( iPrint >= 1 ){
    //---- print parameters using default value (in C++ code but not in script)
    out << "%%%%% PARAMETERS USING DEFAULT VALUE (DEFINED IN C++ CODE BUT VALUE NOT DEFINED IN SCRIPT) " << G4endl;
    std::set<G4String>::const_iterator itec;
    /* G4bool bFound = false;
    for( itec = theParametersInCode.begin(); itec != theParametersInCode.end(); itec++ ){
      if( theParametersInScript.find( *itec ) == theParametersInScript.end() ){
	bFound = true;
      }
    }
    if( bFound) {
    */
    for( itec = theParametersInCode.begin(); itec != theParametersInCode.end(); itec++ ){
      if( theParametersInScript.find( *itec ) == theParametersInScript.end() ){
	out << "PARAMETER: " << *itec << std::endl;      
      }
    }
    //    }
  }

  if( iPrint >= 2 ) {
    std::map<G4String,G4int>::iterator ites;
    //---- print number of times each parameter is used 
    out << "%%%%% NUMBER OF TIMES EACH PARAMETER IS USED IN C++ CODE" << G4endl;
    for( ites = theParametersInScript.begin(); ites != theParametersInScript.end(); ites++ ) {
      out << " PARAMETER " << (*ites).first << " TIMES USED= " << (*ites).second << G4endl;
    }
  }
}
