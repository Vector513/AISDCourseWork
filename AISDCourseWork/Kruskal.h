#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "DisjointSet.h"
#include "Graph.h"

class Kruskal {
private:
    void Heapify(std::vector<Graph::Edge>& edges, int heapSize, int i);
    void BuildHeap(std::vector<Graph::Edge>& edges);
    void HeapSort(std::vector<Graph::Edge>& edges);

public:
    Graph findMST(const Graph& graph);
};

void Kruskal::Heapify(std::vector<Graph::Edge>& edges, int heapSize, int i) 
{
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < heapSize && edges[left].weight > edges[largest].weight) {
            largest = left;
        }

        if (right < heapSize && edges[right].weight > edges[largest].weight) {
            largest = right;
        }

        if (largest != i) {
            std::swap(edges[i], edges[largest]);
            i = largest;
        }
        else {
            break;
        }
    }
}

void Kruskal::BuildHeap(std::vector<Graph::Edge>& edges)
{
    for (int i = (edges.size() - 1) / 2; i >= 0; --i) {
        Heapify(edges, edges.size(), i);
    }
}

void Kruskal::HeapSort(std::vector<Graph::Edge>& edges) 
{
    BuildHeap(edges);

    for (int i = edges.size() - 1; i > 0; --i) {
        std::swap(edges[0], edges[i]);
        Heapify(edges, i, 0);
    }
}

Graph Kruskal::findMST(const Graph& graph)
{
    const std::vector<Graph::Edge>& edges = graph.getEdges();
    int numVertices = graph.getVertexCount();

    std::vector<Graph::Edge> sortedEdges = edges;
    HeapSort(sortedEdges);

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
            std::cout << "Добавлено ребро: " << graph.getVertexName(vertex1) << " - " << graph.getVertexName(vertex2)
                << ", Вес: " << edge.weight << '\n';
        }
    }

    std::cout << "\nРебра в MST:\n";
    const auto& mstEdges = mstGraph.getEdges();
    for (const auto& edge : mstEdges) {
        std::cout << "Ребро: " << graph.getVertexName(edge.vertex1) << " - " << graph.getVertexName(edge.vertex2)
            << ", Вес: " << edge.weight << '\n';
    }

    std::cout << "\nОбщий вес MST: " << totalWeight << '\n';

    return mstGraph;
}


#endif // KRUSKAL_H
