..\run1.win.bat exercise1 exercise1a1
..\run1.win.bat exercise1 exercise1a2
..\run1.win.bat exercise1 exercise1b
..\run1.win.bat exercise2 exercise2
..\run1.win.bat exercise3 exercise3.angle.gg
..\run1.win.bat exercise3 exercise3.angle.e-g
..\run1.win.bat exercise3 exercise3.angle.e+g
..\run1.win.bat exercise3 exercise3.angle.ng
..\run1.win.bat exercise3 exercise3.angle.nn
python $PYTHONPATH/sumHist.py -fOut exercise3/angleDeviation.gammaALL.csv -f exercise3/angleDeviation.*gamma.csv
python $PYTHONPATH/sumHist.py -fOut exercise3/angleDeviation.neutronALL.csv -f exercise3/angleDeviation.*neutron.csv
..\run1.win.bat exercise3 exercise3a
..\run1.win.bat exercise3 exercise3b
..\run1.win.bat exercise4 exercise4a
cd exercise4; extractActivity out.exercise4a; cd ..
..\run1.win.bat exercise4 exercise4b
..\run1.win.bat exercise5 exercise5a
..\run1.win.bat exercise5 exercise5b
..\run1.win.bat exercise5 exercise5c
..\run1.win.bat exercise5 exercise5d
..\run1.win.bat exercise5 exercise5e
..\run1.win.bat exercise5 exercise5f
..\run1.win.bat exercise5 exercise5g
