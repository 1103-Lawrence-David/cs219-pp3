execute: main.o helpers.o conditionals.o
	g++ -o execute main.o helpers.o conditionals.o

main.o: helpers.h main.cpp 
	g++ -c main.cpp

helpers.o: helpers.h helpers.cpp conditionals.h
	g++ -c helpers.cpp

conditionals.o: conditionals.h conditionals.cpp
	g++ -c conditionals.cpp
	
make clean:
	rm *.o execute