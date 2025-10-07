#include <vector>
#include <string>
// Задание 2.7. Список смежности Ia
std::vector<std::string> getIsolatedVertices(const Graph& g);
void printIsolatedVertices(const Graph& g);
// Задание 3.8.
void printVerticesWithGreaterOutDegree(const Graph& g);

// Задание 4.9.
Graph symmetricDifference(const Graph& g1, const Graph& g2);

// Задание 5.12.
bool isStronglyConnected(const Graph& graph);

// Задание 6.18
void classifyGraph(const Graph& g);

// Задание 7 (Краскал)
Graph kruskalMST(const Graph& graph);
double getMSTWeight(const Graph& mst);
