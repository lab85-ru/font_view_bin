#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# Просмотр шрифтов бинарного файла (из BIOS например).
#
# v1.0 2023

import pygame as pg
import sys
import numpy as np
#from pylab import *
#from scipy import *
import os

# importing "cmath" for complex number operations 
#import cmath

#===============================================================================
# Read data from file
#===============================================================================
def read_bin_file(filename):
    f=open(filename, 'rb')
    values = np.fromfile(f, dtype="byte")
    return values

#===============================================================================
# Set pixel 
# w - windows
# x - x position
# y - y position
#===============================================================================
def set_pixel(w, x, y, color):
    pg.draw.line(w, (color, color, color), [x, y], [x, y], 1)

#===============================================================================
# Main
#===============================================================================
def main():
    WINDOW_WIDTH = 320
    WINDOW_HEIGHT = 240
    x = 0
    y = 0
    row_count = 0

    if len(sys.argv) == 1 or len(sys.argv) < 3 :
        print("--------------------------------------------------------------------------------")
        print(" view_font.py <row in font> <bin file font>") 
        print(" ") 
        print(" view_font.py 16 8X16WIN1251.FNT") 
        print("--------------------------------------------------------------------------------")
        sys.exit(0)

    row_in_font  = int(sys.argv[1])
    file_font_in = sys.argv[2]

    font_data_from_file = read_bin_file(file_font_in)
    print("Data load OK.")

    if (len(font_data_from_file) != 256 * row_in_font):
        print("ERROR: file size != ", 256 * row_in_font)

    sc = pg.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))

    for ch in range(0, 256):
        for row in range(0, row_in_font):
            p = font_data_from_file[ch * row_in_font + row]
            for bit in range(0, 8):
                pp = p & (0x80 >> bit)
                if pp == 0:
                    color = 0
                else:
                    color = 255

                set_pixel(sc, x + bit, y + row, color)

        x = x + 8
        if x >= WINDOW_WIDTH:
            x = 0
            y = y + row_in_font

            
    pg.display.update()
 
    while 1:
        pg.display.update()
        for i in pg.event.get():
            if i.type == pg.QUIT:
                sys.exit()
        pg.time.delay(40)



    
if __name__ == "__main__":
    main()
