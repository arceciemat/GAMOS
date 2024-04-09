#include "DicomMgr.hh"

#include "DicomVImage.hh"
#include "DicomVOperator.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4UIcommand.hh"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcuid.h"

#ifndef ROOT5
#include "SEAL_Foundation/PluginManager/PluginManager/PluginManager.h"
#endif 
DicomMgr* DicomMgr::theInstance = 0;
 
//-----------------------------------------------------------------------------
DicomMgr* DicomMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new DicomMgr;
  }
  return theInstance;
}

//-----------------------------------------------------------------------------
DicomMgr::DicomMgr()
{
  theInPolygonNCheck = 4;
  bRotateXY180 = true;
  bImagePositionPatientIsPosVoxelCentre = true;
  
#ifndef ROOT5
  seal::PluginManager::get()->initialise();
#endif
}

//-----------------------------------------------------------------------------
void DicomMgr::AddImage( DicomVImage* image ) 
{
 if( DicomVerb(infoVerb) ) G4cout << " DicomMgr::AddImage " << image->GetName() << " " << DicomVImage::GetModalityStr(image->GetModality()) << G4endl;
 theImages.insert(mmmodi::value_type(image->GetModality(),image));
 
}

//-----------------------------------------------------------------------------
void DicomMgr::AddOperator( DicomVOperator* oper, G4int iOrder )
{
  std::vector<DicomVOperator*>::iterator ite;
  for(ite = theOperators.begin(); ite != theOperators.end(); ite++ ) {
    if( (*ite)->GetOrder() >= iOrder ) break;    
  }
  theOperators.insert( ite, oper );
  theOperatorsOrdered.insert(std::multimap<G4int,DicomVOperator*>::value_type(oper->GetOrder(),oper));
}

//-----------------------------------------------------------------------------
DicomVOperator* DicomMgr::FindOperator( G4String name )
{
  for(size_t ii = 0; ii < theOperators.size(); ii++ ) {
    if( name == theOperators[ii]->GetName() ) return theOperators[ii];
  }
  return (DicomVOperator*)0;
}

//-----------------------------------------------------------------------------
void DicomMgr::OperateAll()
{
 if( DicomVerb(debugVerb) ) G4cout << " N IMAGES " << theImages.size() << " N OPERS " << theOperators.size() << G4endl;
  mmmodi::const_iterator ite;
  for( ite = theImages.begin(); ite != theImages.end(); ite++ ) {
    DicomVImage* image = (*ite).second;
    if( DicomVerb(debugVerb) ) G4cout << "DicomMgr::OperateAll " << image->GetName() << " : " << image->GetModalityStr() << " auto " << image->IsAutomaticOperate() << G4endl; 
    if( image->IsAutomaticOperate() ) {
      std::multimap<G4int,DicomVOperator*>::const_iterator iteo;
      if( DicomVerb(infoVerb) ) G4cout << "DicomMgr::OperateAll NOperators " << theOperatorsOrdered.size() << G4endl; 
      for( iteo = theOperatorsOrdered.begin(); iteo != theOperatorsOrdered.end(); iteo++ ){
	DicomVOperator* oper = (*iteo).second;
	if( DicomVerb(infoVerb) ) G4cout << "DicomMgr::OperateAll Operator " << oper->GetName() << " auto " << oper->IsAutomaticOperate() << G4endl; 
	if( oper->IsAutomaticOperate() ) {
#ifndef GAMOS_NO_VERBOSE
	  if( DicomVerb(infoVerb) ) G4cout << "DicomMgr::OperateAll Operator " << oper->GetName() << " on image " << image->GetName() << G4endl;
#endif
 	  oper->Operate( image );
	}
      }
    }
  }
  
}

 //-----------------------------------------------------------------------------
void DicomMgr::OperateAll( DicomVImage* image )
{
  std::multimap<G4int,DicomVOperator*>::const_iterator iteo;
  if( DicomVerb(debugVerb) ) G4cout << "DicomMgr::OperateAll NOperators " << theOperatorsOrdered.size() << " ON IMAGE " << image->GetName() <<  G4endl;
  for( iteo = theOperatorsOrdered.begin(); iteo != theOperatorsOrdered.end(); iteo++ ){
    DicomVOperator* oper = (*iteo).second; 
    if( DicomVerb(debugVerb) ) G4cout << "DicomMgr::OperateAll Operator " << oper->GetName() << " auto " << oper->IsAutomaticOperate() << G4endl;
    if( oper->IsAutomaticOperate() ) {
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(debugVerb) ) G4cout << "DicomMgr::OperateAll Operator " << oper->GetName() << " on image " << image->GetName() << G4endl;
#endif
      oper->Operate( image );
    }
  }
}

//-----------------------------------------------------------------------------
std::vector<DicomVImage*> DicomMgr::GetImages(DIModality mod, G4bool bMustExist ) const
{
  std::vector<DicomVImage*> vf;
  std::pair<mmmodi::const_iterator,mmmodi::const_iterator> itep = theImages.equal_range(mod);
  mmmodi::const_iterator ite;
  for( ite = itep.first; ite != itep.second; ite++ ) {
    vf.push_back( (*ite).second );
  }
  if( vf.size() == 0 ) {
    if( bMustExist ) {
      G4Exception("DicomMgr::GetImages",
		  "",
		  FatalException,
		  ("NO IMAGE FOUND WITH MODALITY " + DicomVImage::GetModalityStr(mod)).c_str());
    }
  }
  if( DicomVerb(debugVerb) ) G4cout << " DicomMgr::GetImages " << DicomVImage::GetModalityStr(mod) << " N IMAGES " << vf.size() << G4endl;

  return vf;
}

//-----------------------------------------------------------------------------
G4int DicomMgr::GetNofImages( DIModality mod ) const
{
  std::pair<mmmodi::const_iterator,mmmodi::const_iterator> itep = theImages.equal_range(mod);
  //  G4cout << " DicomMgr::NofImages TOTAL " << theImages.size() << " : " << std::distance(itep.first,itep.second) << G4endl; //GDEB
  mmmodi::const_iterator ite;
  //  for( ite = theImages.begin(); ite != theImages.end(); ite++ ) {
    //    G4cout << " DicomMgr::NofImages " <<  DicomVImage::GetModalityStr(mod) << " = ? " <<  DicomVImage::GetModalityStr((*ite).first) << " : " << std::distance(itep.first,itep.second) << G4endl; //GDEB
  //  }

  return std::distance(itep.first,itep.second);
}

//-----------------------------------------------------------------------------
void DicomMgr::ReadLineDrawingOptios( G4String drawLinesFN )
{
  if( drawLinesFN != "" ) { 
    if( DicomVerb(debugVerb) ) G4cout << "@@@@@@@ Reading STRUCTURES TO DRAW FILE: " << drawLinesFN << G4endl;
    GmFileIn fin = GmFileIn::GetInstance(drawLinesFN,true);
    std::vector<G4String> wl;
    for(;;) {
      if( !fin.GetWordsInLine(wl) ) break;
      DicomLineDrawingOptions* ldo = new DicomLineDrawingOptions(wl);
      theLineDrawingOptions[ldo->GetName()] = ldo;
    }
  }
  
}
