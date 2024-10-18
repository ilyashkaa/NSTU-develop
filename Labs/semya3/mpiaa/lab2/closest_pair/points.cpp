#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
using namespace std::chrono;

using namespace std;

struct Point {
    double x, y;
};

// Расстояние между двумя точками
double distance(const Point& p1, const Point& p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// Структура для хранения пары точек
struct Pair {
    Point p1, p2;
    double dist;

    Pair(const Point& p1, const Point& p2) : p1(p1), p2(p2), dist(distance(p1, p2)) {}
};

// Сравнение пар по расстоянию
bool comparePairs(const Pair& p1, const Pair& p2) {
    return p1.dist < p2.dist;
}

// Нахождение пары ближайших точек в PStripe
Pair closest_pair_between(const vector<Point>& PLeft, const vector<Point>& PRight, double d) {
    // Найти Xm - границу между PLeft и PRight по x-координате
    double Xm = (PLeft.back().x + PRight.front().x) / 2;

    // Построить PStripe - массив из точек, которые отстоят от Xm менее чем на d
    vector<Point> PStripe;
    for (const Point& p : PLeft) {
        if (abs(p.x - Xm) < d) {
            PStripe.push_back(p);
        }
    }
    for (const Point& p : PRight) {
        if (abs(p.x - Xm) < d) {
            PStripe.push_back(p);
        }
    }

    // Отсортировать точки в PStripe по y-координате
    sort(PStripe.begin(), PStripe.end(), [](const Point& p1, const Point& p2) { return p1.y < p2.y; });

    if (PStripe.size() < 2) {
    return Pair(PLeft.front(), PLeft.back()); 
    }

    // Найти пару ближайших точек в PStripe
    Pair closestPair(PStripe[0], PStripe[1]);
    for (int i = 0; i < PStripe.size() - 1; i++) {
        for (int j = i + 1; j < PStripe.size() && (PStripe[j].y - PStripe[i].y) < d; j++) {
            Pair currentPair(PStripe[i], PStripe[j]);
            if (currentPair.dist < closestPair.dist) {
                closestPair = currentPair;
            }
        }
    }
    return closestPair;
}

// Нахождение пары ближайших точек во всем наборе точек
Pair closest_pair(vector<Point> points) {
    // Если число точек меньше или равно 3, решить задачу тривиальным способом
    if (points.size() <= 1) {
        return Pair(points[0], points[0]); 
    }
    if (points.size() == 2) {
        return Pair(points[0], points[1]);
    }

    if (points.size() <= 3) {
        Pair closestPair(points[0], points[1]);
        for (int i = 0; i < points.size() - 1; i++) {
            for (int j = i + 1; j < points.size(); j++) {
                Pair currentPair(points[i], points[j]);
                if (currentPair.dist < closestPair.dist) {
                    closestPair = currentPair;
                }
            }
        }
        return closestPair;
    }

    // Отсортировать точки по x-координате
    sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) { return p1.x < p2.x; });

    // Разбить точки на две половины
    int mid = points.size() / 2;
    while(mid == points.size()) {
        mid--; 
    }
    vector<Point> PLeft(points.begin(), points.begin() + mid);
    vector<Point> PRight(points.begin() + mid, points.end());

    // Рекурсивно найти пары ближайших точек в каждой половине
    Pair pl = closest_pair(PLeft);
    Pair pr = closest_pair(PRight);

    // Найти минимальное расстояние
    double d = min(pl.dist, pr.dist);

    // Найти пару ближайших точек между половинами
    Pair pb = closest_pair_between(PLeft, PRight, d);

    // Выбрать пару с минимальным расстоянием
    return min({pl, pr, pb}, comparePairs);
}
Pair bruteForceClosestPair(const vector<Point>& points) {
    Pair closestPair(points[0], points[1]);
    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            Pair currentPair(points[i], points[j]);
            if (currentPair.dist < closestPair.dist) {
                closestPair = currentPair;
            }
        }
    }
    return closestPair;
}

int main() {
    // Генерация случайных точек
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(-100.0, 100.0);

    // Ввод числа точек
    int n = 100000;

    // Создание массива точек
    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        points[i].x = dist(gen);
        points[i].y = dist(gen);
    }

        // Измерение времени работы алгоритма "разделяй и властвуй"
    auto t1 = steady_clock::now();
    Pair closestPairDivideAndConquer = closest_pair(points);
    auto t2 = steady_clock::now();
    auto durationDivideAndConquer = duration<double>(t2 - t1).count();


    // Измерение времени работы алгоритма полного перебора
    t1 = steady_clock::now();
    Pair closestPairBruteForce = bruteForceClosestPair(points);
    t2 = steady_clock::now();
    auto durationBruteForce = duration<double>(t2 - t1).count();

    // Вывод результатов
    cout << "Пара ближайших точек (разделяй и властвуй): (" << closestPairDivideAndConquer.p1.x << ", " << closestPairDivideAndConquer.p1.y << ") и (" << closestPairDivideAndConquer.p2.x << ", " << closestPairDivideAndConquer.p2.y << ")" << endl;
    cout << "Расстояние (разделяй и властвуй): " << closestPairDivideAndConquer.dist << endl;
    cout << "Время выполнения (разделяй и властвуй): " << durationDivideAndConquer << " с" << endl;


    cout << "Пара ближайших точек (полный перебор): (" << closestPairBruteForce.p1.x << ", " << closestPairBruteForce.p1.y << ") и (" << closestPairBruteForce.p2.x << ", " << closestPairBruteForce.p2.y << ")" << endl;
    cout << "Расстояние (полный перебор): " << closestPairBruteForce.dist << endl;
    cout << "Время выполнения (полный перебор): " << durationBruteForce << " с" << endl;

    return 0;
}