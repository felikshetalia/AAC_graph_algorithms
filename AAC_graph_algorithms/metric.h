// metric.h
#ifndef METRIC_H
#define METRIC_H

#include <stdlib.h>
#include "adjacencyMatrixces.h"  // Assuming you have adjacencyMatrices.h

typedef struct {
    int* set;
    int size;
} ResolvingSet;


typedef struct Node {
    ResolvingSet resolvingSet;
    struct Node* next;
} Node;

Node* resolvingSetsList = NULL;  // Global linked list to store resolving sets

void freeResolvingSet(ResolvingSet* resolvingSet);

void freeResolvingSetsList() {
    Node* current = resolvingSetsList;
    while (current != NULL) {
        Node* next = current->next;
        freeResolvingSet(&current->resolvingSet);
        free(current);
        current = next;
    }
    resolvingSetsList = NULL;
}

void freeResolvingSet(ResolvingSet* resolvingSet) {
    free(resolvingSet->set);
}

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

void storeResolvingSet(int* subset, int subsetIndex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->resolvingSet.set = (int*)malloc(subsetIndex * sizeof(int));
    newNode->resolvingSet.size = subsetIndex;
    for (int i = 0; i < subsetIndex; ++i) {
        newNode->resolvingSet.set[i] = subset[i];
    }
    newNode->next = resolvingSetsList;
    resolvingSetsList = newNode;
}

void generateSubsetsUtil(Graph* graph, int* subset, int index, int* subsetIndex) {
    int vertices = graph->vertices;

    if (index == vertices) {
        printSubset(subset, *subsetIndex);
        storeResolvingSet(subset, *subsetIndex);
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

bool isResolvingSet(Graph* graph, int* set, int setSize) {
    int** distanceLists = (int**)malloc(setSize * sizeof(int*));

    for (int i = 0; i < setSize; ++i) {
        distanceLists[i] = (int*)malloc(graph->vertices * sizeof(int));
    }

    // Calculate distances and store them in the array
    for (int i = 0; i < setSize; ++i) {
        for (int j = 0; j < graph->vertices; ++j) {
            distanceLists[i][j] = calculateDistance(graph, set[i], j);
        }
    }

    // Print the distance lists for debugging
    for (int i = 0; i < setSize; ++i) {
        printf("[ ");
        for (int j = 0; j < graph->vertices; ++j) {
            printf("%d ", distanceLists[i][j]);
        }
        printf("]\n");
    }

    // Free allocated memory
    for (int i = 0; i < setSize; ++i) {
        free(distanceLists[i]);
    }
    free(distanceLists);

    // printf("True\n");
    return true;
}

// bool isResolvingSet(Graph* graph, int* set, int setSize) {
//     // Create an array to store distances for each vertex in the set
//     // printf("ResolvingSet is called!\n");
//     int** distanceLists = (int**)malloc(setSize * sizeof(int*));
//     int* encountered = (int*)malloc(setSize * sizeof(int));
//
//     for (int i = 0; i < setSize; ++i) {
//         distanceLists[i] = (int*)malloc(graph->vertices * sizeof(int));
//     }
//
//     // Calculate distances and store them in the array
//     for (int i = 0; i < setSize; ++i) {
//         for (int j = 0; j < graph->vertices; ++j) {
//             distanceLists[i][j] = calculateDistance(graph, set[i], j);
//         }
//     }
//
//     for (int i = 0; i < setSize; ++i) {
//         printf("[ ");
//         for (int j = 0; j < graph->vertices; ++j) {
//             printf("%d ",distanceLists[i][j]);
//         }
//         printf("]\n");
//     }
//
//     // Check for duplicate lists
//     for (int i = 0; i < setSize; ++i) {
//         if (!encountered[i]) {
//             for (int j = i + 1; j < setSize; ++j) {
//                 if (!encountered[j]) {
//                     int isDuplicate = 1;
//                     for (int k = 0; k < graph->vertices; ++k) {
//                         if (distanceLists[i][k] != distanceLists[j][k]) {
//                             isDuplicate = 0;
//                             break;
//                         }
//                     }
//                     if (isDuplicate) {
//                         // Free allocated memory
//                         for (int l = 0; l < setSize; ++l) {
//                             free(distanceLists[l]);
//                         }
//                         free(distanceLists);
//                         free(encountered);
//                         printf("False\n");
//                         return false; // Found a duplicate list
//                     }
//                 }
//             }
//             encountered[i] = 1;  // Mark this list as encountered
//         }
//     }
//
//     // Free allocated memory
//     for (int i = 0; i < setSize; ++i) {
//         free(distanceLists[i]);
//     }
//     free(distanceLists);
//     // printf("True\n");
//     return true;
// }

void findAllAndPrintResolvingSets(Graph* graph) {
    int vertices = graph->vertices;
    int* subset = (int*)malloc(vertices * sizeof(int));
    int subsetIndex = 0;

    generateSubsetsUtil(graph, subset, 0, &subsetIndex);

    // Pointer to keep track of the previous node while traversing the list
    Node* prev = NULL;

    // Iterate through the stored resolving sets
    Node* current = resolvingSetsList;
    while (current != NULL) {
        ResolvingSet resolvingSet = current->resolvingSet;

        // printf("Calling isResolvingSet...\n");

        if (isResolvingSet(graph, resolvingSet.set, resolvingSet.size)) {
            // If the subset is a resolving set, save and print it

            // Print the resolving set directly
            // printf("Resolving Set: {");
            // printf("resolving set size: %d\n", resolvingSet.size);

            for (int j = 0; j < resolvingSet.size; ++j) {
                printf("%d", resolvingSet.set[j]);
                if (j < resolvingSet.size - 1) {
                    printf(", ");
                }
            }
            printf("}\n");

            // Move to the next node
            prev = current;
            current = current->next;
        } else {
            // If isResolvingSet returns false, remove the node from the list

            // Update the pointers
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                resolvingSetsList = current->next;
            }

            // Free memory associated with the resolving set and the node
            freeResolvingSet(&resolvingSet);
            free(current);

            // Move to the next node
            current = prev != NULL ? prev->next : resolvingSetsList;
        }
    }

    free(subset);
}


#endif // METRIC_H
