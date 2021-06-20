all:
	g++ clusterization.cpp -o hello
	g++ main.o factorial.o hello.o -o hello
	./hello