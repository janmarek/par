ARGS=-Wall -pedantic -g
COMMON_FILES=Graph.cpp GraphFactory.cpp GraphWalker.cpp Solution.cpp EdgeCombination.cpp

program: clean
	g++ ${ARGS} ${COMMON_FILES} main.cpp -o bin/program

clean:
	rm -rf bin
	mkdir bin
	
runtests: clean
	rm -f testprogram
	g++ ${ARGS} ${COMMON_FILES} test/test.cpp -o bin/testprogram
	./bin/testprogram
