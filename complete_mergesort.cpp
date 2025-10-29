#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

// ------------------ Merge Function ------------------
void mergeArrays(vector<double> &arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<double> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

// ------------------ Sequential Merge Sort ------------------
void mergeSortSequential(vector<double> &arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSequential(arr, left, mid);
        mergeSortSequential(arr, mid + 1, right);
        mergeArrays(arr, left, mid, right);
    }
}

// ------------------ Parallel Merge Sort (Static Depth) ------------------
void mergeSortParallelStatic(vector<double> &arr, int left, int right, int depth = 0) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (depth < 3) { // static depth cutoff
            #pragma omp task shared(arr)
            mergeSortParallelStatic(arr, left, mid, depth + 1);

            #pragma omp task shared(arr)
            mergeSortParallelStatic(arr, mid + 1, right, depth + 1);

            #pragma omp taskwait
        } else {
            mergeSortSequential(arr, left, mid);
            mergeSortSequential(arr, mid + 1, right);
        }

        mergeArrays(arr, left, mid, right);
    }
}

// ------------------ Parallel Merge Sort (Dynamic Threshold) ------------------
void mergeSortParallelDynamic(vector<double> &arr, int left, int right, int THRESHOLD = 40000) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (right - left > THRESHOLD) {
            #pragma omp task shared(arr)
            mergeSortParallelDynamic(arr, left, mid, THRESHOLD);

            #pragma omp task shared(arr)
            mergeSortParallelDynamic(arr, mid + 1, right, THRESHOLD);

            #pragma omp taskwait
        } else {
            mergeSortSequential(arr, left, mid);
            mergeSortSequential(arr, mid + 1, right);
        }

        mergeArrays(arr, left, mid, right);
    }
}

// ------------------ Main Function ------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<double> arr(n);
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];

    vector<double> arr_seq = arr;
    vector<double> arr_static = arr;
    vector<double> arr_dynamic = arr;

    double start, end;
    cout << "Sorting " << n << " elements...\n";
    // 1. Sequential Merge Sort
    start = omp_get_wtime();
    mergeSortSequential(arr_seq, 0, n - 1);
    end = omp_get_wtime();
    cout << fixed << setprecision(5);
    cout << "Sequential Merge Sort Time: " << (end - start) << " s\n";

    // 2. Parallel Merge Sort (Static Depth)
    start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        mergeSortParallelStatic(arr_static, 0, n - 1);
    }
    end = omp_get_wtime();
    cout << "Parallel Merge Sort (Static Depth) Time: " << (end - start) << " s\n";

    // 3. Parallel Merge Sort (Dynamic Threshold)
    start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        mergeSortParallelDynamic(arr_dynamic, 0, n - 1, 10000); // threshold = 10k
    }
    end = omp_get_wtime();
    cout << "Parallel Merge Sort (Dynamic Threshold) Time: " << (end - start) << " s\n";

    // Verify correctness
    if (arr_seq == arr_static && arr_seq == arr_dynamic)
        cout << "All sorted arrays are identical\n";
    else
        cout << "Error: Arrays differ\n";

    return 0;
}

/*
Compile:
    g++ -fopenmp -O2 mergesort_double.cpp -o mergesort_double

Generate Input:
    python3 - <<EOF
    import random
    n = 10000000
    print(n)
    for _ in range(n):
        print(round(random.uniform(0, 10000), 3))
    EOF > input.txt

Run:
    ./mergesort_double < input.txt
*/
