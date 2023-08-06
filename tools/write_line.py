import argparse
import time

import serial

import touchscreen


def main(args: argparse.Namespace):
    sp = serial.Serial(port=args.port, baudrate=9600)

    time.sleep(0.5)
    touchscreen.release(sp)
    time.sleep(0.5)
    touchscreen.erase_screen(sp)

    for x in range(1, 321):
        touchscreen.hold(sp, x, 120)
        time.sleep(0.025)
    touchscreen.release(sp)
    time.sleep(5)

    for y in range(1, 241):
        touchscreen.hold(sp, 160, y)
        time.sleep(0.025)
    touchscreen.release(sp)
    time.sleep(5)

    for x in range(1, 321):
        touchscreen.hold(sp, x, 1)
        time.sleep(0.025)
    touchscreen.release(sp)
    time.sleep(5)

    for y in range(1, 241):
        touchscreen.hold(sp, 1, y)
        time.sleep(0.025)
    touchscreen.release(sp)
    time.sleep(5)

    for y in range(1, 241):
        touchscreen.hold(sp, 320, y)
        time.sleep(0.025)
    touchscreen.release(sp)
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
