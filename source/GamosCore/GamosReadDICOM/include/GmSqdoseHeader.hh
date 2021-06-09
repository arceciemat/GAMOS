#ifndef GmSqdoseHeader__HH
#define GmSqdoseHeader__HH

#include <vector>
#include <fstream>
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class GmFileIn;

class GmSqdoseHeader 
{
public:
  GmSqdoseHeader(){};
  ~GmSqdoseHeader(){};

  G4bool operator==(const GmSqdoseHeader& right) const;
  G4bool operator!=(const GmSqdoseHeader& right) const;

  void Read( FILE* fin );

  // Get and Set methods
  float GetNumberOfEvents() const {
    return theNoEvent; }
  size_t GetNoVoxelX() const {
    return theNoVoxelX; }
  size_t GetNoVoxelY() const {
    return theNoVoxelY; }
  size_t GetNoVoxelZ() const {
    return theNoVoxelZ; }
  std::vector<float> GetVoxelLimitsX() const {
    return theVoxelLimitsX; }
  std::vector<float> GetVoxelLimitsY() const {
    return theVoxelLimitsY; }
  std::vector<float> GetVoxelLimitsZ() const {
    return theVoxelLimitsZ; }
  float GetVoxelHalfX() const {
    return (theVoxelLimitsX[theNoVoxelX]-theVoxelLimitsX[0])/theNoVoxelX/2.; }
  float GetVoxelHalfY() const {
    return (theVoxelLimitsY[theNoVoxelY]-theVoxelLimitsY[0])/theNoVoxelY/2.; }
  float GetVoxelHalfZ() const {
    return (theVoxelLimitsZ[theNoVoxelZ]-theVoxelLimitsZ[0])/theNoVoxelZ/2.; }
  G4ThreeVector GetTranslation() const {
    return G4ThreeVector((theVoxelLimitsX[theNoVoxelX]+theVoxelLimitsX[0])/2,
			 (theVoxelLimitsY[theNoVoxelY]+theVoxelLimitsY[0])/2,
			 (theVoxelLimitsZ[theNoVoxelZ]+theVoxelLimitsZ[0])/2); }
    
  G4RotationMatrix GetRotation() const {
    return theRotationMatrix; }

  void SetNumberOfEvents( float nev ) {
    theNoEvent = nev; }
  void SetNoVoxelX( size_t nv ) {
    theNoVoxelX = nv; }
  void SetNoVoxelY( size_t nv ) {
    theNoVoxelY = nv; }
  void SetNoVoxelZ( size_t nv ) {
    theNoVoxelZ = nv; }

  void SetVoxelLimitsX( std::vector<float> vl ) {
    theVoxelLimitsX = vl; }
  void SetVoxelLimitsY( std::vector<float> vl ) {
    theVoxelLimitsY = vl; }
  void SetVoxelLimitsZ( std::vector<float> vl ) {
    theVoxelLimitsZ = vl; }

  void SetRotation( G4RotationMatrix rotmat ) {
    theRotationMatrix = rotmat; }

  void Print( FILE* fout );
private:
  size_t theNoVoxelX,theNoVoxelY,theNoVoxelZ;
  float theNoEvent;  // it can be fractional, as obtained with the fraction of events that reach the phase space
  // Number of voxel in x, y and z dimensions.
  std::vector<float> theVoxelLimitsX, theVoxelLimitsY, theVoxelLimitsZ;
  G4RotationMatrix theRotationMatrix;
  // List of voxel limits
};

#endif
