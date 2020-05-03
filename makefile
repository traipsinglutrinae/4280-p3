run: main.o token.o semantics.o scanner.o parser.o
	g++ main.o token.o semantics.o scanner.o parser.o -o statSem -std=c++11

main.o: main.cpp
	g++ -c main.cpp -std=c++11

token.o: token.cpp
	g++ -c token.cpp -std=c++11

semantics.o: semantics.cpp
	g++ -c semantics.cpp -std=c++11

scanner.o: scanner.cpp
	g++ -c scanner.cpp -std=c++11

parser.o: parser.cpp
	g++ -c parser.cpp -std=c++11

clean:
	rm -rf *.o statSem
