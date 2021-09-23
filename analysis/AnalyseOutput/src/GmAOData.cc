#include "GmAOData.hh"
#include "GmAODataSearch.hh"
#include "GmAOParam.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <iomanip>
std::map<G4String,G4int> GmAOData::theNEvents; 

//----------------------------------------------------------------------
GmAOData::GmAOData( const std::vector<G4String>& wl, GmAODataSearch* ds, G4String fname ) : theLine(wl), theDataSearch(ds), theFileName(fname)
{ 
}

//----------------------------------------------------------------------
G4String GmAOData::GetMessage() const
{
  return theDataSearch->GetMessage(); // should allow more flexibility
  /*
  G4String msg ;
  std::map<G4int,G4String>::const_iterator ite;
  for( ite = theMessages.begin(); ite != theMessages.end(); ite++ ){
    if( theMessages.size() == 1 && (*ite).second == "" ) {
      //      return BuildMessageFromLine();
    } else {
      msg += (*ite).second;
    }
  }
  return msg;
  */
}


//----------------------------------------------------------------------
void GmAOData::PrintData() const
{
  std::vector<G4int> wordn = theDataSearch->GetWordNumbers();
  for( unsigned int ii = 0; ii <theLine.size(); ii++ ){
    //    G4cout << " line " << ii << " = " << theLine[ii] << G4endl;
  }

  for( unsigned int ii = 0; ii < wordn.size(); ii++ ){
    if( verbose > 2 ) G4cout << "GmAOData::PrintData " << ii << " word " << wordn[ii] << " size " << theLine.size() << G4endl;

    if( G4int(theLine.size()) <= wordn[ii] ) {
      for( unsigned int jj = 0; jj < theLine.size(); jj++ ){
	G4cerr << " LINE " << jj << " " << theLine[jj] << G4endl;
      }
      G4Exception("GmAOData::PrintData",
		  "Too few words in line",
		  JustWarning,
		  G4String("Asking for word number " +GmGenUtils::itoa(wordn[ii]+1)+ " while there are only "+GmGenUtils::itoa(theLine.size())).c_str());
    } else {
      G4String linew = theLine[wordn[ii]];
      if( GmGenUtils::IsNumber(linew) ) {
	if( GmGenUtils::IsInt(linew) ) {
	  G4cout << " " << std::setw(theColumnWidth) << linew;
	} else {
	  G4cout << " " << std::setw(theColumnWidth) << std::setprecision(theColumnPrecision) << GmGenUtils::GetValue(linew);
	}
      } else {
	G4cout << " " << std::setw(8) << linew;
      }      
    }
    
  }
  
}

//----------------------------------------------------------------------
G4String GmAOData::GetWord( G4int nw ) const
{
  if( nw < 0 || nw > G4int(theLine.size()) ) {
    G4Exception("GmAOData::GetWord",
		"Error",
		FatalException,
		G4String("Asking for word number " + GmGenUtils::itoa(nw) + " while there are " + GmGenUtils::itoa(theLine.size())).c_str());
  }

  return theLine[nw];
}

G4int GmAOData::GetNEvents() 
{
  //first llok for n events read from "Number of events processed "
  std::map<G4String,G4int>::const_iterator ite = theNEvents.find(theFileName);
  if( ite != theNEvents.end() ) {
    return (*ite).second;
  } else {
    G4int nev = theDataSearch->GetNEvents();
    if( nev == -1 ) {
      G4Exception("GmAOData::GetNEvents",
		  "Error",
		  FatalException,
		  G4String("No line with 'Number of events processed ' found in file " + theFileName + " use the command '/run/verbose 1' or alternatively set the number of events in the meta file adding ':NEV NEVENTS'").c_str());
    } else {
      return nev; 
    }
  }

  return -1;
}
