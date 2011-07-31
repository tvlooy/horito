#!/usr/bin/env python
"""
    Linux x86

    Author: Tom Van Looy <tom$ctors,net>
    License: http://ctors.net/isc_license.txt

    This program is a GUI frontend to send text or bitmaps over RS232 to the
    MikroE UNI-DS 3 development board. The board displays the data on a 128x64
    GLCD display.

    This program depends on the binary "createbitmap" that needs to be compiled
    separately. The source of this program lives in:
    https://github.com/tvlooy/horito/blob/master/7_bmpToGlcd/createBitmap.c
"""

import pygtk
pygtk.require('2.0')
import gtk
import pango
import time
import serial

"""
    Program main class
"""
class Py2rs232:

    """
        Close window, exit program
    """
    def delete_event(self, widget, event, data=None):
        gtk.main_quit()
        return False

    """
        Send text over RS232
    """
    def send_text_action(self, widget, data=None):
        if self.init_serial() == False:
            return

        # 27 = esc, ends transmission
        # 13 = carriage return, starts new line
        self.ser.write(chr(1) +
            self.txtRow1.get_text() + chr(13) +
            self.txtRow2.get_text() + chr(13) +
            self.txtRow3.get_text() + chr(13) +
            self.txtRow4.get_text() + chr(13) +
            self.txtRow5.get_text() + chr(13) +
            self.txtRow6.get_text() + chr(13) +
            self.txtRow7.get_text() + chr(13) +
            self.txtRow8.get_text() + chr(27)
        )
        self.ser.close()

    """
        Clear text input boxes
    """
    def clear_text_action(self, widget, data=None):
        self.txtRow1.set_text('')
        self.txtRow2.set_text('')
        self.txtRow3.set_text('')
        self.txtRow4.set_text('')
        self.txtRow5.set_text('')
        self.txtRow6.set_text('')
        self.txtRow7.set_text('')
        self.txtRow8.set_text('')

    """
        Send bitmap over RS232
    """
    def send_bitmap_action(self, widget, data=None):
        from subprocess import Popen, PIPE

        proc = Popen(['./createbitmap', self.txtFilename.get_text(), 'python'], stdout=PIPE, stderr=PIPE)
        return_code = proc.wait()
        if return_code == 0:
            image = str.split(proc.stdout.read(), ',')
        else:
            if return_code == 3 or return_code == 6:
                errorText = "Given file could not be converted."
            else:
                errorText = "Error calling conversion program."

            message = gtk.MessageDialog(self.window, 
                gtk.DIALOG_DESTROY_WITH_PARENT, gtk.MESSAGE_INFO, 
                gtk.BUTTONS_CLOSE, errorText)
            message.run()
            message.destroy()
            return

        if self.init_serial() == False:
            return

        self.ser.write(chr(2))
        for val in image:
            if val != ' ':
                self.ser.write(chr(int(val)))
        self.ser.close()

    """
        Clear selected bitmap
    """
    def clear_bitmap_action(self, widget, data=None):
        self.txtFilename.set_text('')

    """
        Select bitmap show file browser
    """
    def select_bitmap_action(self, widget, data=None):
        self.fileSelect = gtk.FileChooserDialog("Select file")
        self.fileSelect.set_modal(True)
        self.fileSelect.connect("file-activated", self.file_ok_sel)
        selectButton = self.fileSelect.add_button('Select', gtk.RESPONSE_NONE)
        selectButton.connect("pressed", self.file_ok_sel)
        self.fileSelect.show()

    """
        File chosen with file browser
    """
    def file_ok_sel(self, w):
        self.txtFilename.set_text(self.fileSelect.get_filename())
        self.fileSelect.destroy()

    """
        Initialize serial port, show message box on error
    """
    def init_serial(self):
        try:
            self.ser = serial.Serial(
                port = '/dev/ttyUSB0',
                baudrate = 9600,
                parity = serial.PARITY_NONE,
                stopbits = serial.STOPBITS_ONE,
                bytesize = serial.EIGHTBITS
            )

            self.ser.open()
        except serial.serialutil.SerialException:
            message = gtk.MessageDialog(self.window, 
                gtk.DIALOG_DESTROY_WITH_PARENT, gtk.MESSAGE_INFO, 
                gtk.BUTTONS_CLOSE, "Could not connect to serial port.")
            message.run()
            message.destroy()

            return False

        return True

    """
        Py2rs232 class constructor
    """
    def __init__(self):
        """ Main window components """
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.set_title("Send to GLCD")
        self.window.resize(225, 325)

        self.window.connect("delete_event", self.delete_event)
        self.window.set_border_width(10)

        self.boxSendText = gtk.VBox(False, 0)
        self.boxSendBitmap = gtk.VBox(False, 0)

        self.tabPane = gtk.Notebook()
        self.tabPane.insert_page(self.boxSendText, gtk.Label("Send text"), gtk.POS_TOP)
        self.tabPane.insert_page(self.boxSendBitmap, gtk.Label("Send bitmap"), gtk.POS_TOP)
        self.window.add(self.tabPane)

        """ Input fields for sendText """
        fontMonoSpaced = pango.FontDescription('monospace')

        self.txtRow1 = gtk.Entry(max = 21)
        self.txtRow1.modify_font(fontMonoSpaced)
        self.boxSendText.pack_start(self.txtRow1, False, False, 0)
        self.txtRow1.show()

        self.txtRow2 = gtk.Entry(max = 21)
        self.txtRow2.modify_font(fontMonoSpaced)
        self.boxSendText.pack_start(self.txtRow2, False, False, 0)
        self.txtRow2.show()

        self.txtRow3 = gtk.Entry(max = 21)
        self.txtRow3.modify_font(fontMonoSpaced)
        self.boxSendText.pack_start(self.txtRow3, False, False, 0)
        self.txtRow3.show()

        self.txtRow4 = gtk.Entry(max = 21)
        self.txtRow4.modify_font(fontMonoSpaced)
        self.boxSendText.pack_start(self.txtRow4, False, False, 0)
        self.txtRow4.show()

        self.txtRow5 = gtk.Entry(max = 21)
        self.txtRow5.modify_font(fontMonoSpaced)
        self.boxSendText.pack_start(self.txtRow5, False, False, 0)
        self.txtRow5.show()

        self.txtRow6 = gtk.Entry(max = 21)
        self.txtRow6.modify_font(fontMonoSpaced)
        self.boxSendText.pack_start(self.txtRow6, False, False, 0)
        self.txtRow6.show()

        self.txtRow7 = gtk.Entry(max = 21)
        self.txtRow7.modify_font(fontMonoSpaced)
        self.boxSendText.pack_start(self.txtRow7, False, False, 0)
        self.txtRow7.show()

        self.txtRow8 = gtk.Entry(max = 21)
        self.txtRow8.modify_font(fontMonoSpaced)
        self.boxSendText.pack_start(self.txtRow8, False, False, 0)
        self.txtRow8.show()

        """ Send and clear button for sendText """
        self.btnSendText = gtk.Button("Send")
        self.boxSendText.pack_start(self.btnSendText, False, False, 0)
        self.btnSendText.connect("clicked", self.send_text_action)
        self.btnSendText.show()

        self.btnClearText = gtk.Button("Clear")
        self.boxSendText.pack_start(self.btnClearText, False, False, 0)
        self.btnClearText.connect("clicked", self.clear_text_action)
        self.btnClearText.show()

        """ Input fields for sendBitmap """
        self.txtFilename = gtk.Entry()
        self.txtFilename.set_editable(False)
        self.boxSendBitmap.pack_start(self.txtFilename, False, False, 0)
        self.txtFilename.show()

        self.btnSelectFile = gtk.Button("Select")
        self.boxSendBitmap.pack_start(self.btnSelectFile, False, False, 0)
        self.btnSelectFile.connect("clicked", self.select_bitmap_action)
        self.btnSelectFile.show()

        self.btnSendFile = gtk.Button("Send")
        self.boxSendBitmap.pack_start(self.btnSendFile, False, False, 0)
        self.btnSendFile.connect("clicked", self.send_bitmap_action)
        self.btnSendFile.show()

        self.btnClearFile = gtk.Button("Clear")
        self.boxSendBitmap.pack_start(self.btnClearFile, False, False, 0)
        self.btnClearFile.connect("clicked", self.clear_bitmap_action)
        self.btnClearFile.show()

        """ Show window """
        self.boxSendText.show()
        self.boxSendBitmap.show()
        self.tabPane.show()
        self.window.show()

"""
    Program main entry point
"""
if __name__ == "__main__":
    program = Py2rs232()
    gtk.main()

