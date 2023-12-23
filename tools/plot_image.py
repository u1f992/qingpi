import argparse
import pathlib
import random
import time

import cv2
import serial

import qingpi
from qingpi import *


def get_non_transparent_coordinates(img: cv2.Mat) -> list[tuple[int, int]]:
    return [
        (w, h)
        for h, row in enumerate(img)
        for w, pixel in enumerate(row)
        if pixel[-1] != 0
    ]


def main(args: argparse.Namespace) -> None:
    img = cv2.imread(str(args.input), cv2.IMREAD_UNCHANGED)
    assert img.shape == (
        240,
        320,
        4,
    ), "The image must be of size 320x240 and have an alpha channel."
    coords = get_non_transparent_coordinates(img)

    sp = serial.Serial(port=args.port, baudrate=9600)
    hold, release = qingpi.init(sp)

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

    random.shuffle(coords)
    for i, (x, y) in enumerate(coords):
        hold(TouchScreen(x, y))
        time.sleep(0.05)
        release()
        time.sleep(0.05)

        print(i / len(coords))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="plot_image.py",
        description="This script identifies the coordinates of non-transparent pixels in an image.",
    )
    parser.add_argument(
        "input",
        type=pathlib.Path,
        help="Path to the input image. The image must be of size 320x240 and have an alpha channel.",
    )
    parser.add_argument(
        "port",
        type=str,
        help="Serial port name.",
    )
    args = parser.parse_args()
    main(args)
