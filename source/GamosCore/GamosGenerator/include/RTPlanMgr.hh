#ifndef RTPlanMgr_HH
#define RTPlanMgr_HH

/*---------------------------------------------------------------------------   
ClassName:   RTPlanMgr    
Author:      P. Arce
Changes:     18/02/21: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <vector>
#include "RTVPlanControlPoint.hh"
class RTVPlanSource;
class RTVPlanBeam;
class G4Event;
class GmParameterMgr;
class GmNumericDistributionLinLin;
class GmAnalysisMgr;
class G4EventManager;
class G4RunManager;

//------------------------------------------------------------------------
class RTPlanMgr 
{ 
 private:
  RTPlanMgr();

 public:
  ~RTPlanMgr();

  //! Get the only instance 
  static RTPlanMgr* GetInstance();

  void SetRTSource( RTVPlanSource* rts );

  void CalculateMetersetEvts();
  RTBeamStateData GetRTBeamStateData( G4double& time, G4Event* evt, G4bool& bOK ); // time is only used for printing 
  RTBeamStateData GetNextBeamStateData(G4int ie);
  void BookHistoControlPoint(G4int iib, G4int iicp);
  void FillRTHistoControlPoint( G4int hNum, const RTBeamStateData& spdata );
  void FillRTIonHistoControlPoint( G4int hNum, const RTBeamStateData& spdata );

  void DumpHistosAndScorers( G4bool bIsBeam );
  //-  void DumpHistosAndScorersBeam();
  void DumpHistosAndScorersControlPoint();
  void DumpHistosAndScorersSubControlPoint();
  
  void SetBeams( std::vector<RTVPlanBeam*> beams ) {
    theBeams = beams;
  }

  G4bool IsChangedBeam() const {
    return bChangedBeam;
  }
  G4bool IsChangedControlPoint() const {
    return bChangedControlPoint;
  }
  G4bool IsChangedBeamState() const {
    return bChangedBeamState;
  }
  void SetChangedControlPoint( G4bool bccp ) {
    bChangedControlPoint = bccp;
  }
  GmNumericDistributionLinLin * GetTimeEnergyDataDistribution() const {
    return theTimeEnergyDataDistribution;
  }
  G4bool IsInitProtonsPerMU() const {
    return bInitProtonsPerMU;
  }

private:
  void Initialize();
  
private:
  static RTPlanMgr* theInstance;
  RTVPlanSource* theRTSource;

  GmParameterMgr* theParamMgr;
  G4bool bInitProtonsPerMU;
  GmNumericDistributionLinLin * theTimeEnergyDataDistribution;
  std::vector<RTVPlanBeam*> theBeams;
  G4double theMSUnitsToRun;
  G4double theMSUnitsToSkip;
  G4int theCurrentMetersetEvtID;
  G4double theNEventsPerMSUnit;
  std::vector<G4double> theMetersetEvt; // event ID corresponding to accumulated meterset
  std::vector<RTVPlanSubControlPoint*> theSubCPEvt; // sub control point corresponding to accumulated meterset
  G4int theCurrentControlPoint;
  G4int theCurrentBeam;
  G4bool bChangedBeam;
  G4bool bChangedControlPoint;
  G4bool bChangedBeamState;
  G4int theEventID;
  G4EventManager* theG4EventManager;
  G4RunManager* theG4RunManager;

  GmAnalysisMgr* theAnaMgr;
  G4bool bRTHistoControlPoint;
  G4bool bRTIonHistoControlPoint;
  G4int theMaxNCP;
  G4int theHCurrentBeam;
  G4int theHCurrentControlPoint;
  G4double theHCurrentPosX;
  G4double theHCurrentPosY;
  G4int theHCurrentNumber;

  G4bool bDumpAtBeam;
  G4bool bDumpAtCP;
  G4bool bDumpAtControlPoint;
  G4bool bDumpAtSubControlPoint;
  G4double theLastBeamCHANGEDSubCPEvt;
  G4double theLastControlPointCHANGEDSubCPEvt; 


};

#endif
