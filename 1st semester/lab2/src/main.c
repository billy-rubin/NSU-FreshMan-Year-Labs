#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void Reverse(int *mas, int start, int end){
    while (start < end) {
        Swap(&mas[start], &mas[end]);
        start++;
        end--;
    }
}

int IsSorted(int mas[], int n) {
    for (int i = 1; i < n; i++) {
        if (mas[i] > mas[i - 1]) {
            return 0;
        }
    }
    return 1;
}

int NextPermutation(int mas[], int n){
    int i = n - 2;
    while (i >= 0 && mas[i] >= mas[i + 1])
        i--;

    if (i == -1 || IsSorted(mas, n)) {
        return 0;
    }

    int j = n - 1;
    while (mas[j] <= mas[i])
        j--;

    Swap(&mas[i], &mas[j]);
    Reverse(mas, i + 1, n - 1);

    return 1;
}

int main(){
    char str[100];
    if (scanf("%34[^\n]", str) != 1)
        return 0;

    int P;
    if (scanf("%d", &P) != 1)
        return 0;

    int len = (int)strlen(str);
    int duplicate[10] = {0};

    int* mas = (int*)malloc(len * sizeof(int));

    for (int i = 0; i < len; i++){
        if (str[i] < '0' || str[i] > '9' || str[i] == ' '){
            printf("bad input");
            free(mas);
            return 0;
        } else {
            mas[i] = str[i] - '0';
        }
        if (duplicate[mas[i]]){
            printf("bad input");
            free(mas);
            return 0;
        }
        duplicate[mas[i]] = 1;
    }

    for (int i = 0; i < P; i++){
        if (!NextPermutation(mas, len)) {
            free(mas);
            return 0;
        }
        for (int j = 0; j < len; j++) {
            printf("%d", mas[j]);
        }
        printf("\n");
    }

    free(mas);
    return 0;
}
