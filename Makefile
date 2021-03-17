CXX = g++
CXXFLAGS =

main.exe: main.o
	$(CXX) $(CXXFLAGS) $^ -o main.exe

main.o: main.cpp digraph.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

.PHONY: clear
clear:
	rm *.o *.exe