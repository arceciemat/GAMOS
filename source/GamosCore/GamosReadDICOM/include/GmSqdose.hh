#ifndef GmSqdose__HH
#define GmSqdose__HH

#include <vector>
#include "GmSqdoseHeader.hh"
enum SqdoseType{ SqTALL=1, SqTFILLED=2 };

class GmSqdose
{
public:
  GmSqdose();
  //  GmSqdose(const GmSqdose& dold);
  ~GmSqdose();

  GmSqdose operator+=( const GmSqdose& dose );

  void Read( G4String fileName );
  void Read( FILE* fin);
  void Print( FILE* fout );
  void CalculateErrors();

public:

  GmSqdoseHeader* GetHeader() const {
    return theHeader; }
  std::vector<float> GetDoses() const {
    return theDoses; }
  std::vector<float> GetDoseSqs() const {
    return theDoseSqs; }
  float GetDose(G4int ii ) const {
    return theDoses[ii]; }
  float GetDoseSq(G4int ii ) const {
    return theDoseSqs[ii]; }
  std::vector<float> GetDoseErrors() const {
    return theErrors; }
  float GetDoseError(G4int ii) const {
    return theErrors[ii]; }

  void SetHeader( GmSqdoseHeader* header ){
    theHeader = header; }
void SetDoses( std::vector<float>& doses ) {
    theDoses = doses; }
  void SetDoseSqs( std::vector<float>& dosesqs ) {
    theDoseSqs = dosesqs; }

  void SetSqdoseType( SqdoseType typ ) {
    theType = typ; }

private:
  GmSqdoseHeader* theHeader;
  std::vector<float> theDoses;
  std::vector<float> theDoseSqs;
  std::vector<float> theErrors;

  SqdoseType theType;
};

#endif
