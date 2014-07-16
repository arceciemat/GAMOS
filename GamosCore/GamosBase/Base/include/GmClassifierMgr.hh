#ifndef GmClassifierMgr_hh
#define GmClassifierMgr_hh

#include "G4RunManager.hh"
#include <map>
class GmVClassifier;
// FindOrBuildClassifier : first looks if a classifier with same name already exists (using GetClassifierFromList)
// if it dose not exists creates a new one (using CreateClassifier)
// CreateClassifier : it looks for a plug-in classifier with the corresponding name and if it exits add parameters to it: when creating a classifier from a user action command no parameters are passed, while creating it from '/gamos/classifier' command parameters may exist 

class GmClassifierMgr
{
private:
  GmClassifierMgr();

public:
  ~GmClassifierMgr();

  static GmClassifierMgr* GetInstance();

  GmVClassifier* CreateClassifier( std::vector<G4String> wl, G4bool bExists = true );
  GmVClassifier* FindOrBuildClassifier( std::vector<G4String> params, G4bool bExists = true );

  void SetClassifierIndices( std::vector<G4String> wl );

  GmVClassifier* GetClassifierFromList(const G4String& classifierName ) const;

  void DeleteClassifiers();

private:
  void AddClassifier( GmVClassifier* classifier );

  static GmClassifierMgr* theInstance;

  std::map<G4String,GmVClassifier*> theClassifiers;

};

#endif
