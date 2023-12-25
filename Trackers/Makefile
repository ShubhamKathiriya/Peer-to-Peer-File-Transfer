CC = g++
CFLAGS = -Wall
DEPS = headers.h 
OBJ = tracker.o custom_logger.o request_handler.o utility.o request_exection.o data_structure.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

tracker: $(OBJ)
	$(CC) $(CFLAGS) -pthread -o $@ $^ 

clean:
	rm -rf *o tracker