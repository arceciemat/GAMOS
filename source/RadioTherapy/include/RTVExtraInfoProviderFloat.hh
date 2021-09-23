#ifndef RTVExtraInfoProviderFloat_HH
#define RTVExtraInfoProviderFloat_HH

#include "globals.hh"
#include <vector>
#include <bitset>
#include "RTVExtraInfoProvider.hh"
 
class RTVExtraInfoProviderFloat : public RTVExtraInfoProvider
{
public: // With description

  // Construct with filter name
  RTVExtraInfoProviderFloat();

  virtual ~RTVExtraInfoProviderFloat(){};

  virtual void DumpIndices(std::ostream& out);

  virtual void InitVariables();

  virtual void CheckZStop( G4double ){};

  float* GetWord() const{
    return theWord;
  }
  RTEIPos GetWordPos() const {
    return theWordPos;
  }

protected:
  float* theWord;
  RTEIPos theWordPos;
};

std::ostream & operator<<(std::ostream & out, const RTVExtraInfoProviderFloat& eip); 

#endif
