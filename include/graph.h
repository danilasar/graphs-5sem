#pragma once
#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <optional>
#include <filesystem>

struct Edge {
	std::string target;
	std::optional<std::string> label;
	std::optional<double> weight;
	std::partial_ordering operator<=>(const Edge& other) const {
		double lw = weight.has_value() ? weight.value() : 0.0,
		       rw = other.weight.has_value() ? other.weight.value() : 0.0;
		return lw <=> rw;
	}
};

class Graph {
public:
	// Тип графа
	enum class Type { DIRECTED, UNDIRECTED };

private:
	Type graphType;
	std::unordered_map<std::string, std::vector<Edge>> adjList;

public:
	Graph(Type type = Type::DIRECTED);

	explicit Graph(const std::string& filename);

	Graph(const Graph& other);

	Graph(Type type, const std::vector<std::pair<std::string, std::vector<Edge>>>& data);

	const std::unordered_map<std::string, std::vector<Edge>>& getAdjList() const;

	Type getGraphType() const;

	bool addVertex(const std::string& v);

	bool addEdge(const std::string& from, const std::string& to,
				 std::optional<std::string> label = {},
				 std::optional<double> weight = {});

	bool addEdge(const std::string& from, Edge e);

	bool removeVertex(const std::string& v);

	bool removeEdge(const std::string& from, const std::string& to);

	void printAdjacencyList(const std::string& filename = "") const;

	void saveToFile(const std::string& filename) const;

	bool loadFromFile(const std::string& filename);
};

