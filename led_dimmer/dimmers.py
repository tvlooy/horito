#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Author: Tom Van Looy <tom$ctors,net>
# License: http://ctors.net/isc_license.txt
#
# Draw some sliders that control the brightness of a LED.

from Tkinter import *

class App:

   def __init__(self, master):

       frame = Frame(master)
       frame.pack()

       self.canvas1 = Canvas(frame)
       self.canvas1.grid(row=0, column=1)

       self.scale1 = Scale(frame, command=self.say_value)
       self.scale1.grid(row=0, column=0)

   def say_value(self, value):
       value = int(value) * 2.55;
       print "hi " + str(value)
       value = int(value)
       color = "#%02x%02x%02x" % (value, value, value)
       item = self.canvas1.create_oval(0, 0, 100, 100, fill=color)

root = Tk()
app = App(root)
root.mainloop()

