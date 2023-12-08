#ifndef ADJACENCY_MATRIXES_H
#define ADJACENCY_MATRIXES_H

typedef struct VertexColorPair {
    int vertex;
    int color;
} VertexColorPair;

typedef struct Graph {
    int vertices;
    int** adjacencyMatrix;
} Graph;

#endif // ADJACENCY_MATRIXES_H
