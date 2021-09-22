#ifndef DicomVImageStr__HH
#define DicomVImageStr__HH

#include "DicomVImage.hh"
#include <set>

class DicomVImageStr : public DicomVImage 
{ 
public:
  DicomVImageStr(G4String name, DIModality mod);
  DicomVImageStr(DicomVImage* dfi, G4String name, DIModality modality, DIOrientation = DIO_XY);
  DicomVImageStr(const DicomVImageStr& dfi);
  ~DicomVImageStr();
  void SetHeader(const DicomVImage* imageNew );

  DicomVImageStr operator=( const DicomVImageStr& rhs );
  DicomVImageStr operator!=( const DicomVImageStr& rhs );
  // Get and set methods
   
  virtual G4bool ReadDataFromTextFile( std::ifstream& fin, G4bool bReadHeader = true );
    
  virtual void DumpDataToTextFile(std::ostream& fout, G4bool bFloat = false);
  virtual void DumpToSqdoseFile(FILE* , DicomVImageStr* ) { //errorImage = 0 ) {
    G4Exception("DicomVImageStr::DumpToSqdoseFile","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
  }

  std::vector<G4String>* GetDataStr() const {
    return theDataStr;
  }
  G4String GetDataStr(size_t ii) const {
    return theDataStr->at(ii);
  }
  std::set<G4int> GetIDList( G4int ii );

  void SetDataStr( std::vector<G4String>* dat ) {
    if( theDataStr && theDataStr != dat ) theDataStr->clear();
    theDataStr = dat;
  }

  void SetDataStr( size_t id, G4String val ) {
    theDataStr->at(id) = val;
  }

  virtual G4double GetMaxValue() {
    G4Exception("DicomVImageStr::GetMaxValue","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
    return 0.;
  }
  virtual std::pair<G4double,G4ThreeVector> GetMaxValueAndPoint() {
    G4Exception("DicomVImageStr::GetMaxValueAndPoint","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
    return std::pair<G4double,G4ThreeVector>();
  }
  virtual G4double GetValueAtPoint(G4ThreeVector , G4ThreeVector ){ //textension=G4ThreeVector()) {
    G4Exception("DicomVImageStr::GetValueAtPoint","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
    return 0.;
  }

  virtual void ClearData();

protected:
  virtual void DrawIsodPoints( std::vector<ppidv> , DicomVLineList* , G4double , DPOrientation  ) {
    G4Exception("DicomVImageStr::DrawIsodPoints","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
  }
  
public:
  std::vector<G4String>* theDataStr;
  //  std::vector<std::set<G4int>>* theDataIDs;
//  static double DP_PRECISION;

  virtual void BuildIsodLinesXY(std::vector<G4double> ) {
    G4Exception("DicomVImageStr::DrawIsodPoints","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
  }
  virtual void BuildIsodLinesXZ(std::vector<G4double> ) {
    G4Exception("DicomVImageStr::DrawIsodPoints","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
  }
  virtual void BuildIsodLinesYZ(std::vector<G4double> ) {
    G4Exception("DicomVImageStr::DrawIsodPoints","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
  }
  virtual DicomVLineSet* GetIsodosesXY () const {
    G4Exception("DicomVImageStr::DrawIsodPoints","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
    return (DicomVLineSet*)(0);
  }
  virtual DicomVLineSet* GetIsodosesXZ () const {
    G4Exception("DicomVImageStr::DrawIsodPoints","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
    return (DicomVLineSet*)(0);
  }
  virtual DicomVLineSet* GetIsodosesYZ () const {
    G4Exception("DicomVImageStr::DrawIsodPoints","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
    return (DicomVLineSet*)(0);
  }
  
  G4bool IsDose() const {
    G4Exception("DicomVImageStr::DrawIsodPoints","",FatalException,"This method should not be called for a DicomVImageStr, please contact GAMOS authors");
    return FALSE;
  }
};

std::ostream& operator<<(std::ostream& os, const DicomVImageStr& );

#endif
  
