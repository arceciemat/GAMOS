#include "DicomOperIntersectStruct.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"
#include "DicomPolygonList.hh"
#include "DicomPolygon.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4ExtrudedSolid.hh"
#include "G4Box.hh"
#include "G4IntersectionSolid.hh"

//-----------------------------------------------------------------------------
DicomOperIntersectStruct::DicomOperIntersectStruct( G4int iOrder, G4String name)
  : DicomVOperator( iOrder, name )
{  
  bAutomaticOperate = false;
}

//-----------------------------------------------------------------------------
void DicomOperIntersectStruct::Operate( DicomVImage* )
{
  G4Exception("DicomOperIntersectStruct::Operate",
	      "",
	      FatalException,
	      "THIS METHOD SHOULD NOT BE CALLED");
}
//-----------------------------------------------------------------------------
void DicomOperIntersectStruct::Operate( DicomVImage* image, DicomPolygonList* polyList )
{
  G4cout << " DicomOperIntersect::Operate " << image->GetName() << " with " << polyList->GetName() << " NLINES " << polyList->GetLines().size() << G4endl; //GDEB
  G4RotationMatrix rotMat0;
  
  std::vector<DicomVLine*> lines = polyList->GetLines();
  for( size_t ii = 0; ii < lines.size(); ii++ ) {
    G4cout << " DicomOperIntersect poly " << ii << " " << G4endl; //GDEB
    DicomPolygon* poly = static_cast<DicomPolygon*>(lines[ii]);

    //----- Extract image data
    size_t imNoVoxelsX = image->GetNoVoxelsX();
    size_t imNoVoxelsY = image->GetNoVoxelsY();
    size_t imNoVoxelsXY = imNoVoxelsX*imNoVoxelsY;
    size_t imNoVoxelsZ = image->GetNoVoxelsZ();
    G4double imMinX = image->GetMinX();
    G4double imMinY = image->GetMinY();
    G4double imMinZ = image->GetMinZ();
    G4double imMaxX = image->GetMaxX();
    G4double imMaxY = image->GetMaxY();
    G4double imVoxelDimX = image->GetVoxelDimX();
    G4double imVoxelDimY = image->GetVoxelDimY();
    G4double imVoxelDimZ = image->GetVoxelDimZ();
    G4double imVoxelVolume = imVoxelDimX*imVoxelDimY*imVoxelDimZ;

    G4ExtrudedSolid* extrSolid = poly->BuildExtrudedSolid(imVoxelDimZ);
    G4cout << " DicomOperIntersect extr " << extrSolid->GetName() << " " << extrSolid->GetNofVertices() << " " << extrSolid->GetNofZSections() << G4endl; //GDEB

    //--- Get contour Zplane ID and check it is in limits
    G4int iPolygonZ = poly->GetPolygonZIndex(image, imVoxelDimZ*0.25);
    if( iPolygonZ < 0 || iPolygonZ >= G4int(imNoVoxelsZ) ) {
      continue;
    }

    std::vector<G4ThreeVector> points = poly->GetPoints();
    if( DicomVerb(debugVerb) ) G4cout << " INTERS CONTOUR WITH Z SLICE " << points[0].z() << " zMin " << imMinZ << " zMax " << imMinZ+imVoxelDimZ*image->GetNoVoxelsZ() << G4endl;
 
    //--- Set min/max extensions
    double minXc = DBL_MAX;
    double maxXc = -DBL_MAX;
    double minYc = DBL_MAX;
    double maxYc = -DBL_MAX;
    for( size_t ll = 0; ll < points.size(); ll++ ){
      minXc = std::min(minXc,points[ll].x());
      maxXc = std::max(maxXc,points[ll].x());
      minYc = std::min(minYc,points[ll].y());
      maxYc = std::max(maxYc,points[ll].y());
    }
    if( minXc < imMinX || maxXc > imMaxX || minYc < imMinY || maxYc > imMaxY ){
      G4cerr << " minXc= " << minXc << " < " << imMinX
	     << " maxXc= " << maxXc << " > " << imMaxX
	     << " minYc= " << minYc << " < " << imMinY
	     << " maxYc= " << maxYc << " > " << imMaxY << G4endl;
      G4Exception("DicomPolygon::FindVoxelsInXY",
		  "",
		  JustWarning,
		  "Contour limits exceed Z slice extent");
    }
    //--- Set contour min/max voxel IDs 
    int idMinX = std::max(0,int((minXc-imMinX)/imVoxelDimX));
    int idMaxX = std::min(int(imNoVoxelsX-1),int((maxXc-imMinX)/imVoxelDimX+1));
    int idMinY = std::max(0,int((minYc-imMinY)/imVoxelDimY));
    int idMaxY = std::min(int(imNoVoxelsY-1),int((maxYc-imMinY)/imVoxelDimY+1));
    if( DicomVerb(debugVerb) )
      G4cout << " OK CONTOUR INSIDE X/Y OF Z SLICE "
	     << " minXc " << minXc << " < " << imMinX
	     << " maxXc " << maxXc << " > " << imMaxX
	     << " minYc " << minYc << " < " << imMinY
	     << " maxYc " << maxYc << " > " << imMaxY << G4endl;
    if( DicomVerb(debugVerb) )
      G4cout << " VOXEL ID LIMITS OF CONTOUR : idMinX " << idMinX 
	     << " idMaxX " << idMaxX 
	     << " idMinY " << idMinY 
	     << " idMaxY " << idMaxY << G4endl;
    //for each voxel: build 4 lines from the corner towards the center and check how many contour segments it crosses, and the minimum distance to a segment
    
    //  if( DicomVerb(debugVerb) ) G4cout << " LOOPING POLYGON " << theName << " from " << theLineList->GetName() << G4endl; 
    for( int ix = idMinX; ix <= idMaxX; ix++ ) {
      for( int iy = idMinY; iy <= idMaxY; iy++ ) {
	if( DicomVerb(testVerb) ) G4cout << ix << " " << iy << "@@@@@ CHECKING VOXEL AT (" <<  imMinX + imVoxelDimX*(ix+0.5) << "," <<  imMinY + imVoxelDimY*(iy+0.5) << ")" << G4endl;
	// intersect a G4Box of this voxel with polygon G4ExtrudedSolid
	double c0x = imMinX + imVoxelDimX * (ix+0.5); // CENTRE X
	double c0y = imMinY + imVoxelDimY * (iy+0.5); // CENTRE Y
	G4Box voxelBox("BOX",imVoxelDimX/2.,imVoxelDimY/2.,imVoxelDimZ/2.);
	G4IntersectionSolid intersSolid("INTERS",extrSolid,&voxelBox,&rotMat0,G4ThreeVector(c0x,c0y,0.));
	G4double voluIntersect = intersSolid.GetCubicVolume();
	//	G4double voluIntersect = intersSolid.CreatePolyhedron()->GetSurfaceArea()*imVoxelDimZ;
	
	if( DicomVerb(testVerb) ) G4cout << " VOLUME INTERSECT " << voluIntersect << " BOX " << voxelBox.GetCubicVolume() << " : " << extrSolid->GetCubicVolume() << " NV " << extrSolid->GetNofVertices() << " NZ " << extrSolid->GetNofZSections() << G4endl;
        image->SetData(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY, voluIntersect/imVoxelVolume);
      }
    }
  }
}

