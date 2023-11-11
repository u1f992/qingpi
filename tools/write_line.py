import argparse
import time

import serial

import ncm
from ncm import *


def main(args: argparse.Namespace):
    sp = serial.Serial(port=args.port, baudrate=9600)
    hold, release = ncm.init(sp)

    def erase_screen():
        hold(TouchScreen(300, 220))
        time.sleep(0.5)
        release()
        time.sleep(1.5)

        hold(TouchScreen(300, 180))
        time.sleep(0.5)
        release()
        time.sleep(1.5)

        hold(TouchScreen(270, 200))
        time.sleep(0.5)
        release()
        time.sleep(5)

    time.sleep(0.5)
    release()
    time.sleep(0.5)
    erase_screen()

    for x in range(1, 321):
        hold(TouchScreen(x, 120))
        time.sleep(0.025)
    release()
    time.sleep(5)

    for y in range(1, 241):
        hold(TouchScreen(160, y))
        time.sleep(0.025)
    release()
    time.sleep(5)

    for x in range(1, 321):
        hold(TouchScreen(x, 1))
        time.sleep(0.025)
    release()
    time.sleep(5)

    for y in range(1, 241):
        hold(TouchScreen(1, y))
        time.sleep(0.025)
    release()
    time.sleep(5)

    for y in range(1, 241):
        hold(TouchScreen(320, y))
        time.sleep(0.025)
    release()
    time.sleep(5)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="write_line.py",
        description="",
    )
    parser.add_argument(
        "port",
        type=str,
        help="Serial port name.",
    )
    args = parser.parse_args()
    main(args)
