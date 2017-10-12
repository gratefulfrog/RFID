#!/usr/bin/python3

import ctypes

_printer = ctypes.CDLL('./lib.so')
_printer.printInt.argtypes = ([ctypes.c_int])
_printer.printChar.argtypes = ([ctypes.c_byte])
_printer.printFloat.argtypes = ([ctypes.c_float])
_printer.printString.argtypes = ([ctypes.c_char_p])

# let's avoid the use of wide types
#_printer.printCharC.argtypes = ([ctypes.c_wchar])
#_printer.printStringC.argtypes = ([ctypes.c_wchar_p])

def printInt(numb):
    _printer.printInt(ctypes.c_int(numb))

def printChar(ch):    
    # need to conver the python string into a byte!
    charToSend = ch.encode('utf-8')[0]
    _printer.printChar(ctypes.c_byte(charToSend))

"""
Let's avoid the use of wide types!!
def printCharC(ch):    
    # arg is string of length 1
    _printer.printCharC(ctypes.c_wchar(ch))
"""

def printFloat(fString):
    f = float(fString)
    _printer.printFloat(ctypes.c_float(f))

def printString(ch):    
    # need to conver the python string into a byte!
    charsToSend = ch.encode('utf-8')
    _printer.printString(ctypes.c_char_p(charsToSend))

"""
This does not seem to work...
def printStringC(ch):    
    # arg is string of type w_char*
    toPrint = ctypes.c_wchar_p(ch)
    print(toPrint)
    _printer.printStringC(toPrint.value)
"""
    
if __name__ == '__main__' :
    import sys
    if len(sys.argv) < 2:
        print('usage : $ ./tests.py <args>')
    #printStringC(sys.argv[1][0])
    printString(sys.argv[1])
