CXX = g++
CXXFLAGS =

main.exe: main.o
	$(CXX) $(CXXFLAGS) $^ -o main.exe

main.o: main.cpp digraph.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

.PHONY: clear docs run_v
clear:
	rm *.o *.exe
docs:
	doxygen
run:
	make
	./main.exe
run_v:
	make
	valgrind --leak-check=full ./main.exe