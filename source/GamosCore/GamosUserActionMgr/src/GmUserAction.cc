#include "GmUserAction.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "globals.hh"

//-----------------------------------------------------------------------
GmUserAction::GmUserAction()
{
  isEnabled = TRUE;
  theClassifier = 0;

  bApplyFiltersToStepping = TRUE;
  bApplyFiltersToPreTracking = TRUE;
  bApplyFiltersToPostTracking = TRUE;
  bApplyFiltersToStacking = TRUE;

  bCheckAllFiltersAtStepping = false;
  bCheckAllFiltersAtPreTracking = false;
  bCheckAllFiltersAtPostTracking = false;
  bCheckAllFiltersAtStacking = false;
}

//-----------------------------------------------------------------------
GmUserAction::~GmUserAction() 
{
}
//-----------------------------------------------------------------------
void GmUserAction::SetFilterOrClassifier(const G4String& objectName)
{
  std::vector<G4String> params;
  params.push_back( objectName );
  params.push_back( objectName );

  GmVFilter* filter = GmFilterMgr::GetInstance()->FindOrBuildFilter(params, false );
  GmVClassifier* classifier = 0;
  if( filter ) {
    theFilters.push_back(filter);
    theName += "_" + filter->GetName();
  } else {
    classifier = GmClassifierMgr::GetInstance()->FindOrBuildClassifier(params, false );
    if( classifier ) {
      if( theClassifier ) {
	G4Exception("GmUserAction::SetFilterOrClassifier ",
		    "More than one classifier, use GmCompoundClassifier",
		    FatalErrorInArgument,
		    G4String("Classifier= "+objectName+" to user action= " + theName).c_str());
      }
      theClassifier = classifier;
      theName += "_" + classifier->GetName();
    }
  }
  
  if( !filter && !classifier ) {
    G4Exception("GmUserAction::SetFilterOrClassifier ","No filter or classifier found",FatalErrorInArgument
		,G4String("Filter/Classifier= "+objectName+" to user action= " + theName).c_str());
  }

  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  bApplyFiltersToStepping = G4bool(paramMgr->GetNumericValue(theName+":ApplyFiltersToStepping", bApplyFiltersToStepping));
  bApplyFiltersToPreTracking = G4bool(paramMgr->GetNumericValue(theName+":ApplyFiltersToPreTracking", bApplyFiltersToPreTracking));
  bApplyFiltersToPostTracking = G4bool(paramMgr->GetNumericValue(theName+":ApplyFiltersToPostTracking", bApplyFiltersToPostTracking));
  bApplyFiltersToStacking = G4bool(paramMgr->GetNumericValue(theName+":ApplyFiltersToStacking", bApplyFiltersToStacking));


  bCheckAllFiltersAtStepping = G4bool(paramMgr->GetNumericValue(theName+":CheckAllFiltersAtStepping", bApplyFiltersToStepping));
  bCheckAllFiltersAtPreTracking = G4bool(paramMgr->GetNumericValue(theName+":CheckAllFiltersAtPreTracking", bApplyFiltersToPreTracking));
  bCheckAllFiltersAtPostTracking = G4bool(paramMgr->GetNumericValue(theName+":CheckAllFiltersAtPostTracking", bApplyFiltersToPostTracking));
  bCheckAllFiltersAtStacking = G4bool(paramMgr->GetNumericValue(theName+":CheckAllFiltersAtStacking", bApplyFiltersToStacking));

}
