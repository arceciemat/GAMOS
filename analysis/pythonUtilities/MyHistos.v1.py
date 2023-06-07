from collections.abc import MutableMapping

class Histo1D(MutableMapping):
#    def __init__(self, a, b, c, d, e, f):
#        self.type = a
#        self.name = b
#        self.nbin = c
#        self.xmin = d
#        self.xmax = e
#        self.data = f
    type = "1D"
    name = ""
    nbin = int(0)
    xmin = float(0)
    xmax = float(0)
    under = 0
    data = []
    over = 0
    underErr = 0
    dataErr = []
    overErr = 0
    mean = 0
    meanErr = 0
    RMS = 0
    RMSErr = 0
    def __init__(self, *args, **kwargs):
        #args -- tuple of anonymous arguments
        #kwargs -- dictionary of named arguments
        if len(args) > 0 :  self.type = args[0]
        if len(args) > 1 : 
            self.name = args[1]
        if len(args) > 2 :
            self.nbin = args[2]
        if len(args) > 3 :
            self.xmin = args[3]
        if len(args) > 4 :
            self.xmax = args[4]
        if len(args) > 5 :
            self.under = args[5]
        if len(args) > 6 :
            self.underErr = args[6]
        if len(args) > 7 :
            self.data = args[7]
        if len(args) > 8 :
            self.dataErr = args[8]
        if len(args) > 9 :
            self.over = args[9]
        if len(args) > 10 :
            self.overErr = args[10]
        if len(args) > 11 :
            self.mean = args[11]
        if len(args) > 12 :
            self.meanErr = args[12]
        if len(args) > 13 :
            self.RMS = args[13]
        if len(args) > 14 :
            self.RMSErr = args[14]
#        self.num_holes = kwargs.get('num_holes',random_holes())
    def __setitem__(self, k, v):
        setattr(self, k, v)
    def __getitem__(self, k):
        getattr(self, k)
    def __len__(self):
        return 2
    def __delitem__(self, k):
        self[k] = None
    def __iter__(self):
        yield self.name
        yield self.nbin
        yield self.xmin
        yield self.xmax
        yield self.under
        yield self.data
        yield self.over
        yield self.underErr
        yield self.dataErr
        yield self.overErr
        yield self.nent
        yield self.mean
        yield self.meanErr
        yield self.RMS
        yield self.RMSErr
    def __str__(self):
        return ("Histo1D: "+self.type+" "+self.name+" "+str(self.nbin)
                +" "+str(self.xmin)+" "+str(self.xmax)
                +" U="+str(self.under)+"+-"+str(self.underErr)
                +" O="+str(self.over)+"+-"+str(self.overErr)
                +" mean="+str(self.mean)+"+-"+str(self.meanErr)
                +" RMS="+str(self.RMS)+"+-"+str(self.RMSErr))
    @staticmethod
    def Xstep(obj):
        xStep = (float(obj.xmax)-float(obj.xmin))/int(obj.nbin)
        return xStep
    @staticmethod
    def Xbins(obj):
        Xbins = []
        xStep = (float(obj.xmax)-float(obj.xmin))/int(obj.nbin)
        for ii in range(0,int(obj.nbin)):
            Xbins.append(float(obj.xmin)+(0.5+ii)*xStep)
        return Xbins 
