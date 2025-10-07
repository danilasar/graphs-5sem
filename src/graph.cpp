#include "graph.h"
#include "pugixml.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstring>

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

const std::unordered_map<std::string, std::vector<Edge>>& Graph::getAdjList() const {
		return adjList;
}

Graph::Type Graph::getGraphType() const {
		return graphType;
}

bool Graph::addVertex(const std::string& v) {
	if (adjList.find(v) != adjList.end())
		return false;
	adjList[v] = {};
	return true;
}

bool Graph::addEdge(const std::string& from, Edge e) {
	auto& to = e.target;
	if (adjList.find(from) == adjList.end() || adjList.find(to) == adjList.end())
		return false;
	adjList[from].push_back(e);
	if (graphType == Type::UNDIRECTED && from != to)
		adjList[to].push_back({from, e.label, e.weight});
	return true;
}

// WARN: легаси
bool Graph::addEdge(const std::string& from, const std::string& to,
					std::optional<std::string> label,
					std::optional<double> weight) {
	return addEdge(from, Edge {to, label, weight});
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
	pugi::xml_document doc;

	auto declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "UTF-8";

	pugi::xml_node root = doc.append_child("graph");
	root.append_attribute("type") = (graphType == Type::DIRECTED ? "DIRECTED" : "UNDIRECTED");

	for (const auto& [vertexName, edges] : adjList) {
		pugi::xml_node vertexNode = root.append_child("vertex");
		vertexNode.append_attribute("name") = vertexName.c_str();

		for (const auto& edge : edges) {
			pugi::xml_node edgeNode = vertexNode.append_child("edge");
			edgeNode.append_attribute("target") = edge.target.c_str();

			if (edge.label) {
				edgeNode.append_attribute("label") = edge.label->c_str();
			}

			if (edge.weight) {
				edgeNode.append_attribute("weight") = *edge.weight;
			}
		}
	}

	doc.save_file(filename.c_str(), "  ");
}

bool Graph::loadFromFile(const std::string& filename) {
	adjList.clear();

	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(filename.c_str());
	if (!result) {
		std::cerr << "Ошибка чтения XML: " << result.description()
				  << ", по смещенрию " << result.offset << std::endl;
		return false;
	}

	pugi::xml_node root = doc.child("graph");
	if (!root) {
		std::cerr << "Ошибка чтения XML: корневой элемент не найден" << std::endl;
		return false;
	}

	const char* typeStr = root.attribute("type").value();
	if (strcmp(typeStr, "UNDIRECTED") == 0) graphType = Type::UNDIRECTED;
	else if(strcmp(typeStr, "DIRECTED") == 0) graphType = Type::DIRECTED;
	else {
			std::cerr << "Неизвестный тип графа: " << typeStr << std::endl;
			return false;
	}

	for (pugi::xml_node vertexNode : root.children("vertex")) {
		const char* vertexName = vertexNode.attribute("name").value();
		if (!vertexName || std::string(vertexName).empty()) {
			std::cerr << "Предупреждение: пропускаю безымянную вершину" << std::endl;
			continue;
		}

		std::string vName(vertexName);

		if (adjList.find(vName) == adjList.end()) {
			adjList[vName] = {};
		}

		for (pugi::xml_node edgeNode : vertexNode.children("edge")) {
			const char* targetName = edgeNode.attribute("target").value();
			if (!targetName || std::string(targetName).empty()) {
				std::cerr << "Предупреждение: пропускаю ребро из " << vName << " без целевой вершины" << std::endl;
				continue;
			}

			std::string target(targetName);

			if (adjList.find(target) == adjList.end()) {
				adjList[target] = {};
			}

			std::optional<std::string> label;
			pugi::xml_attribute labelAttr = edgeNode.attribute("label");
			if (labelAttr) {
				label = std::string(labelAttr.value());
			}

			std::optional<double> weight;
			pugi::xml_attribute weightAttr = edgeNode.attribute("weight");
			if (weightAttr) {
				weight = weightAttr.as_double();
			}

			adjList[vName].push_back({target, label, weight});

			if (graphType == Type::UNDIRECTED && vName != target) {
				adjList[target].push_back({vName, label, weight});
			}
		}
	}

	return true;
}
