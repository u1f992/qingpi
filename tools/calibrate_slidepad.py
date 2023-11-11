import argparse
import math
import time

import serial

import ncm
from ncm import *


def approximate_circle(center_x: float, center_y: float, radius: float):
    return [
        (x, y)
        for x in range(256)
        for y in range(256)
        if abs((((x - center_x) ** 2 + (y - center_y) ** 2) ** 0.5) - radius) < 0.5
    ]


def sort_points_clockwise(
    points: list[tuple[float, float]], center_x: float, center_y: float
) -> list[tuple[float, float]]:
    # Calculate the angle for each point and sort based on the angle
    sorted_points = sorted(
        points,
        key=lambda p: math.atan2(p[1] - center_y, p[0] - center_x),
        reverse=True,  # Sort in clockwise direction
    )
    return sorted_points


def main(args: argparse.Namespace):
    sp = serial.Serial(port=args.port, baudrate=9600)
    hold, release = ncm.init(sp)

    time.sleep(2)

    try:
        release()
        time.sleep(0.5)
        for x in range(127, -1, -1):
            hold(LeftSlidePad(x, 128))
            time.sleep(0.025)
        release()
        time.sleep(0.5)

        release()
        time.sleep(0.5)
        for x in range(129, 256):
            hold(LeftSlidePad(x, 128))
            time.sleep(0.025)
        release()
        time.sleep(0.5)

        release()
        time.sleep(0.5)
        for y in range(127, -1, -1):
            hold(LeftSlidePad(128, y))
            time.sleep(0.025)
        release()
        time.sleep(0.5)

        release()
        time.sleep(0.5)
        for y in range(129, 256):
            hold(LeftSlidePad(128, y))
            time.sleep(0.025)
        release()
        time.sleep(0.5)

        # Draw circle
        points = sort_points_clockwise(
            approximate_circle(127.5, 127.5, 127.5), 127.5, 127.5
        )
        for x, y in points:
            hold(LeftSlidePad(x, y))
            time.sleep(0.01)
        release()
        time.sleep(0.5)

    finally:
        release()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="calibrate_slidepad.py",
        description="",
    )
    parser.add_argument(
        "port",
        type=str,
        help="Serial port name.",
    )
    args = parser.parse_args()
    main(args)
