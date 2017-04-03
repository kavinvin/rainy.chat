CC = gcc
CFLAGS = -lcrypto -pthread -std=c11
OBJECTS = server.o socket.o websocket.o helper.o base64.o structure.o
EXEC = server

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXEC)

server.o: server.c server.h websocket.o
	$(CC) -std=c11 -c server.c

websocket.o: websocket.c websocket.h socket.o base64.o
	$(CC) -std=c11 -c websocket.c

socket.o: socket.c socket.h helper.o
	$(CC) -std=c11 -c socket.c

helper.o: helper.c helper.h structure.o
	$(CC) -std=c11 -c helper.c

structure.o: structure.c structure.h includes.h
	$(CC) -std=c11 -c structure.c

base64.o: base64.c base64.h includes.h
	$(CC) -std=c11 -c base64.c


.PHONY : clean
clean:
	-rm $(EXEC) *.o
