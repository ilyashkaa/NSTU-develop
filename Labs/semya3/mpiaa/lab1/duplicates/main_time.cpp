#include <vector>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <cmath>
#include <unordered_map>
#include <algorithm>

using namespace std;
using namespace std::chrono;


bool has_duplicates_fast(const vector<int> &data) {
    unordered_map<int, int> m;

    for(auto it = data.begin(); it != data.end(); it++){
        if(m[*it] != 0) return true;
        else m[*it] ++;
    }

    return false;
}



vector<int> get_duplicates_fast(const vector<int> &data) {
    vector<int> result;
    unordered_map<int, int> m;

    for(auto it = data.begin(); it != data.end(); it++) m[*it] ++;
    for(auto it = m.begin(); it != m.end(); it++) if(it->second > 1) result.push_back(it->first);

    return result;
}

bool has_duplicates_slow(const vector<int> &data) {
    for(auto it = data.begin(); it != data.end(); it++){
        for(auto it1 = it + 1; it1 != data.end(); it1++){
            if(*it == *it1) return true;
        }   
    }
    return false;
}


vector<int> get_duplicates_slow(const vector<int> &data) {
    vector<int> result;

    for(auto it = data.begin(); it != data.end(); it++){
        for(auto it1 = it + 1; it1 != data.end(); it1++){
            if(*it == *it1){
                if(find(result.begin(), result.end(), *it) == result.end()) result.push_back(*it);
                break;
            }
        }   
    }

    return result;
}

vector<int> gen_random_array(int size) {
	vector<int> result(size);
	for (int i = 0; i < size; i++) {
		result[i] = rand();		
	}
	return result;
}

void measure(int size) {	
	const auto data = gen_random_array(size);
	
	auto t1 = steady_clock::now();
	vector<int> res = get_duplicates_fast(data);
	auto t2 = steady_clock::now();
    cout << "N: " << size << ", время (fast_get): " << duration<double>(t2 - t1).count() << endl;

    t1 = steady_clock::now();
	res = get_duplicates_slow(data);
	t2 = steady_clock::now();
    cout << "N: " << size << ", время (slow_get): " << duration<double>(t2 - t1).count() << endl;

    t1 = steady_clock::now();
	bool b = has_duplicates_fast(data);
	t2 = steady_clock::now();
    cout << "N: " << size << ", время (fast_has): " << duration<double>(t2 - t1).count() << endl;

    t1 = steady_clock::now();
	b = has_duplicates_slow(data);
	t2 = steady_clock::now();
    cout << "N: " << size << ", время (slow_has): " << duration<double>(t2 - t1).count() << endl;
}

int main(int argc, char **argv) {
    int max_pow = argc > 1 ? stoi(argv[1]) : 6;
    
	for (int i = 1; i <= max_pow; i++) {
		measure(pow(10, i));
	}

	return 0;
}