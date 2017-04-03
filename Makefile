CC = gcc
CFLAGS = -lcrypto -std=c11
# CFLAGS = -lcrypto -pthread -std=c11
OBJECTS = server.o socket.o websocket.o helper.o base64.o
EXEC = server

$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(CFLAGS) $(OBJECTS)

server.o: server.c websocket.h
	$(CC) -c server.c

socket.o: socket.c socket.h helper.h
	$(CC) -c socket.c

websocket.o: websocket.c websocket.h socket.h helper.h
	$(CC) -c websocket.c

helper.o: helper.c helper.h includes.h
	$(CC) -c helper.c

base64.o: base64.c base64.h
	$(CC) -c base64.c

.PHONY : clean
clean:
	-rm $(EXEC) *.o
