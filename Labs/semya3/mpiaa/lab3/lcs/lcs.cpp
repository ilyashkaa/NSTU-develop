#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std::chrono;
using namespace std;

// Алгоритм полного перебора
string lcs_brute_force(const string& X, const string& Y) {
  int N = X.size();
  int M = Y.size();
  string best_lcs = "";

  // Перебираем все возможные подпоследовательности первой строки
  for (int i = 0; i < (1 << N); ++i) {
    string subsequence = "";
    for (int j = 0; j < N; ++j) {
      if (i & (1 << j)) {
        subsequence += X[j];
      }
    }

    // Проверяем, является ли подпоследовательность подпоследовательностью второй строки
    int k = 0;
    for (int j = 0; j < subsequence.size() && k < M; ++j) {
      if (subsequence[j] == Y[k]) {
        ++k;
      }
    }
    if (k == M && subsequence.size() > best_lcs.size()) {
      best_lcs = subsequence;
    }
  }

  return best_lcs;
}

// Алгоритм динамического программирования
string lcs_dp(const string& X, const string& Y) {
  int N = X.size();
  int M = Y.size();
  vector<vector<int>> dp(N + 1, vector<int>(M + 1, 0));

  // Строим таблицу LCS
  for (int i = 1; i <= N; ++i) {
    for (int j = 1; j <= M; ++j) {
      if (X[i - 1] == Y[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }

  // Восстанавливаем наибольшую общую подпоследовательность
  string lcs = "";
  int i = N;
  int j = M;
  while (i > 0 && j > 0) {
    if (X[i - 1] == Y[j - 1]) {
      lcs = X[i - 1] + lcs;
      --i;
      --j;
    } else if (dp[i - 1][j] > dp[i][j - 1]) {
      --i;
    } else {
      --j;
    }
  }

  return lcs;
}

// Генерация случайной строки
string generate_random_string(int length) {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dist('a', 'z');
  string result;
  for (int i = 0; i < length; ++i) {
    result += dist(gen);
  }
  return result;
}

int main() {
  // Фиксируем размер первой строки
  int fixed_size = 100;
  string X = generate_random_string(fixed_size);

  // Изменяем размер второй строки
  for (int i = 1; i <= 10; i ++) {
    int size = pow(10, i);
    string Y = generate_random_string(size);

    // Замеряем время работы алгоритма полного перебора
    auto t1 = steady_clock::now();
    string lcs_brute_force_result = lcs_brute_force(X, Y);
    auto t2 = steady_clock::now();
    chrono::duration<double> elapsed_brute_force = t2 - t1;

    // Замеряем время работы алгоритма динамического программирования
    t1 = steady_clock::now();
    string lcs_dp_result = lcs_dp(X, Y);
    t2 = steady_clock::now();
    chrono::duration<double> elapsed_dp = t2 - t1;

    // Выводим результаты
    cout << "Размер второй строки: " << size << endl;
    cout << "Время полного перебора: " << elapsed_brute_force.count() << " сек." << endl;
    cout << "Время динамического программирования: " << elapsed_dp.count() << " сек." << endl;
    cout << "Наибольшая общая подпоследовательность: " << lcs_dp_result << endl;
    cout << endl;
  }

  return 0;
}