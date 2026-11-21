#include "GmMTMergeMgr.hh"
#include "G4AutoLock.hh"
#include <cstdio>
#include <algorithm>
#include <vector>
#include <fstream>

namespace {
  G4Mutex mergeMutex = G4MUTEX_INITIALIZER;
  G4Mutex registerMutex = G4MUTEX_INITIALIZER;
}

//-----------------------------------------------------------------------
std::map<G4String,std::function<void()> >& GmMTMergeMgr::GetMergers()
{
  static std::map<G4String,std::function<void()> > theMergers;
  return theMergers;
}

//-----------------------------------------------------------------------
void GmMTMergeMgr::RegisterMerger( const G4String& name, std::function<void()> merger )
{
  G4AutoLock l(&registerMutex);
  std::map<G4String,std::function<void()> >& mergers = GetMergers();
  if( mergers.find(name) == mergers.end() ) mergers[name] = merger;
}

//-----------------------------------------------------------------------
void GmMTMergeMgr::MergeAllToMaster()
{
  if( !IsWorkerThread() ) return;
  std::map<G4String,std::function<void()> > mergers;
  {
    G4AutoLock l(&registerMutex);
    mergers = GetMergers();
  }
  G4AutoLock l(&mergeMutex);
  for( auto ite = mergers.begin(); ite != mergers.end(); ite++ ) {
    (ite->second)();
  }
}

//-----------------------------------------------------------------------
namespace {
  struct ThreadFileInfo {
    std::map<G4int,G4String> threadFiles; // thread id - file name
    GmMTMergeMgr::ThreadFileMergeMode mode = GmMTMergeMgr::TFMConcatenate;
    G4int nHeaderLines = 0;
  };
  G4Mutex threadFilesMutex = G4MUTEX_INITIALIZER;
  std::map<G4String,ThreadFileInfo>& GetThreadFiles() {
    static std::map<G4String,ThreadFileInfo> theThreadFiles; // merged file name - info
    return theThreadFiles;
  }
}

G4String GmMTMergeMgr::GetThreadFileName( const G4String& fileName )
{
  if( !IsWorkerThread() || fileName == "" ) return fileName;
  G4int thID = G4Threading::G4GetThreadId();
  G4String thStr = ".th" + std::to_string(thID);
  size_t idot = fileName.rfind('.');
  size_t islash = fileName.rfind('/');
  G4String thName;
  if( idot == std::string::npos || (islash != std::string::npos && idot < islash) ) {
    thName = fileName + thStr;
  } else {
    thName = fileName.substr(0,idot) + thStr + fileName.substr(idot);
  }
  G4AutoLock l(&threadFilesMutex);
  GetThreadFiles()[fileName].threadFiles[thID] = thName;
  return thName;
}

//-----------------------------------------------------------------------
void GmMTMergeMgr::SetThreadFileMergeMode( const G4String& threadFileName, ThreadFileMergeMode mode, G4int nHeaderLines )
{
  G4AutoLock l(&threadFilesMutex);
  for( auto ite = GetThreadFiles().begin(); ite != GetThreadFiles().end(); ite++ ) {
    for( auto itet = ite->second.threadFiles.begin(); itet != ite->second.threadFiles.end(); itet++ ) {
      if( itet->second == threadFileName ) {
	ite->second.mode = mode;
	ite->second.nHeaderLines = nHeaderLines;
	return;
      }
    }
  }
}

//-----------------------------------------------------------------------
namespace {
  G4bool EndsWith( const G4String& str, const G4String& end ) {
    return str.size() >= end.size() && str.compare(str.size()-end.size(), end.size(), end) == 0;
  }
  // append the bytes of fin from its current position to fout
  void CopyRest( std::ifstream& fin, std::ofstream& fout ) {
    std::vector<char> buf(1<<20);
    while( fin ) {
      fin.read(buf.data(), buf.size());
      std::streamsize n = fin.gcount();
      if( n > 0 ) fout.write(buf.data(), n);
    }
  }
  G4bool MergeEGSPhsp( const G4String& outName, const std::vector<G4String>& inNames ) {
    char mode[6] = {'\0'};
    G4int nPart = 0, nPhot = 0;
    G4float maxE = 0., minE = 0., nInc = 0.;
    G4int recLen = 28;
    G4bool bFirst = true;
    for( size_t ii = 0; ii < inNames.size(); ii++ ) {
      std::ifstream fin(inNames[ii], std::ios::binary);
      char md[5]; G4int np, nph; G4float mx, mn, ni;
      if( !fin.read(md,5) || !fin.read((char*)&np,4) || !fin.read((char*)&nph,4) || !fin.read((char*)&mx,4) || !fin.read((char*)&mn,4) || !fin.read((char*)&ni,4) ) return false;
      if( bFirst ) {
	for( G4int jj = 0; jj < 5; jj++ ) mode[jj] = md[jj];
	recLen = ( md[4] == '2' ) ? 32 : 28;
	maxE = mx; minE = mn;
	bFirst = false;
      } else {
	maxE = std::max(maxE,mx); minE = std::min(minE,mn);
      }
      nPart += np; nPhot += nph; nInc += ni;
    }
    std::ofstream fout(outName, std::ios::binary);
    fout.write(mode,5); fout.write((char*)&nPart,4); fout.write((char*)&nPhot,4);
    fout.write((char*)&maxE,4); fout.write((char*)&minE,4); fout.write((char*)&nInc,4);
    for( G4int jj = 25; jj < recLen; jj++ ) fout.put('\0');
    for( size_t ii = 0; ii < inNames.size(); ii++ ) {
      std::ifstream fin(inNames[ii], std::ios::binary);
      fin.seekg(recLen, std::ios::beg);
      CopyRest(fin, fout);
    }
    return true;
  }
}

