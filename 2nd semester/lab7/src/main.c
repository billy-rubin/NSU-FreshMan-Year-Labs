#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8;

const int kBitsInUInt = sizeof(uint8) * 8;

enum Color { BLACK = 2, GREY = 1, WHITE = 0 };

typedef struct Graph {
  int vertices_num;
  uint8* matrix;
} Graph;

typedef struct StackNode {
  int value;
  struct StackNode* top;
  struct StackNode* next;
} StackNode;

Graph* InitializeGraph(int N) {
  Graph* graph = (Graph*)malloc(sizeof(Graph));
  graph->vertices_num = N;
  int matrix_size = ((N + 1) * (N + 1)) / kBitsInUInt + 1;
  graph->matrix = (uint8*)calloc(matrix_size, sizeof(uint8));
  return graph;
}

void FreeGraph(Graph* graph) {
  free(graph->matrix);
  free(graph);
}

StackNode* CreateStack() {
  StackNode* stack = (StackNode*)malloc(sizeof(StackNode));
  stack->top = NULL;
  stack->next = NULL;
  return stack;
}

void Push(StackNode* stack, int value) {
  StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
  newNode->value = value;
  newNode->next = stack->top;
  stack->top = newNode;
}

int Pop(StackNode* stack) {
  int value = stack->top->value;
  StackNode* temp = stack->top;
  stack->top = stack->top->next;
  free(temp);
  return value;
}

void FreeStack(StackNode* stack) {
  while (stack->top != NULL) {
    StackNode* temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
  }
  free(stack);
}

uint8 GetMask(int to) {
  uint8 bit = to % kBitsInUInt;
  uint8 res = 1;
  res = res << (kBitsInUInt - bit - 1);
  return res;
}

bool DFS(Graph* graph, int vertex, char* color_of_vertices, StackNode* stack) {
  color_of_vertices[vertex] = GREY;
  for (int i = 1; i <= graph->vertices_num; i++) {
    int matrix_shift = (vertex - 1) * graph->vertices_num + (i - 1);
    if (graph->matrix[matrix_shift / kBitsInUInt] &
        GetMask(matrix_shift % kBitsInUInt)) {
      if (color_of_vertices[i] == GREY) {
        return false;
      } else if (color_of_vertices[i] != BLACK) {
        if (!DFS(graph, i, color_of_vertices, stack)) {
          return false;
        }
      }
    }
  }
  color_of_vertices[vertex] = BLACK;
  Push(stack, vertex);
  return true;
}

bool TopologicalSort(Graph* graph, StackNode* stack) {
  char color_of_vertices[graph->vertices_num + 1];
  for (int i = 1; i <= graph->vertices_num; i++) {
    color_of_vertices[i] = WHITE;
  }
  for (int vertex = 1; vertex <= graph->vertices_num; vertex++) {
    if (color_of_vertices[vertex] != BLACK) {
      if (!DFS(graph, vertex, color_of_vertices, stack)) {
        return false;
      }
    }
  }
  return true;
}

bool PrintCheckInput(int N, int M) {
  if (N < 0 || N > 2000) {
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

void PrintResult(Graph* graph, StackNode* stack, int N) {
  if (TopologicalSort(graph, stack)) {
    for (int i = 0; i < N; i++) {
      printf("%d ", Pop(stack));
    }
  } else {
    printf("impossible to sort\n");
  }
}

int main() {
  int N, M;
  if (scanf("%d", &N) != 1 || scanf("%d", &M) != 1) {
    printf("bad number of lines");
    return 0;
  }
  if (!PrintCheckInput(N, M)) {
    return 0;
  }
  Graph* graph = InitializeGraph(N);
  for (int i = 0; i < M; i++) {
    int start, finish;
    if (scanf("%d %d", &start, &finish) != 2) {
      printf("bad number of lines");
      FreeGraph(graph);
      return 0;
    } else if (!PrintCheckVertex(start, N) || !PrintCheckVertex(finish, N)) {
      FreeGraph(graph);
      return 0;
    } else {
      int matrix_shift = (start - 1) * graph->vertices_num + (finish - 1);
      int num = matrix_shift / kBitsInUInt;
      graph->matrix[num] |= GetMask(matrix_shift % kBitsInUInt);
    }
  }

  StackNode* stack = CreateStack();
  PrintResult(graph, stack, N);

  FreeStack(stack);
  FreeGraph(graph);
  return 0;
}
