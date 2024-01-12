// metric.h
#ifndef METRIC_H
#define METRIC_H

#include <stdlib.h>
#include "adjacencyMatrixces.h"  // Assuming you have adjacencyMatrices.h

typedef struct {
    int* set;
    int size;
} ResolvingSet;

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
    // Iterate through all pairs of vertices
    for (int u = 0; u < graph->vertices; ++u) {
        for (int v = u + 1; v < graph->vertices; ++v) {
            int uToSet = calculateDistance(graph, u, set[0]);
            int vToSet = calculateDistance(graph, v, set[0]);

            // Check if the distances from u and v to the set are different
            if (uToSet == vToSet) {
                return 0; // Not resolving
            }

            // Check if adding any other vertex to the set makes the distances the same
            for (int i = 1; i < setSize; ++i) {
                int uDistance = calculateDistance(graph, u, set[i]);
                int vDistance = calculateDistance(graph, v, set[i]);

                if (uToSet + uDistance == vToSet + vDistance) {
                    return 0; // Not resolving
                }
            }
        }
    }

    return 1; // Resolving
}

// Function to find and print all resolving sets
void findAllAndPrintResolvingSets(Graph* graph) {
    printf("------------ All Resolving Sets --------------\n");

    bool found = 0;  // Variable to track if any resolving set is found

    // Find and print all resolving sets
    for (int mask = 1; mask < (1 << graph->vertices); ++mask) {
        ResolvingSet currentSet;
        currentSet.size = 0;
        currentSet.set = (int*)malloc(graph->vertices * sizeof(int));

        // Check which vertices are in the current subset
        for (int i = 0; i < graph->vertices; ++i) {
            if (mask & (1 << i)) {
                currentSet.set[currentSet.size++] = i;
            }
        }

        // Check if the current set is resolving
        if (isResolvingSet(graph, currentSet.set, currentSet.size)) {

            found = 1;
            printf("Resolving Set: ");
            for (int i = 0; i < currentSet.size - 1; ++i) {
                printf("%d, ", currentSet.set[i]);
            }
            printf("%d\n", currentSet.set[currentSet.size - 1]);
        }

        free(currentSet.set);
    }

    if (!found) {
        printf("Not Found\n");
    }
}

// Function to free memory associated with a ResolvingSet
void freeResolvingSet(ResolvingSet* resolvingSet) {
    free(resolvingSet->set);
}


ResolvingSet findMinResolvingSet(Graph* graph) {
    ResolvingSet minResolvingSet;
    minResolvingSet.size = graph->vertices;
    minResolvingSet.set = (int*)malloc(minResolvingSet.size * sizeof(int));

    // Initialize the set with all vertices
    for (int i = 0; i < minResolvingSet.size; ++i) {
        minResolvingSet.set[i] = i;
    }

    // Iterate through all possible subsets
    for (int mask = 0; mask < (1 << graph->vertices); ++mask) {
        ResolvingSet currentSet;
        currentSet.size = 0;
        currentSet.set = (int*)malloc(graph->vertices * sizeof(int));

        // Check which vertices are in the current subset
        for (int i = 0; i < graph->vertices; ++i) {
            if (mask & (1 << i)) {
                currentSet.set[currentSet.size++] = i;
            }
        }

        // Check if the current set is resolving and has fewer elements
        if (isResolvingSet(graph, currentSet.set, currentSet.size) && currentSet.size < minResolvingSet.size) {
            // Update the minimum resolving set
            minResolvingSet.size = currentSet.size;
            free(minResolvingSet.set);
            minResolvingSet.set = (int*)malloc(minResolvingSet.size * sizeof(int));
            for (int i = 0; i < minResolvingSet.size; ++i) {
                minResolvingSet.set[i] = currentSet.set[i];
            }
        }

        free(currentSet.set);
    }

    return minResolvingSet;
}



#endif // METRIC_H
