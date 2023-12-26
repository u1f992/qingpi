.PHONY: all clean

ifeq ($(OS),Windows_NT)
    RM = del /Q
    FIXPATH = $(subst /,\,$1)
    CONVERT = magick convert
else
    RM = rm -f
    FIXPATH = $1
    CONVERT = convert
endif

all: dist/qingpi-controller.exe

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

icon/icon.ico: icon/16.bmp icon/32.bmp icon/48.bmp icon/64.bmp icon/128.bmp
	$(CONVERT) $^ $@

dist/qingpi-controller.exe: qingpi/__main__.py icon/icon.ico
	poetry run pyinstaller --onefile --windowed --name qingpi-controller --icon icon/icon.ico ./qingpi/__main__.py

clean:
	$(RM) \
	    $(call FIXPATH,dist/*) \
	    $(call FIXPATH,icon/*) \
	    $(call FIXPATH,*.spec)