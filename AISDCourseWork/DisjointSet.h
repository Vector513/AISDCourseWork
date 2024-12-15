#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <vector>

class DisjointSet {
public:
    // Конструктор, инициализирующий родительский элемент и ранг для всех элементов
    DisjointSet(int size);

    // Находит корень множества, сжатие пути для улучшения эффективности
    int find(int x);

    // Объединяет два множества, объединяя их по рангу
    void unionSets(int x, int y);

private:
    std::vector<int> parent;  // Родительский элемент для каждого элемента
    std::vector<int> rank;    // Ранг (или высота) каждого элемента
};

// Конструктор
DisjointSet::DisjointSet(int size) : parent(size), rank(size, 0) {
    for (int i = 0; i < size; ++i) {
        parent[i] = i;  // Каждый элемент сам себе родитель (инициализация)
    }
}

// Функция поиска с сжатием пути
int DisjointSet::find(int x) {
    if (x < 0 || x >= parent.size()) {
        throw std::out_of_range("DisjointSet: index out of range in find()");
    }

    // Если элемент не является своим собственным родителем
    if (parent[x] != x) {
        // Рекурсивный вызов с сжатием пути
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

// Функция объединения двух множеств
void DisjointSet::unionSets(int x, int y) {
    if (x < 0 || x >= parent.size() || y < 0 || y >= parent.size()) {
        throw std::out_of_range("DisjointSet: index out of range in unionSets()");
    }

    int rootX = find(x);  // Находим корень для x
    int rootY = find(y);  // Находим корень для y

    if (rootX != rootY) {  // Если корни разные, объединяем множества
        // Объединение по рангу
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;  // rootY становится родителем
        }
        else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;  // rootX становится родителем
        }
        else {
            parent[rootY] = rootX;  // Если ранги равны, rootX становится родителем
            ++rank[rootX];  // Увеличиваем ранг
        }
    }
}

#endif // DISJOINTSET_H
