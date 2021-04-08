all: client server

server: server.c
	gcc -lpthread -o server server.c

client: client.c
	gcc -lpthread -o client client.c

clean:
	rm client server
