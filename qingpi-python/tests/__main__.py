import enum
import typing

import qingpi
from qingpi import *


class ButtonFlag(enum.IntFlag):
    Y = enum.auto()
    B = enum.auto()
    A = enum.auto()
    X = enum.auto()
    L = enum.auto()
    R = enum.auto()
    __0 = enum.auto()
    __1 = enum.auto()
    SELECT = enum.auto()
    START = enum.auto()
    POWER = enum.auto()
    WIRELESS = enum.auto()
    HOME = enum.auto()
    __2 = enum.auto()


HAT_NEUTRAL = 8
SLIDEPAD_NEUTRAL = 128, 128
TOUCHSCREEN_RELEASED = 0, 0
INIT_STATE = (
    ButtonFlag(0),
    HAT_NEUTRAL,
    SLIDEPAD_NEUTRAL,
    TOUCHSCREEN_RELEASED,
)


class MockWriter:
    state: tuple[ButtonFlag, int, tuple[int, int], tuple[int, int]]

    def __init__(self) -> None:
        self.state = INIT_STATE

    def write(self, data: typing.Sequence[typing.Any]):
        assert isinstance(data, typing.Iterable)
        s = list(data)
        assert len(s) == 11
        assert s[0] == 0xAB

        self.state = (
            ButtonFlag(s[2] << 8 | s[1]),
            s[3],
            (s[4], s[5]),
            (s[9] << 8 | s[8], s[10]),
        )


def main():
    mock = MockWriter()
    hold, release = qingpi.init(mock)

    # Hold one button.
    hold(Y)
    assert mock.state == (
        ButtonFlag.Y,
        HAT_NEUTRAL,
        SLIDEPAD_NEUTRAL,
        TOUCHSCREEN_RELEASED,
    )
    # Hold multiple buttons.
    hold(B, A, X)
    assert mock.state == (
        ButtonFlag.Y | ButtonFlag.B | ButtonFlag.A | ButtonFlag.X,
        HAT_NEUTRAL,
        SLIDEPAD_NEUTRAL,
        TOUCHSCREEN_RELEASED,
    )
    # Release one button.
    release(Y)
    assert mock.state == (
        ButtonFlag.B | ButtonFlag.A | ButtonFlag.X,
        HAT_NEUTRAL,
        SLIDEPAD_NEUTRAL,
        TOUCHSCREEN_RELEASED,
    )
    # Release multiple buttons.
    release(B, A)
    assert mock.state == (
        ButtonFlag.X,
        HAT_NEUTRAL,
        SLIDEPAD_NEUTRAL,
        TOUCHSCREEN_RELEASED,
    )
    # Release all buttons.
    release(Button)
    assert mock.state == INIT_STATE

    # Change hat direction.
    hold(DOWN)
    assert mock.state == (
        ButtonFlag(0),
        int(DOWN),
        SLIDEPAD_NEUTRAL,
        TOUCHSCREEN_RELEASED,
    )
    hold(UP)
    assert mock.state == (
        ButtonFlag(0),
        int(UP),
        SLIDEPAD_NEUTRAL,
        TOUCHSCREEN_RELEASED,
    )
    # Put hat in neutral position.
    release(Hat)
    assert mock.state == INIT_STATE

    hold(SlidePad(0, 0))
    assert mock.state == (
        ButtonFlag(0),
        HAT_NEUTRAL,
        (0, 0),
        TOUCHSCREEN_RELEASED,
    )
    release(SlidePad)
    assert mock.state == INIT_STATE

    hold(TouchScreen(1, 1))
    assert mock.state == (ButtonFlag(0), HAT_NEUTRAL, SLIDEPAD_NEUTRAL, (1, 1))
    release(TouchScreen)
    assert mock.state == INIT_STATE

    # All at the same time.
    hold(
        Y,
        B,
        A,
        X,
        L,
        R,
        SELECT,
        START,
        POWER,
        WIRELESS,
        HOME,
        UPLEFT,
        SlidePad(255, 255),
        TouchScreen(320, 240),
    )
    assert mock.state == (
        ButtonFlag(0b0001_1111_0011_1111),
        int(UPLEFT),
        (255, 255),
        (320, 240),
    )
    # Release all.
    release()
    assert mock.state == INIT_STATE

    # Type check
    def _(hold: HoldFunction, release: ReleaseFunction):
        hold(A, UP, SlidePad(100, 100), TouchScreen(100, 100))
        release(Button, Hat, SlidePad, TouchScreen)

    _(hold, release)


if __name__ == "__main__":
    main()
    print("OK")
