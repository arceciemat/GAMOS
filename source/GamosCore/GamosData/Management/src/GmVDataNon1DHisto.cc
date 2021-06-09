#include "GmVDataNon1DHisto.hh"
#include "GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmVDataNon1DHisto::GmVDataNon1DHisto() : GmVData()
{
}

//----------------------------------------------------------------
GmVDataNon1DHisto::~GmVDataNon1DHisto()
{
}


//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteText( const G4Step* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteText( const G4Track* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteText( const G4Track* , const G4Track* , G4bool  )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteText( const G4Event* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteText( const G4Run* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}


//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteBin( const G4Step* )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteBin( const G4Track* )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteBin( const G4Track* , const G4Track* )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteBin( const G4Event* )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteBin( const G4Run* )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}


//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteCout( const G4Step* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteCout",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteCout( const G4Track* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteCout",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteCout( const G4Track* , const G4Track* , G4bool  )
{
  G4Exception("GmVDataNon1DHisto::WriteCout",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteCout( const G4Event* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteCout",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}
//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteCout( const G4Run* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteCout",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}

