# Python program showing
# abstract base class work
 
from abc import ABC, abstractmethod
 
class Polygon(ABC):

    def __init__(self):
        print("MOTHER init 0 args")

    def __init__(self, *args):
        self.coche=3
        print("MOTHER init",len(args))
        
    def Initialize(self) :
        print("initialize")
        self.noofsides()
        self.noofsides3()
        self.coche2=2
        
    @abstractmethod
    def noofsides(self):
       print("dd",self.coche)#,self.coche2)
 
    def noofsides3(self):
       print("dd3")

class Triangle(Polygon):
 
    def __init__(self, *args):
        print("TRIANGLE init",len(args))
        super().__init__()
       
        # overriding abstract method
    def noofsides(self):
        print("I have 3 sides")
        super().noofsides()

    def noofsides2(self):
        print("2I have 3 sides")
 
class Pentagon(Polygon):
 
    # overriding abstract method
    def noofsides(self):
        print("I have 5 sides")
 
class Hexagon(Polygon):
 
    # overriding abstract method
    def noofsides(self):
        print("I have 6 sides")
 
class Quadrilateral(Polygon):
 
    # overriding abstract method
    def noofsides(self):
        print("I have 4 sides")
 
# Driver code
R = Triangle()
R.Initialize()
#print("COCHES",R.coche,R.coche2)
#R.noofsides()
 
#K = Quadrilateral()
#K.noofsides()
 
#R = Pentagon()
#R.noofsides()
 
#K = Hexagon()
#K.noofsides()
