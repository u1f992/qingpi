# Qingpi Application

GUI application to control Qingpi with transparent Window and Citra compatible keyboard operation.

Currently, only Windows version is available.

```
.\qingpi-app.exe [port]
```

## Input mapping

### Button

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

### Hat

| Control | Input |
| ------- | :---: |
| UP      |  `T`  |
| RIGHT   |  `H`  |
| DOWN    |  `G`  |
| LEFT    |  `F`  |

### SlidePad

| Control |   Input   |
| ------- | :-------: |
| UP      |  `<Up>`   |
| RIGHT   | `<Right>` |
| DOWN    | `<Down>`  |
| LEFT    | `<Left>`  |

### Circle Pad Pro

| Control   | Input |
| --------- | :---: |
| ~~UP~~    |  `I`  |
| ~~RIGHT~~ |  `L`  |
| ~~DOWN~~  |  `K`  |
| ~~LEFT~~  |  `J`  |

### Hotkeys

| Control       | Input |
| ------------- | :---: |
| Expand window |  `+`  |
| Shrink window |  `-`  |

## Development

### Setup

```
poetry install
poetry run python -m app [port] --debug
```

### Build

The build script requires ImageMagick.

```
make all
```

To avoid security issues,

```
poetry run python -m pip uninstall pyinstaller
cd ..\..\
git clone https://github.com/pyinstaller/pyinstaller --depth 1
cd .\pyinstaller\bootloader\
py .\waf distclean all
cd ..\..\qingpi\app\
poetry run python -m pip install ..\..\pyinstaller\
```