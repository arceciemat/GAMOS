#ifndef DicomVerbosity_HH
#define DicomVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class DicomVerbosity : public GmVVerbosity
{
public:
  DicomVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};


#ifdef WIN32
  #if defined DICOMBase_EXPORTS 
		extern __declspec(dllexport) GmVerbosity DicomVerb;
	#else 
		extern __declspec(dllimport) GmVerbosity DicomVerb;
	#endif
#else
  extern GmVerbosity DicomVerb;
#endif

#endif
