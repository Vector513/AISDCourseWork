#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <vector>

class DisjointSet {
public:
    DisjointSet(int size);

    int find(int x);

    void unionSets(int x, int y);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

DisjointSet::DisjointSet(int size) : parent(size), rank(size, 0) {
    for (int i = 0; i < size; ++i) {
        parent[i] = i;
    }
}

int DisjointSet::find(int x) {
    if (x < 0 || x >= parent.size()) {
        throw std::out_of_range("DisjointSet: index out of range in find()");
    }

    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

void DisjointSet::unionSets(int x, int y) {
    if (x < 0 || x >= parent.size() || y < 0 || y >= parent.size()) {
        throw std::out_of_range("DisjointSet: index out of range in unionSets()");
    }

    int rootX = find(x);
    int rootY = find(y);

    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        }
        else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        }
        else {
            parent[rootY] = rootX;
            ++rank[rootX];
        }
    }
}

#endif // DISJOINTSET_H
