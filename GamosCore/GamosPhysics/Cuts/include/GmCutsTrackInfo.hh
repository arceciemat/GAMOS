// --------------------------------------------------------------
#ifndef GmCutsTrackInfo_hh
#define GmCutsTrackInfo_hh


enum CutsTI{ CTI_ProdCuts = -1, CTI_MinRange = -2, CTI_RangeRejection = -3 };

#include "G4VUserTrackInformation.hh"

class GmCutsTrackInfo : public G4VUserTrackInformation
{
public:

  GmCutsTrackInfo(); 

  virtual ~GmCutsTrackInfo();

  void Print() const {};

  void SetCutsInfo( CutsTI ti );
  CutsTI GetCutsInfo() const { return theCutsTI; };

private:

  CutsTI theCutsTI;

};

#endif
