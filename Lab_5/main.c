#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <time.h>

#define MAX_THREADS 8

// Структура, передаваемая в качестве аргумента каждому потоку
typedef struct {
    int *array;
    int start_index;
    int end_index;
} ThreadData;

// Функция для слияния двух отсортированных подмассивов
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Функция, выполняемая каждым потоком для сортировки своего фрагмента массива
// Функция, выполняемая каждым потоком для сортировки своего фрагмента массива
void *threadSort(void *arg) {
    ThreadData *data = (ThreadData *) arg;
    int *arr = data->array;
    int l = data->start_index;
    int r = data->end_index;

    if (l < r) {
        int m = l + (r - l) / 2;
        ThreadData left_data = {arr, l, m}; // Создаем данные для левого подмассива
        ThreadData right_data = {arr, m + 1, r}; // Создаем данные для правого подмассива
        threadSort(&left_data); // Сортируем левый подмассив
        threadSort(&right_data); // Сортируем правый подмассив
        merge(arr, l, m, r); // Сливаем отсортированные подмассивы
    }
    pthread_exit(NULL);
}


// Функция для создания потоков и сортировки массива
void parallelSort(int arr[], int size, int num_threads) {
    pthread_t threads[MAX_THREADS];
    ThreadData thread_data[num_threads];
    int segment_size = size / num_threads;
    int remainder = size % num_threads;
    int index = 0;

    // Создаем и запускаем потоки
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].array = arr;
        thread_data[i].start_index = index;
        thread_data[i].end_index = index + segment_size - 1 + (i < remainder ? 1 : 0);
        pthread_create(&threads[i], NULL, threadSort, (void *) &thread_data[i]);
        index += segment_size + (i < remainder ? 1 : 0);
    }

    // Ждем завершения всех потоков
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Слияние отсортированных фрагментов
    int temp[size];
    for (int i = 0; i < size; i++) {
        temp[i] = arr[i];
    }
    int thread_index[num_threads];
    for (int i = 0; i < num_threads; i++) {
        thread_index[i] = thread_data[i].start_index;
    }
    for (int i = 0; i < size; i++) {
        int min_index = -1;
        int min_val = INT_MAX;
        for (int j = 0; j < num_threads; j++) {
            if (thread_index[j] <= thread_data[j].end_index && temp[thread_index[j]] < min_val) {
                min_index = j;
                min_val = temp[thread_index[j]];
            }
        }
        arr[i] = temp[thread_index[min_index]];
        thread_index[min_index]++;
    }
}

int main() {
    int size, num_threads;
    clock_t start, end;
    double cpu_time_used;

    size = 100000; // Размер массива
    int arr[size];

    // Заполнение массива случайными значениями в диапазоне от 0 до 999
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1000; // Предполагаем, что значения в массиве не превышают 1000
    }

    num_threads = 4; // Количество потоков (можно изменить по желанию)

    start = clock();
    parallelSort(arr, size, num_threads);
    end = clock();

    printf("sorted:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time: %f sec\n", cpu_time_used);

    return 0;
}
