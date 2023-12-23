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

all: dist/qingpi-app.exe

icon.ico: ../logo/icon.svg
	$(CONVERT) $< -transparent white -background none -resize 16x16 icon-16.bmp
	$(CONVERT) $< -transparent white -background none -resize 32x32 icon-32.bmp
	$(CONVERT) $< -transparent white -background none -resize 48x48 icon-48.bmp
	$(CONVERT) $< -transparent white -background none -resize 64x64 icon-64.bmp
	$(CONVERT) $< -transparent white -background none -resize 128x128 icon-128.bmp
	$(CONVERT) icon-16.bmp icon-32.bmp icon-48.bmp icon-64.bmp icon-128.bmp $@

dist/qingpi-app.exe: app/__main__.py icon.ico
	poetry run pyinstaller --onefile --windowed --name qingpi-app --icon icon.ico ./app/__main__.py

clean:
	$(RM) $(call FIXPATH,dist/* *.spec icon.ico icon-16.bmp icon-32.bmp icon-48.bmp icon-64.bmp icon-128.bmp)