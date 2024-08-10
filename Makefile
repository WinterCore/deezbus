OS := $(shell uname)
CFLAGS = -std=c11 -Wall -Wextra `pkg-config --cflags dbus-1`
LDFLAGS = `pkg-config --libs dbus-1`

.PHONY: clean debug all


debug: CFLAGS += -ggdb -DDEBUG
debug: Out

release: CFLAGS += -DNDEBUG
release: Out

SRC_C=$(shell find . -name "*.c")
SRC_ALL=$(shell find . -name "*.c" -o -name '*.h')

Out: $(SRC_ALL)
	cc $(CFLAGS) -o Out $(SRC_C) $(LDFLAGS)

all: Out

clean:
	rm -rf Out ./src/*.o ./src/**/*.o ./Out.dSYM
