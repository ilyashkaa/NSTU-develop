#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Returns shuffled sequence of unique numbers of specified size, with values from start to start + size - 1.
vector<int> shuffled_sequence(int size, int start = 0) {
    vector<int> result(size);
    iota(result.begin(), result.end(), start);
    random_shuffle(result.begin(), result.end());
    return result;
}

// Returns sequence of random numbers of specified size, with values from 0 to max.
vector<int> random_sequence(int size, int max) {
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0, max);
    vector<int> result;
    for (int i = 0; i < size; i++) {
        result.push_back(distribution(generator));
    }
    return result;
}

int main() {
    
    int size = pow(10, 1);
    
    // Container to use.
    vector<int> container;
    
    // Insert elements into container.
    auto t1 = steady_clock::now();
    
    const auto elems_to_add = shuffled_sequence(size);
    for (const auto &elem: elems_to_add) {
        container.push_back(elem);
        cout << elem << " ";
    }
    cout << endl;
    
    auto t2 = steady_clock::now();
    auto timein = duration<double>(t2 - t1).count();
    cout << "Время вставки: " << timein << " сек." << endl;
    
    // Iterate through elements.
    auto t1 = steady_clock::now();
    long long sum = 0;
    for (const auto &elem: container) {
        sum += elem;
    }
    cout << "Sum is " << sum << endl;
    
    auto t2 = steady_clock::now();
    auto times = duration<double>(t2 - t1).count();
    cout << "Время суммирования: " << times << " сек." << endl;
    // Perform search into container.
    auto t1 = steady_clock::now();
    
    int hits = 0;
    const auto elems_to_search = random_sequence(10, 2 * size);
    for (const auto &elem: elems_to_search) {
        auto it = find(container.begin(), container.end(), elem);
        if (it != container.end()) {
            hits++;
        }        
    }
    cout << "Found " << hits << " elements" << endl;
    
    auto t2 = steady_clock::now();
    auto timef = duration<double>(t2 - t1).count();
    cout << "Время поиска: " << timef << " сек." << endl;
}