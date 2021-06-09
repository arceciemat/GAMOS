#ifndef GmPositionUserVolumes_HH
#define GmPositionUserVolumes_HH

#include <map>
class G4VSolid;
#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmVPositionOrigin.hh"

class GmPositionUserVolumes : public GmVPositionOrigin
{
public:
  GmPositionUserVolumes();
  ~GmPositionUserVolumes();

  virtual std::vector<GVSTouchableInfo*> AddTouchableInfos( const std::vector<G4String>& params ) const;
  void SetNewValue(G4UIcommand * command,G4String newValues);


private:
  void CheckNoSolidParams( const G4String& solidType, const unsigned int NoParamExpected, const unsigned int NoParam ) const; 
  GVSTouchableInfo* AddVolume( G4VSolid* solid, const std::vector<G4String>& params, unsigned int iSolid ) const;

};

#endif
