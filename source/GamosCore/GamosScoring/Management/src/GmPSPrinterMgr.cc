#include "GmPSPrinterMgr.hh"

#include "GmVPSPrinter.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GmPSPrinterFactory.hh"
#include "GmPSPrinterSpectrumFactory.hh"
#endif

GmPSPrinterMgr* GmPSPrinterMgr::theInstance = 0;

//----------------------------------------------------------------------
GmPSPrinterMgr::GmPSPrinterMgr()
{
}

//----------------------------------------------------------------------
GmPSPrinterMgr* GmPSPrinterMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmPSPrinterMgr;
  }

  return theInstance;
}

//----------------------------------------------------------------------
GmPSPrinterMgr::~GmPSPrinterMgr()
{
}

//----------------------------------------------------------------------
GmVPSPrinter* GmPSPrinterMgr::CreatePSPrinter( std::vector<G4String> params, G4bool )
{
#ifdef ROOT5
  GmVPSPrinter* PSPrinter = Reflex::PluginService::Create<GmVPSPrinter*>(params[1],params[0]);
#else
  GmVPSPrinter* PSPrinter = GmPSPrinterFactory::get()->create(params[1],params[0]);
#endif
  
  if( PSPrinter == 0 ) {
    PSPrinter = GmPSPrinterSpectrumFactory::get()->create(params[1],params[0]);
  }
  if( PSPrinter == 0 ) {
    G4Exception(" GmPSPrinterMgr::CreatePSPrinter",
		"Wrong argument",
		FatalException,
		G4String(" PSPrinter type not found " + params[1] + " .Please check documentation and your '/gamos/scoring/printer' commands").c_str());
  }
  
  params.erase(params.begin()); 
  params.erase(params.begin());  
  
  PSPrinter->SetParameters( params );
  AddPSPrinter( PSPrinter );
  
  return PSPrinter;
}

//----------------------------------------------------------------------
void GmPSPrinterMgr::AddPSPrinter( GmVPSPrinter* PSPrinter )
{
  std::map<G4String,GmVPSPrinter*>::const_iterator ite = thePSPrinters.find( PSPrinter->GetName() );
  if( ite == thePSPrinters.end() ) {
    thePSPrinters[PSPrinter->GetName()] = PSPrinter;
  } else {
    G4Exception("GmPSPrinterMgr::AddPSPrinter",
" Adding two PSPrinters with the same name, review your '/gamos/scoring/printer' commands",FatalErrorInArgument,PSPrinter->GetName());
  }

}


//----------------------------------------------------------------------
GmVPSPrinter* GmPSPrinterMgr::FindOrBuildPSPrinter(std::vector<G4String> params, G4bool bExists )
{
  GmVPSPrinter* PSPrinter = GetPSPrinterFromList( params[0] );
  if( PSPrinter != 0 ) {
    return PSPrinter;
  } else {
    return CreatePSPrinter( params, bExists );
  }

}

//----------------------------------------------------------------------
GmVPSPrinter* GmPSPrinterMgr::GetPSPrinterFromList(const G4String& PSPrinterName ) const
{
  std::map<G4String,GmVPSPrinter*>::const_iterator ite = thePSPrinters.find( PSPrinterName );
  if( ite != thePSPrinters.end() ) {
    return (*ite).second;
  } else {
    return 0;
  }
}
