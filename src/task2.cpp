#include "graph.h"

std::vector<std::string> getIsolatedVertices(const Graph& g) {
    std::vector<std::string> isolated;
    std::unordered_map<std::string, int> inDegree;

	auto& adjList = g.getAdjList();

    // Инициализация входящих степеней для всех вершин
    for (const auto& [vertex, edges] : adjList) {
        inDegree[vertex] = 0;
    }

    // Подсчет входящих степеней
    for (const auto& [vertex, edges] : adjList) {
        for (const auto& edge : edges) {
            inDegree[edge.target]++;
        }
    }

    // Поиск изолированных вершин
    for (const auto& [vertex, edges] : adjList) {
        // Проверка: out-degree = 0 (нет исходящих ребер) и in-degree = 0
        if (edges.empty() && inDegree[vertex] == 0) {
            isolated.push_back(vertex);
        }
    }

    return isolated;
}

void printIsolatedVertices(const Graph& g) {
    auto isolated = getIsolatedVertices(g);

    if (isolated.empty()) {
        std::cout << "Изолированных вершин не найдено.\n";
    } else {
        std::cout << "Изолированные вершины (степень 0):\n";
        for (const auto& vertex : isolated) {
            std::cout << "  " << vertex << "\n";
        }
    }
}

