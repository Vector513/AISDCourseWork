#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "DisjointSet.h"
#include "Graph.h"

class Kruskal {
public:
    Graph findMST(const Graph& graph);
};

Graph Kruskal::findMST(const Graph& graph)
{
    const std::vector<Graph::Edge>& edges = graph.getEdges();
    int numVertices = graph.getVertexCount();

    std::vector<Graph::Edge> sortedEdges = edges;
    std::sort(sortedEdges.begin(), sortedEdges.end(), [](const Graph::Edge& a, const Graph::Edge& b) {
        return a.weight < b.weight;
        });

    DisjointSet ds(numVertices);

    Graph mstGraph;
    int totalWeight = 0;

    for (const auto& edge : sortedEdges) {
        int vertex1 = edge.vertex1;
        int vertex2 = edge.vertex2;

        if (ds.find(vertex1) != ds.find(vertex2)) {
            ds.unionSets(vertex1, vertex2);
            mstGraph.addEdge(graph.getVertexName(vertex1), graph.getVertexName(vertex2), edge.weight);
            totalWeight += edge.weight;
            std::cout << "��������� �����: " << graph.getVertexName(vertex1) << " - " << graph.getVertexName(vertex2)
                << ", ���: " << edge.weight << '\n';
        }
    }

    std::cout << "\n����� � MST:\n";
    const auto& mstEdges = mstGraph.getEdges();
    for (const auto& edge : mstEdges) {
        std::cout << "�����: " << graph.getVertexName(edge.vertex1) << " - " << graph.getVertexName(edge.vertex2)
            << ", ���: " << edge.weight << '\n';
    }

    std::cout << "\n����� ��� MST: " << totalWeight << '\n';

    return mstGraph;
}


#endif // KRUSKAL_H
