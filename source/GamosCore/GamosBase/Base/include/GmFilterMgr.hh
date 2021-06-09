#ifndef GmFilterMgr_hh
#define GmFilterMgr_hh

#include "G4RunManager.hh"
#include <map>
class GmVFilter;
// FindOrBuildFilter : first looks if a filter with same name already exists (using GetFilterFromList)
// if it dose not exists creates a new one (using CreateFilter)
// CreateFilter : it looks for a plug-in filter with the corresponding name and if it exits add parameters to it: when creating a filter from a user action command no parameters are passed, while creating it from '/gamos/filter' command parameters may exist 

class GmFilterMgr
{
private:
  GmFilterMgr();

public:
  ~GmFilterMgr();

  static GmFilterMgr* GetInstance();

  // for a user action it maybe a filter or an indexer
  GmVFilter* CreateFilter( std::vector<G4String> wl, G4bool bExists = true );

  GmVFilter* FindOrBuildFilter(std::vector<G4String> params, G4bool bExists = true );

  GmVFilter* GetFilterFromList(const G4String& filterName ) const;

  void DeleteFilters();

private:
  void AddFilter( GmVFilter* filter );

  static GmFilterMgr* theInstance;

  std::map<G4String,GmVFilter*> theFilters;

};

#endif
