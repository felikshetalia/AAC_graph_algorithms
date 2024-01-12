// metric.h
#ifndef METRIC_H
#define METRIC_H

#include <stdlib.h>
#include "adjacencyMatrixces.h"  // Assuming you have adjacencyMatrices.h

typedef struct {
    int* set;
    int size;
} ResolvingSet;

void printSubset(int* subset, int size) {
    printf("{");
    for (int i = 0; i < size; i++) {
        printf("%d", subset[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

void generateSubsetsUtil(Graph* graph, int* subset, int index, int* subsetIndex) {
    int vertices = graph->vertices;

    if (index == vertices) {
        printSubset(subset, *subsetIndex);
        return;
    }

    // Exclude the current vertex
    generateSubsetsUtil(graph, subset, index + 1, subsetIndex);

    // Include the current vertex
    subset[(*subsetIndex)++] = index;
    generateSubsetsUtil(graph, subset, index + 1, subsetIndex);

    // Backtrack
    (*subsetIndex)--;
}

void generateSubsets(Graph* graph) {
    int vertices = graph->vertices;
    int* subset = (int*)malloc(vertices * sizeof(int));
    int subsetIndex = 0;

    generateSubsetsUtil(graph, subset, 0, &subsetIndex);

    free(subset);
}

// Function to calculate distance using BFS
int calculateDistance(Graph* graph, int start, int end) {
    if (start == end) {
        return 0;
    }

    int* visited = (int*)malloc(graph->vertices * sizeof(int));
    for (int i = 0; i < graph->vertices; ++i) {
        visited[i] = 0;
    }

    int* distance = (int*)malloc(graph->vertices * sizeof(int));
    for (int i = 0; i < graph->vertices; ++i) {
        distance[i] = -1;
    }

    visited[start] = 1;
    distance[start] = 0;

    int queue[MAX_VERTICES];
    int front = -1, rear = -1;
    queue[++rear] = start;

    while (front != rear) {
        int current = queue[++front];

        for (int i = 0; i < graph->vertices; i++) {
            if (graph->adjacencyMatrix[current][i] == 1 && !visited[i]) {
                visited[i] = 1;
                distance[i] = distance[current] + 1;
                queue[++rear] = i;

                if (i == end) {
                    int res = distance[i];
                    free(visited);
                    free(distance);
                    return res;
                }
            }
        }
    }

    free(visited);
    free(distance);

    return -1;
}

// Function to check if a set is resolving
int isResolvingSet(Graph* graph, int* set, int setSize) {

}

// Function to find and print all resolving sets
void findAllAndPrintResolvingSets(Graph* graph) {
  
}

// Function to free memory associated with a ResolvingSet
void freeResolvingSet(ResolvingSet* resolvingSet) {
    free(resolvingSet->set);
}

#endif // METRIC_H
