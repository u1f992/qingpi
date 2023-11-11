import enum
import tkinter as tk
import typing


class ButtonID(enum.Enum):
    A = enum.auto()
    B = enum.auto()
    X = enum.auto()
    Y = enum.auto()
    L = enum.auto()
    R = enum.auto()
    ZL = enum.auto()
    ZR = enum.auto()
    START = enum.auto()
    SELECT = enum.auto()
    HOME = enum.auto()
    POWER = enum.auto()
    WIRELESS = enum.auto()


def bind_button_controller(
    master: tk.Tk, on_button_change: typing.Callable[[ButtonID, bool], None]
):
    prev = {
        ButtonID.A: False,
        ButtonID.B: False,
        ButtonID.X: False,
        ButtonID.Y: False,
        ButtonID.L: False,
        ButtonID.R: False,
        ButtonID.ZL: False,
        ButtonID.ZR: False,
        ButtonID.START: False,
        ButtonID.SELECT: False,
        ButtonID.HOME: False,
        ButtonID.POWER: False,
        ButtonID.WIRELESS: False,
    }

    def _on_keyevent(press: bool):
        def _inner(event: "tk.Event[tk.Misc]"):
            match event.char:
                case "a":
                    id = ButtonID.A
                case "s":
                    id = ButtonID.B
                case "z":
                    id = ButtonID.X
                case "x":
                    id = ButtonID.Y
                case "q":
                    id = ButtonID.L
                case "w":
                    id = ButtonID.R
                case "1":
                    id = ButtonID.ZL
                case "2":
                    id = ButtonID.ZR
                case "m":
                    id = ButtonID.START
                case "n":
                    id = ButtonID.SELECT
                case "b":
                    id = ButtonID.HOME
                case "v":
                    id = ButtonID.POWER
                case "p":
                    id = ButtonID.WIRELESS
                case _:
                    return

            if prev[id] == press:
                return

            on_button_change(id, press)
            prev[id] = press

        return _inner

    master.bind("<KeyPress>", _on_keyevent(True), add=True)
    master.bind("<KeyRelease>", _on_keyevent(False), add=True)
