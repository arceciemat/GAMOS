#ifndef Gm3ddoseHeader__HH
#define Gm3ddoseHeader__HH

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include <vector>
#include <fstream>
#include "globals.hh"
class GmFileIn;
class GmSqdoseHeader;

class Gm3ddoseHeader 
{
public:
  Gm3ddoseHeader(){};
  Gm3ddoseHeader(const GmSqdoseHeader& sqdh);
  ~Gm3ddoseHeader(){};

  G4bool operator==(const Gm3ddoseHeader& right) const;
  G4bool operator!=(const Gm3ddoseHeader& right) const;
  
  void Read( GmFileIn& fin);
  
  // Get and Set methods
  float GetNumberOfEvents() const {
    return theNoEvent; }
  
  size_t GetNoVoxelsX() const {
    return theNoVoxelsX; }
  size_t GetNoVoxelsY() const {
    return theNoVoxelsY; }
  size_t GetNoVoxelsZ() const {
    return theNoVoxelsZ; }
  std::vector<float> GetVoxelLimitsX() const {
    return theVoxelLimitsX; }
  std::vector<float> GetVoxelLimitsY() const {
    return theVoxelLimitsY; }
  std::vector<float> GetVoxelLimitsZ() const {
    return theVoxelLimitsZ; }

  void SetNumberOfEvents(float nev ) {
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

private:
  float theNoEvent;
  size_t theNoVoxelsX,theNoVoxelsY,theNoVoxelsZ;
  // Number of voxel in x, y and z dimensions.
  std::vector<float> theVoxelLimitsX, theVoxelLimitsY, theVoxelLimitsZ;
  // List of voxel limits
};

std::ostream& operator<<(std::ostream &out, 
                           const Gm3ddoseHeader &right);

#endif
