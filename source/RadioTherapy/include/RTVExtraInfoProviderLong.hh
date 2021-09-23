#ifndef RTVExtraInfoProviderLong_HH
#define RTVExtraInfoProviderLong_HH

#include "globals.hh"
#include <vector>
#include <bitset>
#include "RTVExtraInfoProvider.hh"

typedef std::bitset<32> bs32;
 
class RTVExtraInfoProviderLong : public RTVExtraInfoProvider
{
public: // With description

  // Construct with filter name
  RTVExtraInfoProviderLong();

  virtual ~RTVExtraInfoProviderLong(){};

  virtual void InitVariables();

  virtual void DumpIndices(std::ostream& out);

  void SetWordIndex( G4int index );

  static std::vector<bs32*> GetWords(){
    return theWords;
  }

  std::vector<G4String> GetIndexNames() const {
    return theIndexNames;
  }
 
  virtual G4String GetIndexName( G4int index ) const = 0;
 
  void SetWordsReset( G4bool wr ){
    bWordsReset = wr;
  }

  G4int GetShift() const {
    return theShift;
  }

  G4int GetNBits() const {
    return theNBits;
  }

  G4bool GetWordsReset() const {
    return  bWordsReset;
  }

protected:
  std::vector<G4String> theIndexNames;

  static std::vector<bs32*> theWords;

  G4int theShift;
  G4int theNBits;
  G4bool bWordsReset;

};

#endif
