#include "graph.h"
#include "tasks.h"
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>

// Принимает список смежности, чтобы можно было засунуть транспонированный граф
static void dfs(
	const std::string& vertex,
	std::unordered_set<std::string>& visited,
	const std::unordered_map<std::string, std::vector<Edge>>& adjList
) {
	visited.insert(vertex);
	auto it = adjList.find(vertex);
	if (it == adjList.end()) return;
	for (const auto& edge : it->second) {
		if (visited.find(edge.target) == visited.end()) {
			dfs(edge.target, visited, adjList);
		}
	}
}

// Функция для получения транспонированного графа (рёбра наоборот)
static std::unordered_map<std::string, std::vector<Edge>>
getTranspose(const Graph& graph) {
	const auto& adjList = graph.getAdjList();
	std::unordered_map<std::string, std::vector<Edge>> transposed;

	for (const auto& [v, _] : adjList)
		transposed[v] = {};

	for (const auto& [from, edges] : adjList) {
		for (const auto& edge : edges) {
			transposed[edge.target].push_back({from, edge.label, edge.weight});
		}
	}
	return transposed;
}

// Алгоритм Косараджу
bool isStronglyConnected(const Graph& graph) {
	const auto& adjList = graph.getAdjList();
	if (adjList.empty()) return true;

	const std::string& start = adjList.begin()->first;

	std::unordered_set<std::string> visited;
	dfs(start, visited, adjList);
	if (visited.size() != adjList.size())
		return false;

	auto transposed = getTranspose(graph);
	visited.clear();
	dfs(start, visited, transposed);

	return visited.size() == adjList.size();
}

