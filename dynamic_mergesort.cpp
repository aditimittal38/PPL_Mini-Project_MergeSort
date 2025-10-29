#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

void mergeArrays(vector<double> &arr, int left, int mid, int right) {
    vector<double> L(arr.begin() + left, arr.begin() + mid + 1);
    vector<double> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < (int)L.size() && j < (int)R.size()) {
        arr[k++] = (L[i] < R[j]) ? L[i++] : R[j++];
    }
    while (i < (int)L.size()) arr[k++] = L[i++];
    while (j < (int)R.size()) arr[k++] = R[j++];
}

void mergeSortSequential(vector<double> &arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSequential(arr, left, mid);
        mergeSortSequential(arr, mid + 1, right);
        mergeArrays(arr, left, mid, right);
    }
}

void mergeSortParallelDynamic(vector<double> &arr, int left, int right, int threshold) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        int size = right - left + 1;

        if (size > threshold) {
            #pragma omp task shared(arr)
            mergeSortParallelDynamic(arr, left, mid, threshold);

            #pragma omp task shared(arr)
            mergeSortParallelDynamic(arr, mid + 1, right, threshold);

            #pragma omp taskwait
        } else {
            mergeSortSequential(arr, left, mid);
            mergeSortSequential(arr, mid + 1, right);
        }

        mergeArrays(arr, left, mid, right);
    }
}

int main() {
    int n = 10000000;
    vector<double> arr(n);

    // Generate random double values up to 1e9 with 3 decimal places
    cout << "Enter elements:\n";
    for(int i=0;i<n;i++) cin >> arr[i];

    vector<double> arr_copy = arr;

    double start, end;

    // Sequential
    start = omp_get_wtime();
    mergeSortSequential(arr_copy, 0, n - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << end - start << " s" << endl;

    // Dynamic Parallel (varying thresholds)
    int thresholds[] = {1, 10, 100, 1000, 10000, 100000, 200000, 500000, 1000000};
    for (int th : thresholds) {
        arr_copy = arr;
        start = omp_get_wtime();
        #pragma omp parallel
        {
            #pragma omp single
            mergeSortParallelDynamic(arr_copy, 0, n - 1, th);
        }
        end = omp_get_wtime();
        cout << "Threshold " << setw(8) << th << " -> Time: " << end - start << " s" << endl;
    }

    return 0;
}
