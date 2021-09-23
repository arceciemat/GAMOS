#ifndef GmParameterMgr_hh
#define GmParameterMgr_hh

/*---------------------------------------------------------------------------   
ClassName:   GmParameterMgr    
Author:      P. Arce
Changes:     10/01: creation  
//----------------------------------------------- */

#include <vector>
#include <map>
#include <set>
#include "globals.hh"
#include "GmParameter.hh"
#include "GmParameterList.hh"

//enum paramType { PTint, PTdouble, PTstring, PTVint, PTVdouble, PTVstring, PTundef }; // no int because if first tima a parameter is used the "." is omitted, in case a second time is used, it will take an integer
enum paramType { PTdouble, PTstring, PTVdouble, PTVstring, PTundef }; 

//------------------------------------------------------------------------
class GmParameterMgr 
{ 
 protected:
  GmParameterMgr(){ };

 public:
  ~GmParameterMgr(){};

  //! Get the only instance 
  static GmParameterMgr* GetInstance();
  G4bool AddParam( const G4String& parstr, paramType typ = PTundef );

  G4double GetNumericValue( const G4String& parstr, const G4double def, G4bool mustExist = FALSE  );
  G4int GetIntegerValue( const G4String& parstr, const G4int def, G4bool mustExist = FALSE  );
  G4bool GetBooleanValue( const G4String& parstr, const G4bool def, G4bool mustExist = FALSE  );
  G4String GetStringValue( const G4String& parstr, const G4String& def, G4bool mustExist = FALSE  );
  std::vector<G4double> GetVNumericValue( const G4String& parstr, const std::vector<G4double> def, G4bool mustExist = FALSE );
  std::vector<G4int> GetVIntegerValue( const G4String& parstr, const std::vector<G4int> def, G4bool mustExist = FALSE );
  std::vector<G4String> GetVStringValue( const G4String& parstr, const std::vector<G4String>& defs, G4bool mustExist = FALSE );

  G4bool IsParameterInScript( const G4String& param );  
  
  void CheckParametersUsage();
  void PrintParametersUsage(G4int iPrint, std::ostream& out = G4cout, std::ostream& err = G4cerr);

public:
  G4bool AddParamOfDefinedType( const G4String& name, const std::vector<G4String>& valstr, paramType typ );

private:
  paramType SetParamType( G4bool noNumber, G4int nwords );

private:
  //template<class TYP>
  static GmParameterMgr* theInstance;

  int verbose;

  GmParameterList<double> theParametersDouble;
  GmParameterList<G4String> theParametersString;
  GmParameterList<std::vector<double> > theParametersVDouble;
  GmParameterList<std::vector<G4String> > theParametersVString;

  std::map<G4String,G4int> theParametersInScript;  
  std::set<G4String> theParametersInCode;  
};

#endif
