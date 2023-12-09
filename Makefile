all: antidbg

antidbg: antidbg.c
	gcc -O -o $@ $^

clean:
	rm antidbg
