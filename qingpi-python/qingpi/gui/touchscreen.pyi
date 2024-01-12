import dataclasses
import tkinter as tk
import typing

@dataclasses.dataclass(frozen=True)
class TouchScreenSize:
    width: int
    height: int
    def __init__(self, width: int, height: int) -> None: ...

def bind_touchscreen_controller(master: tk.Tk, filler: tk.Misc, size: TouchScreenSize, on_hold: typing.Callable[[int, int], None], on_release: typing.Callable[[], None]) -> None: ...
