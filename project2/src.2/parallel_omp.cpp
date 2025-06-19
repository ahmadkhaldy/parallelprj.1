
#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int N = 1000; // or test with 1000, 2000, etc.

vector<vector<int>> A(N, vector<int>(N));
vector<vector<int>> B(N, vector<int>(N));
vector<vector<int>> C(N, vector<int>(N, 0));

void fillMatrix(vector<vector<int>>& mat) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            mat[i][j] = rand() % 10;
}

int computeChecksum(const vector<vector<int>>& mat) {
    int sum = 0;
    for (const auto& row : mat)
        for (int val : row)
            sum += val;
    return sum;
}

int main() {
    fillMatrix(A);
    fillMatrix(B);

    auto start = high_resolution_clock::now();

    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int sum = 0;
            for (int k = 0; k < N; ++k)
                sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }

    auto end = high_resolution_clock::now();
    duration<double> time = end - start;

    cout << "OpenMP Time: " << time.count() << " seconds\n";
    cout << "Checksum: " << computeChecksum(C) << endl;

    return 0;
}
