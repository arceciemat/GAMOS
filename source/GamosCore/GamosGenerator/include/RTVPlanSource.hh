#ifndef RTVPlanSource_H
#define RTVPlanSource_H
//
#include "GmParticleSource.hh"
#include "G4ParticleDefinition.hh"
#include "RTIonPlanControlPoint.hh"
#include "RTPlanParamReader.hh"
#include "RTPlanMgr.hh"
#include "G4RotationMatrix.hh"

class RTVPlanBeam;
class G4RunManager;
class G4VPhysicalVolume;
class GmVModule;
class GmGeometryUtils;
class GmModuleMgr;
class GmVGenerDistPosition2D;
class G4Event;
class GmNumericDistributionLinLin;
class GmParameterMgr;
class GmAnalysisMgr;

class RTVPlanSource : public GmParticleSource, public RTPlanParamReader
{
public:
  RTVPlanSource( const G4String& name);
  ~RTVPlanSource(){}

  virtual G4PrimaryVertex* GenerateVertex( G4double time );
  RTBeamStateData GetRTBeamStateData( G4double& time, G4Event* evt, G4bool& bOK ); // time is only used for printing 

  virtual G4String GetParticleName() const {
    return theParticleDef->GetParticleName(); }

  virtual void AssociatePlanVolumes( std::vector<G4String> wl );
  virtual void AssociatePlanLimitingDeviceAngle( std::vector<G4String> wl );
  virtual void DefineAcceleratorVolume( std::vector<G4String> wl );

  size_t GetNoBeams() const {
    return theBeams.size();
  }
  RTVPlanBeam* GetBeam( G4int ii );
  virtual void PlotBeamStates(){};
  std::map<G4String,GmVModule*> GetRTPlanGmModuleAssoc() const {
    return theRTPlanGmModuleAssoc;
  }
  std::map<G4String,std::vector<G4VPhysicalVolume*> > GetRTPlanG4PVsAssoc() const {
    return theRTPlanG4PVsAssoc;
  }

protected:
  //  virtual G4double GetSpotSize( RTBeamStateData spData ) = 0;
  virtual void SetSpotSize( GmVGenerDistPosition2D* psitionDistribution2D, const RTBeamStateData& spdata ) = 0;
  void Initialize( const G4String& partName);
  void InitializeGeometry();
  virtual void ReadPlanData() = 0;

  void BuildRTPlan2G4Associations();
  void MoveAccelerator(const RTBeamStateData& spdata);
  void MovePhantom(const RTBeamStateData& spdata);

  void PredictInitialDisplacement(const RTBeamStateData& spdata );

  void CheckParamInAllControlPoints(G4String rtVName);

  void MoveBeam(const RTBeamStateData& spdata);

private:
  void InitializeDistributions();
  void InitializeGeom();
  
protected:
  G4String thePlanFileName;
  G4String theBeamFileName;
  G4String theCPFileName;
  G4ParticleDefinition* theParticleDef;

  std::vector<RTVPlanBeam*> theBeams;

  G4bool bInitProtonsPerMU;
  GmNumericDistributionLinLin * theTimeEnergyDataDistribution;
  G4double theRotAngleX;
  G4double theRotAngleY;
  G4double theRotAngleZ;

  G4bool bEGauss;  
  G4double theEnergySigma;

  G4bool bDiscGaussian;
  G4double theDiscRadius;
  G4double theDiscSigma;

  G4bool bDirAngle;
  G4bool bDirAngle2D;
  G4double theOpeningAngle;
  G4double theOpeningAngleX;
  G4double theOpeningAngleY;
  
  GmGeometryUtils* theGeomUtils;
  GmModuleMgr* theModuleMgr;
  G4RunManager* theG4RunManager;
  
  G4int theEventID;
  G4bool bInitialized;
  
  std::map<G4String,GmVModule*> theRTPlanGmModuleAssoc; 
  std::map<G4String,G4String> theRTPlanVoluAssoc;
  std::map<G4String,std::vector<G4VPhysicalVolume*> > theRTPlanG4PVsAssoc;
  G4String theLDAVolu;
  G4VPhysicalVolume* theLDAG4PV;
  G4String theAcceleratorVolume;
  G4VPhysicalVolume* theAcceleratorPV;
  G4ThreeVector theAcceleratorInitialPos;
  G4ThreeVector theOrigPhantomTranslation;
  G4RotationMatrix theOrigPhantomRotMat;

  G4bool bCoordSystemPatient;
  GmVGenerDistPosition2D* thePositionDistribution2D;

  G4bool bInitializedGeom;
  G4bool bPhantom; 
  //  G4bool bBeamOriginAtZero;
  G4bool bIsocenterAtZero;  
  //  G4bool bIsocentreAtZero;

  GmParameterMgr* theParamMgr;
  void BookHistoControlPoint(G4int iib, G4int iicp);
  void FillRTHistoControlPoint(G4int hNum, const RTBeamStateData& data);
  void FillRTIonHistoControlPoint(G4int hNum, const RTBeamStateData& data);
  
  RTPlanMgr* theRTPlanMgr;

  G4bool bRTHistoControlPoint;
  G4double theLastLimitingDeviceAngle;
};

#endif // RTVPlanSource_H
