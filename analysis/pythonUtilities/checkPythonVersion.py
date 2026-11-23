#!/bin/python

import sys

def checkPythonVersion():
    PYTHON_VERSION = sys.version_info.major
    #    print("PYTHON_VERSION= ",PYTHON_VERSION)
    if PYTHON_VERSION < 3:
        print("YOU NEED python version 3!!")
        sys.exit()
   
   
