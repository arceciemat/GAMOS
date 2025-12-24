from scipy.optimize import curve_fit

def poly1(x, C0, C1):
    return C0+C1*x

def FitPoly1( hx, hy, paramIni ) :
#    print("FITPoly1 PARAMINI",paramIni) #GDEB
    try:
        param, covariance = curve_fit(poly1, hx, hy, p0=[paramIni])
    except RuntimeError:
        print("Error - FitPoly1 cannot be done")
        param = [0.,0.,0.]
        covariance = [ [0.,0.,0.], [0.,0.,0.], [0.,0.,0.]]

    return param,covariance

def poly2(x, C0, C1, C2 ):
    return C0+C1*x+C2*x*x

def FitPoly2( hx, hy, paramIni ) :
    try:
        param, covariance = curve_fit(poly2, hx, hy, p0=[paramIni])
    except RuntimeError:
        print("Error - FitPoly2 cannot be done")
        param = [0.,0.,0.]
        covariance = [ [0.,0.,0.], [0.,0.,0.], [0.,0.,0.]]

    return param,covariance

def poly3(x, C0, C1, C2, C3 ):
    return C0+C1*x+C2*x*x+C3*pow(x,3)

def FitPoly3( hx, hy, paramIni ) :
    try:
        param, covariance = curve_fit(poly3, hx, hy, p0=[paramIni])
    except RuntimeError:
        print("Error - FitPoly3 cannot be done")
        param = [0.,0.,0.]
        covariance = [ [0.,0.,0.], [0.,0.,0.], [0.,0.,0.]]

    return param,covariance
