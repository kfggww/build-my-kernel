
all: reader writer test_pipe

test_pipe: test_pipe.c
	gcc -lpthread -o test_pipe test_pipe.c

reader: reader.c
	gcc -lpthread -o reader reader.c

writer: writer.c
	gcc -lpthread -o writer writer.c

clean:
	rm reader writer test_pipe
