#ifndef GmCompoundScorer_hh
#define GmCompoundScorer_hh 1

class GmScoringMgr;
#include <vector>
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"

class GmCompoundScorer : public GmVPrimitiveScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmCompoundScorer(G4String name);
  virtual ~GmCompoundScorer(){ };
  
protected: // with description

  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

public: 
  virtual void EndOfEvent(G4HCofThisEvent*);
  virtual void DrawAll();
  virtual G4double GetNEvents( G4int index );
  virtual std::map<G4int,size_t> GetNFilled() const;
  
  void AddSubScorer(GmVPrimitiveScorer* data );

  void AddSubExpression( G4String expr );

  virtual void BuildCompoundScores();

  GmVPrimitiveScorer* FindOrCreateScorer(G4String scorerClass,G4String scorerName );

  virtual void SetParameters( const std::vector<G4String>& params );
  void SetNameSuffix( G4String ns ) {
    theNameSuffix = "_"+ns;
  }
  
  virtual void PropagateMFDToSubScorers();
  virtual void PropagateFilterToSubScorers();
  virtual void PropagateClassifierToSubScorers();
  virtual void PropagatePrinterToSubScorers(GmVPSPrinter* prt);
  virtual void PropagateTrkWeightToSubScorers(); 
  virtual void PropagateScoreErrorsToSubScorers();

  void SetExtraParams( const std::vector< std::vector<G4String> > ep ) {
    theExtraParams = ep;
  }
  

protected:

  std::vector<GmVPrimitiveScorer*> theSubScorers;
  std::vector<G4String> theSubScorerClasses;
  std::vector<G4String> theSubExpressions;
  G4bool bInitialized;
  
  GmScoringMgr* theScoringMgr;
  G4String theNameSuffix;

  std::vector< std::vector<G4String> > theExtraParams;

};

#endif
