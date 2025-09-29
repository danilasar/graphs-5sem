#include "graph.h"
#include <algorithm>
#include <fstream>
#include <iostream>

Graph::Graph(Type type) : graphType(type) {}

Graph::Graph(const std::string& filename) {
	loadFromFile(filename);
}

Graph::Graph(const Graph& other)
	: graphType(other.graphType), adjList(other.adjList) {}

Graph::Graph(Type type, const std::vector<std::pair<std::string, std::vector<Edge>>>& data)
	: graphType(type) {
	for (auto& [v, edges] : data)
		adjList[v] = edges;
}

bool Graph::addVertex(const std::string& v) {
	if (adjList.find(v) != adjList.end())
		return false;
	adjList[v] = {};
	return true;
}

bool Graph::addEdge(const std::string& from, const std::string& to,
					std::optional<std::string> label,
					std::optional<double> weight) {
	if (adjList.find(from) == adjList.end() || adjList.find(to) == adjList.end())
		return false;
	adjList[from].push_back({to, label, weight});
	if (graphType == Type::UNDIRECTED && from != to)
		adjList[to].push_back({from, label, weight});
	return true;
}

bool Graph::removeVertex(const std::string& v) {
	if (adjList.find(v) == adjList.end())
		return false;
	adjList.erase(v);
	for (auto& [u, edges] : adjList)
		edges.erase(std::remove_if(edges.begin(), edges.end(),
								   [&](const Edge& e) { return e.target == v; }),
					edges.end());
	return true;
}

bool Graph::removeEdge(const std::string& from, const std::string& to) {
	if (adjList.find(from) == adjList.end())
		return false;
	auto& edges = adjList[from];
	auto sz = edges.size();
	edges.erase(std::remove_if(edges.begin(), edges.end(),
							   [&](const Edge& e) { return e.target == to; }),
				edges.end());
	bool removed = sz != edges.size();
	if (graphType == Type::UNDIRECTED && adjList.find(to) != adjList.end()) {
		auto& e2 = adjList[to];
		sz = e2.size();
		e2.erase(std::remove_if(e2.begin(), e2.end(),
							   [&](const Edge& e) { return e.target == from; }),
				 e2.end());
		removed = removed || (sz != e2.size());
	}
	return removed;
}

void Graph::printAdjacencyList(const std::string& filename) const {
	std::ostream* out = &std::cout;
	std::ofstream fout;
	if (!filename.empty()) {
		fout.open(filename);
		out = &fout;
	}
	for (const auto& [v, edges] : adjList) {
		*out << v << ": ";
		for (const auto& e : edges) {
			*out << e.target;
			if (e.label) *out << "(" << *e.label << ")";
			if (e.weight) *out << "[" << *e.weight << "]";
			*out << "; ";
		}
		*out << std::endl;
	}
}

void Graph::saveToFile(const std::string& filename) const {
	std::ofstream fout(filename);
	fout << (graphType == Type::DIRECTED ? "DIRECTED\n" : "UNDIRECTED\n");
	for (const auto& [v, edges] : adjList) {
		for (const auto& e : edges) {
			fout << v << " " << e.target;
			if (e.label) fout << " " << *e.label;
			if (e.weight) fout << " " << *e.weight;
			fout << "\n";
		}
	}
}

bool Graph::loadFromFile(const std::string& filename) {
	adjList.clear();
	std::ifstream fin(filename);
	std::string type;
	if (!(fin >> type)) return false;
	graphType = (type == "UNDIRECTED") ? Type::UNDIRECTED : Type::DIRECTED;
	std::string from, to, label;
	double weight;
	while (fin >> from >> to) {
		std::optional<std::string> optLabel;
		std::optional<double> optWeight;
		if (fin.peek() == ' ') {
			fin >> label;
			optLabel = label;
		}
		if (fin.peek() == ' ') {
			fin >> weight;
			optWeight = weight;
		}
		if (adjList.find(from) == adjList.end()) adjList[from] = {};
		if (adjList.find(to) == adjList.end()) adjList[to] = {};
		adjList[from].push_back({to, optLabel, optWeight});
		if (graphType == Type::UNDIRECTED && from != to)
			adjList[to].push_back({from, optLabel, optWeight});
	}
	return true;
}

