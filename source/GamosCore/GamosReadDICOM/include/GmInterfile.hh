#ifndef GmInterfile__HH
#define GmInterfile__HH

#include <vector>
#include "GmInterfileHeader.hh"
#include "G4ThreeVector.hh"

class GmInterfile
{
public:
  GmInterfile();
  ~GmInterfile();

  GmInterfile operator+=( const GmInterfile& data );
  GmInterfile operator*=( double factor );

  void Read( G4String fileName );
  void Read( FILE* fin);
  void Print( std::string fileName );
  void Print( FILE* fout );
  void Displace( G4ThreeVector theDisp );
  void SumDisplaced( GmInterfile* dataNew );
  
public:

  GmInterfileHeader* GetHeader() const {
    return theHeader; }
  std::vector<float> GetData() const {
    return theData; }
  float GetData(G4int ii ) const {
    return theData[ii]; }

  void SetHeader( GmInterfileHeader* header ){
    theHeader = header; }
  void SetData( std::vector<float>& data ) {
    theData = data; }
  private:
  GmInterfileHeader* theHeader;
  std::vector<float> theData;

  G4bool bBelowFloatPrecisionSetTo0;

  size_t fwriteLittleEndian( const void * ptr, size_t size, size_t count, FILE * stream );

};


#endif
