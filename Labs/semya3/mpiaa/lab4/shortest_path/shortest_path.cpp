#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <limits> // for numeric_limits
#include <queue>
#include <random>
#include <chrono>

using namespace std::chrono;
using namespace std;

const double INF = numeric_limits<double>::infinity();

// Структура для представления ребра
struct Edge {
    int to;
    double weight;
};

// Функция для построения пути
vector<int> BuildPath(const map<int, int>& parent, int start, int end) {
    vector<int> path;
    if (parent.find(end) == parent.end()) return path; // Путь не найден

    int curr = end;
    while (curr != start) {
        path.push_back(curr);
        curr = parent.at(curr);
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

// Алгоритм Дейкстры
vector<int> shortest_path(const vector<vector<Edge>>& graph, int start, int end) {
    int n = graph.size();
    map<int, double> distance;
    map<int, int> parent;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

    for (int i = 0; i < n; ++i) {
        distance[i] = INF;
    }
    distance[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (u == end) {
            return BuildPath(parent, start, end);
        }

        for (const Edge& edge : graph[u]) {
            if (distance[edge.to] > distance[u] + edge.weight) {
                distance[edge.to] = distance[u] + edge.weight;
                parent[edge.to] = u;
                pq.push({distance[edge.to], edge.to});
            }
        }
    }

    return {}; // Путь не найден
}


// Генерация случайного связного графа
vector<vector<Edge>> generate_random_graph(int num_vertices, int num_edges) {
    vector<vector<Edge>> graph(num_vertices);
    vector<pair<int,int>> edges;
    
    // Создаем минимальное остовное дерево для связности
    for (int i = 1; i < num_vertices; ++i) {
        edges.push_back({i-1, i});
    }
    
    // Добавляем случайные ребра
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist_vertices(0, num_vertices - 1);
    uniform_int_distribution<> dist_weight(1, 10); // Веса ребер от 1 до 10

    while (edges.size() < num_edges) {
        int u = dist_vertices(gen);
        int v = dist_vertices(gen);
        bool exists = false;
        for (const auto& e : edges) {
            if ((e.first == u && e.second == v) || (e.first == v && e.second == u)) {
                exists = true;
                break;
            }
        }
        if (!exists && u != v) {
            edges.push_back({u, v});
        }
    }

    for (const auto& edge : edges) {
        Edge first, second;
        first.to = edge.second;
        first.weight = dist_weight(gen);
        second.to = edge.first;
        second.weight = dist_weight(gen);
        graph[edge.first].push_back(first);
        graph[edge.second].push_back(second); // Неориентированный граф
    }

    return graph;
}


int main() {
    // Пример использования
    vector<vector<Edge>> graph = {
        {{1, 4}, {7, 8}},
        {{0, 4}, {2, 8}, {7,11}},
        {{1, 8}, {3, 7}, {5,4}},
        {{2, 7}, {4, 9}, {5,10}},
        {{3, 9}, {5, 14}},
        {{2, 4}, {3, 10}, {4, 14}, {6,2}},
        {{5,2}},
        {{0,8}, {1,11}, {6,1}}
    };


    int start_node = 0;
    int end_node = 6;
    auto start = steady_clock::now();
    vector<int> path = shortest_path(graph, start_node, end_node);
    auto end = steady_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Кратчайший путь от" << start_node << " до " << end_node << ": ";
    if (path.empty()) {
        cout << "Путь не найден" << endl;
    } else {
        for (int i = 0; i < path.size(); ++i) {
            cout << path[i] << (i == path.size() - 1 ? "" : " -> ");
        }
        cout << endl;
    }
    cout << "Время: " << duration.count() << " с." << endl;


    // Замер времени на случайных графах
    for (int num_vertices : {10000, 1000000}) {
        for (int num_edges : {num_vertices, 2 * num_vertices, 3 * num_vertices}) {
            auto graph = generate_random_graph(num_vertices, num_edges);
            double total_time = 0;
            int num_tests = 5; // Количество тестов для усреднения
            for (int i = 0; i < num_tests; ++i) {
                int start_node = rand() % num_vertices;
                int end_node = rand() % num_vertices;
                start = steady_clock::now();
                shortest_path(graph, start_node, end_node);
                end = steady_clock::now();
                chrono::duration<double> duration = end - start;
                total_time += (duration).count();
            }
            cout << "Ребра: " << num_vertices << ", Вершины: " << num_edges << ", Среднее время: " << (double)total_time / num_tests << " c." << endl;
        }
    }

    return 0;
}