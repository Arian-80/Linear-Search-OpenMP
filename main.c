#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

long long parallel_linear_search(const int* array, int itemToSearch, size_t size,
                        int threadCount) {
    omp_set_dynamic(0);
    omp_set_num_threads(threadCount);
    long long firstOccurrence = LONG_LONG_MAX;
    #pragma omp parallel for
    for (long long i = 0; i < size; i++) {
        if (itemToSearch == array[i]) {
        #pragma omp critical
            if (i < firstOccurrence) {
                firstOccurrence = i;
            }
        }
    }
    return firstOccurrence;
}

long long linear_search(const int* array, int itemToSearch, size_t size, int threadCount) {
    long long result = parallel_linear_search(array, itemToSearch, size, threadCount);
    return result == LONG_LONG_MAX ? -1 : result;
}

int main() {
    size_t size = 8000000000;
    int item = 12845612;
    int* array = (int*) malloc(size * sizeof(int));
    array[856318842] = item;
    array[456318842] = item;
    array[156318842] = item;
    time_t start, end;
    time(&start);
    long long index = linear_search(array, item, size, 8);
    time(&end);
    if (index >= 0) {
        printf("First occurrence of item [%d] is at index: %lli\n", item, index);
    } else {
        printf("Item not found in list.\n");
    }
    printf("Time taken: %g seconds.\n", difftime(end, start));
    return 0;
}
