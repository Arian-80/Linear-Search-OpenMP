#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

long long parallel_linear_search(const int* array, int itemToSearch, size_t size,
                                 int threadCount) {
    omp_set_dynamic(0);
    omp_set_num_threads(threadCount);
    long long firstOccurrence = LONG_LONG_MAX;
    #pragma omp parallel for default(none) \
    shared(size, itemToSearch, array, firstOccurrence)
    for (long long i = 0; i < size; i++) {
        // Exit if occurrence already found earlier
        if (firstOccurrence < i) i = size;

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
    for (int k = 1; k < 9; k++) {
        if (k == 3) k = 4;
        if (k == 5) k = 6;
        if (k == 7) k = 8;
        size_t size = 1000000000;
        int item = 12845612;
        int *array = (int *) calloc(size, sizeof(int));
        if (!array) {
            printf("Failed to create list.\n");
            return -1;
        }
        array[size-1] = item;
        array[size-2] = item;
        double start, end;
        start = omp_get_wtime();
        long long index = linear_search(array, item, size, k);
        end = omp_get_wtime();
        if (index >= 0) {
            printf("First occurrence of item [%d] is at index: %lli\n", item, index);
        } else {
            printf("Item not found in list.\n");
        }
        printf("Time taken: %g seconds.\n", end - start);
        FILE *f = fopen("times.txt", "a");
        fprintf(f, "%g,", end - start);
        free(array);
    }
    return 0;
}