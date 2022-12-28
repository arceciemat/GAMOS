## CHANGE FIGURE SIZE
import math
import sys
if sys.version_info[0] < 3:
  from Tkinter import Tk, Button, Frame, Canvas, Scrollbar
  import Tkconstants
else:
  from tkinter import Tk, Button, Frame, Canvas, Scrollbar
  import tkinter.constants as Tkconstants

import matplotlib
from matplotlib import pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import pprint, inspect

frame = None
canvas = None
ax = None

def printBboxes(label=""):
  global canvas, mplCanvas, interior, interior_id, cwid, figure
  print("  "+label,
    "canvas.bbox:", canvas.bbox(Tkconstants.ALL),
    "mplCanvas.bbox:", mplCanvas.bbox(Tkconstants.ALL),
    "subplotpars:", figure.subplotpars.__dict__ )

def addScrollingFigure(figure, frame):
  global canvas, mplCanvas, interior, interior_id, cwid
  # set up a canvas with scrollbars
  canvas = Canvas(frame)
  canvas.grid(row=1, column=1, sticky=Tkconstants.NSEW)

  xScrollbar = Scrollbar(frame, orient=Tkconstants.HORIZONTAL)
  yScrollbar = Scrollbar(frame)

  xScrollbar.grid(row=2, column=1, sticky=Tkconstants.EW)
  yScrollbar.grid(row=1, column=2, sticky=Tkconstants.NS)

  canvas.config(xscrollcommand=xScrollbar.set)
  xScrollbar.config(command=canvas.xview)
  canvas.config(yscrollcommand=yScrollbar.set)
  yScrollbar.config(command=canvas.yview)

  # plug in the figure
  figAgg = FigureCanvasTkAgg(figure, canvas)
  mplCanvas = figAgg.get_tk_widget()

  # and connect figure with scrolling region
  cwid = canvas.create_window(0, 0, window=mplCanvas, anchor=Tkconstants.NW)
  printBboxes("Init")
  changeSize(figure, 1)

def changeSize(figure, factor):
  global canvas, mplCanvas, interior, interior_id, frame, cwid
  oldSize = figure.get_size_inches()
  print("old size is", oldSize)
  figure.set_size_inches([factor * s for s in oldSize])
  wi,hi = [i*figure.dpi for i in figure.get_size_inches()]
  print("new size is", figure.get_size_inches())
  print("new size pixels: ", wi,hi)
  mplCanvas.config(width=wi, height=hi) ; printBboxes("A")
  canvas.itemconfigure(cwid, width=wi, height=hi) ; printBboxes("B")
  canvas.config(scrollregion=canvas.bbox(Tkconstants.ALL),width=200,height=200)
  tz.set_fontsize(tz.get_fontsize()*factor)
  for item in ([ax.title, ax.xaxis.label, ax.yaxis.label] +
               ax.get_xticklabels() + ax.get_yticklabels()):
    item.set_fontsize(item.get_fontsize()*factor)
  ax.xaxis.labelpad = ax.xaxis.labelpad*factor
  ax.yaxis.labelpad = ax.yaxis.labelpad*factor
  #figure.tight_layout() # matplotlib > 1.1.1
  figure.subplots_adjust(left=0.2, bottom=0.15, top=0.86)
  figure.canvas.draw() ; printBboxes("C")
  print()

if __name__ == "__main__":
  global root, figure
  root = Tk()
  root.rowconfigure(1, weight=1)
  root.columnconfigure(1, weight=1)

  frame = Frame(root)
  frame.grid(column=1, row=1, sticky=Tkconstants.NSEW)
  frame.rowconfigure(1, weight=1)
  frame.columnconfigure(1, weight=1)

  figure = plt.figure(dpi=150, figsize=(4, 4))
  ax = figure.add_subplot(111)
  ax.plot(range(10), [math.sin(x) for x in range(10)])
  #tz = figure.text(0.5,0.975,'The master title',horizontalalignment='center', verticalalignment='top')
  tz = figure.suptitle('The master title')

  ax.set_title('Tk embedding')
  ax.set_xlabel('X axis label')
  ax.set_ylabel('Y label')
  print(tz.get_fontsize()) # 12.0
  print(ax.title.get_fontsize(), ax.xaxis.label.get_fontsize(), ax.yaxis.label.get_fontsize()) # 14.4 12.0 12.0

  addScrollingFigure(figure, frame)

  buttonFrame = Frame(root)
  buttonFrame.grid(row=1, column=2, sticky=Tkconstants.NS)
  biggerButton = Button(buttonFrame, text="larger",
                        command=lambda : changeSize(figure, 1.2))
  biggerButton.grid(column=1, row=1)
  smallerButton = Button(buttonFrame, text="smaller",
                         command=lambda : changeSize(figure, 0.833))
  smallerButton.grid(column=1, row=2)
  qButton = Button(buttonFrame, text="quit",
                         command=lambda :  sys.exit(0))
  qButton.grid(column=1, row=3)

  root.mainloop()
