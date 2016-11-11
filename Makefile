CFLAGS := -Wall -Wextra -O2
TARGET ?= /usr/bin/intelbrightness

all: bright

bright: bright.c setuid.c
	$(CC) $(CFLAGS) -o $@ $<

install: bright
	cp $< $(TARGET)
	chown root:root $(TARGET)
	chmod 4711 $(TARGET)

uninstall:
	rm $(TARGET)

clean:
	rm -f bright
