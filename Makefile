CC = gcc
CFLAGS = -std=c11 -Isrc/include -Wall -g
OBJECTS = main.o server.o room.o socket.o websocket.o helper.o base64.o structure.o error.o
EXEC = Rainy.Chat
VPATH = src src/include

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXEC) -lcrypto -ljansson -pthread

main.o: main.c server.o
	$(CC) $(CFLAGS) -c $<

server.o: server.c server.h websocket.o room.o
	$(CC) $(CFLAGS) -c $<

room.o: room.c room.h structure.o
	$(CC) $(CFLAGS) -c $<

websocket.o: websocket.c websocket.h socket.o base64.o
	$(CC) $(CFLAGS) -c $<

socket.o: socket.c socket.h helper.o
	$(CC) $(CFLAGS) -c $<

helper.o: helper.c helper.h structure.o
	$(CC) $(CFLAGS) -c $<

structure.o: structure.c structure.h includes.h error.o
	$(CC) $(CFLAGS) -c $<

base64.o: base64.c base64.h includes.h
	$(CC) $(CFLAGS) -c $<

error.o: error.c error.h
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean:
	-rm $(EXEC) *.o
