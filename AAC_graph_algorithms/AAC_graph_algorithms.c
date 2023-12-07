#include <stdio.h>
#include <stdlib.h>
#include "maxClique.h"
//Malloc
int** allocateMatrix(int rows, int cols) {
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; ++i) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }
    return matrix;
}

//Dealloc
void freeMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

void freeGraph(Graph* graph) {
    freeMatrix(graph->adjacencyMatrix, graph->vertices);
    free(graph);
}

// This function is checking if the matrix we use is valid.
int isSymmetric(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        if(matrix[i][i] == 1){
            return 0; //diagonal elements cannot be 0
        }

        for (int j = 0; j < i; ++j) {
            if (matrix[i][j] != matrix[j][i]) {
                return 0; // Not symmetric (not valid)
            }
        }
    }

    return 1; // Symmetric (valid)
}



// Function to read graph from file and create the Graph
Graph* createGraphFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Read the number of vertices
    int vertices;
    fscanf(file, "%d", &vertices);

    //Apply malloc
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    graph->vertices = vertices;
    graph->adjacencyMatrix = allocateMatrix(vertices, vertices);

    // Read the adjacency matrix
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            fscanf(file, "%d", &(graph->adjacencyMatrix[i][j]));
        }
    }

    fclose(file);

    // Check if the matrix is symmetric
    if (!isSymmetric(graph->adjacencyMatrix, vertices)) {
        fprintf(stderr, "Error: The adjacency matrix is not symmetric or contains 1 on diagonal. Not a valid graph.\n");
        freeGraph(graph);
        exit(EXIT_FAILURE);
    }
    return graph;
}



void printAdjacencyMatrix(int** matrix, int length) {
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < length; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}


int main() {
    const char* filename = "../example graphs/graph1.txt";

    Graph* graph = createGraphFromFile(filename);
    VertexColorPair *colors = greedyVertexColoring(graph);
    int Q =0;
    int Qmax = 0;
    // Print the obtained vertex colors
    printGraph(graph);
    printf("Vertex Colors:\n");
    for (int i = 0; i < graph->vertices; ++i) {
        printf("Vertex %d: Color %d\n", colors[i].vertex+1, colors[i].color);
    }

    // Call maxClique function
    maxClique(graph, colors, &Q, &Qmax);
    printf("Number of Cliques %d",Qmax);

    free(colors);
    freeGraph(graph);
    return 0;
}