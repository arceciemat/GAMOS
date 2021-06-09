#ifndef GmVTouchableFilterVolume_h
#define GmVTouchableFilterVolume_h 1

#include "globals.hh"
class G4VTouchable;

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Base class of volume checkers for GmVolumeConditionFilter
// 
///////////////////////////////////////////////////////////////////////////////

class GmVTouchableFilterVolume 
{
public: // with description
  GmVTouchableFilterVolume(){};
  
  virtual ~GmVTouchableFilterVolume(){};
  
public: // with description
  virtual G4bool AcceptTouchable( const G4VTouchable* ) = 0;
  
};

#endif
