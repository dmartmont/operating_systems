all: interprete file

interprete: interprete.o
		$(CXX) -o $@ $^

file: fileEwe.o
		$(CXX) -o $@ $^

*.o: *.cpp
