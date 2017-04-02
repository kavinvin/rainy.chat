CC = gcc
CFLAGS = -lcrypto -pthread -std=c11
OBJECTS = socket.o websocket.o helper.o base64.o
EXEC = server

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(EXEC).c -o $(EXEC)

socket.o: socket.c socket.h
	$(CC) -c socket.c

websocket.o: websocket.c websocket.h base64.h
	$(CC) -c websocket.c

helper.o: helper.c helper.h
	$(CC) -c helper.c

base64.o: base64.c base64.h
	$(CC) -c base64.c

clean:
	rm -f $(EXEC) $(OBJECTS)
