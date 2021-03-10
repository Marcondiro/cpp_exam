CXX = g++
CXXFLAGS =

main.exe: main.o
	$(CXX) $(CXXFLAGS) $^ -o main.exe

#TODO: Aggiungere classe container alle dipendenze di main.o
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

.PHONY: clear
clear:
	rm *.o *.exe