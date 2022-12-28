#ifndef GmInterfileHeader__HH
#define GmInterfileHeader__HH

#include <vector>
#include <fstream>
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class GmFileIn;

class GmInterfileHeader 
{
public:
  GmInterfileHeader();
  ~GmInterfileHeader(){};

  G4bool operator==(const GmInterfileHeader& right) const;
  G4bool operator!=(const GmInterfileHeader& right) const;

  void Read( G4String fileName );
  void Read( std::ifstream* fin );
  void ConvertParametersIntoData();

  /*  G4String GetParam( G4String name, G4String value = "") const;
  void SetParam( G4String name, G4String value) {
    theParameters[name] = value;
    }*/
  
  // Get and Set methods
  float GetNumberOfEvents() const {
    return theNoEvent; }
  float GetNoEvents() const {
    return theNoEvent; }
  size_t GetNoVoxelsX() const {
    return theNoVoxelsX; }
  size_t GetNoVoxelsY() const {
    return theNoVoxelsY; }
  size_t GetNoVoxelsZ() const {
    return theNoVoxelsZ; }
  size_t GetNoVoxels() const {
    return theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ; }
  float GetVoxelDimX() const {
    return theVoxelDimX; }
  float GetVoxelDimY() const {
    return theVoxelDimY; }
  float GetVoxelDimZ() const {
    return theVoxelDimZ; }
  float GetVoxelHalfX() const {
    return theVoxelDimX/2.; }
  float GetVoxelHalfY() const {
    return theVoxelDimY/2.; }
  float GetVoxelHalfZ() const {
    return theVoxelDimZ/2.; }
  float GetMinX() const {
    return theMinX; }
  float GetMinY() const {
    return theMinY; }
  float GetMinZ() const {
    return theMinZ; }
  float GetMaxX() const {
    return theMinX+theVoxelDimX*theNoVoxelsX; }
  float GetMaxY() const {
    return theMinY+theVoxelDimY*theNoVoxelsY; }
  float GetMaxZ() const {
    return theMinZ+theVoxelDimZ*theNoVoxelsZ; }
  G4ThreeVector GetTranslation() const {
    return G4ThreeVector(theMinX+theVoxelDimX*theNoVoxelsX/2.,
  			 theMinY+theVoxelDimY*theNoVoxelsY/2.,
			 theMinZ+theVoxelDimZ*theNoVoxelsZ/2.); }
    
  G4String GetNumberFormat() const {
    return theNumberFormat; }
  G4int GetNoBytesPerPixel() const {
    return theNoBytesPerPixel; }
  std::string GetDataFileName() const {
    return theDataFileName; }

  void SetNumberOfEvents( float nev ) {
    theNoEvent = nev; }
  void SetNoVoxelsX( size_t nv ) {
    theNoVoxelsX = nv; }
  void SetNoVoxelsY( size_t nv ) {
    theNoVoxelsY = nv; }
  void SetNoVoxelsZ( size_t nv ) {
    theNoVoxelsZ = nv; }
  void SetVoxelDimX( size_t dim ){
    theVoxelDimX = dim; }
  void SetVoxelDimY( size_t dim ){
    theVoxelDimY = dim; }
  void SetVoxelDimZ( size_t dim ){
    theVoxelDimZ = dim; }
  void SetMinX( float dim ){
    theMinX = dim; }
  void SetMinY( float dim ){
    theMinY = dim; }
  void SetMinZ( float dim ){
    theMinZ = dim;
  }
  void SetDataFileName( std::string dfn ) {
    theDataFileName = dfn; }

  void ChangeVoxelLimits( G4int ixmin, G4int ixmax, G4int iymin, G4int iymax, G4int izmin, G4int izmax );

 
  void Displace( G4ThreeVector theDisp );
  
  void Print( std::ostream& fout );
  void Print( G4String foutName );
private:

  std::pair<int,std::string> getIdSVal(std::string stemp);
  std::pair<int,double> getIdDVal(std::string stemp);
  double getDVal(std::string stemp);
  std::string getSVal(std::string stemp);
  
  size_t theNoVoxelsX,theNoVoxelsY,theNoVoxelsZ;
  size_t theVoxelDimX,theVoxelDimY,theVoxelDimZ;
  float theMinX,theMinY,theMinZ;
  float theNoEvent;  // it can be fractional, as obtained with the fraction of events that reach the phase space
  std::string theDataFileName;
  int theNoBytesPerPixel;
  G4int theNDimensions;
  G4String thePatientPosition;
  G4String theDataType;
  std::map<int,std::string> theMatrixAxisLabels;
  G4String theNumberFormat;
  
  //  std::map<G4String,G4String> theParameters;

};

#endif
