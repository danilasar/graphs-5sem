all:
	g++ -std=c++23 src/main.cpp src/graph.cpp src/pugixml.cpp src/task2.cpp src/task3.cpp src/task4.cpp src/task5.cpp src/task6.cpp src/task7.cpp -I include -o graphene