void GmMTMergeMgr::MergeThreadFiles( G4bool bKeepThreadFiles )
{
  std::map<G4String,ThreadFileInfo> threadFiles;
  {
    G4AutoLock l(&threadFilesMutex);
    threadFiles = GetThreadFiles();
    GetThreadFiles().clear();
  }
  for( auto ite = threadFiles.begin(); ite != threadFiles.end(); ite++ ) {
    const G4String& outName = ite->first;
    const ThreadFileInfo& info = ite->second;
    std::vector<G4String> inNames;
    for( auto itet = info.threadFiles.begin(); itet != info.threadFiles.end(); itet++ ) {
      std::ifstream ftest(itet->second);
      if( ftest.good() ) inNames.push_back(itet->second); // may have been merged and deleted already (e.g. IAEA phase space)
    }
    if( inNames.size() == 0 ) continue;
    if( EndsWith(outName,".root") ) {
      G4cout << "GmMTMergeMgr: ROOT files of the threads are not merged, use: hadd " << outName;
      for( size_t ii = 0; ii < inNames.size(); ii++ ) G4cout << " " << inNames[ii];
      G4cout << G4endl;
      continue;
    }
    if( EndsWith(outName,".IAEAphsp") || EndsWith(outName,".IAEAheader") ) continue; // merged by RTPhaseSpaceUA
    if( info.mode == TFMNoMerge ) {
      G4cout << "GmMTMergeMgr: thread files of " << outName << " cannot be merged by concatenation (binary header), they are kept:";
      for( size_t ii = 0; ii < inNames.size(); ii++ ) G4cout << " " << inNames[ii];
      G4cout << G4endl;
      continue;
    }
    G4bool bOK = true;
    if( info.mode == TFMEGSPhsp ) {
      bOK = MergeEGSPhsp( outName, inNames );
    } else {
      std::ofstream fout(outName, std::ios::binary);
      for( size_t ii = 0; ii < inNames.size(); ii++ ) {
	std::ifstream fin(inNames[ii], std::ios::binary);
	if( ii != 0 && info.mode == TFMSkipHeaderLines ) {
	  std::string line;
	  for( G4int jj = 0; jj < info.nHeaderLines; jj++ ) std::getline(fin,line);
	}
	CopyRest(fin, fout);
      }
      bOK = fout.good();
    }
    if( !bOK ) {
      G4cerr << "!!! GmMTMergeMgr: ERROR merging thread files into " << outName << ", thread files are kept" << G4endl;
      continue;
    }
    G4cout << "GmMTMergeMgr: merged " << inNames.size() << " thread files into " << outName << G4endl;
    if( !bKeepThreadFiles ) {
      for( size_t ii = 0; ii < inNames.size(); ii++ ) std::remove(inNames[ii].c_str());
    }
  }
}

//-----------------------------------------------------------------------
std::map<G4String,std::function<void()> >& GmMTMergeMgr::GetMasterEndOfRunActions()
{
  static std::map<G4String,std::function<void()> > theActions;
  return theActions;
}

//-----------------------------------------------------------------------
void GmMTMergeMgr::RegisterMasterEndOfRunAction( const G4String& name, std::function<void()> action )
{
  G4AutoLock l(&registerMutex);
  std::map<G4String,std::function<void()> >& actions = GetMasterEndOfRunActions();
  if( actions.find(name) == actions.end() ) actions[name] = action;
}

//-----------------------------------------------------------------------
void GmMTMergeMgr::RunMasterEndOfRunActions()
{
  if( !IsMTMaster() ) return;
  std::map<G4String,std::function<void()> > actions;
  {
    G4AutoLock l(&registerMutex);
    actions = GetMasterEndOfRunActions();
  }
  for( auto ite = actions.begin(); ite != actions.end(); ite++ ) {
    (ite->second)();
  }
}
