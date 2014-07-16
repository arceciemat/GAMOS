#ifndef GmPSPrinterMgr_H
#define GmPSPrinterMgr_H

#include <map>
#include <vector>
#include "globals.hh"
class GmVPSPrinter;
// FindOrBuildPSPrinter : first looks if a PSPrinter with same name already exists (using GetPSPrinterFromList)
// if it dose not exists creates a new one (using CreatePSPrinter)
// CreatePSPrinter : it looks for a plug-in PSPrinter with the corresponding name and if it exits add parameters to it: when creating a PSPrinter from a user action command no parameters are passed, while creating it from '/gamos/PSPrinter' command parameters may exist 

class GmPSPrinterMgr
{
private:
  GmPSPrinterMgr();

public:
  ~GmPSPrinterMgr();

  static GmPSPrinterMgr* GetInstance();

  // for a user action it maybe a PSPrinter or an indexer
  GmVPSPrinter* CreatePSPrinter( std::vector<G4String> wl, G4bool bExists = true );

  GmVPSPrinter* FindOrBuildPSPrinter(std::vector<G4String> params, G4bool bExists = true );

  GmVPSPrinter* GetPSPrinterFromList(const G4String& PSPrinterName ) const;

private:
  void AddPSPrinter( GmVPSPrinter* PSPrinter );

  static GmPSPrinterMgr* theInstance;

  std::map<G4String,GmVPSPrinter*> thePSPrinters;

};

#endif
