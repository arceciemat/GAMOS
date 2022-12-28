import tkinter as tk
from tkinter import ttk
from tkinter.simpledialog import askfloat
from matplotlib.figure import Figure
from matplotlib.axes   import Axes
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
          
class InteractivePlot(tk.Frame):
    def __init__(self,master,**kwargs):
        super().__init__(master,**kwargs)
        self._figure = Figure(dpi=150)
        self._canvas = FigureCanvasTkAgg(self._figure, master=self)
        buttonframe = tk.Frame(self)
        self._sizebutton = tk.Button(buttonframe,text="Size (in.)", command=self._change_size)
        self._dpibutton = tk.Button(buttonframe,text="DPI", command=self._change_dpi)
        self._axis = self._figure.add_subplot(111)

        # Plot some data just to have something to look at.
        self._axis.plot([0,1,2,3,4,5],[1,1,3,3,5,5],label='Dummy Data')

        self._cwidg = self._canvas.get_tk_widget()
        self._scrx = ttk.Scrollbar(self,orient="horizontal", command=self._cwidg.xview)
        self._scry = ttk.Scrollbar(self,orient="vertical", command=self._cwidg.yview)
        self._cwidg.configure(yscrollcommand=self._scry.set, xscrollcommand=self._scrx.set)

        self._cwidg.bind("<Configure>",self._refresh)

        self._sizebutton.grid(row=0,column=0,sticky='w')
        self._dpibutton.grid(row=0,column=1,sticky='w')
        buttonframe.grid(row=0,column=0,columnspan=2,sticky='W')
        self._cwidg. grid(row=1,column=0,sticky='news')
        self._scrx.  grid(row=2,column=0,sticky='ew')
        self._scry.  grid(row=1,column=1,sticky='ns')

        self.rowconfigure(1,weight=1)
        self.columnconfigure(0,weight=1)

        # Refresh the canvas to show the new plot
        self._canvas.draw()
    
    # Figure size change button callback
    def _change_size(self):
        newsize = askfloat('Size','Input new size in inches')
        if newsize is None:
            return
        w = newsize
        h = newsize/1.8
        self._figure.set_figwidth(w)
        self._figure.set_figheight(h)
        self._refresh()
        print("FIGURE SIZE", self._figure.get_size_inches())

    # Figure DPI change button callback
    def _change_dpi(self):
        newdpi = askfloat('DPI', 'Input a new DPI for the figure')
        if newdpi is None:
            return
        self._figure.set_dpi(newdpi)
        self._refresh()
    
    # Refresh function to make the figure canvas widget display the entire figure
    def _refresh(self,event=None):
        # Get the width and height of the *figure* in pixels
        w = self._figure.get_figwidth()*self._figure.get_dpi()
        h = self._figure.get_figheight()*self._figure.get_dpi()

        # Generate a blank tkinter Event object
        evnt = tk.Event()
        # Set the "width" and "height" values of the event
        evnt.width = w
        evnt.height = h

        # Set the width and height of the canvas widget
        self._cwidg.configure(width=w,height=h)
        self._cwidg.update_idletasks()

        # Pass the generated event object to the FigureCanvasTk.resize() function
        self._canvas.resize(evnt)
        # Set the scroll region to *only* the area of the last canvas item created.
        # Otherwise, the scrollregion will always be the size of the largest iteration
        # of the figure.
        self._cwidg.configure(scrollregion=self._cwidg.bbox(self._cwidg.find_all()[-1]))
        
root = tk.Tk()

plt = InteractivePlot(root,width=400,height=400)

plt.pack(fill=tk.BOTH,expand=True)

root.mainloop()
