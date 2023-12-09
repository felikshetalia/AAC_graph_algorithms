#include <stdbool.h>
#define MAX_VERTICES 500
#ifndef bruteMaxCliquE
#define bruteMaxCliqE
bool isClique(int vertex, int* clique, int size, Graph* graph) {
    for (int i = 0; i < size; ++i) {
        if (!graph->adjacencyMatrix[vertex][clique[i]]) {
            return false;
        }
    }
    return true;
}
// Recursive function to find maximum clique starting from a specific vertex
void findMaxCliqueRec(Graph* graph, int* clique, int size, int* maxSize) {
    for (int i = clique[size - 1] + 1; i < graph->vertices; ++i) {
        if (isClique(i, clique, size, graph)) {
            clique[size] = i;
            findMaxCliqueRec(graph, clique, size + 1, maxSize);
        }
    }

    // Check if the current clique is larger than the maximum found so far
    if (size > *maxSize) {
        *maxSize = size;
    }
}
// Function to find the maximum clique in the graph using brute force
void bruteMaxClique(Graph* graph) {
    int clique[MAX_VERTICES];
    int maxSize = 0;

    // Iterate over each vertex and start finding cliques
    for (int i = 0; i < graph->vertices; ++i) {
        clique[0] = i;
        findMaxCliqueRec(graph, clique, 1, &maxSize);
    }

    printf("Maximum Clique Size: %d\n", maxSize);
    printf("\n");
}
#endif