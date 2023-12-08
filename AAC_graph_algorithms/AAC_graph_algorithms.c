#include <stdio.h>
#include <stdlib.h>
#include "maxClique.h"
#include "MSC.h"
#include "bruteMaxClique.h"
#include <time.h>
#include "graphEditDistApproximation.h"
#include "graphEditDist.h"
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
    free(graph->rowIDs);
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
    graph->rowIDs = (int*)malloc(graph->vertices * sizeof(int));

    // Initialize row IDs with unique values
    for (int i = 0; i < graph->vertices; ++i) {
        graph->rowIDs[i] = i + 1; // You can use any unique identifier logic
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

    int choice =0;

    printf("Enter 1,2 or 3 \n1)GREEDY MAX CLIQUE ALGORITHM \n2)BRUTE FORCE MAX CLIQUE ALGORITHM\n3)METRIC DISTANCE\n ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: {

            const char* filename = "../example graphs/graph1.txt";
            Graph* graph = createGraphFromFile(filename);
            VertexColorPair *colors = greedyVertexColoring(graph);
            int Q = 0;
            int Qmax = 0;
            // Print the obtained vertex colors
            printGraph(graph);
            printf("Vertex Colors:\n");
            for (int i = 0; i < graph->vertices; ++i) {
                printf("Vertex %d: Color %d\n", colors[i].vertex + 1, colors[i].color);
            }

            graph->rowIDs = (int *) malloc(graph->vertices * sizeof(int));
            for (int i = 0; i < graph->vertices; ++i) {
                graph->rowIDs[i] = i; // You can use any unique identifier logic
            }
            // Call maxClique function
            clock_t start = clock();
            maxClique(graph, colors, &Q, &Qmax);
            clock_t end = clock();
            double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("Time spent: %f\n",time_spent);
            printf("Number of Vertices in Clique= %d", Qmax);
            free(colors);
            freeGraph(graph);
        }
        break;
        case 2:
        {
            const char* filename = "../example graphs/graph1.txt";
            Graph* graph = createGraphFromFile(filename);
            clock_t start = clock();
            bruteMaxClique(graph);
            clock_t end = clock();
            double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("time spent %f\n",time_spent);
            freeGraph(graph);

        }break;
        case 3:{
            const char* filename1 = "../example graphs/graph1.txt";
            const char* filename2 = "../example graphs/graph2.txt";
            Graph* graph1 = createGraphFromFile(filename1);
            Graph* graph2 = createGraphFromFile(filename2);


            //----- Exact solution -----
             clock_t start_time1 = clock();
             int result1 = graphEditDistance(graph1, graph2);
             clock_t end_time1 = clock();
             double cpu_time_used1 = ((double) (end_time1 - start_time1)) / CLOCKS_PER_SEC;

            // ----- Polynomial solution -----
            clock_t start_time2 = clock();
            int result2 = graphEditDistancePolynomialApproximation(graph1, graph2);
            clock_t end_time2 = clock();
            double cpu_time_used2 = ((double) (end_time2 - start_time2)) / CLOCKS_PER_SEC;

             printf("------------ Minumun Edit Distance --------------\n");
             printf("Exact Solution \n");
             printf("Minimum edit distance is %d \n", result1);

            printf("Approximation Solution \n");
            printf("Minimum edit distance is %d \n", result2);

            printf("%f \n", cpu_time_used1);
            printf(" %f \n", cpu_time_used2);

        }break;


        default:
            printf("Invalid choice\n");
    }



//    printf("Vertices in Maximum Clique: ");
//    for (int i = 0; i < Qmax; ++i) {
//        printf("%d ", &cliqueVertices[i] + 1); // Adding 1 to convert from 0-based to 1-based index
//    }



   // free(cliqueVertices);
    return 0;
}