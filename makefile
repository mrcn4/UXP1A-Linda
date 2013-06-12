all: clean client server

server:
	g++ -std=c++11 TupleServer.cpp HelperFunctions.cpp PosixSemaphore.cpp Tuple.cpp TupleDatabase.cpp TupleQuery.cpp -o server -lpthread -lrt

client:
	 g++ -std=c++11 SampleClient.cpp TupleClient.cpp Tuple.cpp  HelperFunctions.cpp PosixSemaphore.cpp -o client -lpthread -lrt

clean:
	rm -f server client *.o

