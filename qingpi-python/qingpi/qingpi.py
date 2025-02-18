import dataclasses
import enum
import typing


class Button(enum.IntEnum):
    Y = 0
    B = 1
    A = 2
    X = 3
    L = 4
    R = 5
    _ZL = 6
    _ZR = 7
    SELECT = 8
    START = 9
    POWER = 10
    WIRELESS = 11
    HOME = 12
    _CAPTURE = 13


Y: typing.Literal[Button.Y] = Button.Y
B: typing.Literal[Button.B] = Button.B
A: typing.Literal[Button.A] = Button.A
X: typing.Literal[Button.X] = Button.X
L: typing.Literal[Button.L] = Button.L
R: typing.Literal[Button.R] = Button.R
SELECT: typing.Literal[Button.SELECT] = Button.SELECT
START: typing.Literal[Button.START] = Button.START
POWER: typing.Literal[Button.POWER] = Button.POWER
WIRELESS: typing.Literal[Button.WIRELESS] = Button.WIRELESS
HOME: typing.Literal[Button.HOME] = Button.HOME


class Hat(enum.IntEnum):
    UP = 0
    UPRIGHT = 1
    RIGHT = 2
    DOWNRIGHT = 3
    DOWN = 4
    DOWNLEFT = 5
    LEFT = 6
    UPLEFT = 7
    NEUTRAL = 8


UP: typing.Literal[Hat.UP] = Hat.UP
UPRIGHT: typing.Literal[Hat.UPRIGHT] = Hat.UPRIGHT
RIGHT: typing.Literal[Hat.RIGHT] = Hat.RIGHT
DOWNRIGHT: typing.Literal[Hat.DOWNRIGHT] = Hat.DOWNRIGHT
DOWN: typing.Literal[Hat.DOWN] = Hat.DOWN
DOWNLEFT: typing.Literal[Hat.DOWNLEFT] = Hat.DOWNLEFT
LEFT: typing.Literal[Hat.LEFT] = Hat.LEFT
UPLEFT: typing.Literal[Hat.UPLEFT] = Hat.UPLEFT


@dataclasses.dataclass(frozen=True)
class SlidePad:
    """
    The upper left corner of the coord is (0,0) and the lower right one is (255,255).

    Values outside the range are automatically clamped.
    """

    x: int
    y: int

    def __post_init__(self):
        object.__setattr__(self, "x", min(max(self.x, 0), 255))
        object.__setattr__(self, "y", min(max(self.y, 0), 255))


SLIDEPAD_NEUTRAL = 128


@dataclasses.dataclass(frozen=True)
class TouchScreen:
    """
    The upper left corner of the coord is (1,1) and the lower right one is (320,240).

    Values outside the range are automatically clamped.
    """

    x: int
    y: int

    def __post_init__(self):
        object.__setattr__(self, "x", min(max(self.x, 1), 320))
        object.__setattr__(self, "y", min(max(self.y, 1), 240))


class _Writer(typing.Protocol):
    def write(self, __b: typing.Any) -> typing.Any:
        raise NotImplementedError()


HoldOp = Button | Hat | SlidePad | TouchScreen


class HoldFunction(typing.Protocol):
    def __call__(self, *ops: HoldOp) -> None:  # type: ignore
        pass


ReleaseOp = Button | type[Button] | type[Hat] | type[SlidePad] | type[TouchScreen]


class ReleaseFunction(typing.Protocol):
    def __call__(self, *ops: ReleaseOp) -> None:  # type: ignore
        pass


def init(writer: _Writer) -> tuple[HoldFunction, ReleaseFunction]:
    HEADER = 0xAB
    HAT_NEUTRAL = int(Hat.NEUTRAL)

    state = [
        HEADER,
        0,
        0,
        HAT_NEUTRAL,
        SLIDEPAD_NEUTRAL,
        SLIDEPAD_NEUTRAL,
        SLIDEPAD_NEUTRAL,
        SLIDEPAD_NEUTRAL,
        0,
        0,
        0,
    ]

    def hold(*ops: HoldOp):
        for op in ops:
            if isinstance(op, Button):
                btn = state[2] << 8 | state[1]
                btn |= 0b1 << op
                state[1] = btn & 0xFF
                state[2] = btn >> 8

            elif isinstance(op, Hat):
                state[3] = int(op)

            elif isinstance(op, SlidePad):
                state[4] = op.x
                state[5] = op.y

            else:  # if isinstance(op, TouchScreen):
                state[8] = op.x & 0xFF
                state[9] = op.x >> 8
                state[10] = op.y

        writer.write(state)

    def release(*ops: ReleaseOp):
        if len(ops) == 0:
            release(Button, Hat, SlidePad, TouchScreen)
            return

        for op in ops:
            if isinstance(op, Button):
                btn = state[2] << 8 | state[1]
                btn ^= 0b1 << op
                state[1] = btn & 0xFF
                state[2] = btn >> 8

            elif op == Button:
                state[1] = 0
                state[2] = 0

            elif op == Hat:
                state[3] = HAT_NEUTRAL

            elif op == SlidePad:
                state[4] = SLIDEPAD_NEUTRAL
                state[5] = SLIDEPAD_NEUTRAL

            elif op == TouchScreen:
                state[8] = 0
                state[9] = 0
                state[10] = 0

        writer.write(state)

    return hold, release
