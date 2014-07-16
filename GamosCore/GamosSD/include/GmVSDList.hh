#ifndef GmVSDList_HH
#define GmVSDList_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"
#include <vector>
#include <set>

class GmVSDList : public std::vector<GmVSD*>
{

private:
  GmVSDList();

public:
  static GmVSDList* GetInstance();
  ~GmVSDList();
  void AddSD( GmVSD* sd );
  
private:
  static GmVSDList* theInstance;
  std::set<G4String> theSDTypeList;
};

#endif
