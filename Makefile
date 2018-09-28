#LCD Tools Makefile
CC = g++
CPPFLAGS = -mfloat-abi=hard -marm -mtune=cortex-a15.cortex-a7 -mcpu=cortex-a15

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

all: hdmi_signal

hdmi_signal: hdmi_signal.cpp
	$(CC) -o hdmi_signal hdmi_signal.cpp
clean:
	rm -f hdmi_signal img2fb show_info osd2fb

install:
	install -d $(PREFIX)/bin/
	install -m 4755 hdmi_signal $(PREFIX)/bin/
