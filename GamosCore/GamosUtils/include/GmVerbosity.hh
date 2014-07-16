#ifndef VERBOSE_H
#define VERBOSE_H

#include <iosfwd>

#include "globals.hh"

enum FilterLevel {silentVerb = -1, errorVerb = 0, warningVerb = 1, infoVerb=2, debugVerb=3, testVerb=4}; 

class GmVerbosity {
  
public:
  
  enum { nFilterLevel=6};  

  static const FilterLevel * levels();
  
  static const G4String * levelNames();
  
  static const FilterLevel & level(const G4String & name);
  
  static const G4String & levelName(int il);
  
public:
	
#ifdef WIN32
  #if defined GmUtils_ALLOC_EXPORT 
    G4DLLEXPORT static std::ostream & out;
  #else 
    G4DLLIMPORT static std::ostream & out;
  #endif
#else
  static std::ostream & out;
#endif

  ///
  explicit GmVerbosity(int il=warningVerb);

  ///
  GmVerbosity & operator()(int il=warningVerb) { 
	  level_=il;
	  return *this;}

  GmVerbosity & operator()(const G4String& il) { level_=level(il); return *this;}
  
  ///
  operator bool() const { return verbose_ && (level_ <= filter_);}

  ///
  void Verbose();

  ///
  void Silent();

  ///
  void SetFilterLevel(int il);
  ///
  int GetFilterLevel() const;

  // int level() const { return level_;}
  

private:

  bool verbose_;
  int filter_;
  int level_;
};

#include <ostream>

inline std::ostream & operator<<(std::ostream& o, const GmVerbosity& v) {
  
  return v ? o : GmVerbosity::out;
  
}


#endif // VERBOSE_H
