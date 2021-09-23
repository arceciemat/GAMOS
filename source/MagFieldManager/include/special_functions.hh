///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef SPECFUNC_h
#define SPECFUNC_h 1
#include <fstream>
#include <complex>
#include <vector>



namespace special_functions{
	double rf(double x, double y, double z);
	double rd(double x, double y, double z);
	double CompleteEllipticIntegral1stKindK(double m);
	double CompleteEllipticIntegral2ndKindE(double m);
	void CompleteEllipticIntegrals(double m, double& K, double& E);
	void bfield_from_IZ_line(double r[3],double o[3],double l,  double b[3]);
	void bfield_from_IX_line(double r[3],double o[3],double l,  double b[3]);
	void bfield_from_IY_line(double r[3],double o[3],double l,  double b[3]);
	void bx_from_IYZ_surface(double r[3],double o[3],double ly,double lz,  double& bx);
	void by_from_IXZ_surface(double r[3],double o[3],double lx,double lz,  double& by);
	

}
#endif
