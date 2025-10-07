#include "graph.h"
#include "tasks.h"
#include <unordered_set>
#include <string>
#include <unordered_map>
#include <vector>

using AdjList = std::unordered_map<std::string, std::vector<Edge>>;

bool hasCycleUndirected(
	const std::string& v, 
	const std::string& parent,
	std::unordered_set<std::string>& visited,
	const AdjList& adjList
) {
	visited.insert(v);
	for (const auto& edge : adjList.at(v)) {
		const std::string& u = edge.target;
		if (u == parent) continue;
		if (visited.find(u) != visited.end()) {
			return true;
		}
		if (hasCycleUndirected(u, v, visited, adjList))
			return true;
	}
	return false;
}

void classifyGraph(const Graph& graph) {
	const auto& adjList = graph.getAdjList();
	
	if (adjList.empty()) {
		std::cout << "Пустой граф" << std::endl;
		return;
	}

	if (graph.getGraphType() == Graph::Type::DIRECTED) {
		std::cout << "Функция для ориентированных графов не реализована\n"
		             "Причина в том, что без заданного корня количество компонент связности может варьироваться."
		          << std::endl;
	}

	bool has_cycle = false;
	std::unordered_set<std::string> visited;
	int connected_components = 0;
	for (const auto& [vertex, _] : adjList) {
		if (visited.find(vertex) == visited.end()) {
			++connected_components;
			if (hasCycleUndirected(vertex, "", visited, adjList)) {
				has_cycle = true;
			}
		}
	}
	
	if (has_cycle) {
		std::cout << "Неориентированный граф содержит цикл, не является деревом или лесом" << std::endl;
	} else {
		if (connected_components == 1)
			std::cout << "Неориентированный граф является деревом" << std::endl;
		else if (connected_components > 1)
			std::cout << "Неориентированный граф является лесом из " 
					  << connected_components << " компонент связности" << std::endl;
	}
}

