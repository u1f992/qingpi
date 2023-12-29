import enum
import tkinter as tk
import typing

from .util import *


class HatState(enum.Enum):
    UP = enum.auto()
    UPRIGHT = enum.auto()
    RIGHT = enum.auto()
    DOWNRIGHT = enum.auto()
    DOWN = enum.auto()
    DOWNLEFT = enum.auto()
    LEFT = enum.auto()
    UPLEFT = enum.auto()
    NEUTRAL = enum.auto()


def bind_hat_controller(
    master: tk.Tk, on_hat_change: typing.Callable[[HatState], None]
):
    keys_state = [False, False, False, False]  # up, right, down, left
    keys_to_hat: dict[int, HatState] = {
        0b0000: HatState.NEUTRAL,
        0b0001: HatState.LEFT,
        0b0010: HatState.DOWN,
        0b0011: HatState.DOWNLEFT,
        0b0100: HatState.RIGHT,
        0b0101: HatState.NEUTRAL,
        0b0110: HatState.DOWNRIGHT,
        0b0111: HatState.DOWN,
        0b1000: HatState.UP,
        0b1001: HatState.UPLEFT,
        0b1010: HatState.NEUTRAL,
        0b1011: HatState.LEFT,
        0b1100: HatState.UPRIGHT,
        0b1101: HatState.UP,
        0b1110: HatState.RIGHT,
        0b1111: HatState.NEUTRAL,
    }
    prev: int = convert_bool_array_to_int(keys_state)

    def _on_hat_change():
        nonlocal prev
        next = convert_bool_array_to_int(keys_state)
        if prev == next:
            return

        on_hat_change(keys_to_hat[next])
        prev = next

    def _on_keypress(event: "tk.Event[tk.Misc]"):
        match event.char:
            case "t":
                keys_state[0] = True
            case "h":
                keys_state[1] = True
            case "g":
                keys_state[2] = True
            case "f":
                keys_state[3] = True
        _on_hat_change()

    def _on_keyrelease(event: "tk.Event[tk.Misc]"):
        match event.char:
            case "t":
                keys_state[0] = False
            case "h":
                keys_state[1] = False
            case "g":
                keys_state[2] = False
            case "f":
                keys_state[3] = False
        _on_hat_change()

    master.bind("<KeyPress>", _on_keypress, add=True)
    master.bind("<KeyRelease>", _on_keyrelease, add=True)
