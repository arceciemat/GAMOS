#include "SHNthValueLayerUA.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4Box.hh"

#ifndef GAMOS_NO_ROOT
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#endif

//-----------------------------------------------------------------
SHNthValueLayerUA::SHNthValueLayerUA()
{
  thePrimaryParticleName = "";
  thePrimaryEnergy = -1.;

}


//-----------------------------------------------------------------
SHNthValueLayerUA::~SHNthValueLayerUA() 
{
}

//-----------------------------------------------------------------
void SHNthValueLayerUA::BeginOfRunAction(const G4Run* )
{
  theReductions.push_back(2.);
  theReductions = GmParameterMgr::GetInstance()->GetVNumericValue(theName+":Reductions",theReductions);

  theNEvents = 0;
  theLayerWidth = GmParameterMgr::GetInstance()->GetNumericValue(theName+":LayerWidth", -1.);

  theNSteps.clear();
}

//-----------------------------------------------------------------
void SHNthValueLayerUA::BeginOfEventAction(const G4Event* )
{
  theNEvents++;

  G4PrimaryParticle* primary = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex(0)->GetPrimary(0);
  thePrimaryParticleName = primary->GetG4code()->GetParticleName();
  thePrimaryEnergy = GmGenUtils::GetKineticEnergy( primary->GetMass(), primary->GetMomentum().mag() );

}

//-----------------------------------------------------------------
void SHNthValueLayerUA::UserSteppingAction(const G4Step* aStep )
{
  G4int index;
  if (theClassifier ) {
    index = theClassifier->GetIndexFromStep(aStep);
  } else {
    index = 0;
  }
  if( theLayers.find(index) != theLayers.end() ) return;
  theLayers.insert(index);

  theNSteps[index]++;  

  //-  G4cout << " SHNthValueLayerUA NSteps " << index << " " << theNSteps[index] << " " << aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName() <<  G4endl;

}

//-----------------------------------------------------------------
void SHNthValueLayerUA::EndOfRunAction(const G4Run* )
{
  G4String Name = "%%% SHNthValueLayerUA";
  unsigned int ii;
  for( ii = 0; ii < theFilters.size(); ii++ ){
    Name += "_";
    Name += theFilters[ii]->GetName();
  }
  if( theClassifier ) {
    if( ii != 0 ) Name += "_";
    Name += theClassifier->GetName();
  }
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  Name += sepa+ " " + thePrimaryParticleName;

  G4String mateName = GmParameterMgr::GetInstance()->GetStringValue(theName+":MaterialName","");
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    if( (*cite)->IsParameterised() ) {
      if( mateName != "" ) {
	G4Exception("SHNthValueLayerUA::EndOfRunAction",
		    "Two parameterised volumes defined",
		    FatalErrorInArgument,
		    G4String("Defined material directly using parameter  /gamos/setParam " + theName + ":MaterialName NAME").c_str());
      }
      mateName = (*cite)->GetLogicalVolume()->GetMaterial()->GetName();

      if( theLayerWidth == -1. ) {
	G4VSolid* solid = (*cite)->GetLogicalVolume()->GetSolid();
	if( solid->GetEntityType() != "G4Box" ){
	  G4Exception("SHNthValueLayerUA::EndOfRunAction",
		      "Fatal Exception",
		      FatalException,
		      G4String("Trying to get the layer width while the parameterised solid is not a box: " + solid->GetName() + ", it is a " + solid->GetEntityType()).c_str());
	} else {
	  G4Box* box = (G4Box*)(solid);
	  theLayerWidth = 2.*box->GetZHalfLength();
	}
      }

    }
  }
  Name += " " + mateName;
  Name += " " + GmGenUtils::ftoa(thePrimaryEnergy) + " MeV";

  if( theLayerWidth == -1. ) {
    G4Exception("SHNthValueLayerUA::BeginOfRunAction",
		"Layer width is not defined",
		FatalErrorInArgument,
		G4String("Use parameter  /gamos/setParam "+theName+ ":LayerWidth VALUE").c_str());
  }

  if( theNSteps.size() == 0 ) {
    G4cout << Name << " NLayers " << theNSteps.size() << " NO REDUCTION CAN BE CALCULATED! " << G4endl;
    return;
  }

  //  G4cout << Name << " NLayers " << theNSteps.size() << G4endl;
  std::map< G4int,G4int>::reverse_iterator rite = theNSteps.rbegin();
  //  std::map< G4int,G4int>::iterator ite = theNSteps.begin();
  G4int nLayers = (*rite).first;
  GmAnalysisMgr* anaMgr =  GmAnalysisMgr::GetInstance("reduction");
  //  GmHisto1* his = new GmHisto1(Name.c_str(),Name.c_str(),nLayers,theLayerWidth*0.5, theLayerWidth*(nLayers+0.5));

  G4int histoNumber = G4int(100000000 * CLHEP::RandFlat::shoot());
  anaMgr->CreateHisto1D(Name.c_str(),nLayers,theLayerWidth*0.5, theLayerWidth*(nLayers+1.5), histoNumber); // the first bin is associated to layer 0, that is tracks that exit the layer 0 
  GmHisto1* his = anaMgr->GetHisto1(histoNumber);
  his->SetBinContent( 1, theNEvents );

  std::map<G4int,G4int>::const_iterator itei;  
  G4int jj = 2;
  for( itei =theNSteps.begin(); itei != theNSteps.end(); itei++, jj++ ){
    G4int index = (*itei).first;
    G4cout << "%%% " << theName << " NSTEPS: ";
    if( theClassifier ) {
      G4cout << theClassifier->GetIndexName(index);
    } else {
      G4cout << index;
    }
    G4cout << " = " << float((*itei).second)/theNEvents << G4endl;
    his->SetBinContent( jj, (*itei).second );
  }

