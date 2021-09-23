#ifndef GmSqdose__HH
#define GmSqdose__HH

#include <vector>
#include "GmSqdoseHeader.hh"
class Gm3ddose;
enum SqdoseType{ SqTALL, SqTFILLED, SqTCROSS_PHANTOM };

class GmSqdose
{
public:
  GmSqdose();
  GmSqdose(const Gm3ddose& dold);
  ~GmSqdose();

  GmSqdose operator+=( const GmSqdose& dose );
  GmSqdose operator*=( double factor );

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
    return theDoseErrors; }
  float GetDoseError(G4int ii) const {
    return theDoseErrors[ii]; }

  void SetHeader( GmSqdoseHeader* header ){
    theHeader = header; }
  void SetDoses( std::vector<float>& doses ) {
    theDoses = doses; }
  void SetDoseSqs( std::vector<float>& dosesqs ) {
    theDoseSqs = dosesqs; }

  void SetSqdoseType( SqdoseType typ ) {
    theType = typ; }

  SqdoseType GetType() const {
    return theType; }

  G4String GetTypeStr() const;

private:
  GmSqdoseHeader* theHeader;
  std::vector<float> theDoses;
  std::vector<float> theDoseSqs;
  std::vector<float> theDoseErrors;

  SqdoseType theType;

  G4bool bBelowFloatPrecisionSetTo0;
};


#endif
