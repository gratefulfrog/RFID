#!/usr/bin/python3
import ctypes

_rfid = ctypes.CDLL('./lib.so')


_rfid.uart_open.argtypes(ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int)

#char* dev, int speed, int databits, int stopbis,int parity,int readTime)
    
def open():
    global _rfid
    result = _rfid.uart_open('/dev/ttyUSB0',
                              115200, 8, 1, int('N'), 0)
    return result

if __name__ == '__main__':
    #import sys
    print(open)
