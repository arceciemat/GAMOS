#################################################################
#units m,s,MeV,kg,eio=35.eV
#
m=1.
m2=1.
m3=1.
cm=1e-2*m
cm2=cm*cm
cm3=cm*cm*cm
mm=1.e-3*m
mm2=mm*mm
mm3=mm*mm2
km=1000.*m
km2=km*km
MeV=1. 
inch=2.54*cm
eV=1.e-6*MeV
keV=1.e-3*MeV
GeV=1.e3*MeV
TeV=1.e6*MeV
erg=1.e12*eV/1.602
MV=1.
GV=1.e3*MV
V=1e-6
s=1.
e=1.
kg=1. 
g=1.e-3*kg
rad=erg*100./g
eio=35.*eV
light_speed=2.99792458e8*m/s
sr=1./4./3.1415926
minute=60.*s
hour=60.*minute
day=24.*hour
year=365.*day
ampere=1
pi=3.1415926
degree=pi/180.
amu_in_MeV=931.49432*MeV
inch=2.54*cm


str_units=["m","m2","m3","cm","cm2","cm3",
	   "km","km2","MeV","eV","keV","GeV","TeV","erg",
	   "MV","GV","V",
	   "s","e",
	   "kg","g",
	   "rad","eio","light_speed","sr","nb_part","ampere","pi","degree"]

units_vec=[m,m2,m3,cm,cm2,cm3,
	   km,km2,MeV,eV,keV,GeV,TeV,erg,
	   MV,GV,V,
	   s,e,
	   kg,g,
	   rad,eio,light_speed,sr,1.,ampere,pi,degree]	   
	   
def find_unit(unit_str):
	res=1.
	unit_str1=unit_str.replace(" ","")
	unit_str2=unit_str1.replace("[","")
	unit_str1=unit_str2.replace("]","")
	
	str_vec=unit_str1.split("*")
	for str1 in str_vec:
		str1_vec= str1.split("/")
		i=0
		for str2 in str1_vec:
			if (i==0):
				i=1
				if (str2 !='' and str2 !="1." and str2 !="1"):
					res*=find_simple_unit(str2)
			else:
				res=res/find_simple_unit(str2)
	print res
	return res

def find_simple_unit(unit_str):
	unit=0.
	for i in range(len(str_units)):
		if (str_units[i] == unit_str):
			unit=units_vec[i]
			break
	if (unit ==0.):
		print "The unit %s is not defined in this program!" %(unit_str)
		  
	return unit
	
