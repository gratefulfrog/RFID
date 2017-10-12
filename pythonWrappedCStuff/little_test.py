#!/usr/bin/python3

import ctypes
import RPi.GPIO as GPIO
from time import sleep

buttonPin = 7

_rfid = ctypes.CDLL('./lib.so')

_rfid.openPort.argtypes = ([])
_rfid.showFWVersion.argtypes = ([])
_rfid.showHWVersion.argtypes = ([])
_rfid.showTxPower.argtypes = ([])
_rfid.showReaderID.argtypes = ([])
_rfid.showRSSI.argtypes = ([])
_rfid.showTemp.argtypes = ([])
_rfid.showReadMore.argtypes = ([])
_rfid.doStopReadMore.argtypes = ([])
_rfid.shutDown.argtypes = ([ctypes.c_int])

def openPort():
    return int(openPort())
               
def showFWVersion():
    _rfid.showFWVersion()
    
def showHWVersion():
    _rfid.showFWVersion()

def showTxPower():
    _rfid.showTxPower()
    
def showReaderID():
    _rfid.showReaderID()

def showRSSI():
    _rfid.showRSSI()
    
def showTemp():
    _rfid.showTemp()
    
def showReadMore():
    return _rfid.showReadMore()
    
def doStopReadMore():
    _rfid.doStopReadMore()

def shutDown(fd):
    _rfid.shutDown(ctypes.c_int(fd))

def run():
    GPIO.setup(buttonPin, GPIO.IN)
    try:
        uartFD = openPort()
        sleep(0.5)
        
        showFWVersion()
        sleep(0.5)
        
        showHWVersion()
        sleep(0.5)
        
        showTxPower()
        sleep(0.5)
        
        showReaderID()
        sleep(0.5)
        
        showRSSI()
        sleep(0.5)
    
        showTemp()
        sleep(0.5)

        while True:
            if GPIO.input(buttonPin):
                if showReadMore() > 0:
                    sleep(0.5)
                    doStopReadMore()
                    sleep(0.5)
    except Excpetion as e:
        pass
    finally:
        shutdown(uartFD)

    
if __name__ == '__main__' :
    import sys
    if len(sys.argv) < 2:
        print('usage : $ ./tests.py <args>')
    #printStringC(sys.argv[1][0])
    printString(sys.argv[1])
