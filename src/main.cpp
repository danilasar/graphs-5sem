#include "graph.h"
#include "tasks.h"
#include <regex>
#define TRACE_NEWLINE
#define TRACE_ROW(f) std::cout << "> " \
	<< std::regex_replace(#f, std::regex(" *TRACE_NEWLINE *"), "\n> ") << '\n'; \
	f;

void cliLoad(Graph& g) {
	std::string fn; std::cin >> fn;
	g.loadFromFile(fn);
}

void runTask(int n, Graph& g) {
	switch(n) {
		case 2:
			std::cout << "Задание 2.7. Список смежности Ia\n"
			             "Вывести все изолированные вершины орграфа степени 0\n";
			printIsolatedVertices(g);
			break;
		case 3:
			std::cout << "Задание 3.8. Список смежности Ia\n"
			             "Вывести те вершины, у которых полустепень исхода больше полустепени захода.\n";
			printVerticesWithGreaterOutDegree(g);
			break;
		case 4:
			std::cout << "Задание 4.9. Список смежности Iб\n"
			             "Построить граф, являющийся симметрической разностью двух заданных.\n\n"
			             "Введите путь к файлу второго графа:\n > ";
			{
				Graph g1;
				cliLoad(g1);
				g = symmetricDifference(g, g1);
			}
			break;
		case 5:
			std::cout << "Задание 5.12. Обходы графа II\n"
			             "Выяснить, является ли орграф сильно связным.\n";
			if(g.getGraphType() != Graph::Type::DIRECTED) {
					std::cerr << "Граф не является ориентированным" << std::endl;
					break;
			}
			if(isStronglyConnected(g)) {
					std::cout << "Граф сильно связный" << std::endl;
			} else {
					std::cout << "Граф не является сильно связным" << std::endl;
			}
			break;
		case 6:
			std::cout << "Задание 6.18. Обходы графа II\n"
			             "Проверить, является ли граф деревом, или лесом, или не является ни тем, ни другим.\n";
			classifyGraph(g);
			break;
		case 7:
			std::cout << "Задание 7. Каркас III\n"
			             "Дан взвешенный неориентированный граф из N вершин и M ребер.\n"
			             "Требуется найти в нем каркас минимального веса по алгоритму Краскала.\n";
			try {
				g = kruskalMST(g);
				std::cout << "Построено минимальное остовное дерево с весом " << getMSTWeight(g) << std::endl;
			} catch (const std::exception& e) {
				std::cerr << "Ошибка: " << e.what() << std::endl;
			}
			break;
		case 8:
			std::cout << "Задание 8. Веса IV а\n"
			             "Вывести кратчайший путь из вершины u до вершины v.\n"
			             "Алгоритм (Дейкстры, Беллмана-Форда или Флойда) выбирается таким образом,\n"
						 "чтобы не было повторений (для каждой из трёх задач этого раздела свой алгоритм).\n";
			break;
		case 9:
			std::cout << "Задание 9. Веса IV b\n"
			             "Найти k кратчайших путей между вершинами u и v.\n"
			             "Алгоритм (Дейкстры, Беллмана-Форда или Флойда) выбирается таким образом,\n"
						 "чтобы не было повторений (для каждой из трёх задач этого раздела свой алгоритм).\n";
			break;
		case 10:
			std::cout << "Задание 10. Веса IV c\n"
			             "Вывести все отрицательные циклы.\n"
			             "Алгоритм (Дейкстры, Беллмана-Форда или Флойда) выбирается таким образом,\n"
						 "чтобы не было повторений (для каждой из трёх задач этого раздела свой алгоритм).\n";
			break;
		case 11:
			std::cout << "Задание 11. Максимальный поток V\n"
			             "Решить задачу на нахождение максимального потока любым алгоритмом.\n"
			             "Алгоритм (Дейкстры, Беллмана-Форда или Флойда) выбирается таким образом,\n"
						 "чтобы не было повторений (для каждой из трёх задач этого раздела свой алгоритм).\n";
			break;
		default: std::cerr << "Такого задания нет" << std::endl; break;
	}
}

int main() {
	Graph graph;
	std::string cmd;
	std::cout << "help для справки" << std::endl;
	while (true) {
		std::cout << " $ ";
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
			cliLoad(graph);
		} else if (cmd == "help") {
			std::cout << "Интерактивная утилита для работы с графами\n"
			             " - addv {id}\t\t- добавить вершину\n"
			             " - adde {fromid} {toid}\t- добавить ребро\n"
			             " - remv {id}\t\t- удалить вершину\n"
			             " - reme {fromid} {toid}\t- удалить ребро\n"
		                 " - print \t\t- вывести граф\n"
			             " - save {file}\t\t- сохранить граф в файл\n"
			             " - load {file}\t\t- загрузить граф из файла\n"
			             " - demo \t\t- продемонстрировать работу класса\n"
			             " - task {n}\t\t- выполнить задание (№2-11)" << std::endl;
		} else if (cmd == "demo") {
			TRACE_ROW(
				graph.addVertex("A"); TRACE_NEWLINE
				graph.addVertex("B"); TRACE_NEWLINE
				graph.addVertex("C"); TRACE_NEWLINE
				graph.addEdge("A", "B"); TRACE_NEWLINE
				graph.printAdjacencyList()
			);
			TRACE_ROW(
				graph.removeVertex("C"); TRACE_NEWLINE
				graph.printAdjacencyList()
			);
			TRACE_ROW(
				graph.removeEdge("A", "B"); TRACE_NEWLINE
				graph.printAdjacencyList()
			);
		} else if(cmd == "task") {
			int n; std::cin >> n;
			runTask(n, graph);
		} else if (cmd == "exit") break;
	}
	return 0;
}
