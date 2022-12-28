
#!/usr/bin/env python 
# -*- coding: cp1252 -*-
import tkinter as tk

class Application(tk.Frame): 
    def __init__(self, master=None):
        tk.Frame.__init__(self, master) 
        self.grid()
        self.createWidgets()       
    def createWidgets(self):
        yScroll = tk.Scrollbar(self, orient=tk.VERTICAL)
        yScroll.grid(row=0, column=1, sticky=tk.NW+tk.S)
        self.xScroll = tk.Scrollbar(self, orient=tk.HORIZONTAL)
        self.xScroll.grid(row=1, column=0, sticky=tk.E+tk.W)
        self.listbox = tk.Listbox(self, xscrollcommand=self.xScroll.set, yscrollcommand=yScroll.set)
        self.listbox.grid(row=0, column=0, sticky=tk.N+tk.S+tk.E+tk.W)
        cities = ('CASTELL DE L''ARENY',
             'CASTELLADRAL',
             'CASTELLAR',
             'CASTELLAR DE N''HUG',
             'CASTELLAR DEL RIU',
             'CASTELLBELL I VILLAR',
             'CASTELLBISBAL',
             'CASTELLCIR',
             'CASTELLDEFELS',
             'CASTELLET',
             'CASTELLFOLLIT DE RIUBREGOS',
             'CASTELLFOLLIT DEL BOIX',
             'CASTELLGALI',
             'CASTELLNOU DE BAGES',
             'CASTELLOLI',
             'CASTELLTALLAT',
             'CASTELLTERCOL',
             'CASTELLVI DE LA MARCA',
             'CASTELLVI DE ROSANES',
             'CENTELLES')    
        for ii in range(1,20):
            self.listbox.insert(tk.END,"line "+str(ii))

#        self.listbox.insert(tk.END,*cities)
        self.xScroll['command'] = self.listbox.xview
        yScroll['command'] = self.listbox.yview
        
app = Application() 
app.master.title('Sample application') 
app.mainloop()
