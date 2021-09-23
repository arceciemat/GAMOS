..\run1.win.bat exercise1 exercise1a
..\run1.win.bat exercise1 exercise1b
..\run1.win.bat exercise1 exercise1c

..\run1.win.bat exercise2 exercise2a
..\run1.win.bat exercise2 exercise2b
..\run1.win.bat exercise2 exercise2b.1M
cd exercise2 
copy test* ..\exercise3
cd ..
..\run1.win.bat exercise2 exercise2b
..\run1.win.bat exercise2 exercise2c
..\run1.win.bat exercise2 exercise2c1
..\run1.win.bat exercise2 exercise2c2
..\run1.win.bat exercise2 exercise2c3

..\run1.win.bat exercise3 exercise3a
..\run1.win.bat exercise3 exercise3b
..\run1.win.bat exercise3 exercise3c
..\run1.win.bat exercise3 exercise3d

..\run1.win.bat exercise4 exercise4a
..\run1.win.bat exercise4 exercise4a.NoCuts
..\run1.win.bat exercise4 exercise4a.Cuts
..\run1.win.bat exercise4 exercise4b.UBS
..\run1.win.bat exercise4 exercise4b.ZBS

copy exercise3\test_899.IAEA* exercise4
..\run1.win.bat exercise4 exercise4b.Dose
move gamos.exercise4b.Dose.log gamos.exercise4b.Dose_899.log
move gamos_error.exercise4b.Dose.log gamos_error.exercise4b.Dose_899.log

..\run1.win.bat exercise4 exercise4b.DoseUBS

..\run1.win.bat exercise4 exercise4b.DoseZBS

cd exercise4 
sed s:ZBS:_899:g exercise4b.Dose.in > ttt ; mv ttt exercise4b.Dose.in
cd ..

..\run1.win.bat exercise4 exercise4c
..\run1.win.bat exercise4 exercise4c1
..\run1.win.bat exercise4 exercise4c2
..\run1.win.bat exercise4 exercise4c3

..\run1.win.bat exercise5 exercise5a
..\run1.win.bat exercise5 exercise5b
