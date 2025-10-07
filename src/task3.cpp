#include "graph.h"
#include "tasks.h"
#include <ranges>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

std::vector<std::string> getVerticesWithGreaterOutDegree(const Graph& g) {
	const std::unordered_map<std::string, std::vector<Edge>>& adjList = g.getAdjList();
	std::unordered_map<std::string, int> inDegree;

	for (const auto& [vertex, edges] : adjList) {
		inDegree[vertex] = 0;
	}
	for (const auto& [vertex, edges] : adjList)
		for (const auto& edge : edges)
			inDegree[edge.target]++;

	auto filtered = adjList 
		| std::views::filter([&](const auto& pair) {
			return static_cast<int>(pair.second.size()) > inDegree[pair.first];
		})
		| std::views::transform([](const auto& pair) {
			return pair.first;
		});
	
	return {filtered.begin(), filtered.end()};
	// return std::ranges::to<std::vector<std::string>>(filtered); не
	// поддерживается в моей версии gcc
}

void printVerticesWithGreaterOutDegree(const Graph& g) {
	auto vertices = getVerticesWithGreaterOutDegree(g);
	if (vertices.empty()) {
		std::cout << "Таких вершин не найдено.\n";
		return;
	}
	std::cout << "Вершины с полустепенью исхода больше полустепени захода:\n";
	for (const auto& v : vertices) {
		std::cout << "  " << v << '\n';
	}
}

