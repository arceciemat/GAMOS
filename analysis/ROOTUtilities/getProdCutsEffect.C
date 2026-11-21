// getProdCutsEffect.C: production cuts (or minimum-range user limits) that lose less than a given
// percentage of the particles reaching the target. ROOT version of getProdCutsEffect.py, same output.
//
// It reads the ROOT histogram file written by GmProdCutsStudyUA (prodcuts.root) or by
// GmMinRangeLimitsStudyUA (minrange.root). For each region and particle it takes the histogram
// "<UA>: REGION: PARTICLE: ALL:  log10(Range)" and finds the biggest cut such that the number of entries
// with range smaller than the cut is not bigger than PERCENTAGE % of the tracks that reached the target.
//
// Usage:
//    root -b -q 'getProdCutsEffect.C("prodcuts.root",1.)'
#include "TFile.h"
#include "TKey.h"
#include "TH1.h"
#include "TString.h"
#include "TObjArray.h"
#include "TObjString.h"
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace {
  // collect all 1D histograms of a directory and its subdirectories
  void CollectHistos(TDirectory* dir, std::vector<TH1*>& histos)
  {
    TIter next(dir->GetListOfKeys());
    while( TKey* key = (TKey*)next() ) {
      TObject* obj = key->ReadObj();
      if( obj->InheritsFrom("TDirectory") ) {
        CollectHistos((TDirectory*)obj, histos);
      } else if( obj->InheritsFrom("TH1") && ((TH1*)obj)->GetDimension() == 1 ) {
        histos.push_back((TH1*)obj);
      }
    }
  }
}

void getProdCutsEffect(const char* fileName, double percentage)
{
  const TString hisSuffix = "log10(Range)";
  const TString nTargetName = "N trackInfos per Track in target";

  TFile* file = TFile::Open(fileName);
  if( !file || file->IsZombie() ) {
    std::cerr << "getProdCutsEffect.C: cannot open " << fileName << std::endl;
    return;
  }
  std::vector<TH1*> histos;
  CollectHistos(file, histos);

  double nTarget = -1.;
  for( TH1* his : histos ) {
    if( TString(his->GetName()).EndsWith(nTargetName) ) nTarget = his->GetEntries();
  }
  if( nTarget < 0 ) {
    std::cerr << "getProdCutsEffect.C: histogram '" << nTargetName << "' not found in " << fileName
	      << ": is it a file written by GmProdCutsStudyUA or GmMinRangeLimitsStudyUA?" << std::endl;
    return;
  }
  if( nTarget == 0 ) {
    std::cerr << "getProdCutsEffect.C: no track reached the target in " << fileName << std::endl;
    return;
  }
  double maxLost = nTarget*percentage/100.;

  std::cout << "getProdCutsEffect.C: " << fileName << "  tracks reaching the target= " << nTarget
	    << "  maximum lost= " << percentage << " %" << std::endl;
  std::map<std::string,double> finalLost;
  for( TH1* his : histos ) {
    TString name = his->GetName();
    if( !name.EndsWith(hisSuffix) ) continue;
    // name is "UA: REGION: PARTICLE: PROCESS:  log10(Range)"
    TObjArray* words = TString(name(0,name.Length()-hisSuffix.Length())).Tokenize(":");
    if( words->GetEntries() < 4 ) { delete words; continue; }
    TString region = ((TObjString*)words->At(1))->GetString().Strip(TString::kBoth);
    TString particle = ((TObjString*)words->At(2))->GetString().Strip(TString::kBoth);
    TString process = ((TObjString*)words->At(3))->GetString().Strip(TString::kBoth);
    delete words;
    if( process != "ALL" || region == "ALL" ) continue;

    int nbins = his->GetNbinsX();
    // GAMOS normalises the histograms to the number of events by default
    // (parameter <FILE>:NormalizeToNEvents): convert the contents back to number of entries
    double total = his->Integral(0,nbins+1);
    if( total > 0 ) his->Scale(his->GetEntries()/total);
    double lost = his->GetBinContent(0);  // underflow: lost with any cut
    TString cutStr;
    if( lost > maxLost ) {
      cutStr = TString::Format("NONE (the underflow, range < %g mm, has already too many entries)",
			       pow(10.,his->GetXaxis()->GetXmin()));
      lost = 0.;
    } else {
      cutStr = TString::Format("ANY (all %d entries can be lost)", int(his->GetEntries()));
      for( int ib = 1; ib <= nbins+1; ib++ ) {
	if( lost + his->GetBinContent(ib) > maxLost ) {
	  cutStr = TString::Format("%.4g mm", pow(10.,his->GetXaxis()->GetBinLowEdge(ib)));
	  break;
	}
	lost += his->GetBinContent(ib);
      }
    }
    std::cout << "REGION= " << region << " PARTICLE= " << particle << " CUT= " << cutStr
	      << " LOST= " << int(lost+0.5) << " / " << int(nTarget) << std::endl;
    finalLost[particle.Data()] += lost;
  }
  for( auto& pl : finalLost ) {
    std::cout << "PARTICLE= " << pl.first << " FINAL= " << int(pl.second+0.5) << " / " << int(nTarget) << std::endl;
  }
}
