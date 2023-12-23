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

temp_16.bmp: ../logo/icon.svg
	$(CONVERT) $< -transparent white -background none -resize 16x16 temp_temp_16.bmp
	$(CONVERT) temp_temp_16.bmp -gravity center -background none -extent 16x16 $@

temp_32.bmp: ../logo/icon.svg
	$(CONVERT) $< -transparent white -background none -resize 32x32 temp_temp_32.bmp
	$(CONVERT) temp_temp_32.bmp -gravity center -background none -extent 32x32 $@

temp_48.bmp: ../logo/icon.svg
	$(CONVERT) $< -transparent white -background none -resize 48x48 temp_temp_48.bmp
	$(CONVERT) temp_temp_48.bmp -gravity center -background none -extent 48x48 $@

temp_64.bmp: ../logo/icon.svg
	$(CONVERT) $< -transparent white -background none -resize 64x64 temp_temp_64.bmp
	$(CONVERT) temp_temp_64.bmp -gravity center -background none -extent 64x64 $@

temp_128.bmp: ../logo/icon.svg
	$(CONVERT) $< -transparent white -background none -resize 128x128 temp_temp_128.bmp
	$(CONVERT) temp_temp_128.bmp -gravity center -background none -extent 128x128 $@

icon.ico: temp_16.bmp temp_32.bmp temp_48.bmp temp_64.bmp temp_128.bmp
	$(CONVERT) $^ $@

dist/qingpi-app.exe: app/__main__.py icon.ico
	poetry run pyinstaller --onefile --windowed --name qingpi-app --icon icon.ico ./app/__main__.py

clean:
	$(RM) \
	    $(call FIXPATH,dist/*) \
	    $(call FIXPATH,*.spec) \
	    $(call FIXPATH,temp_*)