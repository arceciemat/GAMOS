#ifndef GmMovementMgr_h
#define GmMovementMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmMovementMgr    
Author:      P. Arce
Changes:      
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Class to manage the G4UserAction's and allow load on demand of any UserAction  */ 

#include <vector>
#include "globals.hh"
class GmVMovement;

//------------------------------------------------------------------------
class GmMovementMgr 
{ 
 private:
  GmMovementMgr();

 public:
  ~GmMovementMgr();

  //! Get the only instance 
  #ifdef WIN32
  #if defined GmMove_ALLOC_EXPORT 
    G4DLLEXPORT static GmMovementMgr* GetInstance();
  #else 
    G4DLLIMPORT static GmMovementMgr* GetInstance();
  #endif
#else
   static GmMovementMgr* GetInstance();
#endif

  std::vector<GmVMovement*> GetNewMovements( G4int eventID, G4double time );

  void AddMovement( GmVMovement* mv ) { 
    theMovements.push_back( mv ); }

  void InitialiseLastValue();

  void ReadMovementFile( G4String fileName );

  std::vector<GmVMovement*> GetMovements() const {
    return theMovements;
  }

  G4bool ApplyMovements() const { 
    return bApplyMovements; 
  }

private:
  static GmMovementMgr* theInstance;

  std::vector<GmVMovement*> theMovements;

  G4bool bApplyMovements; 
};

#endif
