import time

import serial


def hold(sp: serial.Serial, x: int, y: int):
    sp.write([0xAB, 0, 0, 8, 128, 128, 128, 128, (x & 0xFF), (x >> 8), y])


def release(sp: serial.Serial):
    sp.write([0xAB, 0, 0, 8, 128, 128, 128, 128, 0, 0, 0])


def erase_screen(sp: serial.Serial):
    hold(sp, 300, 220)
    time.sleep(0.5)
    release(sp)
    time.sleep(1.5)

    hold(sp, 300, 180)
    time.sleep(0.5)
    release(sp)
    time.sleep(1.5)

    hold(sp, 270, 200)
    time.sleep(0.5)
    release(sp)
    time.sleep(5)
