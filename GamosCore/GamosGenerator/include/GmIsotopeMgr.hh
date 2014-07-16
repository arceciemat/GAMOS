#ifndef GmIsotopeMgr_HH
#define GmIsotopeMgr_HH

/*---------------------------------------------------------------------------   
ClassName:   GmIsotopeMgr    
Author:      P. Arce
Changes:     24/02/06: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 


#include <vector>
#include "globals.hh"
class GmIsotope;
class GmIsotopeSource;
class GmFileIn;

//------------------------------------------------------------------------
class GmIsotopeMgr 
{ 
 private:
  GmIsotopeMgr(){ };

 public:
  ~GmIsotopeMgr();

  //! Get the only instance 
  static GmIsotopeMgr* GetInstance();

  void ReadIsotopes( const G4String& filename );
  void ReadIsotopes( GmFileIn& fin );

  GmIsotopeSource* AddActiveIsotopeSource(  const G4String& sourceName, const G4String& isoName, const G4double energy );
  GmIsotope* GetIsotope( const G4String& isoName );

  std::vector<GmIsotope*> GetIsotopes() const { return theIsotopes; }
  //-  std::vector<GmIsotopeSource*> GetActiveIsotopes() const { return theActiveIsotopes; }
  //-  void SetIsotopeActivity(G4double);

private:
  static GmIsotopeMgr* theInstance;

  std::vector<GmIsotope*> theIsotopes;

  //-  std::vector<GmIsotopeSource*> theActiveIsotopes;
  
};

#endif
