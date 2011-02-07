PREFIX = /home/damian/bintest/
ALLOW_UID = 500
ALLOW_GID = 500
TARGET_UID = 0
TARGET_GID = 0
DEBUG = 0

all:
	test -d build || mkdir build
	test -f "elevator.h" && rm "elevator.h"

	cp "elevator.h.template" "elevator.h"
	sed -i "s/@@ALLOW_UID@@/500/" "elevator.h"
	sed -i "s/@@ALLOW_GID@@/500/" "elevator.h"
	sed -i "s/@@TARGET_UID@@/0/" "elevator.h"
	sed -i "s/@@TARGET_GID@@/0/" "elevator.h"

	gcc elevator.c -o build/elevator

install:
	install -d -o $(ALLOW_UID) -g $(ALLOW_GID) -m 0751 -v $(PREFIX)/bin
	install -o $(TARGET_UID) -g $(TARGET_GID) -m 7755 -v build/elevator $(PREFIX)/bin

uninstall:
	rm -rfv $(PREFIX)

clean:
	rm -rfv build
	rm -rfv elevator.h
