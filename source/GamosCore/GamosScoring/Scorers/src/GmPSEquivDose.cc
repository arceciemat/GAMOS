#include "GmPSEquivDose.hh"
#include "GmScoringSurfaceBox.hh"
#include "GmScoringSurfaceTubs.hh"
#include "GmScoringSurfaceSphere.hh"
#include "GmScoringSurfaceOrb.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierMgr.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionUpper.hh"
#include "GamosCore/GamosScoring/Management/include/GmPSPrinterMgr.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

//-----------------------------------------------------------------------
GmPSEquivDose::GmPSEquivDose(G4String name)
 :GmVPrimitiveScorer(name)
{
  theUnit = CLHEP::mm*CLHEP::mm/(CLHEP::cm*CLHEP::cm);
  theUnitName = G4String("pSv");

  theGeometryUtils = GmGeometryUtils::GetInstance();  

  theTotalVolume = 0.;

  bUseVolume = true;

  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  G4String fParticle = paramMgr->GetStringValue(name+":Particle","");
  if( fParticle == "" ) {
    G4Exception("GmPSEquivDose::GmPSEquivDose",
		"",
		FatalException,
		("No Particle name defined, use /P "+name+":Particle").c_str());
  }

  //----- Add distribution to multiply scorer
  //--- Set distribution data
  // /gamos/setParam EquivDoseDist:Data FinalKineticEnergy 
  G4String fDataName = "";
  if( fParticle == "gamma" ) {
    fDataName = "FinalKineticEnergy";
  } else if( fParticle == "neutron" ) {
    fDataName = "log10(FinalKineticEnergy+1e-9)";
  }
  paramMgr->AddParam("EquivDoseDist:Data "+fDataName,PTstring);
  //  GmVData* theData = GmDataMgr::GetInstance()->BuildData(fDataName);
  //  fDistribution->SetData(fData);
  
  //--- Set distribution file name
  // /gamos/setParam EnergyDist:FileName Flux2Dose.gamma.ICRU57.lis
  G4String fDistFile = paramMgr->GetStringValue(name+":FileName","Flux2Dose."+fParticle+".ICRU57.lis");
  paramMgr->AddParam("EquivDoseDist:FileName "+fDistFile,PTstring);

  //--- Create distribution
  // /gamos/distribution EquivDoseDist GmNumericDistributionUpper
  GmNumericDistributionUpper* fDistribution = new GmNumericDistributionUpper("EquivDoseDist");
  //--- Multiply scorer by distribution
  // /gamos/setParam doseScorer:MultiplyByDistribution EquivDoseDist
  //- paramMgr->AddParam(name+":MultiplyByDistribution EquivDoseDist",PTstring);
  SetMultiplyingDistribution( fDistribution );
  
  //----- Add Classifier by Energy
  //  /gamos/classifier energyC GmClassifierByNumericData (FinalKineticEnergy+1e-3) 150 0. 1.5
  std::vector<G4String> classifParams = paramMgr->GetVStringValue(name+":EnergyBins",classifParams);
  if( classifParams.size() == 0 ) {
    G4Exception("GmPSEquivDose::GmPSEquivDose",
		"",
		FatalException,
		("No EnergyBins defined, use /P "+name+":EnergyBins").c_str());
  }
  classifParams.insert(classifParams.begin(),fDataName);
  classifParams.insert(classifParams.begin(),"GmClassifierByNumericData");
  classifParams.insert(classifParams.begin(),"energyC");
  GmVClassifier* fClassifier = GmClassifierMgr::GetInstance()->FindOrBuildClassifier( classifParams );
  SetClassifier( fClassifier );

  //----- Add particle filter
  // /gamos/scoring/addFilter2Scorer GmGammaFilter doseScorer
  std::vector<G4String> filterParams;
  if( fParticle == "gamma" ) {
    filterParams.push_back("GmGammaFilter");
    filterParams.push_back("GmGammaFilter");
  } else if( fParticle == "neutron" ) {
    filterParams.push_back("GmNeutronFilter");
    filterParams.push_back("GmNeutronFilter");
  }
  GmVFilter* fFilter = GmFilterMgr::GetInstance()->FindOrBuildFilter( filterParams );
  SetGmFilter( fFilter );
     
  //----- Add printer
  //  /gamos/scoring/printer histoPr GmPSPrinterHistos energy.vs.Hstar 150 0. 1.5
  //--- createPrinter
  std::vector<G4String> printerParams = paramMgr->GetVStringValue(name+":EnergyBins",printerParams);
  printerParams.insert(printerParams.begin(),"energy.vs.EquivDose ");
  printerParams.insert(printerParams.begin(),"GmPSPrinterHistos");
  printerParams.insert(printerParams.begin(),"histoPr");
  GmVPSPrinter* fPrinter = GmPSPrinterMgr::GetInstance()->FindOrBuildPSPrinter(printerParams);
  //   /gamos/scoring/addPrinter2Scorer histoPr doseScorer
  AddPrinter( fPrinter );
}



//-----------------------------------------------------------------------
G4bool GmPSEquivDose::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

  if( !AcceptByFilter( aStep ) ) return false;

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4double volume = GetVolume(aStep);

  G4double flux = aStep->GetStepLength()/volume;

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmPSEquivDose::ProcessHits flux= " << flux << " stepLength " << aStep->GetStepLength() << " volume= " << volume << G4endl;
#endif     
  FillScorer( aStep, flux, preStepPoint->GetWeight() );

  return TRUE;
}

