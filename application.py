#									
# 
# application.py - python application for Ground-Breaking Next-Gen Smart Pet Door
#  
# 
# Permission to use, copy, modify, and distribute this software and its
# documentation for any purpose, without fee, and without written agreement is
# hereby granted, provided that the above copyright notice and the following
# two paragraphs appear in all copies of this software.
# 
# IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
# ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
# DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
# EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
# OF THE POSSIBILITY OF SUCH DAMAGE.
#  
# THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
# PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
# THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
# SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
# 
# Author:	    Matthew Wei
# Version:	    2.0
# Creation Date:   Mon Oct  30 2023
# Filename:	    application.py
# History:
#   Version	1.0
#		Initial code for application (no WebServer)
#	Version	2.0
#		Redesigned with implementation of WebServer
#
#   Blue wire left for motor
#   Red wire left for latch
#
#   References:
#   https://www.datacamp.com/tutorial/making-http-requests-in-python
#

from tkinter import *
import webbrowser
import requests

new = 1
url1 = "http://172.16.113.201"
url2 = "http://172.16.125.198"
url_post = "http://172.16.112.15/button"

#***********************************************************
#
# Functions
#
#***********************************************************

#
# opencamera1()
# Function: Open URL that camera 1 streams to
#

def opencamera1():
    webbrowser.open(url1, new=new)

#
# opencamera2()
# Function: Open URL that camera 2 streams to
#

def opencamera2():
    webbrowser.open(url2, new=new)

#
# buttonpress()
# Function: Create a get request at url
#

def buttonpress():
    requests.get(url_post)


#***********************************************************
#
# Main Code
#
#***********************************************************

root = Tk() # Open application

# Create buttons with functions and place them
button1 = Button(root, text = "Open Front Camera", command=opencamera1)
button2 = Button(root, text = "Open Back Camera", command=opencamera2)
button3 = Button(text="Open/Close Door", command=buttonpress)
button1.pack()
button2.pack()
button3.pack()

root.mainloop()