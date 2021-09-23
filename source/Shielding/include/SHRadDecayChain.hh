#ifndef SHRadDecayChain_hh
#define SHRadDecayChain_hh 1

#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include "globals.hh"
#include "SHRadDecayChainSet.hh"
class G4Ions;
typedef std::vector<G4Ions*> vi;

class SHRadDecayChain
{

 public:
  SHRadDecayChain(G4String name, SHRadDecayChainSet* set);
  //  SHRadDecayChain( vi* chain, SHRadDecayChainSet* set);
  SHRadDecayChain( G4Ions* ion, SHRadDecayChainSet* set);
  SHRadDecayChain(SHRadDecayChain& chain, SHRadDecayChainSet* set);
  SHRadDecayChain(SHRadDecayChain& chain, G4int nIsot, SHRadDecayChainSet* set);
  ~SHRadDecayChain(){};

  bool operator==( const SHRadDecayChain& chain ) const;

  void AddIon( G4Ions* ion, G4double BR );
  void AddData( std::vector<G4String> wl );
  void ResetBR();
  void AddBR( G4double newBR );
  G4String GetSubName( G4int nIsot ) const;

  void SetName( G4String name );
  void Dump( std::ostream& out );
  void SetNAtomsInJob( G4double npart );
  void SetNAtomsInJob( double time, double npart );
  void SetSet( SHRadDecayChainSet* set ) {
    theSet = set;
  }
  //  void SumBR( G4double BR );

  double GetMinimumActivity() const;
  double GetMaximumActivity() const; 
  double GetActivity( double time) const;

  /*  std::vector<G4String> GetIonNames() const {
    return theIonNames;
  }
  std::vector<double> GetLifeTimes() const {
    return theLifeTimes;
    } */

  void DeleteShortLived( G4double minLifeTime );
  void PrintData( G4double endTime, std::ostream& out = G4cout );

  G4double GetNAtomsProduction( G4double nAtomsPrev, G4double time, G4bool bProduction );
  G4double GetNAtomsProduction1( G4double nAtomsPrev, G4double time );
  G4double GetNAtomsNoProduction1( G4double nAtomsPrev, G4double time );

  G4String GetName() const{
    return theName;
  }

  void AddLifeTime( double lifeTime ) {
    theInvLifeTimes.push_back( 1./lifeTime );
  }

  G4int GetNIons() const {
    return theIonNames.size(); // when read from file, there are not ions, but ion names
  }
  G4Ions* GetIon( int ii ) const;
  G4double GetNAtomsInJob() const;
  std::map<G4double,G4double> GetNAtomsInJob_all() const {
    return theNAtomsInJob;
  }

  double GetLifeTime( int ii ) const;
  double GetInvLifeTime( int ii ) const{
    return theInvLifeTimes[ii];
  }

  void SetInvLifeTime( int ii, double lt ){
    theInvLifeTimes[ii] = lt;
  }
  G4double GetBR() const {
    return theBR;
  }
  void SetBR( G4double br ) {
    theBR = br;
  }
  std::vector<G4double> GetBRs() const {
    return theBRs;
  }

  std::vector<G4Ions*> GetIons() const 
  {
    return theIons;
  }
  std::vector<G4String> GetIonNames() const 
  {
    return theIonNames;
  }
  std::vector<double> GetInvLifeTimes() const 
  {
    return theInvLifeTimes; 
  }

  std::map<double,double> GetActivities() const 
  {
    return theActivities;
  }

  void CheckLifeTimes();

  void AddActivity(G4double time, G4double activity );

  mdpdd* GetPlotData( G4double timeUnit = CLHEP::second );

private:
  G4double GetNAtomsPrev( G4double endTime );

 private:

  G4String theName; // chain name
  SHRadDecayChainSet* theSet;
  G4String theSetName;
  std::vector<G4Ions*> theIons; // one per isotope in the chain
  std::vector<G4String> theIonNames; // one per isotope in the chain. Store then because when reading from output file, G4Ions are not created
  //  G4int theNIons; // number of isotopes in the chain
  // invLifeTime: N = N_0 exp(- invLifeTime * t )
  std::vector<G4double> theInvLifeTimes; // one per isotope in the chain. Store then because when reading from output file, G4Ions are not created
  std::vector<G4double> theBRs; // one per isotope in the chain

  //  std::set<G4double> theTimes; // not really needed, as when printing, object will be asked for new times
  std::map<G4double,G4double> theNAtomsInJob; // counted from run one per time (they are the same for all times in a job, but the time is stored as a trick, to avoid summing them up twice if read twice (different times) in a job)
  // this will be recalculated, but they are stored if read from a file

  //-  std::map<double,double> theNAtomsInTime; // calculated one per time 
  std::map<double,double> theActivities; // calculated one per time 

  G4double theBR;
};

#endif

