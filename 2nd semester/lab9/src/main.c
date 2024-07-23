#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ARRAY_SIZE 5001
#define NO_WAY UINT_MAX

void FindShortestPath(unsigned int* matrix,
                      int N,
                      int start,
                      long long* min_distance,
                      int* path,
                      int* used) {
  for (int i = 1; i <= N; ++i) {
    min_distance[i] = NO_WAY;
    used[i] = 0;
  }
  path[start] = start;
  min_distance[start] = 0;
  int next_vertex = start;
  for (int i = 1; i <= N; i++) {
    used[next_vertex] = 1;
    for (int j = 1; j <= N; j++) {
      if (!used[j] && matrix[next_vertex * (N + 1) + j] != NO_WAY &&
          matrix[next_vertex * (N + 1) + j] != 0) {
        long long next_path_weight =
            (long long)matrix[next_vertex * (N + 1) + j] +
            min_distance[next_vertex];
        if (next_path_weight < min_distance[j]) {
          min_distance[j] = next_path_weight;
          path[j] = next_vertex;
        }
      }
    }
    unsigned int min_path = NO_WAY;
    for (int j = 1; j <= N; j++) {
      if (!used[j] && min_distance[j] < min_path && j != next_vertex) {
        min_path = min_distance[j];
        next_vertex = j;
      }
    }
  }
}

void CreateMatrix(unsigned int* matrix, int N) {
  for (int i = 0; i <= N; i++) {
    for (int j = 0; j <= N; j++) {
      matrix[i * (N + 1) + j] = NO_WAY;
      if (i == j) {
        matrix[i * (N + 1) + j] = 0;
      }
    }
  }
}

bool PrintCheckInput(int N, int M) {
  if (N < 0 || N > 5000) {
    printf("bad number of vertices");
    return false;
  }
  if (M < 0 || M > (N * (N + 1) / 2)) {
    printf("bad number of edges");
    return false;
  }
  return true;
}

bool PrintCheckVertex(int V, int N) {
  if (V < 1 || V > N) {
    printf("bad vertex");
    return false;
  }
  return true;
}

bool PrintCheckLength(int length) {
  if (length < 0 || length > INT_MAX) {
    printf("bad length");
    return false;
  }
  return true;
}

void PrintPath(int* path, int finish) {
  int v = finish;
  while (path[v] != v && path[v] != 0) {
    printf("%d ", v);
    v = path[v];
  }
  printf("%d ", v);
}

void PrintResult(unsigned int* matrix,
                 long long* min_distance,
                 int* path,
                 int N,
                 int finish) {
  for (int i = 1; i <= N; i++) {
    if (min_distance[i] == NO_WAY) {
      printf("oo ");
    } else if (min_distance[i] > INT_MAX) {
      printf("INT_MAX+ ");
    } else {
      printf("%lld ", min_distance[i]);
    }
  }
  printf("\n");

  int number_of_routes = 0;
  for (int i = 1; i <= N; i++) {
    if (min_distance[finish] > INT_MAX &&
        matrix[finish * (N + 1) + i] != NO_WAY &&
        matrix[finish * (N + 1) + i]) {
      number_of_routes++;
    }
  }
  if (min_distance[finish] == NO_WAY) {
    printf("no path");
  } else if (min_distance[finish] > INT_MAX && number_of_routes > 1) {
    printf("overflow");
  } else {
    PrintPath(path, finish);
  }
}

int main() {
  int N;
  if (scanf("%d", &N) != 1) {
    printf("bad number of lines");
    return 0;
  }
  int start, finish;
  if (scanf("%d %d", &start, &finish) != 2) {
    printf("bad number of lines");
    return 0;
  }
  int M;
  if (scanf("%d", &M) != 1) {
    printf("bad number of lines");
    return 0;
  }

  if ((start < 1 || start > N) || (finish < 1 || finish > N)) {
    printf("bad vertex");
    return 0;
  }

  if (!PrintCheckInput(N, M)) {
    return 0;
  }
  unsigned int* matrix = malloc(((N + 1) * (N + 1)) * sizeof(unsigned int));
  CreateMatrix(matrix, N);
  for (int i = 0; i < M; i++) {
    int from, to;
    int length;
    if (scanf("%d %d %d", &from, &to, &length) < 3) {
      printf("bad number of lines");
      free(matrix);
      return 0;
    }

    if (!PrintCheckVertex(from, N) || !PrintCheckVertex(to, N)) {
      free(matrix);
      return 0;
    }
    if (!PrintCheckLength(length)) {
      free(matrix);
      return 0;
    }

    matrix[from * (N + 1) + to] = length;
    matrix[to * (N + 1) + from] = length;
  }

  int path[MAX_ARRAY_SIZE];
  long long min_distance[MAX_ARRAY_SIZE];
  int used[N + 1];
  for (int i = 0; i <= N; i++) {
    used[i] = 0;
  }

  FindShortestPath(matrix, N, start, min_distance, path, used);
  PrintResult(matrix, min_distance, path, N, finish);
  free(matrix);
  return 0;
}
