#!/usr/bin/python
import os
import sys
import units
from units import *
from pylab import *
from numpy import *

def read_grid(file_name):
	file_to_read=open(file_name,'r')
	lines=file_to_read.readlines()
	line=lines[1]
	words=line.split()
	nx=int(words[0])
	ny=int(words[1])
	nz=int(words[2])
	start_ind=2
	
	X=zeros((nx,ny,nz))
	Y=zeros((nx,ny,nz))
	Z=zeros((nx,ny,nz))
	BX=zeros((nx,ny,nz))
	BY=zeros((nx,ny,nz))
	BZ=zeros((nx,ny,nz))
	B=zeros((nx,ny,nz))
	ind=2
	
	for ix in range (nx):
		for iy in range (ny):
			for iz in range (nz):
				ind+=1
				words=lines[ind].split()
				X[ix,iy,iz]=float(words[0])
				Y[ix,iy,iz]=float(words[1])
				Z[ix,iy,iz]=float(words[2])
				BX[ix,iy,iz]=float(words[3])
				BY[ix,iy,iz]=float(words[4])
				BZ[ix,iy,iz]=float(words[5])
				B[ix,iy,iz]=float(words[6])
	return [X,Y,Z,BX,BY,BZ,B]

def read_results_sim_without_phys(file_name):
	file_to_read=open(file_name,'r')
	lines=file_to_read.readlines()
	n=len(lines)-5
	Type=zeros((n))
	Ekin=zeros((n))
	Rig=zeros((n))
	Trans=zeros((n))
	Error=zeros((n))
	i=0
	for line in lines[5:]:
		words=line.split()
		if (words[0] == "Backward"):
			Type[i]=1
		Ekin[i]=float(words[2])	 
		Rig[i]=float(words[3])
		Trans[i]=float(words[6])
		Error[i]=float(words[7])
		i+=1
	return [Type,Ekin,Rig,Trans,Error]



		
	

def plot_2DBfield(r,z,B,phi):
	
	Bmax=amax(B)
	levels = arange(0.001, Bmax*1.0001, Bmax/100)
	cset1 = contourf(r,z,B, levels,
                        	cmap=cm.get_cmap('jet', len(levels)-1),)			
	title("$\phi=%.2f^o$" %(phi/degree))	
	xlabel("r [m]")
	ylabel("z [m]")
        CB=colorbar(cset1,ticks=(2.5,5.,7.5,10.,12.5,15.,17.5,20.,22.5))
	CB.set_label("B[Tesla]")
	
	
	return 	cset1

def plot_2DJcurrent_XY(x,y,J,z,Jref):
	
	Jmax=1.0001*amax(J)
	val1=log10(Jmax)
	val2=int(val1)-3
	val3=int(Jmax*math.pow(10.,-val2))
	Jmax=val3*math.pow(10.,val2)
	
	
	levels = arange(0.01*Jref, Jmax*1.01, Jmax/100)
	cset1 = contourf(x,y,J, levels,
                        	cmap=cm.get_cmap('jet', len(levels)-1),)			
	title("J from a Toroid  at z =%.2f" %(z/m))	
	xlabel("x [m]")
	ylabel("y [m]")		
	t=array((2.,4.,6.,8.,10.))*Jmax/10.
	CB=colorbar(cset1,ticks=t)
	CB.set_label("J[A/m$^2$]")
	return 	cset1

def plot_2DJcurrent_YZ(y,z,J,Jref):
	
	Jmax=amax(J)
	val1=log10(Jmax)
	val2=int(val1)-3
	val3=int(Jmax*math.pow(10.,-val2))
	Jmax=val3*math.pow(10.,val2)
	
	levels = arange(0.01*Jref, Jmax*1.0001, Jmax/100)
	cset1 = contourf(y,z,J, levels,
                        	cmap=cm.get_cmap('jet', len(levels)-1),)			
	title("")	
	xlabel("y [m]")
	ylabel("z [m]")
	t=array((2.,4.,6.,8.,10.))*Jmax/10.
	t=array((0.3,0.6,0.9,1.2,1.5))*1e8		
	CB=colorbar(cset1,ticks=t)
	CB.set_label("J[A/m$^2$]")
	
	return 	cset1

def plot_2DJcurrent_XZ(x,z,J,Jref):
	
	Jmax=amax(J)
	levels = arange(0.01*Jref, Jmax*1.0001, Jmax/100)
	cset1 = contourf(y,z,J, levels,
                        	cmap=cm.get_cmap('jet', len(levels)-1),)			
	title("J from a Coil")	
	xlabel("x [m]")
	ylabel("z [m]")		
	t=array((0.3,0.6,0.9,1.2,1.5))*1e8
	CB=colorbar(cset1,ticks=t)
	
	return 	cset1	

def plot_JAlongY(y,J,Jref,z):
	J_vec=y*0.+Jref
	plot(y, J)
	plot(y, J_vec)
	title("z=%.2f m" %(z/m))
	xlabel("y[m]")
	ylabel("J[A/m$^2$]")
	ylim(0.,1.5*Jref)
