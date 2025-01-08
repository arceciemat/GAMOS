#include <limits>
#include <cfloat>
#include <iostream>
#include <fstream>
#include "ROOTUtilities/PlotData/Definitions.C"
#include "ROOTUtilities/PlotData/PlotUtils.C"
#include "ROOTUtilities/GenUtils.C"
#include "ROOTUtilities/GetValue.C"
#include "ROOTUtilities/GetWordsInLine.C"
#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TPad.h"
#include "TCanvas.h"
#include <TStyle.h>
#include "TMarker.h"
#include "TLine.h"
#include "TText.h"
#include <math.h>
struct organData {
  std::string name;
  double value;
  double error;
  double mass;
};

  
std::map<std::string,std::string> theOrganGroupNames;
std::map<int,int> theOrganGroupIDs;
std::vector< std::map<int,organData> > theOrganData;

void ReadDataFromFile( std::string fileName );
void ReadDataFromFileMCNP( std::string fileName );
void ReadOrganGroups(std::string fileName );
std::vector<std::string> theFileNames;
bool bOrganGrouping;
int nMaxDataPH = 25; // maximum number of data in one histogram
bool bPlotErrors = 0;

//------------------------------------------------------------------------
void plotOrganInfo(std::string fileListName, bool bog=1, bool bpe=0)
{
  bOrganGrouping = bog;
  bOrganGrouping = false;
  bPlotErrors = bpe;
  verbose = 3;
  gStyle->SetOptStat(0);
  TCanvas* canvas = new TCanvas;
  canvas->SetCanvasSize(2000,2000);
  canvas->Draw();
  canvas->SetLogy(1);
  double theMarkerSize = 2.;
  //----- Set map of organ -> organ group
  ReadOrganGroups("organ.ICRP100merge.txt");

  //----- Read file containing organ info file list
  std::ifstream finLis(fileListName.c_str());
  if( finLis.fail() ) {
    std::cerr << " FILE LIST DOES NOT EXISTS " << fileListName << std::endl;
    exit(1);
  }
  std::vector<std::string> wl;
  double valMin = DBL_MAX;
  double valMax = -DBL_MAX;
  std::vector<TH1F*> histos;

  //----- Loop to list of files
  for( int ii = 0;; ii++ ) {
    if( ! GetWordsInLine( finLis, wl ) ) break;
    std::string fileName = wl[0];
    theFileNames.push_back(fileName);
    if( wl.size() > 1 && wl[1] == "MCNP" ) {
      ReadDataFromFileMCNP( fileName );
    } else {
      ReadDataFromFile( fileName );
    }
  }
  
  std::vector< std::map<int,organData> >::const_iterator item;
  std::map<int,organData>::iterator ite;
  //--- get histogram values: min/max label names (=organ names)
  std::map<int,std::string> organNames;
  int nofData = 0;
  for( item = theOrganData.begin(); item != theOrganData.end(); item++ ) {
    std::map<int,organData> organData1 = *item;
    std::map<int,organData>::reverse_iterator rite = organData1.rbegin(); 
    nofData = std::max(nofData,(*rite).first);
    for( ite = organData1.begin(); ite != organData1.end(); ite++ ) {
     double val = (*ite).second.value;
      double err = (*ite).second.error;
      string name = (*ite).second.name;
      valMax = std::max(valMax, val+err);
      if( fabs(val-err) > val*1.E-6 ) {
	valMin = std::min(valMin, val-err);
	if( verbose >= 3 ) std::cout << " VALMIN " << valMin << " " << val << " +- " << err << " " << val-err << std::endl;
      } else {
	valMin = std::min(valMin, val*0.1);
	if( verbose >= 3 ) std::cout << " VALMIN " << valMin << " " << val << std::endl;
      }
      if( verbose >= 3 ) std::cout << " VALMAX " << valMax << " " << val << " +- " << err << " " << val+err << std::endl;
      int id = (*ite).first;
      std::map<int,std::string>::const_iterator iteon = organNames.find(id);
      if( iteon == organNames.end() ) {
	organNames[id] = name;
	std::cout << " ORGAN NAME " << id << " = " << name << std::endl; //GDEB
      } else {
	if( organNames[id] != name ) {
	  std::cerr << "!! ERROR: TWO FILES DO NOT CONTAINS SAME DATA FOR ORGAN ID "
		    << id << " : " << organNames[id] << " != " << name << std::endl;
	  exit(1);
	}
      }
    }
  }
  valMax *= 3.;
  valMin /= 3.;
  if( verbose >= 2 ) std::cout << "FINAL VALMIN " << valMin << " VALMAX " << valMax << std::endl;

  //----- Fill histograms
  canvas->cd();
  TH1F* histo = 0;
  //--- Loop to organ ID's 
  for( int ibin = 0; ibin <= nofData; ibin++ ) {
    int iJpgFile = ibin/nMaxDataPH;
    int ibinInFile = ibin%nMaxDataPH; 
    if( ibin%nMaxDataPH == 0 ) {
      //-- create histograms
      //      for( int ii = 0; ii < nofData/nMaxDataPH; ii++ ) {
      int nbins = std::min(nMaxDataPH,nofData+1-iJpgFile*nMaxDataPH);
      histo = new TH1F("","",nbins+1,-0.5,nbins+0.5);
      if( verbose >= 3 ) std::cout << " NEW HISTO " << nbins << std::endl; //GDEB
      histo->SetMaximum( valMax );
      histo->SetMinimum( valMin );
      //	theHistos.push_back(histo);
      histo->SetLabelSize(0.5/nbins);
      //--- set labels = organ names
      for( int ii = ibin; ii < ibin+nbins; ii++ ) {
	std::map<int,std::string>::const_iterator iteon = organNames.find(ii);
	if( iteon != organNames.end() ) {
	  histo->GetXaxis()->SetBinLabel(ii-ibin+1,(*iteon).second.c_str());
	  if( verbose >= 3 ) std::cout << histo->GetLabelSize() << " histo->GetXaxis()->SetBinLabel " << ii-ibin << " : " << (*iteon).second << std::endl;
	} else {
	  std::cerr << "!! ORGAN NAME FOR LABEL NOT FOUND " << ibin << " : " << ii-ibin << std::endl; //GDEB
	}
      }
      histo->Draw();
    }
    //--- Loop to files to draw marker
    int ifile = 0;
    for( item = theOrganData.begin(); item != theOrganData.end(); item++, ifile++ ) {
      std::map<int,organData> organData1 = *item;
      /*      for( ite = organData1.begin(); ite != organData1.end(); ite++ ) {
	std::cerr << ibin << " MARKERDATA " << (*ite).first << " " << (*ite).second.name << std::endl; //GDEB
      } */
      ite = organData1.find(ibin);
      if( ite != organData1.end() ) {
	//--- Draw organ data for each input file
	EColor color = GetColor(ifile);
	double val = (*ite).second.value;
	double err = (*ite).second.error;
	double idpos = ibinInFile+0.25;
	TMarker* marker = new TMarker( idpos, val, 20+ifile );
	//	std::cout << " MARKER " << idpos << " " << val << " +- " << err << " " << std::endl; //GDEB
	marker->SetMarkerColor(color);
	marker->SetMarkerSize(theMarkerSize);
	marker->Draw();
	if( !bPlotErrors ) {
	  TLine* line = new TLine( idpos, std::max(val-err,valMin), idpos, val+err );
	  line->SetLineColor(color);
	  line->SetLineWidth(2);
	  line->Draw();
	}
	TLine* line2 = new TLine( idpos, valMin/1.2, idpos, val );
	line2->SetLineStyle(3);
	line2->Draw();

	double xInit = 25;
	double yInit = 600-15*ifile;
	Definitions::bLogY = 1;
	DrawWordAndMarker(histo, theFileNames[ifile], xInit, yInit, color, 20+ifile, 0.02, 0);
      }
    }
    if( ibin%nMaxDataPH == nMaxDataPH-1 || ibin == nofData ) {
      std::string fjpgName;
      if( bPlotErrors ) {
	fjpgName = std::string("hisOrganDoseErrors_"+itoa(iJpgFile)+".jpg");
      } else {
	fjpgName = std::string("hisOrganDoses_"+itoa(iJpgFile)+".jpg");
      }
      canvas->Print(fjpgName.c_str());
    }
  }
}


