CFLAGS := -Wall -Wextra -O2
TARGET ?= /usr/bin/intelbrightness

all: plain flame

plain: plain.c bright.o

flame: flame.c bright.o
flame: CFLAGS += $(shell pkg-config --cflags cairo)
flame: LDFLAGS += $(shell pkg-config --libs cairo)

install: plain
	cp $< $(TARGET)
	chown root:root $(TARGET)
	chmod 4711 $(TARGET)

uninstall:
	rm $(TARGET)

clean:
	rm -f plain flame
