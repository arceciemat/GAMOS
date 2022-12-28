from tkinter import *
from tkinter import scrolledtext

master_window = Tk()

# Parent widget for the buttons
buttons_frame = Frame(master_window)
buttons_frame.grid(row=0, column=0, sticky=W+E)    

btn_Image = Button(buttons_frame, text='Image')
btn_Image.grid(row=0, column=0, padx=(10), pady=10)

btn_File = Button(buttons_frame, text='File')
btn_File.grid(row=0, column=1, padx=(10), pady=10)

btn_Folder = Button(buttons_frame, text='Folder')
btn_Folder.grid(row=0, column=2, padx=(10), pady=10)

# Group1 Frame ----------------------------------------------------
group1 = LabelFrame(master_window, text="Text Box", padx=5, pady=5)
group1.grid(row=1, column=0, columnspan=3, padx=10, pady=10, sticky=E+W+N+S)

master_window.columnconfigure(0, weight=1)
master_window.rowconfigure(1, weight=1)

group1.rowconfigure(0, weight=1)
group1.columnconfigure(0, weight=1)

# Create the textbox
txtbox = scrolledtext.ScrolledText(group1, width=40, height=10)
txtbox.grid(row=0, column=0,   sticky=E+W+N+S)

mainloop()