//--------------------------------------------------------------
void ReadOrganGroups(std::string fileName )
{
  std::ifstream fin(fileName.c_str());
  if( fin.fail() ) {
    std::cerr << " FILE DOES NOT EXISTS " << fileName << std::endl;
    exit(1);
  }
  std::vector<std::string> wl;
  int ng1 = 1;
  int ng2 = 0;
  std::map<std::string,int> groups;
  std::map<std::string,int>::iterator iteg;
  for( ;; ) {
    if( ! GetWordsInLine( fin, wl ) ) break;
    std::string newName = "";
    int newID = 0;
    if( wl.size() == 2 ) {
      newName = wl[1];
    } else if( wl.size() == 3 ) {
      newName = wl[2];
    } else if( wl.size() == 4 ) {
      newName = wl[3];
    }
    iteg = groups.find(newName);
    if( iteg == groups.end() ) {
      groups[newName] = groups.size();
      iteg = groups.find(newName);
    }
    theOrganGroupIDs[GetValue(wl[0])] = (*iteg).second+1;
    theOrganGroupNames[wl[1]] = newName;
    
    if( verbose >= 2 ) std::cout << " OGrouping " << wl[0] << " -> " << theOrganGroupIDs[GetValue(wl[0])] 
				 << " OGname " << wl[1] << " -> " << theOrganGroupNames[wl[1]] << std::endl;
  }
}

