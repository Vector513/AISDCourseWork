#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <limits>
#include <fstream>

#include "Graph.h"
#include "Kruskal.h"

class Application {
public:
    Application();
    ~Application();

    void exec(Graph& graph, Kruskal& kruskal);

private:
    std::string pathToCurrentDir = "C:\\LETI\\AISD\\AISDCourseWork\\AISDCourseWork\\AISDCourseWork\\";
    std::string pathToAdjMatrix = pathToCurrentDir + "Adjacency matrix.txt";
    std::string pathToGraphDot = pathToCurrentDir + "graph.dot";
    std::string pathToGraphPng = pathToCurrentDir + "graph.png";
    std::string pathToMSTDot = pathToCurrentDir + "MST.dot";
    std::string pathToMSTPng = pathToCurrentDir + "MST.png";

    Graph MST;

    void generatePngFromDot(const std::string& dotFile, const std::string& pngFile);
    void openPNG(const std::string& filename);
};

void Application::generatePngFromDot(const std::string& pathToDotFile, const std::string& pathToPngFile) 
{
    std::string command = "dot -Tpng \"" + pathToDotFile + "\" -o \"" + pathToPngFile + "\"";
    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "PNG файл создан: " << pathToPngFile << '\n';
    }
    else {
        std::cerr << "Ошибка при генерации PNG файла." << '\n';
    }
}

void Application::openPNG(const std::string& pathToFile) 
{
    std::string command = "start " + pathToFile;
    system(command.c_str());
}

Application::Application() {}

Application::~Application() {}

void Application::exec(Graph& graph, Kruskal& kruskal)
{
    const char separator[] = "------------------------------------------------------------------------------------------------------------------------";
    const char commands[] =
        "1) Инициализировать граф через матрицу смежности в файле\n"
        "2) Вывести матрицу смежности\n"
        "3) Вывести список смежности\n"
        "4) Вывести матрицу инцидентности\n"
        "5) Обход графа в глубину\n"
        "6) Обход графа в ширину\n"
        "7) Построить минимальное остовное дерево\n"
        "fp) Вывести путь к матрице смежности по умолчанию\n"
        "fc) Изменить путь к матрице смежности\n"
        "sg) Открыть граф в png формате\n"
        "smst) Открыть минимальное остовное дерево в png формате\n"
        "c) Вывести список комманд\n"
        "e) Выход из программы\n";

    std::string command = "c";

    do {
        if (command == "e") {
            std::cout << "Программа завершена пользователем\n";
            break;
        }
        else if (command == "c") {
            std::cout << commands;
        }
        else if (command == "sg") {
            if (!graph.isEmpty()) {
                graph.printGraph();
                std::cout << '\n';
                graph.exportToDOT(pathToGraphDot);
                generatePngFromDot(pathToGraphDot, pathToGraphPng);
                openPNG(pathToGraphPng);
            }
            else {
                std::cout << "Граф пуст.\n";
            }
        }
        else if (command == "smst") {
            if (!MST.isEmpty()) {
                MST.printGraph();
                std::cout << '\n';
                MST.exportToDOT(pathToMSTDot);
                generatePngFromDot(pathToMSTDot, pathToMSTPng);
                openPNG(pathToMSTPng);
            }
            else {
                std::cout << "Минимальное остовное дерево не было сгенерировано\n";
            }
        }
        else if (command == "fp") {
            if (!pathToAdjMatrix.empty()) {
                std::cout << "Путь к матрице смежности: " << pathToAdjMatrix << '\n';
            }
            else {
                std::cout << "Путь к матрице смежности пустой!\n";
            }
        }
        else if (command == "fc") {
            std::cout << "Введите новый путь к матрице смежности: ";
            std::getline(std::cin, pathToAdjMatrix);
        }
        else if (command == "1") {
            if (graph.fromFile(pathToAdjMatrix)) {
                std::cout << "Граф был успешно построен на основе матрицы смежности\n";
            }
        }
        else if (command == "2") {
            graph.printAdjMatrix();
        }
        else if (command == "3") {
            graph.printAdjList();
        }
        else if (command == "4") {
            graph.printIncidenceMatrix();
        }
        else if (command == "5") {
            graph.depthFirstSearch("A");
        }
        else if (command == "6") {
            graph.breadthFirstSearch("A");
        }
        else if (command == "7") {
            MST = kruskal.findMST(graph);
        }

        std::cout << separator << '\n';
        std::cout << "Введите команду: ";
        std::getline(std::cin, command);

        if (std::cin.fail()) {
            std::cin.clear();
            std::cout << "Некорректный ввод! Попробуйте снова.\n";
            command = "c";
        }

        std::cout << '\n';

    } while (true);
}

#endif // APPLICATION_H
