#include "graph.h"
#include "tasks.h"
#include <ranges>
#include <numeric>
#include <set>
#include <unordered_map>

// Disjoint Set Union
class UnionFind {
private:
	std::unordered_map<std::string, std::string> parent;
	std::unordered_map<std::string, int> rank;

public:
	void makeSet(const std::string& vertex) {
		parent[vertex] = vertex;
		rank[vertex] = 0;
	}

	// Path compression
	std::string find(const std::string& vertex) {
		if (parent[vertex] != vertex) {
			parent[vertex] = find(parent[vertex]);
		}
		return parent[vertex];
	}

	bool unite(const std::string& x, const std::string& y) {
		std::string rootX = find(x);
		std::string rootY = find(y);

		if (rootX == rootY) {
			return false; // цикл
		}

		// Union by rank
		if (rank[rootX] < rank[rootY]) {
			parent[rootX] = rootY;
		} else if (rank[rootX] > rank[rootY]) {
			parent[rootY] = rootX;
		} else {
			parent[rootY] = rootX;
			rank[rootX]++;
		}
		return true;
	}
};

Graph kruskalMST(const Graph& graph) {
	if (graph.getGraphType() != Graph::Type::UNDIRECTED) {
		throw std::invalid_argument("Алгоритм Краскала работает только с неориентированными графами");
	}

	std::vector<std::pair<std::string, Edge>> edges;
	std::set<std::string> vertices;

	for (const auto& [from, edgeList] : graph.getAdjList()) {
		vertices.insert(from);
		for (const auto& edge : edgeList) {
			if (from < edge.target) {
				if (!edge.weight.has_value()) {
					throw std::invalid_argument("Все рёбра должны иметь вес");
				}
				edges.push_back({from, edge});
			}
			vertices.insert(edge.target);
		}
	}

	std::sort(
		edges.begin(),
		edges.end(),
		[](const auto& l, const auto& r) {
			return (l.second <=> r.second) == std::partial_ordering::less;
		}
	);

	UnionFind uf;
	for (const auto& vertex : vertices) {
		uf.makeSet(vertex);
	}

	Graph mst(Graph::Type::UNDIRECTED);
	
	for (const auto& vertex : vertices) {
		mst.addVertex(vertex);
	}

	int edgesAdded = 0;
	const int targetEdges = static_cast<int>(vertices.size()) - 1;

	for (const auto& [from, edge] : edges) {
		if (uf.unite(from, edge.target)) {
			mst.addEdge(from, edge);
			edgesAdded++;
			
			if (edgesAdded == targetEdges) {
				break; // MST построено
			}
		}
	}

	if (edgesAdded < targetEdges) {
		throw std::runtime_error("Граф не связный, невозможно построить остовное дерево");
	}

	return mst;
}

double getMSTWeight(const Graph& mst) {
	double totalWeight = 0.0;	
	for (const auto& [from, edgeList] : mst.getAdjList()) {
		for (const auto& edge : edgeList) {
			if (from < edge.target && edge.weight.has_value()) {
				totalWeight += edge.weight.value();
			}
		}
	}	
	return totalWeight;
}

