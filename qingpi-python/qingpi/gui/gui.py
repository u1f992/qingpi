import tkinter as tk


def make_window_be_resized_only_by_keypress(master: tk.Tk, min_size: tuple[int, int]):
    min_width, min_height = min_size

    master.geometry(f"{min_width}x{min_height}")
    master.minsize(min_width, min_height)
    master.resizable(False, False)

    def _on_keypress_resize(event: "tk.Event[tk.Misc]"):
        current_width = master.winfo_width()

        if event.char == "+":
            new_width = int(current_width * 1.01)
        elif event.char == "-":
            new_width = int(current_width * 0.99)
        else:
            return

        new_width = max(new_width, min_width)
        new_height = int(new_width * (min_height / min_width))
        new_height = max(new_height, min_height)

        master.geometry(f"{new_width}x{new_height}")

    master.bind("<KeyPress>", _on_keypress_resize, add=True)


def make_window_transparent(master: tk.Tk):
    tk.Frame(master, background="white").pack(expand=True, fill=tk.BOTH)
    master.wm_attributes("-transparentcolor", "white")


ALMOST_TRANSPARENT = 0.0019607843138


def create_transparent_filler(master: tk.Tk):
    filler = tk.Toplevel(master)

    # Erase from the taskbar
    filler.transient(master)
    # Erase window frame
    filler.overrideredirect(True)
    # Make the filler transparent. As close to 0 as possible, as long as it is clickable.
    filler.wm_attributes("-alpha", ALMOST_TRANSPARENT)

    # To learn what's going on, uncomment below.
    # filler.overrideredirect(False)
    # filler.wm_attributes("-alpha", 0.5)

    def _on_configure_let_filler_track(_: "tk.Event[tk.Misc]"):
        """Every time the size and position of root changes, make the size and position of self the same and put it behind root."""
        filler.geometry(
            f"{master.winfo_width()}x{master.winfo_height()}+{master.winfo_rootx()}+{master.winfo_rooty()}"
        )
        filler.lower(master)

    master.bind("<Configure>", _on_configure_let_filler_track, add=True)

    return filler
