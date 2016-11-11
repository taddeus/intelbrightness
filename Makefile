CFLAGS := -Wall -Wextra -O2
TARGET ?= /usr/bin/intelbrightness

all: plain

plain: plain.c bright.o

install: plain
	cp $< $(TARGET)
	chown root:root $(TARGET)
	chmod 4711 $(TARGET)

uninstall:
	rm $(TARGET)

clean:
	rm -f plain
