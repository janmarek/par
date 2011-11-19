ARGS=-Wall -pedantic -g
COMMON_FILES=Graph.cpp GraphFactory.cpp CombinationIterator.cpp Solution.cpp EdgeCombination.cpp

mpi: clean
	mpiCC ${ARGS} ${COMMON_FILES} main.cpp -o bin/program

clean:
	rm -rf bin
	mkdir bin
	
runtests: clean
	rm -f testprogram
	g++ ${ARGS} ${COMMON_FILES} test/test.cpp -o bin/testprogram
	./bin/testprogram

mpirun8: mpi
	mpirun -np 8 ./bin/program
