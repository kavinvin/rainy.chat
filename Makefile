CC = gcc
CFLAGS = -std=c11
OBJECTS = main.o server.o socket.o websocket.o helper.o base64.o structure.o error.o
EXEC = Rainy.Chat

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXEC) -lcrypto -ljansson -pthread

main.o: main.c server.o
	$(CC) $(CFLAGS) -c main.c

server.o: server.c server.h websocket.o
	$(CC) $(CFLAGS) -c server.c

websocket.o: websocket.c websocket.h socket.o base64.o
	$(CC) $(CFLAGS) -c websocket.c

socket.o: socket.c socket.h helper.o
	$(CC) $(CFLAGS) -c socket.c

helper.o: helper.c helper.h structure.o
	$(CC) $(CFLAGS) -c helper.c

structure.o: structure.c structure.h includes.h error.o
	$(CC) $(CFLAGS) -c structure.c

base64.o: base64.c base64.h includes.h
	$(CC) $(CFLAGS) -c base64.c

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c

.PHONY : clean
clean:
	-rm $(EXEC) *.o
