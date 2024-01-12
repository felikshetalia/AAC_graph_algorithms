// #endif
#include <stdio.h>
#include <stdlib.h>

#ifndef GRAPH_EDIT_DIST_APPROXIMATION_H
#define GRAPH_EDIT_DIST_APPROXIMATION_H



int compareDegrees(const void* a, const void* b);

VertexDegreePair* degreeSequence(const Graph* graph);

int graphEditDistancePolynomialApproximation(const Graph* graph1, const Graph* graph2);

VertexDegreePair* degreeSequence(const Graph* graph) {

    VertexDegreePair* degreeSequence = (VertexDegreePair*)malloc(graph->vertices * sizeof(VertexDegreePair));
    int* degrees = (int*)malloc(graph->vertices * sizeof(int));

    // Initialize degrees
    for (int i = 0; i < graph->vertices; ++i) {
        degrees[i] = 0;
    }

    // Compute degrees while traversing the adjacency matrix
    for (int i = 0; i < graph->vertices; ++i) {
        for (int j = 0; j < graph->vertices; ++j) {
            degrees[i] += graph->adjacencyMatrix[i][j];
        }
    }

    // Populate the degreeSequence array
    for (int i = 0; i < graph->vertices; ++i) {
        degreeSequence[i].vertex = i;
        degreeSequence[i].degree = degrees[i];
    }

    // Sort the degree sequence
    qsort(degreeSequence, graph->vertices, sizeof(VertexDegreePair), compareDegrees);

    free(degrees);

    return degreeSequence;
}

// compareDegrees
int compareDegrees(const void* a, const void* b) {
    return ((VertexDegreePair*)a)->degree - ((VertexDegreePair*)b)->degree;
}

int graphEditDistancePolynomialApproximation(const Graph* graph1, const Graph* graph2) {
    int size1 = graph1->vertices;
    int size2 = graph2->vertices;


    int maxNumVertices = (size1 > size2) ? size1 : size2;
    int editDistance = 0;

    editDistance += abs(size1 - size2);

    VertexDegreePair* degrees1 = degreeSequence(graph1);
    VertexDegreePair* degrees2 = degreeSequence(graph2);

    for (int i = 0; i < maxNumVertices; ++i) {
        if (i < size1 && i < size2) {
            editDistance += abs(degrees1[i].degree - degrees2[i].degree);
        } else if (i < size1) {
            editDistance += degrees1[i].degree;
        } else if (i < size2) {
            editDistance += degrees2[i].degree;
        }
    }

    free(degrees1);
    free(degrees2);

    return editDistance /2;
}

#endif
