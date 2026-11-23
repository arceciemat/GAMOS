
#include "GmEventClassifierByGammaInteraction.hh"
#include "GmTrajPoint.hh"
#include "GmTrajStep.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

//---------------------------------------------------------------
GmEventClassifierByGammaInteraction::GmEventClassifierByGammaInteraction()
{
  bProcessNamesBuilt = FALSE;
}

//---------------------------------------------------------------
GmEventClassifierByGammaInteraction::~GmEventClassifierByGammaInteraction()
{
}

//---------------------------------------------------------------
int64_t GmEventClassifierByGammaInteraction::Classify( const std::vector<GmTrajPoint*>& interactionList )
{
  //  G4cout << " GmEventClassifierByGammaInteraction::Classify " << interactionList.size() << G4endl;
  int64_t type = 0;

  std::vector<GmTrajPoint*>::const_iterator ite;
  for( ite = interactionList.begin(); ite != interactionList.end(); ite++ ){ 
    type += Classify1Interaction( (*ite)->GetProcessName() );
  }

  return type;

}

//---------------------------------------------------------------
void GmEventClassifierByGammaInteraction::BuildProcessNames()
{
  /*  thePhotoElecName = GmGetParticleMgr::GetInstance()->GetG4ProcessName("gamma","Phot");
  theComptonName = GmGetParticleMgr::GetInstance()->GetG4ProcessName("gamma","Compt");
  theRayleighName = GmGetParticleMgr::GetInstance()->GetG4ProcessName("gamma","Rayl");
  theConversionName = GmGetParticleMgr::GetInstance()->GetG4ProcessName("gamma","Conv");*/
  thePhotoElecName = "phot"; 
  theComptonName = "compt";
  theRayleighName = "Rayl";
  theConversionName = "conv";
  bProcessNamesBuilt = TRUE;
}

//---------------------------------------------------------------
int64_t GmEventClassifierByGammaInteraction::Classify( const std::vector<GmTrajStep*>& interactionList )
{
  //  G4cout << " GmEventClassifierByGammaInteraction::Classify " << interactionList.size() << G4endl;
  if( !bProcessNamesBuilt ) {
    BuildProcessNames();
  }

  int64_t type = 0;

  std::vector<GmTrajStep*>::const_iterator ite;
  for( ite = interactionList.begin(); ite != interactionList.end(); ite++ ){ 
    type += Classify1Interaction( (*ite)->GetProcessName() );
  }

  return type;

}

//---------------------------------------------------------------
int64_t GmEventClassifierByGammaInteraction::Classify1Interaction( const G4String& processName )
{
  int64_t type = 0;

  if( processName == thePhotoElecName ){ 
    type = 10000;

  }else if( processName == theComptonName ){
    type = 100;

    if( type >= 10000 ) {
      G4Exception("GmEventClassifierByGammaInteraction::Classify1Interaction",
		  "Error",
		  FatalException,
		  "More than 100 LowEnergyGompton  interactions " );
    }
    
  }else if( processName == theRayleighName ){ 
    type = 1;
    
  }else if( processName == theConversionName ){ 

  }else if( processName == "Transportation" ){ 

  }else {
    if( processName != "Rayl" ) G4Exception( "GmEventClassifierByGammaInteraction::Classify1Interaction",
					     "Warning",
					     JustWarning,
					     G4String(" Process not found" + processName).c_str() );
  }
   
  return type;
}

//---------------------------------------------------------------
G4String GmEventClassifierByGammaInteraction::GetClassificationName( int64_t ic ) 
{
  G4String name = "Evt:";

  int iPE = ic/10000;
  int iCOMP= (ic/100)%100;
  int iRAYL = ic%100;

  //  G4cout << " Class name " << " iCOMP  " << iCOMP << " iPE " << iPE << G4endl;
  name += "PE";
  name += GmGenUtils::itoa(iPE);

  name += "COMP";
  name += GmGenUtils::itoa(iCOMP);

  if( iRAYL > 0 ){
    name += "RAYL";
    name += GmGenUtils::itoa(iRAYL);
  }

  return name;
}
