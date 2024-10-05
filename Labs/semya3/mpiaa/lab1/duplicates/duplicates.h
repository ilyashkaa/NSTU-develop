
#pragma once

#include <vector>

using namespace std;

bool has_duplicates_slow(const vector<int> &data);
vector<int> get_duplicates_slow(const vector<int> &data);

bool has_duplicates_fast(const std::vector<int> &data);
vector<int> get_duplicates_fast(const vector<int> &data);
