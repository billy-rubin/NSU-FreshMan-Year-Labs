#include <stdio.h>
#include <stdlib.h>

void Swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void QuickSort(int *mas, int n) {
    if (n <= 1) {
        return;
    }

    int median_index = n / 2;
    int median_value = mas[median_index];
    int left = 0;
    int right = n - 1;

    do {
        while (mas[left] < median_value) {
            left++;
        }
        while (mas[right] > median_value) {
            right--;
        }

        if (left <= right) {
            Swap(&mas[left], &mas[right]);
            left++;
            right--;
        }
    } while (left <= right);

    QuickSort(mas, right + 1); 
    QuickSort(mas + left, n - left); 
}

int main() {
    int n;
    if (scanf("%d", &n) != 1)
        return 0; 

    int* mas = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &mas[i]) != 1){
            free(mas);
            return 0;
        } 
    }
    QuickSort(mas, n);

    for (int i = 0; i < n; i++) {
        printf("%d ", mas[i]);
    }

    free(mas);
    return 0;
}
