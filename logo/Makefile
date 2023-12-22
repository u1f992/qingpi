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

# Intermediate and output files
ICON_PNG = temp_icon.png
STRING_PNG = temp_string.png
SPACER_PNG = temp_spacer.png
LOGO_PNG = temp_logo.png
LOGO_PNM = temp_logo.pnm
LOGO_SVG = logo.svg
LOGO_WHITE_SVG = logo-white.svg

# Input files
ICON_SVG = icon.svg
STRING_SVG = string.svg

$(ICON_PNG): $(ICON_SVG)
	$(CONVERT) $< -resize 1000x1000 $@

$(STRING_PNG): $(STRING_SVG)
	$(CONVERT) $< -resize 3000x3000 $@

# Create a spacer PNG
$(SPACER_PNG):
	$(CONVERT) -size 200x1 xc:white $@

# Combine the PNGs into a single image
$(LOGO_PNG): $(ICON_PNG) $(SPACER_PNG) $(STRING_PNG)
	$(CONVERT) $^ -gravity center -background white +append $@

$(LOGO_PNM): $(LOGO_PNG)
	$(CONVERT) $< $@

$(LOGO_SVG): $(LOGO_PNM)
	$(POTRACE) $< -s -o $@

$(LOGO_WHITE_SVG): $(LOGO_SVG)
	$(PYTHON) -c "import sys, re; sys.stdout.write(re.sub(r'(?i)#000000', '#ffffff', open('$(call FIXPATH,$<)').read()))" > $@

all: $(LOGO_SVG) $(LOGO_WHITE_SVG)

clean:
	$(RM) \
	    $(call FIXPATH,$(ICON_PNG)) \
	    $(call FIXPATH,$(STRING_PNG)) \
	    $(call FIXPATH,$(SPACER_PNG)) \
	    $(call FIXPATH,$(LOGO_PNG)) \
	    $(call FIXPATH,$(LOGO_PNM)) \
	    $(call FIXPATH,$(LOGO_SVG)) \
	    $(call FIXPATH,$(LOGO_WHITE_SVG))

.PHONY: all clean
