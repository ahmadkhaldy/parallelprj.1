
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int N = 500;  // matrix size

void fillMatrix(vector<vector<int>>& matrix) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            matrix[i][j] = rand() % 10;
}

int computeChecksum(const vector<vector<int>>& matrix) {
    int checksum = 0;
    for (auto& row : matrix)
        for (auto val : row)
            checksum += val;
    return checksum;
}

int main() {
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<int>> C(N, vector<int>(N, 0));

    fillMatrix(A);
    fillMatrix(B);

    auto start = high_resolution_clock::now();

    // Sequential matrix multiplication
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k)
                C[i][j] += A[i][k] * B[k][j];

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    cout << "Sequential time: " << elapsed.count() << " seconds\n";
    cout << "Checksum: " << computeChecksum(C) << endl;

    return 0;
}
