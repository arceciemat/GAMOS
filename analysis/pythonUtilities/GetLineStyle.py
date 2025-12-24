import numpy as np

def GetLineStyle( ils ) :
    ilss = np.mod(ils,5)
    if ilss == 0 :
        lLS = '-'
    elif ilss == 1 :
        lLS = '--'
    elif ilss == 2 :
        lLS = '-.'
    elif ilss == 3 :
        lLS = ':'
    elif ilss == 4 :
        lLS = (0, [8, 2, 1, 2]) #	Long dash, short space, dot, short spac
        lLS = (0, [2, 2, 1, 2]) #	short dash, short space, dot, short spac
    return lLS

