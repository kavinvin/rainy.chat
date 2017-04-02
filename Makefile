CC = gcc
CFLAGS = -lcrypto -pthread -std=c11
OBJECTS = server.o socket.o websocket.o helper.o base64.o
EXEC = server

$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(CFLAGS) $(OBJECTS)

server.o: server.c
socket.o: socket.c socket.h
websocket.o: websocket.c websocket.h base64.h
helper.o: helper.c helper.h
base64.o: base64.c base64.h

.PHONY : clean
clean:
	rm -f $(EXEC) *.o
