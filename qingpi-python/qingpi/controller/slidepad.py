import tkinter as tk
import typing

from .util import *


def bind_slidepad_controller(
    master: tk.Tk, on_slidepad_change: typing.Callable[[int, int], None]
):
    arrow_keys_state = [False, False, False, False]
    arrow_keys_to_slidepad: dict[int, tuple[int, int]] = {
        0b0000: (128, 128),
        0b0001: (0, 128),
        0b0010: (128, 255),
        0b0011: (0, 255),
        0b0100: (255, 128),
        0b0101: (128, 128),  # Neutral in case of collision on same axis
        0b0110: (255, 255),
        0b0111: (128, 255),
        0b1000: (128, 0),
        0b1001: (0, 0),
        0b1010: (128, 128),
        0b1011: (0, 128),
        0b1100: (255, 0),
        0b1101: (128, 0),
        0b1110: (255, 128),
        0b1111: (128, 128),
    }
    prev: int = convert_bool_array_to_int(arrow_keys_state)

    def _on_slidepad_change():
        nonlocal prev
        next = convert_bool_array_to_int(arrow_keys_state)
        if prev == next:
            return

        on_slidepad_change(*arrow_keys_to_slidepad[next])
        prev = next

    def _on_up_press(_: "tk.Event[tk.Misc]"):
        arrow_keys_state[0] = True
        _on_slidepad_change()

    def _on_up_release(_: "tk.Event[tk.Misc]"):
        arrow_keys_state[0] = False
        _on_slidepad_change()

    def _on_right_press(_: "tk.Event[tk.Misc]"):
        arrow_keys_state[1] = True
        _on_slidepad_change()

    def _on_right_release(_: "tk.Event[tk.Misc]"):
        arrow_keys_state[1] = False
        _on_slidepad_change()

    def _on_down_press(_: "tk.Event[tk.Misc]"):
        arrow_keys_state[2] = True
        _on_slidepad_change()

    def _on_down_release(_: "tk.Event[tk.Misc]"):
        arrow_keys_state[2] = False
        _on_slidepad_change()

    def _on_left_press(_: "tk.Event[tk.Misc]"):
        arrow_keys_state[3] = True
        _on_slidepad_change()

    def _on_left_release(_: "tk.Event[tk.Misc]"):
        arrow_keys_state[3] = False
        _on_slidepad_change()

    master.bind("<Left>", _on_left_press, add=True)
    master.bind("<KeyRelease-Left>", _on_left_release, add=True)
    master.bind("<Right>", _on_right_press, add=True)
    master.bind("<KeyRelease-Right>", _on_right_release, add=True)
    master.bind("<Up>", _on_up_press, add=True)
    master.bind("<KeyRelease-Up>", _on_up_release, add=True)
    master.bind("<Down>", _on_down_press, add=True)
    master.bind("<KeyRelease-Down>", _on_down_release, add=True)


def bind_ext_slidepad_controller(
    master: tk.Tk, on_ext_slidepad_change: typing.Callable[[int, int], None]
):
    keys_state = [False, False, False, False]
    keys_to_slidepad: dict[int, tuple[int, int]] = {
        0b0000: (128, 128),
        0b0001: (0, 128),
        0b0010: (128, 255),
        0b0011: (0, 255),
        0b0100: (255, 128),
        0b0101: (128, 128),
        0b0110: (255, 255),
        0b0111: (128, 255),
        0b1000: (128, 0),
        0b1001: (0, 0),
        0b1010: (128, 128),
        0b1011: (0, 128),
        0b1100: (255, 0),
        0b1101: (128, 0),
        0b1110: (255, 128),
        0b1111: (128, 128),
    }
    prev: int = convert_bool_array_to_int(keys_state)

    def _on_slidepad_change():
        nonlocal prev
        next = convert_bool_array_to_int(keys_state)
        if prev == next:
            return

        on_ext_slidepad_change(*keys_to_slidepad[next])
        prev = next

    def _on_keypress(event: "tk.Event[tk.Misc]"):
        match event.char:
            case "i":
                keys_state[0] = True
            case "l":
                keys_state[1] = True
            case "k":
                keys_state[2] = True
            case "j":
                keys_state[3] = True
        _on_slidepad_change()

    def _on_keyrelease(event: "tk.Event[tk.Misc]"):
        match event.char:
            case "i":
                keys_state[0] = False
            case "l":
                keys_state[1] = False
            case "k":
                keys_state[2] = False
            case "j":
                keys_state[3] = False
        _on_slidepad_change()

    master.bind("<KeyPress>", _on_keypress, add=True)
    master.bind("<KeyRelease>", _on_keyrelease, add=True)
