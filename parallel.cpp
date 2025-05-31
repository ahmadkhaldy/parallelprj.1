#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int N = 500;
int num_threads = 4;

vector<vector<int>> A(N, vector<int>(N));
vector<vector<int>> B(N, vector<int>(N));
vector<vector<int>> C(N, vector<int>(N, 0));

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

struct ThreadArgs {
    int start_row;
    int end_row;
};

void* multiplyPart(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;

    for (int i = args->start_row; i < args->end_row; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k)
                C[i][j] += A[i][k] * B[k][j];

    return nullptr;
}

int main() {
    fillMatrix(A);
    fillMatrix(B);

    auto start = high_resolution_clock::now();

    pthread_t threads[num_threads];
    ThreadArgs thread_args[num_threads];

    int rows_per_thread = N / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        thread_args[i].start_row = i * rows_per_thread;
        thread_args[i].end_row = (i == num_threads - 1) ? N : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], nullptr, multiplyPart, &thread_args[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    cout << "Parallel time (" << num_threads << " threads): " << elapsed.count() << " seconds\n";
    cout << "Checksum: " << computeChecksum(C) << endl;

    return 0;
}
