# List of all the board related files.
BOARDSRC = $(CHIBIOS)/../../bsp/board.c

# Required include directories
BOARDINC = $(CHIBIOS)/../../bsp

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