#ifndef GAMOS_NO_ROOT
  TF1 *f1 = new TF1("f1", "expo", 0., theLayerWidth*nLayers);
  his->Fit("f1", "R");
  /*  TFitResultPtr fitres = his->Fit("f1", "R");
  //  TMatrixDSym cov = fitres->GetCovarianceMatrix();  //  to access the covariance matrix
  Double_t chi2  = fitres->Chi2(); // to retrieve the fit chi2
  int npar = fitres->NPar();
  std::vector<double> pars = fitres->Parameters(); 
  std::vector<double> parErrors = fitres->Errors(); 
    fitres->Print("");
    //    fitres->Print("V");     // print full information of fit including covariance matrix
  //  fitres->Write();        // store the result in a file
  */
  G4cout << Name << " : FIT chi2= " << f1->GetChisquare() << " nDoF " << f1->GetNDF() << G4endl;
#endif

  for( ii = 0; ii < theReductions.size(); ii++ ) {
    G4double reduc = theReductions[ii];
#ifndef GAMOS_NO_ROOT
    G4cout << Name << " : REDUCTION FROM FIT= " << reduc  
	   << " reached at depth= " << log(reduc) / (-f1->GetParameter(1)) 
	   << " +- " << log(reduc) * f1->GetParError(1) / sqr(f1->GetParameter(1)) << " mm " << G4endl;
#endif
    
    std::map<G4double,G4double> theNEventsInv;
    for( itei =theNSteps.begin(); itei != theNSteps.end(); itei++ ){
      theNEventsInv[float((*itei).second)/theNEvents] = (*itei).first;
    }
    
    G4double reducLengthS = GmGenUtils::InterpolateLogLin( 1./reduc, theNEventsInv );
    G4cout << Name << " : REDUCTION FROM SCORES= " << reduc  
	   << " reached at depth= " << reducLengthS*theLayerWidth << " mm" << G4endl;
  }
}


//-----------------------------------------------------------------
void SHNthValueLayerUA::EndOfEventAction(const G4Event* )
{
  theLayers.clear();
}

