import tkinter as tk
from tkinter import ttk

root = tk.Tk()

# the main window is divided into left and right sections,
# and the sidebar is divided into a top and bottom section.
pw = ttk.PanedWindow(orient="horizontal")
sidebar = tk.Frame(pw, width=400, height=400, background="blue")
main = tk.Frame(pw, width=400, height=400, background="black")

# add the paned window to the root
pw.pack(fill="both", expand=True)

# add the sidebar and main area to the main paned window
pw.add(sidebar)
pw.add(main)


root.mainloop()
