#include "graph.h"
#include "tasks.h"
#include <unordered_set>

Graph symmetricDifference(const Graph& g1, const Graph& g2) {
	// Предполагается, что оба графа имеют одинаковый тип
	Graph result(g1.getGraphType());

	auto edgeKey = [](const std::string& from, const Edge& edge) {
		std::string label = edge.label.value_or("");
		double weight = edge.weight.value_or(0.0);
		return from + "->" + edge.target;
	};

	std::unordered_set<std::string> edges1, edges2;

	for (const auto& [v, _] : g1.getAdjList()) {
		result.addVertex(v);
	}
	for (const auto& [v, _] : g2.getAdjList()) {
		result.addVertex(v);
	}

	for (const auto& [from, edges] : g1.getAdjList()) {
		for (const auto& edge : edges) {
			edges1.insert(edgeKey(from, edge));
		}
	}

	for (const auto& [from, edges] : g2.getAdjList()) {
		for (const auto& edge : edges) {
			edges2.insert(edgeKey(from, edge));
		}
	}

	for (const auto& [from, edges] : g1.getAdjList()) {
		for (const auto& edge : edges) {
			auto key = edgeKey(from, edge);
			if (edges2.find(key) == edges2.end()) {
				result.addEdge(from, edge.target, edge.label, edge.weight);
			}
		}
	}

	for (const auto& [from, edges] : g2.getAdjList()) {
		for (const auto& edge : edges) {
			auto key = edgeKey(from, edge);
			if (edges1.find(key) == edges1.end()) {
				result.addEdge(from, edge.target, edge.label, edge.weight);
			}
		}
	}

	return result;
}

