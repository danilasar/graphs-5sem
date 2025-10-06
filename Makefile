all:
	g++ -std=c++23 src/main.cpp src/graph.cpp src/pugixml.cpp -I include -o graphene
