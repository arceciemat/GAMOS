// --------------------------------------------------------------------
// GEANT 4 class header file 
//
// Class Description:
//    Extends G4Track properties with information needed for the
//    force collision biasing operator.
//    The G4BOptrForceCollision class is made friend of this one in
//    order to keep unexposed to public most of the data members, as
//    they are used to control the logic.
//
// ------------------ GmBOptrForceCollisionTrackData ------------------
//
// Author: M.Verderi (LLR), October 2015
//
// --------------------------------------------------------------------

#ifndef GmBOptrForceCollisionTrackData_hh
#define GmBOptrForceCollisionTrackData_hh

class GmBOptrForceCollision;
#include "G4VAuxiliaryTrackInformation.hh"

enum class ForceCollisionState { free, toBeCloned, toBeForced, toBeFreeFlight };

class GmBOptrForceCollisionTrackData : public G4VAuxiliaryTrackInformation {

friend class GmBOptrForceCollision;
  
public:
  GmBOptrForceCollisionTrackData( const GmBOptrForceCollision* );
  ~GmBOptrForceCollisionTrackData();
  
  // -- from base class:
  void Print() const;

  // -- Get methods:
  G4bool                             IsFreeFromBiasing() const
  { return ( fForceCollisionState == ForceCollisionState::free);}
  // -- no set methods are provided : sets are made under exclusive control of GmBOptrForceCollision objects through friendness.
  
private:
  const GmBOptrForceCollision* fForceCollisionOperator;
  ForceCollisionState             fForceCollisionState;

  void Reset()
  {
    fForceCollisionOperator = nullptr;
    fForceCollisionState    = ForceCollisionState::free;
  }
  
};

#endif
