#ifndef RTPlanSource_H
#define RTPlanSource_H
//
#include "RTVPlanSource.hh"

class RTPlanSource : public RTVPlanSource
{
public:
  RTPlanSource( const G4String& name, const G4String& partName );
  ~RTPlanSource(){}
  virtual void ReadPlanData();

private:
  //  virtual G4double GetSpotSize( RTBeamStateData spData );
  virtual void SetSpotSize( GmVGenerDistPosition2D* psitionDistribution2D, const RTBeamStateData& spdata );
  virtual void PlotBeamStates();
};

#endif // RTPlanSource_H
