.PHONY: all clean zip stub

ifeq ($(OS),Windows_NT)
    RM = del /Q
    FIXPATH = $(subst /,\,$1)
    CONVERT = magick convert
else
    RM = rm -f
    FIXPATH = $1
    CONVERT = convert
endif

all: dist/qingpictl.exe zip

icon:
	mkdir icon

icon/16.bmp: ../logo/icon.svg icon
	$(CONVERT) $< -transparent white -background none -resize 16x16 icon/temp_16.bmp
	$(CONVERT) icon/temp_16.bmp -gravity center -background none -extent 16x16 $@

icon/32.bmp: ../logo/icon.svg icon
	$(CONVERT) $< -transparent white -background none -resize 32x32 icon/temp_32.bmp
	$(CONVERT) icon/temp_32.bmp -gravity center -background none -extent 32x32 $@

icon/48.bmp: ../logo/icon.svg icon
	$(CONVERT) $< -transparent white -background none -resize 48x48 icon/temp_48.bmp
	$(CONVERT) icon/temp_48.bmp -gravity center -background none -extent 48x48 $@

icon/64.bmp: ../logo/icon.svg icon
	$(CONVERT) $< -transparent white -background none -resize 64x64 icon/temp_64.bmp
	$(CONVERT) icon/temp_64.bmp -gravity center -background none -extent 64x64 $@

icon/128.bmp: ../logo/icon.svg icon
	$(CONVERT) $< -transparent white -background none -resize 128x128 icon/temp_128.bmp
	$(CONVERT) icon/temp_128.bmp -gravity center -background none -extent 128x128 $@

icon/256.bmp: ../logo/icon.svg icon
	$(CONVERT) $< -transparent white -background none -resize 256x256 icon/temp_256.bmp
	$(CONVERT) icon/temp_256.bmp -gravity center -background none -extent 256x256 $@

icon/icon.ico: icon/16.bmp icon/32.bmp icon/48.bmp icon/64.bmp icon/128.bmp icon/256.bmp
	$(CONVERT) $^ $@

qingpi/gui/icon.py: icon/256.bmp
	$(CONVERT) $< icon/256.png
	poetry run python -c "import base64; print('ICON = \"' + base64.b64encode(open('icon/256.png', 'rb').read()).decode() + '\"')" > $@

dist/qingpictl.exe: qingpi/__main__.py qingpi/gui/icon.py icon/icon.ico
	poetry run pyinstaller --onefile --windowed --name qingpictl --icon icon/icon.ico ./qingpi/__main__.py

zip: dist/qingpictl.exe
	poetry run python tools/create_zip_archive.py

stub:
	poetry run stubgen -p qingpi -o out
	poetry run python tools/replace.py "out/qingpi/gui/touchscreen.pyi" "def __init__\(self, width, height\)" "def __init__(self, width: int, height: int)"
	poetry run python tools/replace.py "out/qingpi/qingpi.pyi" "def __init__\(self, x, y\)" "def __init__(self, x: int, y: int)"
	poetry run python -c "import shutil, glob, os; [shutil.copy(file, file.replace('out/qingpi', 'qingpi', 1)) or os.makedirs(os.path.dirname(file.replace('out/qingpi', 'qingpi', 1)), exist_ok=True) for file in glob.glob('out/qingpi/**/*.pyi', recursive=True)]"

clean:
	$(RM) \
	    $(call FIXPATH,dist/*) \
	    $(call FIXPATH,icon/*) \
	    $(call FIXPATH,qingpi/gui/icon.py) \
	    $(call FIXPATH,*.spec) \
	    $(call FIXPATH,*.zip) \
	    $(call FIXPATH,out/qingpi/*) \
	    $(call FIXPATH,out/qingpi/gui/*)