debug:
	g++ -g -std=c++1z -DDEBUG -o out.out main.cpp node.cpp tree.cpp

release:
	g++ -O3 -std=c++1z -o out.out main.cpp node.cpp tree.cpp

.phony: all
all: release
