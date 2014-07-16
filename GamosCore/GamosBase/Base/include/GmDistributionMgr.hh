#ifndef GmDistributionMgr_hh
#define GmDistributionMgr_hh

#include "G4RunManager.hh"
#include <map>
class GmVDistribution;
// FindOrBuildDistribution : first looks if a filter with same name already exists (using GetDistributionFromList)
// if it dose not exists creates a new one (using CreateDistribution)
// CreateDistribution : it looks for a plug-in filter with the corresponding name and if it exits add parameters to it: when creating a filter from a user action command no parameters are passed, while creating it from '/gamos/filter' command parameters may exist 

class GmDistributionMgr
{
private:
  GmDistributionMgr();

public:
  ~GmDistributionMgr();

  static GmDistributionMgr* GetInstance();

  // for a user action it maybe a filter or an indexer
  GmVDistribution* CreateDistribution( std::vector<G4String> wl, G4bool bExists = true );

  GmVDistribution* FindOrBuildDistribution(std::vector<G4String> params, G4bool bExists = true );

  GmVDistribution* GetDistributionFromList(const G4String& filterName ) const;

  void DeleteDistributions();

private:
  void AddDistribution( GmVDistribution* filter );

  static GmDistributionMgr* theInstance;

  std::map<G4String,GmVDistribution*> theDistributions;

};

#endif
