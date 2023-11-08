ifeq ($(OS),Windows_NT)
    RM = del /Q
    FIXPATH = $(subst /,\,$1)
    CONVERT = magick convert
	PYTHON = py
else
    RM = rm -f
    FIXPATH = $1
    CONVERT = convert
	PYTHON = python3
endif

POTRACE = potrace

.PHONY: all clean

all: icon.svg logo.svg logo-white.svg

icon-bg-white.png: icon.png
	$(CONVERT) $< -background white -alpha remove -colorspace Gray $@

icon.pnm: icon-bg-white.png
	$(CONVERT) $< $@

icon.svg: icon.pnm
	$(POTRACE) $< -s -o $@

string.png:
	$(CONVERT) -background transparent -fill black -font Pretendo -pointsize 500 label:"NeoCtrlMod" $@

spacer.png:
	$(CONVERT) -size 200x1 xc:transparent $@

logo.png: icon.png spacer.png string.png
	$(CONVERT) $^ -colorspace Gray -gravity center -background none +append $@

logo-bg-white.png: logo.png
	$(CONVERT) $< -background white -alpha remove $@

logo.pnm: logo-bg-white.png
	$(CONVERT) $< $@

logo.svg: logo.pnm
	$(POTRACE) $< -s -o $@

logo-white.svg: logo.svg
	$(PYTHON) -c "import sys, re; sys.stdout.write(re.sub(r'(?i)#000000', '#ffffff', open('$(call FIXPATH,$<)').read()))" > $@

clean:
	$(RM) $(call FIXPATH,icon-bg-white.png icon.pnm icon.svg string.png spacer.png logo.png logo-bg-white.png logo.pnm logo.svg logo-white.svg)
