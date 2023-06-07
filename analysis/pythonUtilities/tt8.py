import tkinter as tk
from tkinter import ttk

root = tk.Tk()

# the main window is divided into left and right sections,
# and the sidebar is divided into a top and bottom section.
pw = ttk.PanedWindow(orient="horizontal")
sidebar = ttk.PanedWindow(pw, orient="vertical")
main = tk.Frame(pw, width=400, height=400, background="black")
histList = tk.Listbox(main,height=20,width=30)
histList.grid(column=0,row=0)
sidebar_top = tk.Frame(sidebar, width=200, height=200, background="gray")
sidebar_bottom = tk.Frame(sidebar, width=200, height=200, background="white")

# add the paned window to the root
pw.pack(fill="both", expand=True)

# add the sidebar and main area to the main paned window
pw.add(sidebar)
pw.add(main)

# add the top and bottom to the sidebar
sidebar.add(sidebar_top)
sidebar.add(sidebar_bottom)

root.mainloop()
