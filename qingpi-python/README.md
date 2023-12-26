# qingpi-python

Python library for Qingpi.

## qingpi-controller

GUI application to control Qingpi with transparent Window and Citra compatible keyboard operation.

Currently, only Windows version is available.

### Run

```shell-session
$ python -m qingpi-python [port]
```

or

```shell-session
> qingpi-controller.exe [port]
```

### Input mapping

<dl>
<dt>

Button

</dt>
<dd>

| Control  | Input |
| -------- | :---: |
| A        |  `A`  |
| B        |  `S`  |
| X        |  `Z`  |
| Y        |  `X`  |
| L        |  `Q`  |
| R        |  `W`  |
| ~~ZL~~   |  `1`  |
| ~~ZR~~   |  `2`  |
| START    |  `M`  |
| SELECT   |  `N`  |
| HOME     |  `B`  |
| POWER    |  `V`  |
| WIRELESS |  `P`  |

</dd>
<dt>

Hat

</dt>
<dd>

| Control | Input |
| ------- | :---: |
| UP      |  `T`  |
| RIGHT   |  `H`  |
| DOWN    |  `G`  |
| LEFT    |  `F`  |

</dd>
<dt>

SlidePad

</dt>
<dd>

| Control |   Input   |
| ------- | :-------: |
| UP      |  `<Up>`   |
| RIGHT   | `<Right>` |
| DOWN    | `<Down>`  |
| LEFT    | `<Left>`  |

</dd>
<dt>

~~Circle Pad Pro~~ (not implemented)

</dt>
<dd>

| Control   | Input |
| --------- | :---: |
| ~~UP~~    |  `I`  |
| ~~RIGHT~~ |  `L`  |
| ~~DOWN~~  |  `K`  |
| ~~LEFT~~  |  `J`  |

</dd>
<dt>

Hotkeys

</dt>
<dd>

| Control       | Input |
| ------------- | :---: |
| Expand window |  `+`  |
| Shrink window |  `-`  |

</dd>
</dl>

## Developer note

```shell-session
$ poetry run python -m qingpi [port] --debug
```

The build script requires ImageMagick.

```shell-session
$ make
```

To avoid security issues,

```shell-session
$ poetry run python -m pip uninstall pyinstaller
$ git clone https://github.com/pyinstaller/pyinstaller --depth 1
$ cd .\pyinstaller\bootloader\
$ py .\waf distclean all
$ cd ..\..\
$ poetry run python -m pip install .\pyinstaller\
```