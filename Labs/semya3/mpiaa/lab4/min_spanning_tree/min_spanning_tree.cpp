#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <random>
#include <chrono>
#include <queue>
#include <algorithm>

using namespace std;
using namespace std::chrono;

const double INF = numeric_limits<double>::infinity();

struct Edge {
    int u, v;
    double weight;
};

// Генерация случайного связного графа
vector<Edge> generateRandomConnectedGraph(int numVertices, int numEdges) {
    if (numEdges < numVertices - 1) {
        throw runtime_error("Невозможно создать связный граф с таким количеством ребер.");
    }

    vector<Edge> edges;
    vector<pair<int, int>> added_edges;

    // Создаем остовное дерево
    for (int i = 1; i < numVertices; ++i) {
        edges.push_back({i - 1, i, (double)rand() / RAND_MAX}); // случайный вес
        added_edges.push_back({i-1,i});
    }


    // Добавляем оставшиеся ребра
    while (edges.size() < numEdges) {
        int u = rand() % numVertices;
        int v = rand() % numVertices;
        if (u != v) {
            bool already_exists = false;
            for(auto e : added_edges){
                if((e.first == u && e.second == v) || (e.first == v && e.second == u)){
                    already_exists = true;
                    break;
                }
            }
            if (!already_exists) {
                edges.push_back({u, v, (double)rand() / RAND_MAX}); // случайный вес
                added_edges.push_back({u,v});
            }
        }
    }
    return edges;
}


vector<pair<int, int>> prim(const vector<Edge>& edges, int numVertices, int startVertex) {
    map<int, double> minWeight;
    map<int, int> parent;
    vector<bool> inMST(numVertices, false);

    for (int i = 0; i < numVertices; ++i) {
        minWeight[i] = INF;
    }
    minWeight[startVertex] = 0;

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push({0, startVertex});


    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true;

        for (const auto& edge : edges) {
            int v = (edge.u == u) ? edge.v : ((edge.v == u) ? edge.u : -1);
            if (v != -1 && !inMST[v] && minWeight[v] > edge.weight) {
                minWeight[v] = edge.weight;
                parent[v] = u;
                pq.push({minWeight[v], v});
            }
        }
    }

    vector<pair<int, int>> mst;
    for (int i = 0; i < numVertices; ++i) {
        if (i != startVertex) {
            mst.push_back({i, parent[i]});
        }
    }
    return mst;
}


int main() {
    srand(time(0));


    for (int numVertices : {10, 100, 1000, 10000, 100000, 1000000}) {
        for (int numEdges : {numVertices, 2 * numVertices, 3 * numVertices}) {
            vector<Edge> graph =  generateRandomConnectedGraph(numVertices, numEdges);
            auto start = steady_clock::now();
            vector<pair<int, int>> mst = prim(graph, numVertices, 0);
            auto end = steady_clock::now();
            chrono::duration<double> duration = end - start;

            cout << "Граф: " << numVertices << " вершин, " << numEdges << " ребер" << endl;
            cout << "Время работы алгоритма Прима: " << duration.count() << " c." << endl;
            cout << "Количество ребер в MST: " << mst.size() << endl << endl;
    }
    }

    return 0;
}