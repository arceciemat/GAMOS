#ifndef GmVData_hh
#define GmVData_hh

#include "globals.hh"
#include <set>
#include <map>
#include <vector>
#ifndef GAMOS_NO_ROOT
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TProfile2D.h"
typedef TH1F GmHisto1;
typedef TH2F GmHisto2;
typedef TProfile GmHistoProfile1;
typedef TProfile2D GmHistoProfile2;
#else 
#include "GamosCore/GamosBase/Base/include/GmHisto1.hh"
#include "GamosCore/GamosBase/Base/include/GmHisto2.hh"
#include "GamosCore/GamosBase/Base/include/GmHistoProfile1.hh"
#include "GamosCore/GamosBase/Base/include/GmHistoProfile2.hh"
#endif

class G4Step;
class G4Track;
class G4Event;
class G4Run;
class GmAnalysisMgr;

enum GmDType { DTStep, DTTrack, DTSeco, DTEvent, DTRun };

class GmVData 
{
public:
  GmVData();
  virtual ~GmVData();

  G4bool CheckAvailability( GmDType );

  virtual void BookHisto( G4String hNamePrefix, G4int hNumber, G4int index, GmAnalysisMgr* anaMgr );

  virtual void FillHisto( const G4Step* aStep, G4int index );
  virtual void FillHisto( const G4Track* aTrack, G4int index );
  virtual void FillHisto( const G4Track* aTrack1, const G4Track* aTrack2, G4int index );
  virtual void FillHisto( const G4Event* anEvent, G4int index );
  virtual void FillHisto( const G4Run* aRun, G4int index );

  virtual void WriteText( const G4Step* aStep, G4bool bIsFirst );
  virtual void WriteText( const G4Track* aTrack, G4bool bIsFirst );
  virtual void WriteText( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst );
  virtual void WriteText( const G4Event* anEvent, G4bool bIsFirst );
  virtual void WriteText( const G4Run* aRun, G4bool bIsFirst );

  virtual void WriteBin( const G4Step* aStep );
  virtual void WriteBin( const G4Track* aTrack );
  virtual void WriteBin( const G4Track* aTrack1, const G4Track* aTrack2 );
  virtual void WriteBin( const G4Event* anEvent );
  virtual void WriteBin( const G4Run* aRun );

  virtual void WriteCout( const G4Step* aStep, G4bool bIsFirst );
  virtual void WriteCout( const G4Track* aTrack, G4bool bIsFirst );
  virtual void WriteCout( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst );
  virtual void WriteCout( const G4Event* anEvent, G4bool bIsFirst );
  virtual void WriteCout( const G4Run* aRun, G4bool bIsFirst );

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );
  virtual G4double GetValueFromEvent( const G4Event* anEvent, G4int index = 0 );
  virtual G4double GetValueFromRun( const G4Run* aRun, G4int index = 0 );

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2 );
  virtual G4String GetStringValueFromEvent( const G4Event* aEvent );
  virtual G4String GetStringValueFromRun( const G4Run* aRun );

  virtual void Initialise();
  virtual void Accumulate( const G4Step*, G4int index = 0 );

  template<class TYPE> void CheckSubData( TYPE& dataVal, TYPE subdataVal, const G4String name );

  G4String GetName() const {
    return theName;
  }
  void SetName( G4String name ) {
    theName = name;
  }

  virtual G4int GetHNBins() const {
    return theHNBins;
  }

  virtual G4double GetHMin() const;

  virtual G4double GetHMax() const;

  GmHisto1* GetHisto1(G4int index) const {
    return (*(theHistos1.find(index))).second; 
  }
  GmHisto2* GetHisto2( G4int index ) const {
    return (*(theHistos2.find(index))).second; 
  }
  GmHistoProfile1* GetHistosProf1(G4int index) const {
    return (*(theHistosProf1.find(index))).second; 
  }
  GmHistoProfile2* GetHistosProf2(G4int index) const {
    return (*(theHistosProf2.find(index))).second; 
  }

  void AddSubData(GmVData* data );

  void AddSubExpression( G4String expr );

  void SetAccumulating( G4bool baccu ) {
     bAccumulating = baccu;
  }
  G4bool IsAccumulating() const {
    return bAccumulating;
  }

  void SetInitial( G4bool baccu ) {
     bInitial = baccu;
  }
  G4bool IsInitial() const {
    return bInitial;
  }

  std::map<G4int, G4double> GetDataAccumulated() const 
  {
    return theDataAccumulated;
  }

  void SetFileOutText( std::ofstream* fout ) 
  {
    theFileOutText = fout;
  }
  void SetFileOutBin( std::ofstream* fout )
  {
    theFileOutBin = fout;
  }
  std::ofstream* GetFileOutText() const 
  {
    return theFileOutText;
  }
  std::ofstream* GetFileOutBin() const 
  {
    return theFileOutBin;
  }

  G4int GetNBytes() const 
  {
    return theNBytes;
  }
  G4String GetCType() const
  { 
    return theCType;
  }

protected:
  G4String theName;
  std::set<GmDType> theExcludedTypes;

  std::map<G4int,GmHisto1*> theHistos1; //one histogram for each classifier index
  std::map<G4int,GmHisto2*> theHistos2;
  std::map< G4int, GmHistoProfile1* > theHistosProf1;
  std::map< G4int, GmHistoProfile2* > theHistosProf2;

  //for the cases where the data contain two sub data (for example to build 2D histograms)
  std::vector<GmVData*> theSubData;
  std::vector<G4String> theSubExpressions;

  G4int theHNBins;
  G4double theHMin;
  G4double theHMax;

  std::ofstream* theFileOutText;
  std::ofstream* theFileOutBin;

  G4bool bAccumulating;

  G4int theNBytes;
  G4String theCType;

  std::map<G4int, G4double> theDataAccumulated;

  G4bool bInitial;

private:
  static G4bool bComposeHLimits;

};
#endif
