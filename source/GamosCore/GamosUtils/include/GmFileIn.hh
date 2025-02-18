#ifndef GmFileIn_HH
#define GmFileIn_HH

#include "globals.hh"
#include <vector>


class GmFileIn 
{
 public:
  GmFileIn();
  ~GmFileIn(){}
 private:
  GmFileIn( const G4String& name );

 public:
  // Get the only instance opening the file
  static GmFileIn& GetInstance( const G4String& name, G4bool bMustExist = false ); 

  // Get the only instance when file should be already opened
  static GmFileIn& GetInstanceOpened( const G4String& name ); 

  // Read a line and transform it to a vector of words 
  // return 0 if EOF found, else 1
  G4int GetWordsInLine( std::vector<G4String>& wl );

  // Read a line and transform it to a vector of doubles
  // return 0 if EOF found, else 1
  G4int GetWordsInLineDouble( std::vector<G4double>& wl );

  // Print out an error message indicating the line being read
  void ErrorInLine();

 // Access data members
  G4int Nline() 
  { 
    return theLineNo[theCurrentFile]; 
  }

  const G4String& GetName() 
  {
    return theName; 
  }

  G4bool eof();
  void Close();
  void Rewind();
  void DumpException( const G4String& sent );

  void SetSeparator( char sep ) 
  {
    theSeparator = sep;
  }
  void SetSuppressQuotes( G4bool sq )
  {
    bSuppressQuotes = sq;
  }
  void SetCheckQuotes( G4bool sq )
  {
    bCheckQuotes = sq;
  }
  std::ifstream* GetIfstream() const 
  {
    return theifstream;
  }
  G4String GetCurrentLine() const {
    return theStrTemp;
  }
  void SetUseCommentDoubleSlash( G4bool buse ) {
    G4cout << " SetUseCommentDoubleSlash " << buse << G4endl; //GDEB
    bUseCommentDoubleSlash = buse;
  }
  void SetUseCommentHashtag( G4bool buse ) {
    bUseCommentHashtag = buse;
  }

 private:
  void OpenNewFile( const char* filename, G4bool bMustExist = false ); 

 private:
  std::vector< std::ifstream* > theFiles;
  std::ifstream* theifstream;
  // Number of line being read
  std::vector<G4int> theLineNo;
  std::vector<G4String> theNames;
  int theCurrentFile;  // index of file being read in theFiles

 // private DATA MEMEBERS
  // Vector of class instances (each one identified by its name)
  static std::vector<GmFileIn*> theInstances;
 
  /// Name of file
  G4String theName;

  char theSeparator;
  G4bool bSuppressQuotes;
  G4bool bCheckQuotes;
  G4String theStrTemp;
  G4bool bUseCommentDoubleSlash;
  G4bool bUseCommentHashtag;
};

#endif 
