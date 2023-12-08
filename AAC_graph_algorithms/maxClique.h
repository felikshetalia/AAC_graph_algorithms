#include "adjacencyMatrixces.h"
#include <stdio.h>
#include <string.h>

//TEST FOR PRINTING GRAPH
void printGraph(const Graph* graph) {
    printf("Number of Vertices: %d\n", graph->vertices);
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < graph->vertices; ++i) {
        for (int j = 0; j < graph->vertices; ++j) {
            printf("%d ", graph->adjacencyMatrix[i][j]);
        }
        printf("\n");
    }
}


void removeRowColumn(Graph *graph, int row, int col) {
    int i, j;

    // Shifting rows to the left
    for (i = row; i < graph->vertices - 1; ++i) {
        for (j = 0; j < graph->vertices; ++j) {
            graph->adjacencyMatrix[i][j] = graph->adjacencyMatrix[i + 1][j];
        }
    }

    // Shifting columns upwards
    for (j = col; j < graph->vertices - 1; ++j) {
        for (i = 0; i < graph->vertices; ++i) {
            graph->adjacencyMatrix[i][j] = graph->adjacencyMatrix[i][j + 1];
        }
    }

    // Decreasing the number of vertices after removing a row and a column
    graph->vertices--;
}
VertexColorPair* greedyVertexColoring(const Graph* graph) {
    VertexColorPair* vertexColors = (VertexColorPair*)malloc(graph->vertices * sizeof(VertexColorPair));
    if (vertexColors == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize all colors as unassigned (-1)
    for (int i = 0; i < graph->vertices; ++i) {
        vertexColors[i].vertex = i;
        vertexColors[i].color = -1;

    }

    // Assign colors to vertices
    for (int vertex = 0; vertex < graph->vertices; ++vertex) {
        // Array to keep track of colors used by neighbors
        int usedColors[graph->vertices];
        for (int i = 0; i < graph->vertices; ++i) {
            usedColors[i] = 0;
        }

        // Check colors of neighbors and mark them as used
        for (int neighbor = 0; neighbor < graph->vertices; ++neighbor) {
            if (graph->adjacencyMatrix[vertex][neighbor] && vertexColors[neighbor].color != -1) {
                usedColors[vertexColors[neighbor].color] = 1;
            }
        }

        // Assign the first available color to the current vertex
        for (int color = 0; color < graph->vertices; ++color) {
            if (!usedColors[color]) {
                vertexColors[vertex].color = color;
                break;
            }
        }
    }

    return vertexColors;
}


//int * pushToArray(int *array, int size, int element) {
//    // Create a new array with one additional element
//    int newSize = size + 1;
//    int* newArray ;
//    memcpy(newArray,array,element*sizeof(int));
//
//    if (newArray == NULL) {
//        fprintf(stderr, "Memory allocation failed\n");
//        exit(EXIT_FAILURE);
//    }
//
//    // Copy elements from the original array to the new array
//    for (int i = 0; i < size; i++) {
//        newArray[i] = array[i];
//    }
//
//    // Add the new element to the end of the new array
//    newArray[newSize-1] = element;
//    return newArray;
//}


void maxClique(Graph* graph, VertexColorPair* colors, int *Q, int* Qmax) {
//    int *ID = NULL;
    int maxVertex = -1;
    int pNoElement = graph->vertices;
    int* P = (int*)malloc(graph->vertices * sizeof(int));
    while (graph->vertices > 0) {
        int maxColor = -1;

        // Choose a vertex p with a maximum color C(p) from set R
        for (int i = 0; i < graph->vertices; ++i) {
            if (colors[i].color > maxColor) {
                maxColor = colors[i].color;
                maxVertex = i;

            }
        }

        memset(P, 0, graph->vertices * sizeof(int));
        for(int i=0;i<graph->vertices;++i){
           P[i] = graph->adjacencyMatrix[maxVertex][i];
        }

        // Remove the selected vertex and its non-adjacent vertices from set R
        for (int i = 0; i < graph->vertices; i++) {
            if (i != maxVertex && !graph->adjacencyMatrix[maxVertex][i]) {
                // Remove the vertex i from set R
                removeRowColumn(graph, i, i);

                if(i<maxVertex){
                    maxVertex--;
                    for(int j = i;j<graph->vertices-1;j++){
                        graph->rowIDs[j] = graph->rowIDs[j + 1];
                    }
                }
                i--;
            }
        }
//       ID = pushToArray(&ID,*Q,graph->rowIDs[maxVertex]);
    // Remove the selected vertex from set R
        removeRowColumn(graph, maxVertex, maxVertex);
        printGraph(graph);
        if (*Q + maxColor > *Qmax) {
            *Q = *Q + 1 ;
            // Check if R ? ?(p) ? Ø
            int hasNeighborsInR = 0;
            for (int i = 0; i < pNoElement; ++i) {
                if (P[i]) {
                    hasNeighborsInR = 1;
                    break;
                }
            }
            if (hasNeighborsInR) {
                // Obtain a vertex-coloring C' of G(R ? ?(p))
                VertexColorPair *newColors = greedyVertexColoring(graph);
                for (int i = 0; i < graph->vertices; ++i) {
                    printf("Vertex %d: Color %d\n", i + 1, newColors[i].color);
                }
                maxClique(graph, newColors, Q, Qmax);
            }
            else if (*Q > *Qmax) {
                *Qmax = *Q;
                *Q = *Q -1;
            }

        }
        else {
            return;
        }
    }

}