//--------------------------------------------------------------
void ReadDataFromFile( std::string fileName )
{
  double totalEnergy = 0.;
  std::map<int,organData> organData1;
  //  organData1.clear();
  std::ifstream fin(fileName.c_str());
  if( fin.fail() ) {
    std::cerr << " FILE DOES NOT EXISTS " << fileName << std::endl;
    exit(1);
  }

  std::vector<std::string> wl;
  GetWordsInLine( fin, wl ); // skip first line
  //----- Loop to lines in this file
  for( ;; ) {
    if( ! GetWordsInLine( fin, wl ) ) break;
    if( wl.size() != 21 ) {
      std::cerr << " LINE DOES NOT HAVE 21 words, but " <<  wl.size() << " " << wl[1] << std::endl;
      continue;
    }
    double val = GetValue(wl[3]);
    double err = GetValue(wl[5]);
    double mass = GetValue(wl[19]);
    totalEnergy += val*mass*1000.*6.24150e9;
    err *= val/100.;
    if( verbose >= 2 ) std::cout << " ORGAN READ " << wl[1] << " : " << wl[3] << " +- " << wl[5] << " : val " << val << " +- " << err << " mass= " << mass << std::endl;
    int id = int(GetValue(wl[0]));
    std::string name = wl[1];
    organData odat;
    //--- If organ grouping, change id and name
    if( bOrganGrouping ) {
      int idOld = id;
      if( theOrganGroupNames.find(wl[1]) != theOrganGroupNames.end() ) {
	id = theOrganGroupIDs[idOld];
	name = theOrganGroupNames[wl[1]];
	std::cout << " SET NAME " << name << " = " << theOrganGroupNames[wl[1]] << std::endl; //GDEB
	double valOld = organData1[id].value;
	double errOld = organData1[id].error;
	double massOld = organData1[id].mass;
	double wOld = massOld/(mass+massOld);
	double w = mass/(mass+massOld);
	//---- If error plotting use error proportion
	if( !bPlotErrors ) {
	  val = (valOld*wOld + val*w);
	  err = sqrt(err*w*err*w+errOld*wOld*errOld*wOld);
	} else {
	  if( valOld != 0 ) valOld = errOld/valOld;
	  val = (valOld*wOld + val*w);
	  err = sqrt(err*w*err*w+errOld*wOld*errOld*wOld);
	  if( val != 0 ) val = err/val;	  
	}
	mass += massOld;
	if( verbose >= 3 ) std::cout << " 1ORGANMG " << idOld << " -> " << id << " : " << wl[1] << " -> " << name << " : val " << valOld << " +- " << errOld << " --> " << val << " +- " << err << " mass " << mass << std::endl;
      } else {
	std::cerr << "!! WARNING  organ not found in organ_group_names " << wl[1] << std::endl;
      }
    }
    //--- Set data and add it to data list 
    odat.name = name;
    odat.value = val;
    odat.error = err;
    odat.mass = mass;
    organData1[id] = odat;
    if( verbose >= 2 ) std::cout << " ADD ORGAN to list " << id << " : " << name << " " << val << " +- " << err << " " << mass << std::endl; //GDEB
  }
  if( organData1.size() == 0 ) {
    std::cerr << " !!!ERROR: NO DATA IN FILE " << fileName << std::endl;
    exit(1);
  }
  theOrganData.push_back(organData1);
  std::cout << " TOTAL ENERGY " << fileName << " = " << totalEnergy << std::endl; 
}


