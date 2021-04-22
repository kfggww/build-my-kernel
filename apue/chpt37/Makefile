##
# Chapter 37
#
# @file
# @version 0.1

all: test daemon_demo
	cp daemon_demo /tmp

daemon_demo: daemon_demo.c
	gcc -o daemon_demo daemon_demo.c

test: test.c
	gcc -o test test.c

.PHONY: clean
clean:
	rm test daemon_demo /tmp/daemon_demo


# end
