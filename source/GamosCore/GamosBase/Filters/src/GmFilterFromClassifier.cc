#include "GmFilterFromClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Step.hh"

//----------------------------------------------------------------------
GmFilterFromClassifier::GmFilterFromClassifier(G4String name)
  :GmVFilter(name)
{
}


//----------------------------------------------------------------------
GmFilterFromClassifier::~GmFilterFromClassifier()
{ 
}


//----------------------------------------------------------------------
G4bool GmFilterFromClassifier::AcceptStep(const G4Step* aStep)
{
  //  G4cout << "  GmFilterFromClassifier::AcceptStep " << theClassifier->GetIndexFromStep( const_cast<G4Step*>(aStep) ) << " =?= " << theIndex << G4endl;
  if( theClassifier->GetIndexFromStep( const_cast<G4Step*>(aStep) ) == theIndex ) {
    return TRUE;
  } else {
    return FALSE;
  }
}


//----------------------------------------------------------------------
void GmFilterFromClassifier::show()
{
  G4cout << "---- GmFilterFromClassifier "<< theClassifier->GetName() << G4endl;
}


//----------------------------------------------------------------------
void GmFilterFromClassifier::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 2  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmFilterFromClassifier::SetParameters",
		"There should be at least two parameters: CLASSIFIER_NAME INDEX",
		FatalErrorInArgument,
		G4String("The current parameters are: "+parastr).c_str());
  }

  params.insert(params.begin(), params[0] );
  theClassifier = GmClassifierMgr::GetInstance()->FindOrBuildClassifier(params);

  theIndex = atoi(params[2]);
   
}
