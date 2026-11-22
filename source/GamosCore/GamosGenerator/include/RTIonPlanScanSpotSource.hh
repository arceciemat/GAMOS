#ifndef RTIonPlanScanSpotSource_H
#define RTIonPlanScanSpotSource_H
//
#include "RTVPlanSource.hh"

class RTIonPlanScanSpotSource : public RTVPlanSource
{
public:
  RTIonPlanScanSpotSource( const G4String& name, const G4String& partName );
  ~RTIonPlanScanSpotSource(){}
  virtual void ReadPlanData();

protected:
  virtual G4double GetSpotSize( RTBeamStateData spData );
  virtual void SetSpotSize( GmVGenerDistPosition2D* psitionDistribution2D, const RTBeamStateData& spdata );
  virtual void PlotBeamStates();
  G4double theBeamDispX;
  G4double theBeamDispY;
};

#endif // RTIonPlanScanSpotSource_H
