import dataclasses
import enum
import typing
from _typeshed import Incomplete

class Button(enum.IntEnum):
    Y: int
    B: int
    A: int
    X: int
    L: int
    R: int
    SELECT: int
    START: int
    POWER: int
    WIRELESS: int
    HOME: int

Y: typing.Literal[Button.Y]
B: typing.Literal[Button.B]
A: typing.Literal[Button.A]
X: typing.Literal[Button.X]
L: typing.Literal[Button.L]
R: typing.Literal[Button.R]
SELECT: typing.Literal[Button.SELECT]
START: typing.Literal[Button.START]
POWER: typing.Literal[Button.POWER]
WIRELESS: typing.Literal[Button.WIRELESS]
HOME: typing.Literal[Button.HOME]

class Hat(enum.IntEnum):
    UP: int
    UPRIGHT: int
    RIGHT: int
    DOWNRIGHT: int
    DOWN: int
    DOWNLEFT: int
    LEFT: int
    UPLEFT: int
    NEUTRAL: int

UP: typing.Literal[Hat.UP]
UPRIGHT: typing.Literal[Hat.UPRIGHT]
RIGHT: typing.Literal[Hat.RIGHT]
DOWNRIGHT: typing.Literal[Hat.DOWNRIGHT]
DOWN: typing.Literal[Hat.DOWN]
DOWNLEFT: typing.Literal[Hat.DOWNLEFT]
LEFT: typing.Literal[Hat.LEFT]
UPLEFT: typing.Literal[Hat.UPLEFT]

@dataclasses.dataclass(frozen=True)
class SlidePad:
    x: int
    y: int
    def __post_init__(self) -> None: ...
    def __init__(self, x: int, y: int) -> None: ...

SLIDEPAD_NEUTRAL: int

@dataclasses.dataclass(frozen=True)
class TouchScreen:
    x: int
    y: int
    def __post_init__(self) -> None: ...
    def __init__(self, x: int, y: int) -> None: ...

class _Writer(typing.Protocol):
    def write(self, __b: typing.Any) -> typing.Any: ...

HoldOp: Incomplete

class HoldFunction(typing.Protocol):
    def __call__(self, *ops: HoldOp) -> None: ...

ReleaseOp: Incomplete

class ReleaseFunction(typing.Protocol):
    def __call__(self, *ops: ReleaseOp) -> None: ...

def init(writer: _Writer) -> tuple[HoldFunction, ReleaseFunction]: ...
