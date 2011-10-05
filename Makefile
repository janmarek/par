ARGS=-Wall -pedantic

program: clean
	g++ ${ARGS} Graph.cpp GraphTest.cpp main.cpp -o program

clean:
	rm -f program
