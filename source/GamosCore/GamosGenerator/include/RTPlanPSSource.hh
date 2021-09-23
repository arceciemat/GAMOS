#ifndef RTPlanPSSource_H
#define RTPlanPSSource_H
//
#include "RTVPlanSource.hh"

class RTPlanPSSource : public RTVPlanSource
{
public:
  RTPlanPSSource( const G4String& name, const G4String& partName );
  ~RTPlanPSSource(){}
  virtual void ReadPlanData();

private:
  //  virtual G4double GetSpotSize( RTBeamStateData spData );
  virtual void SetSpotSize( GmVGenerDistPosition2D* psitionDistribution2D, const RTBeamStateData& spdata );
  virtual void PlotBeamStates();
};

#endif // RTPlanPSSource_H
