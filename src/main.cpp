#include "graph.h"

int main() {
	Graph graph;
	std::string cmd;
	while (true) {
		std::cout << "Введи команду (addv, adde, remv, reme, print, save, load, exit): ";
		std::cin >> cmd;
		if (cmd == "addv") {
			std::string v; std::cin >> v;
			if (graph.addVertex(v))
				std::cout << "Вершина добавлена.\n";
			else std::cout << "Вершина существует или произошла ошибка.\n";
		} else if (cmd == "adde") {
			std::string from, to, label; double weight;
			std::cin >> from >> to;
			std::cout << "Метка (опционально): "; std::cin >> label;
			std::cout << "Вес (опционально): "; std::cin >> weight;
			graph.addEdge(from, to, label, weight);
		} else if (cmd == "remv") {
			std::string v; std::cin >> v;
			graph.removeVertex(v);
		} else if (cmd == "reme") {
			std::string from, to; std::cin >> from >> to;
			graph.removeEdge(from, to);
		} else if (cmd == "print") {
			graph.printAdjacencyList();
		} else if (cmd == "save") {
			std::string fn; std::cin >> fn;
			graph.saveToFile(fn);
		} else if (cmd == "load") {
			std::string fn; std::cin >> fn;
			graph.loadFromFile(fn);
		} else if (cmd == "help") {
			std::cout << " - addv {id}\t\t- добавить вершину\n"
			             " - adde {fromid} {toid}\t- добавить ребро\n"
						 " - remv {id}\t\t- удалить вершину\n"
						 " - reme {fromid} {toid}\t- удалить ребро\n"
						 " - print \t\t- вывести граф\n"
						 " - save {file}\t\t- сохранить граф в файл\n"
						 " - load {file}\t\t- загрузить граф из файла";
		} else if (cmd == "exit") break;
	}
	return 0;
}
