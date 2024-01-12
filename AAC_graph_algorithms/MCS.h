#include "bruteMaxClique.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printAdjacencyMatrix(int** matrix, int length) {
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < length; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void init_graph(Graph *graph, int numVertices) {
    if (graph == NULL) {
        printf("Error: Graph is NULL.\n");
        exit(1);
    }

    graph->vertices = numVertices;

    graph->adjacencyMatrix = (int**)malloc(numVertices * sizeof(int*));
    for (int i = 0; i < numVertices; i++) {
        graph->adjacencyMatrix[i] = (int*)malloc(numVertices * sizeof(int));
        if (graph->adjacencyMatrix[i] == NULL) {
            printf("Error: Unable to allocate memory for adjacency matrix.\n");
            exit(1);
        }
    }

    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            graph->adjacencyMatrix[i][j] = 0;
        }
    }
}

void addEdge(Graph *graph, int src, int dest) {
    graph->adjacencyMatrix[src][dest] = 1;
    graph->adjacencyMatrix[dest][src] = 1;
}

void modularProduct(const Graph *g1, const Graph *g2, Graph *g3) {
    int numVertices = g1->vertices * g2->vertices;
    init_graph(g3, numVertices);

    for (int i = 0; i < g1->vertices; i++) {
        for (int j = 0; j < g2->vertices; j++) {
            for (int x = 0; x < g1->vertices; x++) {
                for (int y = 0; y < g2->vertices; y++) {
                    if ((g1->adjacencyMatrix[i][x] == 1 && g2->adjacencyMatrix[j][y] == 1 && i != x && j != y) ||
                        (g1->adjacencyMatrix[i][y] == 0 && g2->adjacencyMatrix[j][y] == 0 && i != x && j != y)) {
                        int v1 = i * g2->vertices + j;
                        int v2 = x * g2->vertices + y;
                        addEdge(g3, v1, v2);
                    }
                }
            }
        }
    }
}

void MaxCommonSubgraph(Graph *g1, Graph *g2) {
    Graph *res = (Graph *)malloc(sizeof(Graph));
    init_graph(res, g1->vertices * g2->vertices);

    if (res == NULL) {
        printf("Error allocating memory for the result graph.\n");
        exit(1);
    }

    modularProduct(g1, g2, res);
    printf("Number of Vertices: %d\n", res->vertices);
    printAdjacencyMatrix(res->adjacencyMatrix, res->vertices);


    int Q = 0;
    int i = 0;
    VertexColorPair *colors = greedyVertexColoring(res);
    maxClique(res, colors, &Q, &i);
    printf("%d",i);

//    bruteMaxClique(res);
    // Do not free res here
}
