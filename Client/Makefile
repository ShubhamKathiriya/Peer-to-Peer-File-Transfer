CC = g++
CFLAGS = -Wall
DEPS = all_header.h
OBJ = algorithm.o data_structure.o custom_logger.o utility.o peer_to_peer.o leacher.o client.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

client: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -pthread -lssl -lcrypto -I/usr/include/openssl
 
clean:
	rm -rf *o client