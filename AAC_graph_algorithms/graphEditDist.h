#include <stdio.h>
#include <stdlib.h>  // Include necessary header for malloc, free
#include <limits.h>  // Include necessary header for INT_MAX

#ifndef GRAPH_EDIT_DIST_H
#define GRAPH_EDIT_DIST_H

int next_permutation(int *array, int size) {
    // algo for generating permutation
    int i = size - 1;
    while (i > 0 && array[i - 1] >= array[i]) {
        i--;
    }

    if (i == 0) {
        return 0;  // when we reach the last element of permutation
    }

    int j = size - 1;
    while (array[j] <= array[i - 1]) {
        j--;
    }

    int temp = array[i - 1];
    array[i - 1] = array[j];
    array[j] = temp;

    // reverse elements after i
    j = size - 1;
    while (i < j) {
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
        i++;
        j--;
    }

    return 1;  // if next permutation exists
}

int getNumberOfEdges(const Graph* graph) {
    int numberOfEdges = 0;

    for (int i = 0; i < graph->vertices; ++i) {
        for (int j = 0; j < graph->vertices; ++j) {
            if (graph->adjacencyMatrix[i][j] != 0) {
                // Assuming a non-zero entry represents an edge
                numberOfEdges += graph->adjacencyMatrix[i][j];
            }
        }
    }

    return numberOfEdges/2;
}

int graphEditDistance(const Graph* graph1, const Graph* graph2) {

    int size1 = graph1->vertices;
    int size2 = graph2->vertices;

    int maxNumVertices = (size1 > size2) ? size1 : size2;
    int editDistance = 0;

    // printf("------------ INFO: Graph 1 --------------\n");
    // printf("Vertices graph 1: %d \n", size1);
    // printf("Edges graph 1: %d \n", getNumberOfEdges(graph1));
    //
    // printf("------------ INFO: Graph 2 --------------\n");
    // printf("Vertices graph 2: %d \n", size2);
    // printf("Edgs graph 2: %d \n", getNumberOfEdges(graph2));

    // If the number of vertices is different, then the edit distance is the difference between the number of vertices
    editDistance += abs(size1 - size2);

    // For all permutations from 0 to maxNumVertices - 1
    // Find the permutation that minimizes the graph edit distance
    int minEdgesEditDistance = INT_MAX;
    //
    int permutation[maxNumVertices];
    for (int i = 0; i < maxNumVertices; i++) {
        permutation[i] = i;
    }

    do {
        int edgesEditDistance = 0;
        for (int i = 0; i < maxNumVertices; i++) {
            for (int j = i; j < maxNumVertices; j++) {
                if (i == j) {
                    continue;
                }

                // Assume 1st graph is the one being edited into the 2nd graph. We permute the 1st graph mappings.
                int firstGraphI = permutation[i];
                int firstGraphJ = permutation[j];

                // If the vertex is not present in the 1st graph, then its degree is 0.
                int vertex1Degree = 0;
                if (firstGraphI < size1 && firstGraphJ < size1) {
                    vertex1Degree = graph1->adjacencyMatrix[firstGraphI][firstGraphJ];
                }

                int secondGraphI = i;
                int secondGraphJ = j;

                int vertex2Degree = 0;
                if (secondGraphI < size2 && secondGraphJ < size2) {
                    vertex2Degree = graph2->adjacencyMatrix[secondGraphI][secondGraphJ];
                }

                edgesEditDistance += abs(vertex1Degree - vertex2Degree);
            }
        }

        if (edgesEditDistance < minEdgesEditDistance) {
            minEdgesEditDistance = edgesEditDistance;
        }
    } while (next_permutation(permutation, maxNumVertices));

    editDistance += minEdgesEditDistance;
    return editDistance;
}

#endif
