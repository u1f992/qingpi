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


Y = Button.Y
B = Button.B
A = Button.A
X = Button.X
L = Button.L
R = Button.R
SELECT = Button.SELECT
START = Button.START
POWER = Button.POWER
WIRELESS = Button.WIRELESS
HOME = Button.HOME


class Hat(enum.IntEnum):
    UP = 0
    UPRIGHT = 1
    RIGHT = 2
    DOWNRIGHT = 3
    DOWN = 4
    DOWNLEFT = 5
    LEFT = 6
    UPLEFT = 7
    _NEUTRAL = 8


UP = Hat.UP
UPRIGHT = Hat.UPRIGHT
RIGHT = Hat.RIGHT
DOWNRIGHT = Hat.DOWN
DOWN = Hat.DOWN
DOWNLEFT = Hat.DOWNLEFT
LEFT = Hat.LEFT
UPLEFT = Hat.UPLEFT


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
    def write(self, __b):
        raise NotImplementedError()


_HoldOp = Button | Hat | SlidePad | TouchScreen
_ReleaseOp = Button | type[Button] | type[Hat] | type[SlidePad] | type[TouchScreen]


def init(writer: _Writer):
    HEADER = 0xAB
    HAT_NEUTRAL = int(Hat._NEUTRAL)
    SLIDEPAD_NEUTRAL = 128

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

    def hold(*ops: _HoldOp):
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

            elif isinstance(op, TouchScreen):
                state[8] = op.x & 0xFF
                state[9] = op.x >> 8
                state[10] = op.y

        writer.write(state)

    def release(*ops: _ReleaseOp):
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
