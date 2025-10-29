#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

// ----------------- Merge Function -----------------
void mergeArrays(vector<double> &arr, int left, int mid, int right) {
    vector<double> L(arr.begin() + left, arr.begin() + mid + 1);
    vector<double> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < L.size() && j < R.size())
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < L.size()) arr[k++] = L[i++];
    while (j < R.size()) arr[k++] = R[j++];
}

// ----------------- Sequential Merge Sort -----------------
void mergeSortSequential(vector<double> &arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSequential(arr, left, mid);
        mergeSortSequential(arr, mid + 1, right);
        mergeArrays(arr, left, mid, right);
    }
}

// ----------------- Parallel Merge Sort (Static Depth Cutoff) -----------------
void mergeSortParallelStatic(vector<double> &arr, int left, int right, int depth, int maxDepth) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (depth < maxDepth) {
            #pragma omp task shared(arr)
            mergeSortParallelStatic(arr, left, mid, depth + 1, maxDepth);

            #pragma omp task shared(arr)
            mergeSortParallelStatic(arr, mid + 1, right, depth + 1, maxDepth);

            #pragma omp taskwait
        } else {
            mergeSortSequential(arr, left, mid);
            mergeSortSequential(arr, mid + 1, right);
        }
        mergeArrays(arr, left, mid, right);
    }
}

// ----------------- Helper Function to Run and Time Sort -----------------
double testDepthThreshold(vector<double> data, int maxDepth) {
    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        mergeSortParallelStatic(data, 0, data.size() - 1, 0, maxDepth);
    }

    double end = omp_get_wtime();
    return end - start;
}

// ----------------- Main -----------------
int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<double> arr(n);
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];

    cout << fixed << setprecision(6);
    cout << "Depth Threshold vs Execution Time\n";
    cout << "----------------------------------\n";
    cout << setw(10) << "Depth" << setw(20) << "Time (seconds)\n";

    ofstream fout("depth_vs_time.csv");
    fout << "Depth,Time\n";

    for (int depth = 2; depth <= 10; depth++) {
        vector<double> temp = arr;  // copy to avoid sorting same data
        double timeTaken = testDepthThreshold(temp, depth);
        cout << setw(10) << depth << setw(20) << timeTaken << "\n";
        fout << depth << "," << timeTaken << "\n";
    }

    fout.close();
    cout << "\nResults written to depth_vs_time.csv\n";
    return 0;
}
