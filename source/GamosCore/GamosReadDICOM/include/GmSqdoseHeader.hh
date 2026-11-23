#ifndef GmSqdoseHeader__HH
#define GmSqdoseHeader__HH

#include <vector>
#include <fstream>
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "Gm3ddoseHeader.hh"
class Gm3ddoseHeader;
class GmFileIn;

class GmSqdoseHeader 
{
public:
  GmSqdoseHeader(){};
  GmSqdoseHeader(const Gm3ddoseHeader& dh3d);
  ~GmSqdoseHeader(){};

  G4bool operator==(const GmSqdoseHeader& right) const;
  G4bool operator!=(const GmSqdoseHeader& right) const;

  void Read( FILE* fin );

  // Get and Set methods
  float GetNumberOfEvents() const {
    return theNoEvent; }
  size_t GetNoVoxelsX() const {
    return theNoVoxelsX; }
  size_t GetNoVoxelsY() const {
    return theNoVoxelsY; }
  size_t GetNoVoxelsZ() const {
    return theNoVoxelsZ; }
  size_t GetNoVoxels() const {
    return theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ; }
  std::vector<float> GetVoxelLimitsX() const {
    return theVoxelLimitsX; }
  std::vector<float> GetVoxelLimitsY() const {
    return theVoxelLimitsY; }
  std::vector<float> GetVoxelLimitsZ() const {
    return theVoxelLimitsZ; }
  float GetVoxelHalfX() const {
    return (theVoxelLimitsX[theNoVoxelsX]-theVoxelLimitsX[0])/theNoVoxelsX/2.; }
  float GetVoxelHalfY() const {
    return (theVoxelLimitsY[theNoVoxelsY]-theVoxelLimitsY[0])/theNoVoxelsY/2.; }
  float GetVoxelHalfZ() const {
    return (theVoxelLimitsZ[theNoVoxelsZ]-theVoxelLimitsZ[0])/theNoVoxelsZ/2.; }
  float GetMinX() const {
    return (theVoxelLimitsX[0]); }
  float GetMinY() const {
    return (theVoxelLimitsY[0]); }
  float GetMinZ() const {
    return (theVoxelLimitsZ[0]); }
  float GetMaxX() const {
    return (theVoxelLimitsX[theNoVoxelsX]); }
  float GetMaxY() const {
    return (theVoxelLimitsY[theNoVoxelsY]); }
  float GetMaxZ() const {
    return (theVoxelLimitsZ[theNoVoxelsZ]); }
  G4ThreeVector GetTranslation() const {
    return G4ThreeVector((theVoxelLimitsX[theNoVoxelsX]+theVoxelLimitsX[0])/2,
			 (theVoxelLimitsY[theNoVoxelsY]+theVoxelLimitsY[0])/2,
			 (theVoxelLimitsZ[theNoVoxelsZ]+theVoxelLimitsZ[0])/2); }
    
  G4RotationMatrix GetRotation() const {
    return theRotationMatrix; }

  void SetNumberOfEvents( float nev ) {
    theNoEvent = nev; }
  void SetNoVoxelsX( size_t nv ) {
    theNoVoxelsX = nv; }
  void SetNoVoxelsY( size_t nv ) {
    theNoVoxelsY = nv; }
  void SetNoVoxelsZ( size_t nv ) {
    theNoVoxelsZ = nv; }

  void SetVoxelLimitsX( std::vector<float> vl ) {
    theVoxelLimitsX = vl; }
  void SetVoxelLimitsY( std::vector<float> vl ) {
    theVoxelLimitsY = vl; }
  void SetVoxelLimitsZ( std::vector<float> vl ) {
    theVoxelLimitsZ = vl; }

  void SetRotation( G4RotationMatrix rotmat ) {
    theRotationMatrix = rotmat; }

  void Displace( G4ThreeVector theDisp );
  void ChangeVoxelLimits( G4int ixmin, G4int ixmax, G4int iymin, G4int iymax, G4int izmin, G4int izmax );
  
  void Print( FILE* fout );
private:
  size_t theNoVoxelsX,theNoVoxelsY,theNoVoxelsZ;
  float theNoEvent;  // it can be fractional, as obtained with the fraction of events that reach the phase space
  // Number of voxel in x, y and z dimensions.
  std::vector<float> theVoxelLimitsX, theVoxelLimitsY, theVoxelLimitsZ;
  G4RotationMatrix theRotationMatrix;
  // List of voxel limits
};

#endif
