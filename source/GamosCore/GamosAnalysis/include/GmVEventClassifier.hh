#ifndef GmVEventClassifier_H
#define GmVEventClassifier_H

#include "globals.hh"

enum GMEvClType {GMEvCl_Gene, GMEvCl_Simu , GMEvCl_Digi, GMEvCl_Reco };

class GmVEventClassifier
{
public:
  GmVEventClassifier(){};
  virtual ~GmVEventClassifier(){};

  G4String GetName() const {
    return theName; }
  GMEvClType GetType() const {
    return theType; }
  
  virtual G4String GetClassificationName( int64_t ic ) = 0;

private:
  G4String theName;
  GMEvClType theType;
};

#endif
