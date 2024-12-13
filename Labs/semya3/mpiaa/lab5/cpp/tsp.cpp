#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

// Структура для представления ребра
struct Edge {
    int to;
    int weight;
};

// Функция для вычисления длины пути
int pathLength(const vector<vector<Edge>>& graph, const vector<int>& path) {
    int length = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int u = path[i];
        int v = path[i + 1];
        for (const auto& edge : graph[u]) {
            if (edge.to == v) {
                length += edge.weight;
                break;
            }
        }
    }
    int u = path.back();
    int v = path.front();
    for (const auto& edge : graph[u]) {
        if (edge.to == v) {
            length += edge.weight;
            break;
        }
    }
    return length;
}

// Генератор случайных полных графов
vector<vector<Edge>> generateRandomCompleteGraph(int numVertices, int maxWeight) {
    vector<vector<Edge>> graph(numVertices);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, maxWeight); // Веса ребер от 1 до maxWeight

    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            int weight = distrib(gen);
            graph[i].push_back({j, weight});
            graph[j].push_back({i, weight}); // Неориентированный граф
    }
    }
    return graph;
}


// Алгоритм полного перебора
vector<int> tsp_bruteforce(const vector<vector<Edge>>& graph, int start) {
    int n = graph.size();
    vector<int> bestPath;
    int minLength = numeric_limits<int>::max();

    vector<int> permutation(n);
    for (int i = 0; i < n; ++i) permutation[i] = i;

    do {
        if (permutation[0] != start) continue;
        vector<int> currentPath;
        bool valid = true;
        for (int i = 0; i < n; ++i) {
            currentPath.push_back(permutation[i]);
        }
        int currentLength = pathLength(graph, currentPath);
        if (currentLength < minLength) {
            minLength = currentLength;
            bestPath = currentPath;
        }
    } while (next_permutation(permutation.begin(), permutation.end()));

    return bestPath;
}


// Метод ветвей и границ
vector<int> MinPath(const vector<vector<Edge>>& graph, const vector<int>& P1, const vector<int>& P2) {
    return (P1.empty() || pathLength(graph, P1) > pathLength(graph, P2)) ? P2 : P1;
}


int LowerBound(const vector<vector<Edge>>& graph, const vector<int>& visited) {
    int n = graph.size();
    int lowerBound = 0;
    for (int i = 0; i < n; ++i) {
        vector<pair<int, int>> edges;
        for (const auto& edge : graph[i]) {
            edges.push_back({edge.weight, edge.to});
        }
        sort(edges.begin(), edges.end());

        int minWeight1 = numeric_limits<int>::max();
        int minWeight2 = numeric_limits<int>::max();

        for (const auto& edge : edges) {
            bool inVisited = false;
            for (size_t j = 0; j < visited.size() -1; ++j){
                if ((visited[j] == i && visited[j+1] == edge.second) || (visited[j] == edge.second && visited[j+1] == i)){
                    inVisited = true;
                    break;
                }
            }
            if (find(visited.begin(), visited.end(), edge.second) == visited.end()) {
                if (minWeight1 == numeric_limits<int>::max()) minWeight1 = edge.first;
                else if (minWeight2 == numeric_limits<int>::max()) minWeight2 = edge.first;
                
                break;
            }

        }
        lowerBound += minWeight1;
        if (minWeight2 != numeric_limits<int>::max()) lowerBound += minWeight2;
    }
    return lowerBound / 2;
}


vector<int> BnB(const vector<vector<Edge>>& graph, vector<int> visited, vector<int> bestPath) {
    if (visited.size() == graph.size()) {
        return MinPath(graph, bestPath, visited);
    }

    int n = graph.size();
    for (int v = 0; v < n; ++v) {
        bool alreadyVisited = false;
        for (int u : visited) {
            if (u == v) {
                alreadyVisited = true;
                break;
            }
        }
        if (!alreadyVisited) {
            vector<int> vNext = visited;
            vNext.push_back(v);
            if (LowerBound(graph, vNext) < (bestPath.empty() ? numeric_limits<int>::max() : pathLength(graph, bestPath))) {
                bestPath = BnB(graph, vNext, bestPath);
            }
        }
    }
    return bestPath;
}

vector<int> tsp_bnb(const vector<vector<Edge>>& graph, int start) {
    vector<int> visited = {start};
    vector<int> bestPath; // Начальное эвристическое решение - пустое
    return BnB(graph, visited, bestPath);
}


// Жадный алгоритм
vector<int> tsp_greedy(const vector<vector<Edge>>& graph, int start) {
    int n = graph.size();
    vector<int> path = {start};
    vector<bool> visited(n, false);
    visited[start] = true;
    int current = start;

    while (path.size() < n) {
        int next = -1;
        int minWeight = numeric_limits<int>::max();
        for (const auto& edge : graph[current]) {
            if (!visited[edge.to] && edge.weight < minWeight) {
                minWeight = edge.weight;
                next = edge.to;
            }
        }
        path.push_back(next);
        visited[next] = true;
        current = next;
    }
    return path;
}


int main() {
    int numVertices = 8;
    int maxWeight = 100;

    vector<vector<Edge>> graph = generateRandomCompleteGraph(numVertices, maxWeight);

    int startNode = 0;

    auto start = steady_clock::now();
    vector<int> bruteforcePath = tsp_bruteforce(graph, startNode);
    auto end = steady_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Полный перебор: ";
    for (int node : bruteforcePath) cout << node << " ";
    cout << "Длина пути: " << pathLength(graph, bruteforcePath) << " Время: " << duration.count() << "сек." << endl;

    start = steady_clock::now();
    vector<int> bnbPath = tsp_bnb(graph, startNode);
    end = steady_clock::now();
    duration = end - start;
    cout << "Ветви и границы: ";
    for (int node : bnbPath) cout << node << " ";
    cout << "Длина пути: " << pathLength(graph, bnbPath) << " Время: " << duration.count() << "сек." << endl;

    start = steady_clock::now();
    vector<int> greedyPath = tsp_greedy(graph, startNode);
    end = steady_clock::now();
    duration = end - start;
    cout << "Жадный: ";
    for (int node : greedyPath) cout << node << " ";
    cout << "Длина пути: " << pathLength(graph, greedyPath) << " Время: " << duration.count() << "сек." << endl;

    return 0;
}