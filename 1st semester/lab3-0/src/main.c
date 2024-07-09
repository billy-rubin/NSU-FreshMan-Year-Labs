#include <stdio.h>
#include <stdlib.h>

void Swap(int* a, int* b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void Heapify(int* str, int n, int i) {
  int root = i;
  while (1) {
    const int left = 2 * i + 1;
    const int right = 2 * i + 2;
    if (left < n && str[left] > str[root])
      root = left;

    if (right < n && str[right] > str[root])
      root = right;

    if (root != i) {
      Swap(&str[i], &str[root]);
      i = root;
    } else {
      break;
    }
  }
}

void HeapSort(int* str, int n) {
  for (int i = n / 2 - 1; i >= 0; i--)
    Heapify(str, n, i);

  for (int i = n - 1; i >= 0; i--) {
    Swap(&str[0], &str[i]);
    Heapify(str, i, 0);
  }
}

int main() {
  int n = 0;
  if (scanf("%d", &n) != 1)
    return 0;

  int* mas = (int*)malloc(n * sizeof(int));

  for (int i = 0; i < n; ++i) {
    if (scanf("%d", &mas[i]) != 1) {
      free(mas);
      return 0;
    }
  }

  HeapSort(mas, n);

  for (int i = 0; i < n; i++) {
    printf("%d ", mas[i]);
  }

  free(mas);
  return 0;
}
