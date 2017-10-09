APPLICATION = sleepy_blink
RIOTBASE ?= $(CURDIR)/../..
BOARD ?= hamilton

USEMODULE += fxos8700
USEMODULE += rtt_stdio 
CFLAGS += -DRTT_STDIO_DISABLE_STDIN
USEMODULE += xtimer

FEATURES_REQUIRED += periph_timer

include $(RIOTBASE)/Makefile.include
$(warning $(FEATURES_REQUIRED))
