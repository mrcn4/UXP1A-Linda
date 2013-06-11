all: clean client server

server:
	g++ -std=c++11 TupleServer.cpp HelperFunctions.cpp PosixSemaphore.cpp Tuple.cpp TupleDatabase.cpp TupleQuery.cpp -o server -lpthread -lrt

client:
	g++ -std=c++11 TupleClient.cpp HelperFunctions.cpp PosixSemaphore.cpp Tuple.cpp -o client -lpthread -lrt

clean:
	rm -f server client *.o