def plot_JAlongZ(z,J,Jref,y):
	J_vec=z*0.+Jref
	plot(z, J)
	plot(z, J_vec)
	title("y=%.2f m" %(y/m))
	xlabel("z[m]")
	ylabel("J[A/m$^2$]")
	ylim(0.,1.5*Jref)
		
		

def plot_BfieldAlongR(r,B,phi,z,I):
        print r
	print B
	one_over_r=1/r
	mu0=1.256637e-6
	B_theory=mu0*one_over_r*I/2./pi
	plot(r, B)
	plot(r[1:], B_theory[1:])
	Bmax=amax(B)
	xlabel('r[m]')
	ylabel('B[Tesla]')
	title(r'$\phi=%.2f^o$ z = %.2f m' %(phi/units.degree,z/units.m))
	ylim(0.,1.2*Bmax)
	grid(True)

def plot_BfieldAlongZ(z,B,phi,r,I):

	one_over_r=z*0.+1/r
	mu0=1.256637e-6
	B_theory=mu0*one_over_r*I/2./pi
	plot(z, B)
	plot(z, B_theory)
	Bmax=amax(B)
	xlabel('z[m]')
	ylabel('B[Tesla]')
	title(r'$\phi=%.2f^o$ r = %.2f m' %(phi/units.degree,r/units.m))
	ylim(0.,1.2*Bmax)
	grid(True)
	
def plot_Bfield(name_file,phi_ind1,phi_ind2,phi_ind3,phi_ind4,z_ind1,z_ind2,r_ind,fig_name,I):
	res=read_grid(name_file)
	r=res[0]
	phi=res[1]
	z=res[2]
	Br=res[3]
	Bphi=res[4]
	Bz=res[5]
	B=res[6]
	
	fig=figure()
	figtext(0.3,.95,"Computed Magnetic Field" , fontsize=16)
	
	
	subplot(2,2,1)
	X=r[:,0,:]
	Y=z[:,0,:]
	Z=B[:,0,:]
	plot_2DBfield(X,Y,Z,phi[0,phi_ind1,z_ind1])
	
	
	subplot(2,2,2)
	plot_BfieldAlongR(r[:,phi_ind2,z_ind1],B[:,phi_ind2,z_ind1],phi[0,phi_ind2,z_ind1],z[0,phi_ind2,z_ind1],I)
	legend(("Computed B",'B=mu_0I/2piR'))

	subplot(2,2,3)
	plot_BfieldAlongR(r[:,phi_ind3,z_ind2],B[:,phi_ind3,z_ind2],phi[0,phi_ind3,z_ind2],z[0,phi_ind3,z_ind2],I)
	subplot(2,2,4)
	plot_BfieldAlongZ(z[r_ind,phi_ind4,:],B[r_ind,phi_ind4,:],phi[r_ind,phi_ind4,0],r[r_ind,phi_ind4,0],I)
	
	savefig(fig_name+".png")
	savefig(fig_name+".pdf")		
	savefig(fig_name+".ps")




def plot_Jtest(coil_file,fig_name,I,ind_y,ind_z):
	res=read_grid(coil_file)
	Coilx=res[0]
	Coily=res[1]
	Coilz=res[2]
	CoilJ=res[6]
	
	
	fig=figure()
	
	subplot(2,2,3)
	
	Jref=I/0.11/0.11
	
	plot_2DJcurrent_YZ(Coily[0,:,:],Coilz[0,:,:],CoilJ[0,:,:],Jref)
	
	subplot(2,2,1)
	
	plot_JAlongY(Coily[0,:,ind_z],CoilJ[0,:,ind_z],Jref,Coilz[0,0,ind_z])
	
	subplot(2,2,2)
	
	plot_JAlongZ(Coilz[0,ind_y,:],CoilJ[0,ind_y,:],Jref,Coily[0,ind_y,0])
	
	
	figtext(.2,.95,"Computed Current Density of a Coil" , fontsize=16)
	

	savefig(fig_name+".png")
	savefig(fig_name+".pdf")		
	savefig(fig_name+".ps")
	
				
##################################
##The Bfield Plot

rc('figure',figsize=(16.*units.cm/units.inch,16.*units.cm/units.inch))
rc('figure.subplot',bottom=0.2)
rc('figure.subplot',top=0.9)
rc('figure.subplot',hspace=0.4)
rc('figure.subplot',wspace=0.35)
rc('axes',labelsize=10)
rc('axes',titlesize=10)
rc('xtick',labelsize=8)
rc('ytick',labelsize=8)
rc('legend',fontsize=10)

I=1.9e6	
plot_Bfield("total_small_grid.txt",0,0,5,0,100,100,50,"B_small_grid",48.*1.9e6)




##################################
##The J  Plot


rc('figure',figsize=(16.*units.cm/units.inch,16.*units.cm/units.inch))
rc('figure.subplot',bottom=0.1)
rc('figure.subplot',top=0.9)
rc('figure.subplot',hspace=0.3)
rc('figure.subplot',wspace=0.3)
rc('axes',labelsize=12)
rc('axes',titlesize=12)
rc('xtick',labelsize=8)
rc('ytick',labelsize=8)

I=1.9e6	

plot_Jtest("coil_jtest.txt","JCoil",I,30,70)



