#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std::chrono;
using namespace std;

struct Process {
    int start;
    int finish;
};

// Сравнение процессов по времени завершения
bool compare_by_finish(const Process& p1, const Process& p2) {
    return p1.finish < p2.finish;
}

// Функция для проверки, является ли данное подмножество процессов взаимно совместимым
bool is_compatible(const vector<Process>& subset) {
    for (size_t i = 0; i < subset.size(); ++i) {
        for (size_t j = i + 1; j < subset.size(); ++j) {
            if (subset[i].finish > subset[j].start && subset[j].finish > subset[i].start) {
                return false;
            }
        }
    }
    return true;
}


// Функция для генерации всех возможных подмножеств
vector<vector<Process>> generate_subsets(const vector<Process>& activities) {
    vector<vector<Process>> subsets;
    int n = activities.size();
    for (int i = 0; i < (1 << n); ++i) {
        vector<Process> subset;
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.push_back(activities[j]);
            }
        }
        subsets.push_back(subset);
    }
    return subsets;
}

// Основная функция для поиска наибольшего подмножества взаимно совместимых процессов
vector<Process> get_max_activities_brute_force(vector<Process>& activities) {
    sort(activities.begin(), activities.end(), compare_by_finish);
    vector<vector<Process>> subsets = generate_subsets(activities);
    vector<Process> max_subset;
    for (const auto& subset : subsets) {
        if (is_compatible(subset) && subset.size() > max_subset.size()) {
            max_subset = subset;
        }
    }
    return max_subset;
}


// Жадный алгоритм
vector<Process> get_max_activities_greedy(const vector<Process>& processes) {
    vector<Process> sorted_processes = processes;
    sort(sorted_processes.begin(), sorted_processes.end(), compare_by_finish);

    vector<Process> result;
    int current = 0;

    while (current < sorted_processes.size()) {
        result.push_back(sorted_processes[current]);
        int next = current + 1;
        while (next < sorted_processes.size() && sorted_processes[next].start < sorted_processes[current].finish) {
            next++;
        }
        current = next;
    }

    return result;
}

// Генерация случайного множества процессов
vector<Process> generate_processes(int n) {
    vector<Process> processes(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 100);

    for (int i = 0; i < n; ++i) {
        processes[i].start = dist(gen);
        processes[i].finish = processes[i].start + dist(gen);
    }

    return processes;
}

int main() {
    // Тестирование на множествах процессов разного размера
    for (int i = 0; i <= 100; i += 5) {
        int n = i;
        vector<Process> processes = generate_processes(n);

        // Замер времени работы алгоритма полного перебора
        auto t1 = steady_clock::now();
        vector<Process> max_activities_brute_force = get_max_activities_brute_force(processes);
        auto t2 = steady_clock::now();
        chrono::duration<double> duration_brute_force = t2 - t1;

        // Замер времени работы жадного алгоритма
        t1 = steady_clock::now();
        vector<Process> max_activities_greedy = get_max_activities_greedy(processes);
        t2 = steady_clock::now();
        chrono::duration<double> duration_greedy = t2 - t1;

        // Вывод результатов
        cout << "Количество процессов: " << n << endl;
        cout << "Полный перебор: " << duration_brute_force.count() << " сек." << endl;
        cout << "Жадный алгоритм: " << duration_greedy.count() << " сек." << endl;
        cout << "Размер наибольшего подмножества (полный перебор): " << max_activities_brute_force.size() << endl;
        cout << "Размер наибольшего подмножества (жадный алгоритм): " << max_activities_greedy.size() << endl;
        cout << endl;
    }

    return 0;
}