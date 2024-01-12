from .util import *
import enum
import tkinter as tk
import typing
from _typeshed import Incomplete

class HatState(enum.Enum):
    UP: Incomplete
    UPRIGHT: Incomplete
    RIGHT: Incomplete
    DOWNRIGHT: Incomplete
    DOWN: Incomplete
    DOWNLEFT: Incomplete
    LEFT: Incomplete
    UPLEFT: Incomplete
    NEUTRAL: Incomplete

def bind_hat_controller(master: tk.Tk, on_hat_change: typing.Callable[[HatState], None]) -> None: ...
