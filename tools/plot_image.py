import argparse
import pathlib
import random
import time

import cv2
import serial

import touchscreen


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

    time.sleep(0.5)
    touchscreen.release(sp)
    time.sleep(0.5)
    touchscreen.erase_screen(sp)

    random.shuffle(coords)
    for i, (x, y) in enumerate(coords):
        touchscreen.hold(sp, x, y)
        time.sleep(0.05)
        touchscreen.release(sp)
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
