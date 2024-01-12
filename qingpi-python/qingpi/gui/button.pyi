import enum
import tkinter as tk
import typing
from _typeshed import Incomplete

class ButtonID(enum.Enum):
    A: Incomplete
    B: Incomplete
    X: Incomplete
    Y: Incomplete
    L: Incomplete
    R: Incomplete
    ZL: Incomplete
    ZR: Incomplete
    START: Incomplete
    SELECT: Incomplete
    HOME: Incomplete
    POWER: Incomplete
    WIRELESS: Incomplete

def bind_button_controller(master: tk.Tk, on_button_change: typing.Callable[[ButtonID, bool], None]) -> None: ...
