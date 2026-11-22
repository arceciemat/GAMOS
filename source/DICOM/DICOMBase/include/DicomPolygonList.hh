#ifndef DicomPolygonList__HH
#define DicomPolygonList__HH

#include "DicomVLineList.hh"
class DicomVImageStr;
class DicomPolygon;

#include <vector>
#include <map>
typedef std::multimap<G4double, DicomPolygon*> mmddp;
typedef mmddp::const_iterator mmddpi;
typedef std::pair<mmddpi,mmddpi> pmmddpi; // pair of iterators 

class DicomPolygonList : public DicomVLineList
{ 
public:
  DicomPolygonList( G4String name, size_t num, DPOrientation ori);
  DicomPolygonList(DicomPolygonList* polySetOld, DicomVImage* image, DPOrientation ori);
  ~DicomPolygonList(){};

  DicomPolygon* AddDisplacedLine( DicomPolygon* line, G4double disp );
  void FindVoxelsInXY( DicomVImageStr* image, pmmddpi polygonsInPlane);
  
public:
  void FindVoxelsInXY( DicomVImageStr* );
  mmddp BuildPosOrderedLines();
  G4int GetPolygonZIndex( DicomVImage* image, G4double planeZ );

private:
  G4bool bVoxelsInUseClosest;
  G4bool bOrderedLineListBuilt;
  mmddp thePosOrderedLines;  // multimap Z_position, DicomPolygon
};

#endif
  
