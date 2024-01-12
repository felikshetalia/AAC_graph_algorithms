#include <stdio.h>
#include <stdlib.h>
#include "maxClique.h"
#include "MCS.h"
#include <time.h>
#include "graphEditDistApproximation.h"
#include "graphEditDist.h"
// #include <conio.h>

const char* filename = "../example graphs/graphs.txt";
int NumberOfGraph;

//Malloc

int sizeCalculator(Graph* graph){

    int count = 0;

    for(int i = 0 ; i < graph->vertices;i++){
        for(int j = 0 ; j < graph->vertices;j++){

            if(graph->adjacencyMatrix[i][j] == 1){
                count++;
            }

        }
    }
    return (count/2) + graph->vertices;
}


int** allocateMatrix(int**graph,int ver) {
    int** matrix = (int**)malloc(ver * sizeof(int*));
    for (int i = 0; i < ver; ++i) {
        matrix[i] = (int*)malloc(ver * sizeof(int));
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
Graph** createGraphFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Read the number of graphs
    int vertices;
    int numGraphs;
    fscanf(file, "%d", &numGraphs);
    NumberOfGraph = numGraphs;
    //Apply malloc
    Graph** graphs = (Graph**)malloc(numGraphs*sizeof(Graph*));
    if (graphs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int graph = 0; graph < numGraphs; ++graph) {

        graphs[graph] = (Graph*)malloc(sizeof(Graph));
        if (graphs[graph] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        fscanf(file, "%d", &vertices);
        graphs[graph]->adjacencyMatrix = (int**)malloc(vertices * sizeof(int*));

        for (int i = 0; i < vertices; i++) {
            graphs[graph]->adjacencyMatrix[i] = (int*)malloc(vertices * sizeof(int));
        }
        // Read adjacency matrix
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                fscanf(file, "%d", &graphs[graph]->adjacencyMatrix[i][j]);
            }
        }

        graphs[graph]->vertices = vertices;
        // Read and discard empty line
        char buffer[256];
        fgets(buffer, sizeof(buffer), file);
        // Initialize row IDs with unique values

        // Initialize row IDs with unique values
        graphs[graph]->rowIDs = (int*)malloc(vertices * sizeof(int));
        if (graphs[graph]->rowIDs == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < graphs[graph]->vertices; ++i) {
            graphs[graph]->rowIDs[i] = i + 1; // You can use any unique identifier logic
        }

    }


    fclose(file);


    // Check if the matrix is symmetric
    for(int i=0; i<numGraphs;i++) {
        if (!isSymmetric(graphs[i]->adjacencyMatrix, graphs[i]->vertices)) {
            fprintf(stderr,
                    "Error: The adjacency matrix %d is not symmetric or contains 1 on diagonal. Not a valid graph.\n",i+1);
            freeGraph(graphs[i]);
            exit(EXIT_FAILURE);
        }
    }
    return graphs;
}






int main() {
    int cond = 1;
    int choice = 0;
    int graph_choice = 0;

    Graph *tempGraph;
    Graph *tempGraph1;
    Graph **graphs = createGraphFromFile(filename);
    int graphSizes[NumberOfGraph];
    for(int i = 0 ;i < NumberOfGraph; i++ ){
        graphSizes[i] = sizeCalculator(graphs[i] );
    }
    printf("The Graphs are successfully read");
//    for(int i=0;i<NumberOfGraph;i++) {
//        printAdjacencyMatrix(graphs[i]->adjacencyMatrix,graphs[i]->vertices);
//    }


    printf("\n");
    printf("Enter 1 or 2  \n1)CLIQUE ALGORITHMS \n2)EDIT DISTANCE\n3)Maximum Common Subgraph\n999)EXIT\n ");

    scanf("%d", &choice);

    printf("Choose the graph you would like to test:\n");
    for (int i = 0; i < NumberOfGraph; i++) {
        printf("%d) The graph with %d vertices with size of %d \n", i + 1, graphs[i]->vertices,graphSizes[i]);
    }


    printf("\n");
    switch (choice) {
        case 1: {
            scanf("%d", &graph_choice);
            graph_choice -= 1;
            tempGraph = graphs[graph_choice];
            printGraph(tempGraph);
            if(graph_choice == 7)
                printf("Brute force algorithm find the maximum clique in around 5 minutes for choosen graph due to its size");
            clock_t start1 = clock();
            bruteMaxClique(tempGraph);
            clock_t end1 = clock();
            double time_spent1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;
            printf("Time spent for Exponential Algorithm %f\n\n", time_spent1);
            int Q = 0;
            int Qmax = 0;


            // Print the obtained vertex colors
            //printGraph(tempGraph);
//            printf("Vertex Colors:\n");
//            for (int i = 0; i < tempGraph->vertices; ++i) {
//                printf("Vertex %d: Color %d\n", colors[i].vertex + 1, colors[i].color);
//            }

//            tempGraph->rowIDs = (int *) malloc(tempGraph->vertices * sizeof(int));
//            for (int i = 0; i < te1mpGraph->vertices; ++i) {
//                tempGraph->rowIDs[i] = i; // You can use any unique identifier logic
//            }
// Call maxClique function
            clock_t start = clock();
            VertexColorPair *colors = greedyVertexColoring(tempGraph);
            maxClique(tempGraph, colors, &Q, &Qmax);
            clock_t end = clock();
            float time_spent = ((float) (end - start)) / CLOCKS_PER_SEC;
            printf("Maximum Clique Size in Polynomial Algorithm= %d\n", Qmax);
            printf("Time spent for Polynomial Algorithm: %f", time_spent);

            free(colors);

            break;
        }

        case 2: {
            printf("Choose 2 graphs\n");
            scanf("%d", &graph_choice);
            graph_choice -= 1;
            tempGraph = graphs[graph_choice];
            scanf("%d",&graph_choice);
            graph_choice -= 1;
            tempGraph1 = graphs[graph_choice];

            //----- Exact solution -----
            clock_t start_time1 = clock();
            int result1 = graphEditDistance(tempGraph, tempGraph1);
            clock_t end_time1 = clock();
            double cpu_time_used1 = ((double) (end_time1 - start_time1)) / CLOCKS_PER_SEC;

            // ----- Polynomial solution -----
            clock_t start_time2 = clock();
            int result2 = graphEditDistancePolynomialApproximation(tempGraph, tempGraph1);
            clock_t end_time2 = clock();
            double cpu_time_used2 = ((double) (end_time2 - start_time2)) / CLOCKS_PER_SEC;

            printf("------------ Minumun Edit Distance --------------\n");
            printf("Exact Solution \n");
            printf("Minimum edit distance is %d \n", result1);
            printf("Run time of the exact solution is: %f \n", cpu_time_used1);
            printf("\n");
            printf("Approximation Solution \n");
            printf("Minimum edit distance is %d \n", result2);
            printf("Run time of approximation solution is: %f \n", cpu_time_used2);

            break;
        }

        case 3:{

            printf("Choose 2 graphs\n");
            scanf("%d", &graph_choice);
            graph_choice -= 1;
            tempGraph = graphs[graph_choice];
            scanf("%d",&graph_choice);
            graph_choice -= 1;
            tempGraph1 = graphs[graph_choice];

            MaxCommonSubgraph(tempGraph,tempGraph1);


            break;
        }
        default:
            printf("Invalid choice\n");

    }



    getchar();
    return EXIT_SUCCESS;
}
