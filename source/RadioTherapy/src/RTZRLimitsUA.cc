#include "RTZRLimitsUA.hh"
#include "RTVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"


//---------------------------------------------------------------------
RTZRLimitsUA::RTZRLimitsUA()
{
}

//---------------------------------------------------------------------
void RTZRLimitsUA::BuildXYLimits()
{

  G4String fileName = GmParameterMgr::GetInstance()->GetStringValue("RTZRLimitsUA:FileName","xylimits.dat");
  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::map<G4double,XYdata>::const_iterator itexy;

  std::vector<G4String> wl;
  for( ;; ) {
    if( !fin.GetWordsInLine(wl) ) break;
    if( wl.size() != 5 ){
      G4cerr << " RTZRLimitsUA::BuildXYLimits: Line read = ";
      for( unsigned int ii = 0 ; ii < wl.size(); ii++ ){
	G4cerr << wl[ii] << " ";
      }
      G4cerr << G4endl;
      G4Exception("RTZRLimitsUA::BuildXYLimits",
		  "Reading file line with number of words different than 5",
		  FatalErrorInArgument,
		  "Line must have: Z X_MIN X_MAX Y_MIN Y_MAX");
    }
    XYdata dat;
    dat.xmin = GmGenUtils::GetValue(wl[1]);
    dat.xmax = GmGenUtils::GetValue(wl[2]);
    dat.ymin = GmGenUtils::GetValue(wl[3]);
    dat.ymax = GmGenUtils::GetValue(wl[4]);
    G4double zval = GmGenUtils::GetValue(wl[0]);
    //--- look if another dat at same Z
    itexy = theXYlimits.find(zval);
    if( itexy != theXYlimits.end() ) {
      XYdata dat2 = (*itexy).second;
      dat.xmin = std::min( dat.xmin, dat2.xmin );
      dat.xmax = std::max( dat.xmax, dat2.xmax );
      dat.ymin = std::min( dat.ymin, dat2.ymin );
      dat.ymax = std::max( dat.ymax, dat2.ymax );
    }
    theXYlimits[zval] = dat;
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout << " theXYlimits " << zval << " = " << dat << G4endl;
#endif
  }

  //----- Set world as minimum and maximum XY limits 
  //--- Get world logical volume and dimensions
  GetWorld();
  XYdata dat;
  dat.xmin = -theWorldX;
  dat.xmax = theWorldX;
  dat.ymin = -theWorldY;
  dat.ymax = theWorldY;
  theXYlimits[-theWorldZ] = dat;
  theXYlimits[theWorldZ] = dat;

  for( itexy = theXYlimits.begin(); itexy != theXYlimits.end(); itexy++ ) {
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout << " theXYlimits  z= " << (*itexy).first << " dat= " << (*itexy).second << G4endl;
#endif
  }
  
}


//---------------------------------------------------------------------
void RTZRLimitsUA::EndOfRunAction( const G4Run* )
{
  std::map<G4double,G4String>::const_iterator itename;
  std::map<G4double,G4int>::const_iterator itec;
    G4cout << "%%%%%%%%%%%% Number of killed particles with too big X or Y " << G4endl;
  for( itec = theXYcounts.begin(); itec != theXYcounts.end(); itec++ ){
    
    G4cout << " %%% Z: " << (*itec).first << " = " << (*itec).second << "     " << (*(theXYlimits.find( (*itec).first ))).second << G4endl;
  }
}
