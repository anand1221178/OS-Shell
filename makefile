all: witsshell

witsshell:
	gcc witsshell.c -Wall -Wextra -pedantic -o witsshell

clean:
	rm -rf witsshell
