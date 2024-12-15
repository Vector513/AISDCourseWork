#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>

class Graph {
public:
    struct Edge {
        int vertex1;
        int vertex2;
        int weight;
    };

    bool isEmpty() const;

    void clear();

    bool fromFile(const std::string& fileName);

    bool fromAdjMatrix(const std::vector<std::string>& newVertices, const std::vector<std::vector<int>>& adjMatrix);

    int getVertexCount() const;

    int getEdgeCount() const;

    const std::string getVertexName(int index) const;

    const std::vector<Edge>& getEdges() const;

    void addEdge(const std::string& vertex1, const std::string& vertex2, int weight);

    void addEdge(int fromIndex, int toIndex, int weight);

    void printGraph() const;

    void printAdjMatrix() const;

    std::vector<std::vector<std::pair<int, int>>> getAdjList() const;

    void printAdjList() const;

    void printIncidenceMatrix() const;

    void depthFirstSearch(const std::string& startVertex) const;

    void breadthFirstSearch(const std::string& startVertex) const;

    void exportToDOT(const std::string& filename);

private:
    std::unordered_map<std::string, int> vertexIndexMap;
    std::vector<std::string> vertices;
    std::vector<Edge> edges;
};

bool Graph::isEmpty() const 
{
    return vertices.size() == 0;
}

void Graph::clear() {
    vertices.clear();
    edges.clear();
    vertexIndexMap.clear();
}

bool Graph::fromFile(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл: " << fileName << '\n';
        return false;
    }

    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "Ошибка: Файл пуст или некорректен" << '\n';
        return false;
    }

    std::istringstream headerStream(line);
    std::vector<std::string> newVertices;
    std::string vertex;
    while (headerStream >> vertex) {
        newVertices.push_back(vertex);
    }

    size_t n = newVertices.size();
    std::vector<std::vector<int>> adjMatrix;
    size_t rowIndex = 0;

    while (std::getline(file, line) && adjMatrix.size() < n) {
        std::istringstream rowStream(line);
        std::vector<int> row;
        int value;

        while (rowStream >> value) {
            row.push_back(value);
        }

        if (row.size() != n) {
            std::cerr << "Ошибка: Некорректное количество элементов в строке " << rowIndex << '\n';
            return false;
        }

        adjMatrix.push_back(row);
        ++rowIndex;
    }

    if (adjMatrix.size() != n) {
        std::cerr << "Ошибка: Матрица не квадратная" << '\n';
        return false;
    }

    return fromAdjMatrix(newVertices, adjMatrix);
}

bool Graph::fromAdjMatrix(const std::vector<std::string>& newVertices, const std::vector<std::vector<int>>& adjMatrix)
{
    clear();
    vertices = newVertices;

    for (size_t i = 0; i < vertices.size(); ++i) {
        vertexIndexMap[vertices[i]] = i;
    }

    int n = adjMatrix.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            if (adjMatrix[i][j] != 0) {
                edges.push_back(Edge{ i, j, adjMatrix[i][j] });
            }
        }
    }
    return true;
}

int Graph::getVertexCount() const {
    return vertices.size();
}

int Graph::getEdgeCount() const {
    return edges.size();
}

const std::string Graph::getVertexName(int index) const 
{
    return vertices[index];
}

const std::vector<Graph::Edge>& Graph::getEdges() const {
    return edges;
}

void Graph::addEdge(const std::string& vertex1, const std::string& vertex2, int weight) 
{
    if (vertexIndexMap.find(vertex1) == vertexIndexMap.end()) {
        vertexIndexMap[vertex1] = vertices.size();
        vertices.push_back(vertex1);
    }

    if (vertexIndexMap.find(vertex2) == vertexIndexMap.end()) {
        vertexIndexMap[vertex2] = vertices.size();
        vertices.push_back(vertex2);
    }

    int vim1 = vertexIndexMap[vertex1];
    int vim2 = vertexIndexMap[vertex2];

    edges.push_back(Edge{ vim1, vim2, weight });
}

void Graph::addEdge(int vertex1, int vertex2, int weight) 
{
    if (vertex1 < 0 || vertex1 >= vertices.size() || vertex2 < 0 || vertex2 >= vertices.size()) {
        std::cerr << "Error: Invalid vertex indices!" << '\n';
        return;
    }

    edges.push_back(Edge{ vertex1, vertex2, weight });
}

void Graph::printGraph() const {
    std::cout << "Вершины:\n";
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << i + 1 << ": " << vertices[i] << "\n";
    }

    int totalWeight = 0;
    for (const auto& edge : edges) {
        totalWeight += edge.weight;
    }

    std::cout << "\nРёбра:\n";
    for (const auto& edge : edges) {
        std::cout << vertices[edge.vertex1] << " -- " << vertices[edge.vertex2] << " (Вес: " << edge.weight << ")\n";
    }

    std::cout << "\nОбщий вес графа: " << totalWeight << "\n";
}


