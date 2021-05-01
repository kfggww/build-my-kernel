all: client server pipe_demo

pipe_demo: pipe_demo.c
	gcc pipe_demo.c -o pipe_demo

server: server.c
	gcc -lpthread -o server server.c

client: client.c
	gcc -lpthread -o client client.c

clean:
	-rm client server pipe_demo
