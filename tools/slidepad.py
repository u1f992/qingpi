import serial


def hold(sp: serial.Serial, x: int, y: int):
    sp.write([0xAB, 0, 0, 8, x, y, 128, 128, 0, 0, 0])


def release(sp: serial.Serial):
    sp.write([0xAB, 0, 0, 8, 128, 128, 128, 128, 0, 0, 0])
