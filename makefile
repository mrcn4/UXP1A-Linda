all: client server

server:
	g++ -std=c++11 TupleServer.cpp HelperFunctions.cpp -o server -lpthread

client:
	g++ -std=c++11 TupleClient.cpp HelperFunctions.cpp -o client -lpthread

clean:
	rm server client *.o

