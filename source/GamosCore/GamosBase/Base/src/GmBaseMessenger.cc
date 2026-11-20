#include "G4Threading.hh"
#include "GmBaseMessenger.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmSVGCanvas.hh"
#include "GamosCore/GamosUtils/include/GmSVGRaster.hh"
#include "GamosCore/GamosUtils/include/GmSVGPad.hh"
#include "GamosCore/GamosUtils/include/GmSVGFrame.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"

#include "G4UImanager.hh"
#include <set>
#include <cctype>
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

//---------------------------------------------------------------------
GmBaseMessenger::GmBaseMessenger() 
{
  filterCmd = new GmUIcmdWithAString("/gamos/filter",this);
  filterCmd->SetGuidance("Create a new filter assigning parameters to one of the plug-in filters: FILTER_NAME FILTER_CLASS");
  filterCmd->SetParameterName("choice",true);
  filterCmd->AvailableForStates(G4State_Idle);

  classifierCmd = new GmUIcmdWithAString("/gamos/classifier",this);
  classifierCmd->SetGuidance("Create a new classifier assigning parameters to one of the plug-in classifiers: CLASSIFIER_NAME CLASSIFIER_CLASS");
  classifierCmd->SetParameterName("choice",true);
  classifierCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  classifierIndicesCmd = new GmUIcmdWithAString("/gamos/classifier/setIndices",this);
  classifierIndicesCmd->SetGuidance("Set indices for a classifier: CLASSIFIER_NAME KEY_1 VALUE_1 KEY_2 VALUE_2 ...");
  classifierIndicesCmd->SetParameterName("choice",true);
  classifierIndicesCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  distributionCmd = new GmUIcmdWithAString("/gamos/distribution",this);
  distributionCmd->SetGuidance("Create a new distribution assigning parameters to one of the plug-in distributions: DISTRIBUTION_NAME DISTRIBUTION_CLASS");
  distributionCmd->SetParameterName("choice",true);
  distributionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SVGImageFormatCmd = new GmUIcmdWithAString("/gamos/SVG/imageFormat",this);
  SVGImageFormatCmd->SetGuidance("Format to which SVG figures (GmSVGCanvas) are written: png, jpg, gif, bmp, tiff (built-in renderer), pdf, ps, eps, webp (converted from SVG with an external program), svg. It overrides the format chosen by each figure producer");
  SVGImageFormatCmd->SetParameterName("format",false);
  SVGImageFormatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SVGJpgQualityCmd = new GmUIcmdWithAString("/gamos/SVG/jpgQuality",this);
  SVGJpgQualityCmd->SetGuidance("Quality (integer 1-100, default 90) of the JPG files written by the built-in renderer of SVG figures (GmSVGCanvas)");
  SVGJpgQualityCmd->SetParameterName("quality",false);
  SVGJpgQualityCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//---------------------------------------------------------------------
GmBaseMessenger::~GmBaseMessenger() 
{
  if (filterCmd) delete filterCmd;
  if (classifierCmd) delete classifierCmd;
  if (classifierIndicesCmd) delete classifierIndicesCmd;
  if (distributionCmd) delete distributionCmd;
  if (SVGImageFormatCmd) delete SVGImageFormatCmd;
  if (SVGJpgQualityCmd) delete SVGJpgQualityCmd;
}

//---------------------------------------------------------------------
void GmBaseMessenger::SetNewValue(G4UIcommand * command, G4String newValues)    
{ 
  //--- Multithreading: SVG settings are shared, only master
  if( !G4Threading::IsMasterThread() && (command == SVGImageFormatCmd || command == SVGJpgQualityCmd) ) return;
  if (command == filterCmd) {
    GmFilterMgr::GetInstance()->FindOrBuildFilter( GmGenUtils::GetWordsInString(newValues) );
  } else if (command == classifierCmd) {
    GmClassifierMgr::GetInstance()->FindOrBuildClassifier( GmGenUtils::GetWordsInString(newValues) );
    //?    GmClassifierMgr::GetInstance()->CreateClassifier( GmGenUtils::GetWordsInString(newValues) );
  } else if (command == classifierIndicesCmd) {
    GmClassifierMgr::GetInstance()->SetClassifierIndices( GmGenUtils::GetWordsInString(newValues) );
  } else if (command == distributionCmd) {
    GmDistributionMgr::GetInstance()->FindOrBuildDistribution( GmGenUtils::GetWordsInString(newValues) );
  } else if (command == SVGImageFormatCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString(newValues);
    G4String format = wl.size() != 0 ? wl[0] : G4String("");
    for( size_t ic = 0; ic < format.size(); ic++ ) format[ic] = std::tolower(format[ic]);
    if( format == "jpeg" ) format = "jpg";
    if( format == "tif" ) format = "tiff";
    static const std::set<G4String> validFormats = { "svg", "png", "jpg", "gif", "pdf", "ps", "eps", "tiff", "bmp", "webp" };
    if( validFormats.find(format) == validFormats.end() ) {
      G4String validStr;
      for( std::set<G4String>::const_iterator ite = validFormats.begin(); ite != validFormats.end(); ite++ ) {
	validStr += " " + *ite;
      }
      G4Exception("GmBaseMessenger::SetNewValue",
		  "",
		  FatalErrorInArgument,
		  ("/gamos/SVG/imageFormat: image format not accepted: '" + newValues + "' . Accepted values are:" + validStr).c_str());
    }
    GmSVGCanvas::SetImageFormat( format );
  } else if (command == SVGJpgQualityCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString(newValues);
    G4String qualStr = wl.size() != 0 ? wl[0] : G4String("");
    G4bool bOK = GmGenUtils::IsInteger(qualStr);
    G4int quality = bOK ? G4int(GmGenUtils::GetValue(qualStr)) : 0;
    if( !bOK || quality < 1 || quality > 100 ) {
      G4Exception("GmBaseMessenger::SetNewValue",
		  "",
		  FatalErrorInArgument,
		  ("/gamos/SVG/jpgQuality: value must be an integer between 1 and 100, it is '" + newValues + "'").c_str());
    }
    GmSVGRaster::SetJpegQuality( quality );
  }

}

