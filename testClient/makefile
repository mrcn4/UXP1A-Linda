all: clean client server

server:
	g++ -std=c++11 ../TupleServer.cpp ../HelperFunctions.cpp ../PosixSemaphore.cpp ../Tuple.cpp ../TupleDatabase.cpp ../TupleQuery.cpp -o server -lpthread -lrt

client:
	g++ -std=c++11 T1Read1.cpp ../TupleClient.cpp ../Tuple.cpp  ../HelperFunctions.cpp ../PosixSemaphore.cpp -o clientRead -lpthread -lrt
	g++ -std=c++11 T1Input1.cpp ../TupleClient.cpp ../Tuple.cpp  ../HelperFunctions.cpp ../PosixSemaphore.cpp -o clientInput -lpthread -lrt
	g++ -std=c++11 T1Output1.cpp ../TupleClient.cpp ../Tuple.cpp  ../HelperFunctions.cpp ../PosixSemaphore.cpp -o clientOutput -lpthread -lrt

clean:
	rm -f server clientRead clientInput clientOutput *.o

run:
	./server ./clientRead ./clientInput ./clientOutput

