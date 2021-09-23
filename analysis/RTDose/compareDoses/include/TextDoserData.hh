#ifndef TextDoserData__HH
#define TextDoserData__HH
#include "DoserData.hh"

class TextDoserData : public DoserData
{
public:
  TextDoserData();
  ~TextDoserData(){};

public:

  TextDoserData operator+=( const TextDoserData& sd2 );
  bool operator!=( const TextDoserData& sd2 );
  virtual void Print( std::ofstream& fout );

  virtual G4double GetIndexVal( G4String doseName, G4bool bLogX );
  
};

#endif
