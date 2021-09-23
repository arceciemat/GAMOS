#include "DicomBeam.hh"
#include "DicomVBeamDevice.hh"
#include "DicomVBeamRTControlPoint.hh"
#include "DicomBeamCompensator.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <fstream>

//-----------------------------------------------------------------------------
DicomBeam::DicomBeam(bool bIsRef)
{
  bIsReferenced = bIsRef;
}

//-----------------------------------------------------------------------------
void DicomBeam::SetControlPointMetersets()
{
  G4double prevCumulativeMS = 0.;
  for( size_t ii = 0; ii < theControlPoints.size(); ii++ ){
    G4double cumulativeMS = theControlPoints[ii]->GetParam("CumulativeMetersetWeight");
    theControlPoints[ii]->SetParam("MetersetWeight", (cumulativeMS - prevCumulativeMS ));
    //    std::cout << " DicomBeam::SetControlPointMetersets(" << ii << " = " << cumulativeMS - prevCumulativeMS  << " " << cumulativeMS << " - " << prevCumulativeMS << std::endl; //GDEB
    prevCumulativeMS = cumulativeMS;
  }
}

//-----------------------------------------------------------------------------
void DicomBeam::DumpToFileF(std::string fileName)
{
  //  if( bIsReferenced ) fileName += "_Referenced";
  
  std::ofstream fout(fileName);

  std::string name  = "";
  DicomVObjectWithParams::Print( fout, name );

  for( size_t ii = 0; ii < theDevices.size(); ii++) {
    theDevices[ii]->DumpToFile( fout );
  }

  for( size_t ii = 0; ii < theCompensators.size(); ii++) {
    theCompensators[ii]->DumpToFile( fout );
  }

  std::string  fNameCP = "RTPlanControlPoint_";
  if( fileName.find("RTIon") != std::string::npos ) {
    fNameCP = "RTIonPlanControlPoint_";
  }
  for( size_t kk = 0; kk < theControlPoints.size(); kk++ ){
    std::string kkstr = GmGenUtils::itoa(int(theControlPoints[kk]->GetParam("ControlPointIndex")));
					 //    G4cout << theControlPoints[kk] << " " << theControlPoints[kk]->GetParam("NominalBeamEnergy") << " DUMPING theControlPoints " << kk << " : " << theControlPoints[kk]->GetParam("ControlPointIndex") << " = " << kkstr << G4endl; //GDEB
    std::ofstream fout2(fNameCP+std::to_string(theNumber)+"_"+kkstr);
    theControlPoints[kk]->DumpToFile( fout2 );
  }

}

  
