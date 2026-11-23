#include "RTClassifierByEILong.hh"
#include "RTExtraInfoMgr.hh"
#include "RTVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4Step.hh"

//---------------------------------------------------------------
RTClassifierByEILong::RTClassifierByEILong(G4String name) : RTVClassifierByExtraInfo( name )
{
  G4String fileName = GmParameterMgr::GetInstance()->GetStringValue(theName+":FileName","RTExtraInfoProvider.summ");

  //store map of index - names
  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  for( ;; ){
    if( ! fin.GetWordsInLine(wl) ) break;
    if( wl.size() < 6 ) continue;
    //    G4cout << "chk set index names " << wl[2]  << " " << wl[1] << G4endl;
    if( wl[2] == "INDEX" && wl[1] == "LONG" ) {
      theIndexNames[atoi(wl[3])] = wl[5];
    }
    //    G4cout << "set index names " << atoi(wl[3])  << " " << wl[5] << G4endl;
  }
  fin.Close();
}


//---------------------------------------------------------------
void RTClassifierByEILong::SetParameters( std::vector<G4String>& wl )
{
  if( wl.size() != 2 ) {
    G4Exception("RTClassifierByEILong::SetParameters",
		"Different than two parameters",
		FatalException,
		"Parameters should be FIRST_BIT NUMBER_OF_BITS");
  }

  theFirstBit = GmGenUtils::GetInteger(wl[0]);
  theNBits =  GmGenUtils::GetInteger(wl[1]);
  theLastBit = theFirstBit + theNBits;
  theMaxNFound = int64_t(sizeof(int64_t)*8/log(double(theLastBit))*log(float(2)));
  //  G4cout << " MaxNFound " << theMaxNFound << " " << sizeof(long) 
  //	 << " " << log(theNBits)/log(2) << " " << log(theNBits) 
  //	 << " " << log(2) << " " <<  theNBits << G4endl;
}



//---------------------------------------------------------------
int64_t RTClassifierByEILong::GetIndexFromStep(const G4Step* )
{
  G4int nWords = theExtraInfoMgr->GetNumberOfLongs();
  if( nWords == 0 ) return 0;

  int64_t index = 0;
  G4int nFound = 0;
  for( G4int ii = theFirstBit; ii < theLastBit; ii++ ){
    G4int iw = ii/32;
    if( theExtraInfoMgr->GetExtraInfoLong(iw) & 1<<ii ) {
      if( nFound > theMaxNFound ) {
	G4Exception("RTClassifierByEILong::GetIndexFromStep",
		    "Too many bits found, last will not be taken into account",
		    JustWarning,
		    G4String("There are already "+GmGenUtils::itoa(nFound)+" bits").c_str());
	break;
      }
      index += int64_t(pow(double(theLastBit),int(nFound))*ii);
      //      G4cout << " SET index " << ii << " = " << index << " , " << pow(theLastBit,nFound) << G4endl;
      nFound++;
    }
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << theName << " GetIndexFromStep " << index << " FROM " ;
#endif
  /*  for( G4int ii = 0; ii < nWords; ii++ ){
    G4cout << theExtraInfoMgr->GetExtraInfoLong(ii) << " ";
  } 
  G4cout << G4endl;

  G4cout << " Index name " << index << " = " << GetIndexFromStep(index) << G4endl;
  */

 if( theIndexMap.size() != 0 ) {
    std::map<int64_t,int64_t>::const_iterator ite = theIndexMap.find(index);
    if( ite == theIndexMap.end() ) {
      G4Exception("RTClassifierByEILong::GetIndexFromStep",
		  "Index not found",
		  FatalErrorInArgument,
		  G4String("Index = " + GmGenUtils::itoa(index)).c_str());
    } else {
      return (*ite).second;
    }
  }

  return index;
}


//---------------------------------------------------------------
G4String RTClassifierByEILong::GetIndexName(int64_t index)
{
  G4String name;
  int64_t isiz = log(double(index))/log(double(theLastBit)) + 1;
  //  G4cout << "GetIndexName " << isiz << " " << index << " " << log(index) << " " << log(theLastBit) << G4endl;
  for( int64_t ii = 0; ii < isiz;  ii++ ){
    int64_t indexT = int64_t(index/pow(double(theLastBit),ii)) % theLastBit;
    //    G4cout << " indexT " << indexT << " idx " << index << " " << index/pow(theLastBit,ii) << " " << pow(theLastBit,ii) << G4endl; 
    std::map<int64_t,G4String>::const_iterator ite = theIndexNames.find(indexT);
    if( ii != 0 ) name+="+";
    if( ite != theIndexNames.end() ) {
      name += (*ite).second;
    } else {
      name += GmGenUtils::itoa(indexT);
    }
  }

  return name;

}


//---------------------------------------------------------------
RTClassifierByEILong::~RTClassifierByEILong()
{
}


//---------------------------------------------------------------
std::set<int64_t> RTClassifierByEILong::GetIndexSet() const
{ 
  std::set<int64_t> indices;
  std::map<int64_t,G4String>::const_iterator ite;
  for( ite = theIndexNames.begin(); ite !=  theIndexNames.end(); ite++ ){
    indices.insert((*ite).first);
  }

  return indices;
}
    

//-------------------------------------------------------------
void RTClassifierByEILong::SetIndices( std::vector<G4String> wl )
{

  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
    theIndexMap[GmGenUtils::GetValue(wl[ii])] = GmGenUtils::GetValue(wl[ii+1]);
  }

}
