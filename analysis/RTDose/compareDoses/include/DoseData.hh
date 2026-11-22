#ifndef DoseData__HH
#define DoseData__HH

// Windows: static data used from other DLLs/programs must be dllexport/dllimport
#ifdef _WIN32
# ifdef compareDoses_EXPORTS
#  define COMPAREDOSES_DATA_API __declspec(dllexport)
# else
#  define COMPAREDOSES_DATA_API __declspec(dllimport)
# endif
#else
# define COMPAREDOSES_DATA_API
#endif
#include "globals.hh"
#include <cstdlib>
#include <vector>

enum DoseType{ST_ROOT, ST_TEXT};
enum DoseIndexValType{SIVT_LOWER, SIVT_MIDDLE, SIVT_UPPER};

class DoserData;

class DoseData 
{
public:
  DoseData(){};
  ~DoseData(){};

  DoseData operator+=( const DoseData& id2 );
  DoseData & operator*=( G4double wei );
  bool operator!=( const DoseData& id2 );
  bool operator==( const DoseData& id2 );

  long double GetDose() const;
  long double GetDoseSq() const;
  long double GetDoseError() const;
  
  virtual void Print( std::ofstream& ){
    G4cerr << " ERROR Print" << G4endl;
    exit(1);
  };
  void SetDoserData( DoserData* sd );
  //  void MultiplyByNEvents();

  G4String GetName() const {
    return theName;
  }
  void SetName( G4String name ) {
    theName = name;
  }
  G4double GetNEvents() const;

  G4bool IsTotal() const {
    return bTotal;
  }

  G4bool SelfCheck() const;

  COMPAREDOSES_DATA_API static int verbosity;

  COMPAREDOSES_DATA_API static DoseIndexValType theIndexValType;

protected:
  long double theDoseDivideError;
  long double theInvErrorExp2;
  G4int theNumberOfFiles;
  long double theDose;
  long double theDoseSq;
  long double theDoseError;
  G4String theName;

  G4bool bTotal;
  DoserData* theDoserData;

};

#endif