void ReadDataFromFileMCNP( std::string fileName )
{
  double totalEnergy = 0;
  std::map<int,organData> organData1;
  /*  if( theOrganData.size() == 0 ) {
    std::cerr << " ERROR MCNP file cannot be the first one " << fileName << std::endl;
    exit(1);
  }
  std::map<int,organData> organDataOld = theOrganData; */
  //  theOrganData.clear();
  std::ifstream fin(fileName.c_str());
  if( fin.fail() ) {
    std::cerr << " FILE DOES NOT EXISTS " << fileName << std::endl;
    exit(1);
  }

  double unitFactor = 6.24150e9*10; // CLHEP::gray/CLHEP::MeV*CLHEP::g;
  unitFactor = 1.e3;
  std::vector<std::string> wl;
  GetWordsInLine( fin, wl ); // skip first line
  //----- Loop to lines in this file
  for( ;; ) {
    if( ! GetWordsInLine( fin, wl ) ) break;
    if( wl.size() != 5 ) {
      std::cerr << " LINE DOES NOT HAVE 5 words, but " <<  wl.size() << " " << wl[1] << std::endl;
      continue;
    }
    double mass = GetValue(wl[4]);
    double val = GetValue(wl[2])/unitFactor/mass;
    double err = GetValue(wl[3]);
    totalEnergy += GetValue(wl[2]);
    err *= val;
    if( verbose >= -2 ) std::cout << " ORGAN READ " << wl[1] << " : val " << val << " +- " << err << " mass= " << mass << std::endl;
    int id = int(GetValue(wl[0]));
    std::string name = wl[1];
    /*    std::map<int,organData>::const_iterator ite = organDataOld.find(id);
    if( ite != organDataOld.end() ) {
      name = (*ite).second.name;
    } else {      
      name = "organ_"+ itoa(id);
      }*/
    organData odat;
    //--- If organ grouping, change id and name
    if( bOrganGrouping ) {
      int idOld = id;
      if( theOrganGroupNames.find(wl[1]) != theOrganGroupNames.end() ) {
	id = theOrganGroupIDs[idOld];
	name = theOrganGroupNames[wl[1]];
	double valOld = organData1[id].value;
	if( organData1[id].mass != 0)  valOld /= organData1[id].mass;
	std::cout << " VALOLD " << valOld << " " << organData1[id].value << " / " << organData1[id].mass << std::endl; //GDEB
	double errOld = organData1[id].error;
	double massOld = organData1[id].mass;
	double wOld = massOld/(mass+massOld);
	double w = mass/(mass+massOld);	
	//---- If error plotting use error proportion
	if( !bPlotErrors ) {
	  val = (valOld*wOld + val*w);
	  err = sqrt(err*w*err*w+errOld*wOld*errOld*wOld);
	} else {
	  if( valOld != 0 ) valOld = errOld/valOld;
	  val = (valOld*wOld + val*w);
	  err = sqrt(err*w*err*w+errOld*wOld*errOld*wOld);
	  if( val != 0 ) val = err/val;	  
	}
	mass += massOld;
	if( verbose >= 3 ) std::cout << " ORGANMG " << idOld << " -> " << id << " : " << wl[1] << " -> " << odat.name << " : val " << valOld << " +- " << errOld << " --> " << val << " +- " << err << " mass " << mass << std::endl;
      } else {
	std::cerr << "!! WARNING  organ not found in organ_group_names " << wl[1] << std::endl;
      }
    }
    //--- Set data and add it to data list 
    odat.name = name;
    odat.value = val;
    odat.error = err;
    odat.mass = mass;
    organData1[id] = odat;
    if( verbose >= 2 ) std::cout << " ADD ORGAN to list " << id << " : " << name << " " << val << " +- " << err << " " << mass << std::endl; //GDEB
    //      std::cout << " ADD to list " << name << " " << id << " " << theOrganData[id].value << std::endl; //GDEB
  }

  theOrganData.push_back(organData1);

  std::cout << " TOTAL ENERGY " << fileName << " = " << totalEnergy << std::endl; 
}
