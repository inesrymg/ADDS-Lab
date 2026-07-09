CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude

adds-lab: src/main.c src/library.c
	$(CC) $(CFLAGS) -o adds-lab src/main.c src/library.c

clean:
	rm -f adds-lab adds-lab.exe

.PHONY: clean
