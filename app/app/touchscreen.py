import dataclasses
import tkinter as tk
import typing


def _clamp_coord_to_window(master: tk.Tk, event: "tk.Event[tk.Misc]"):
    return max(0, min(event.x, master.winfo_width())), max(
        0, min(event.y, master.winfo_height())
    )


@dataclasses.dataclass(frozen=True)
class TouchScreenSize:
    width: int
    height: int


def _rescale_coord_to_touchscreen(
    master: tk.Tk, coord: tuple[int, int], target: TouchScreenSize
):
    x, y = coord
    return int((x / master.winfo_width()) * target.width), int(
        (y / master.winfo_height()) * target.height
    )


def bind_touchscreen_controller(
    master: tk.Tk,
    filler: tk.Misc,
    size: TouchScreenSize,
    on_hold: typing.Callable[[int, int], None],
    on_release: typing.Callable[[], None],
):
    prev: tuple[int, int] | None = None

    def _on_hold(event: "tk.Event[tk.Misc]"):
        nonlocal prev
        x, y = _rescale_coord_to_touchscreen(
            master, _clamp_coord_to_window(master, event), size
        )
        if prev is None or (x, y) != prev:
            on_hold(x, y)
            prev = (x, y)

    def _on_release(_: "tk.Event[tk.Misc]"):
        nonlocal prev
        on_release()
        prev = None

    filler.bind("<Button-1>", _on_hold, add=True)
    filler.bind("<B1-Motion>", _on_hold, add=True)
    filler.bind("<ButtonRelease-1>", _on_release, add=True)
