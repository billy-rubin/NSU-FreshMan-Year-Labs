#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Edge {
    int from;
    int to;
    int length;
} Edge;

typedef struct Graph {
    struct Edge* edges;
    int vertices_number;
    int edges_number;
} Graph;

typedef struct SubSet {
    int* parent;
    int* rank;
} SubSet;

Graph* InitializeGraph(int N, int M) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vertices_number = N;
    graph->edges_number = M;
    graph->edges = (Edge*)malloc(M * sizeof(Edge));
    return graph;
}

SubSet* InitializeDSU(Graph* graph) {
    SubSet* subsets = (SubSet*)malloc(sizeof(SubSet));
    subsets->parent = (int*)malloc(sizeof(int) * graph->vertices_number);
    subsets->rank = (int*)malloc(sizeof(int) * graph->vertices_number);
    for (int v = 0; v < graph->vertices_number; v++) {
        subsets->parent[v] = v;
        subsets->rank[v] = 0;
    }
    return subsets;
}

void FreeSubSet(SubSet* subsets) {
    free(subsets->parent);
    free(subsets->rank);
    free(subsets);
}

int FindParent(int* parent, int index) {
    if (parent[index] == index) {
        return index;
    }
    parent[index] = FindParent(parent, parent[index]);
    return parent[index];
}

void Union(SubSet* subsets, int from_root, int to_root) {
    if (subsets->rank[from_root] < subsets->rank[to_root]) {
        subsets->parent[from_root] = to_root;
    } else if (subsets->rank[from_root] > subsets->rank[to_root]) {
        subsets->parent[to_root] = from_root;
    } else {
        subsets->parent[to_root] = from_root;
        subsets->rank[from_root]++;
    }
}

void Swap(Edge* a, Edge* b) {
    Edge temp = *a;
    *a = *b;
    *b = temp;
}

void QuickSort(Edge* edges_array, int array_size) {
    if (array_size <= 1) {
        return;
    }
    int size_buffer = array_size - 1;
    const int kMedianIndex = size_buffer / 2;
    const Edge kMedianValue = edges_array[kMedianIndex];
    int count = 0;

    do {
        while (edges_array[count].length < kMedianValue.length) {
            count++;
        }
        while (edges_array[size_buffer].length > kMedianValue.length) {
            size_buffer--;
        }

        if (count <= size_buffer) {
            Swap(&edges_array[count], &edges_array[size_buffer]);
            count++;
            size_buffer--;
        }
    } while (count <= size_buffer);

    if (size_buffer > 0) {
        QuickSort(edges_array, size_buffer + 1);
    }
    if (array_size > count) {
        QuickSort(edges_array + count, array_size - count);
    }
}

Edge* FindMinSpanningTree(Graph* graph, int* edges_count) {
    if (edges_count != NULL){
        *edges_count = 0;
    }
    Edge* result_graph = (Edge*)malloc(sizeof(Edge) * graph->vertices_number);

    QuickSort(graph->edges, graph->edges_number);

    SubSet* subsets = InitializeDSU(graph);

    for (int i = 0; i < graph->edges_number; i++) {
        const int kFromRoot = FindParent(subsets->parent, graph->edges[i].from - 1);
        const int kToRoot = FindParent(subsets->parent, graph->edges[i].to - 1);

        if (kFromRoot != kToRoot) {
            result_graph[(*edges_count)++] = graph->edges[i];
            Union(subsets, kFromRoot, kToRoot);

            if (*edges_count == graph->vertices_number - 1) {
                break;
            }
        }
    }

    FreeSubSet(subsets);
    return result_graph;
}

void PrintResult(Edge* result_graph, int edges_count, int vertices_number) {
    if (edges_count == vertices_number - 1) {
        for (int i = 0; i < edges_count; i++) {
            printf("%d %d\n", result_graph[i].from, result_graph[i].to);
        }
    } else {
        printf("no spanning tree");
    }
}

void FreeGraph(Graph** root) {
    Graph* graph = *root;
    free(graph->edges);
    free(graph);
    *root = NULL;
}

bool PrintCheckInput(int N, int M) {
    if (N < 0 || N > 5000) {
        printf("bad number of vertices");
        return true;
    }
    if (M < 0 || M > (N * (N + 1) / 2)) {
        printf("bad number of edges");
        return true;
    }
    return false;
}

bool PrintCheckVertex(int V, int N) {
    if (V < 1 || V > N) {
        printf("bad vertex");
        return true;
    }
    return false;
}

bool PrintCheckLength(int length) {
    if (length < 0 || length > INT_MAX) {
        printf("bad length");
        return true;
    }
    return false;
}

int main() {
    int N;
    if (scanf("%d", &N) != 1) {
        printf("bad number of lines");
        return 0;
    }
    int M;
    if (scanf("%d", &M) != 1) {
        printf("bad number of edges");
        return 0;
    }

    if (PrintCheckInput(N, M)) {
        return 0;
    }

    Graph* graph = InitializeGraph(N, M);
    for (int i = 0; i < M; i++) {
        int from, to, length;
        if (scanf("%d %d %d", &from, &to, &length) < 3) {
            printf("bad number of lines");
            FreeGraph(&graph);
            return 0;
        }

        if (PrintCheckVertex(from, N) || PrintCheckVertex(to, N)) {
            FreeGraph(&graph);
            return 0;
        }
        if (PrintCheckLength(length)) {
            FreeGraph(&graph);
            return 0;
        }

        graph->edges[i].from = from;
        graph->edges[i].to = to;
        graph->edges[i].length = length;
    }

    int edges_count;
    Edge* result_graph = FindMinSpanningTree(graph, &edges_count);
    PrintResult(result_graph, edges_count, graph->vertices_number);

    free(result_graph);
    FreeGraph(&graph);
    return 0;
}
