#include <Windows.h>
#include "Application.h"

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Application app;
    Graph graph;
    Kruskal kruskal;

    app.exec(graph, kruskal);

    return 0;
}