void Graph::printAdjMatrix() const 
{
    size_t n = vertices.size();
    if (n == 0) {
        std::cout << "Граф пуст.\n";
        return;
    }

    std::vector<std::vector<int>> adjMatrix(n, std::vector<int>(n, 0));
    for (const auto& edge : edges) {
        adjMatrix[edge.vertex1][edge.vertex2] = edge.weight;
        adjMatrix[edge.vertex2][edge.vertex1] = edge.weight;
    }

    std::cout << "Матрица смежности:\n";

    std::cout << "  ";
    for (const auto& vertex : vertices) {
        std::cout << vertex << " ";
    }
    std::cout << "\n";

    for (size_t i = 0; i < n; ++i) {
        std::cout << vertices[i] << " ";
        for (int val : adjMatrix[i]) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}

std::vector<std::vector<std::pair<int, int>>> Graph::getAdjList() const
{
    std::vector<std::vector<std::pair<int, int>>> adjList(vertices.size());
    for (const auto& edge : edges) {
        adjList[edge.vertex1].emplace_back(edge.vertex2, edge.weight);
        adjList[edge.vertex2].emplace_back(edge.vertex1, edge.weight);
    }
    return adjList;
}

void Graph::printAdjList() const
{
    if (vertices.empty()) {
        std::cout << "Граф пуст.\n";
        return;
    }

    std::vector<std::vector<std::pair<int, int>>> adjList = getAdjList();

    std::cout << "Список смежности:\n";
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << vertices[i] << ": ";
        for (const auto& neighbor : adjList[i]) {
            std::cout << vertices[neighbor.first] << "(" << neighbor.second << ") ";
        }
        std::cout << "\n";
    }
}

void Graph::printIncidenceMatrix() const {
    size_t n = vertices.size();
    size_t m = edges.size();
    if (n == 0 || m == 0) {
        std::cout << "Граф пуст.\n";
        return;
    }

    std::vector<std::vector<int>> incidenceMatrix(n, std::vector<int>(m, 0));
    for (size_t i = 0; i < m; ++i) {
        incidenceMatrix[edges[i].vertex1][i] = 1;
        incidenceMatrix[edges[i].vertex2][i] = -1;
    }

    std::cout << "Матрица инцидентности:\n";
    for (const auto& row : incidenceMatrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}

void Graph::depthFirstSearch(const std::string& startVertex) const 
{
    auto it = vertexIndexMap.find(startVertex);
    if (it == vertexIndexMap.end()) {
        std::cerr << "Ошибка: Вершина " << startVertex << " не найдена." << '\n';
        return;
    }

    int startVertexIndex = it->second;

    std::unordered_set<int> visited;
    std::stack<int> stack;

    stack.push(startVertexIndex);
    std::cout << "Обход в глубину (DFS): ";

    std::vector<std::vector<std::pair<int, int>>> adjList = getAdjList();

    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();

        if (visited.find(current) == visited.end()) {
            std::cout << vertices[current] << " ";
            visited.insert(current);
        }

        for (const auto& neighbor : adjList[current]) {
            if (visited.find(neighbor.first) == visited.end()) {
                stack.push(neighbor.first);
            }
        }
    }
    std::cout << '\n';
}

void Graph::breadthFirstSearch(const std::string& startVertex) const {
    auto it = vertexIndexMap.find(startVertex);
    if (it == vertexIndexMap.end()) {
        std::cerr << "Ошибка: Вершина " << startVertex << " не найдена." << '\n';
        return;
    }

    int startVertexIndex = it->second;
    std::unordered_set<int> visited;
    std::queue<int> queue;
    queue.push(startVertexIndex);
    visited.insert(startVertexIndex);

    std::cout << "Обход в ширину (BFS): ";

    std::vector<std::vector<std::pair<int, int>>> adjList = getAdjList();

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        std::cout << vertices[current] << " ";

        for (const auto& neighbor : adjList[current]) {
            if (visited.find(neighbor.first) == visited.end()) {
                queue.push(neighbor.first);
                visited.insert(neighbor.first);
            }
        }
    }
    std::cout << '\n';
}


void Graph::exportToDOT(const std::string& pathToFile) {
    std::ofstream file(pathToFile);

    if (!file.is_open()) {
        std::cerr << "Ошибка: Unable to open file " << pathToFile << '\n';
        return;
    }

    file << "graph G {" << '\n';

    for (const auto& edge : edges) {
        file << "  " << vertices[edge.vertex1] << " -- " << vertices[edge.vertex2]
            << " [label=\"" << edge.weight << "\"]" << ";\n";
    }

    file << "}" << '\n';
    file.close();

    std::cout << "Граф экспортирован " << pathToFile << " в DOT формат." << '\n';
}

#endif // GRAPH_H