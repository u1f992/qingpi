import argparse
import base64
import tkinter as tk

import serial

import qingpi
from qingpi import *  # type: ignore

from qingpi.gui.gui import *
from qingpi.gui.button import *
from qingpi.gui.hat import *
from qingpi.gui.slidepad import *
from qingpi.gui.touchscreen import *
from qingpi.gui.icon import ICON


def on_button_change(hold, release):
    def _on_button_change(id: ButtonID, pressed: bool):
        match id:
            case ButtonID.A:
                op = A
            case ButtonID.B:
                op = B
            case ButtonID.X:
                op = X
            case ButtonID.Y:
                op = Y
            case ButtonID.L:
                op = L
            case ButtonID.R:
                op = R
            case ButtonID.ZL:
                print(f"[NotImplemented] hold/release(ZL)")
                return
            case ButtonID.ZR:
                print(f"[NotImplemented] hold/release(ZR)")
                return
            case ButtonID.START:
                op = START
            case ButtonID.SELECT:
                op = SELECT
            case ButtonID.HOME:
                op = HOME
            case ButtonID.POWER:
                op = POWER
            case ButtonID.WIRELESS:
                op = WIRELESS

        if pressed:
            hold(op)
        else:
            release(op)

    return _on_button_change


def on_hat_change(hold, release):
    def _on_hat_change(hat: HatState):
        match hat:
            case HatState.UP:
                hold(UP)
            case HatState.UPRIGHT:
                hold(UPRIGHT)
            case HatState.RIGHT:
                hold(RIGHT)
            case HatState.DOWNRIGHT:
                hold(DOWNRIGHT)
            case HatState.DOWN:
                hold(DOWN)
            case HatState.DOWNLEFT:
                hold(DOWNLEFT)
            case HatState.LEFT:
                hold(LEFT)
            case HatState.UPLEFT:
                hold(UPLEFT)
            case HatState.NEUTRAL:
                release(Hat)

    return _on_hat_change


def on_slidepad_change(hold, _):
    def _on_slidepad_change(x: int, y: int):
        hold(SlidePad(x, y))

    return _on_slidepad_change


def on_ext_slidepad_change(_0, _1):
    def _on_ext_slidepad_change(x: int, y: int):
        print(f"[NotImplemented] hold(ExtSlidePad({x}, {y}))")

    return _on_ext_slidepad_change


def on_touchscreen_hold(hold, _):
    def _on_touchscreen_hold(x: int, y: int):
        hold(TouchScreen(x, y))

    return _on_touchscreen_hold


def on_touchscreen_release(_, release):
    def _on_touchscreen_release():
        release(TouchScreen)

    return _on_touchscreen_release


def main() -> None:
    parser = argparse.ArgumentParser(
        prog="__main__.py",
        description="GUI application to control Qingpi with transparent Window and Citra compatible keyboard operation.",
    )
    parser.add_argument(
        "port",
        type=str,
        help="Serial port name.",
    )
    parser.add_argument("--debug", action="store_true")
    args = parser.parse_args()

    ser = serial.Serial(args.port)
    if not args.debug:
        hold, release = qingpi.init(ser)
    else:

        class DebugWriter:
            def __init__(self, ser):
                self.__ser = ser

            def write(self, data):
                print(list(data))
                return self.__ser.write(data)

        hold, release = qingpi.init(DebugWriter(ser))

    root = tk.Tk()
    root.title(f"qingpictrl - {args.port}")

    touchscreen_size = TouchScreenSize(width=320, height=240)

    root.attributes("-topmost", True)
    make_window_be_resized_only_by_keypress(
        root, (touchscreen_size.width, touchscreen_size.height)
    )

    make_window_transparent(root)
    filler = create_transparent_filler(root)

    bind_button_controller(root, on_button_change(hold, release))
    bind_hat_controller(root, on_hat_change(hold, release))
    bind_slidepad_controller(root, on_slidepad_change(hold, release))
    bind_ext_slidepad_controller(root, on_ext_slidepad_change(hold, release))
    bind_touchscreen_controller(
        root,
        filler,
        touchscreen_size,
        on_touchscreen_hold(hold, release),
        on_touchscreen_release(hold, release),
    )
    
    # Kind of a bit buggy.
    # It won't work properly unless write it here.
    iconphoto = tk.PhotoImage(data=base64.b64decode(ICON))
    root.iconphoto(False, iconphoto)

    root.mainloop()


if __name__ == "__main__":
    main()
