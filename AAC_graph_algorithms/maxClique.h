#include "adjacencyMatrixces.h"
#include <stdio.h>

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

// void removeRowColumn(Graph* graph, int targetRow, int targetCol) {
//     // Decrease the number of vertices
//     graph->vertices--;
//
//     // Remove the selected row
//     free(graph->adjacencyMatrix[targetRow]);
//
//     // Move data up for the remaining rows
//     for (int row = targetRow; row < graph->vertices; ++row) {
//         graph->adjacencyMatrix[row] = graph->adjacencyMatrix[row + 1];
//     }
//
//     // Resize each remaining row to have the correct number of columns
//     for (int row = 0; row < graph->vertices; ++row) {
//         graph->adjacencyMatrix[row] = realloc(graph->adjacencyMatrix[row], sizeof(int) * graph->vertices);
//     }
//
//     // Move data to the left for the remaining columns
//
//     for (int row = 0; row < graph->vertices; ++row) {
//         for (int col = targetCol; col < graph->vertices; ++col) {
//             graph->adjacencyMatrix[row][col] = graph->adjacencyMatrix[row][col + 1];
//         }
//     }
// }

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

        // Check colors of neighbors and mark them as u sed
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

// void maxClique(Graph* graph, VertexColorPair* colors, int* Q, int* Qmax) {
//     int maxVertex = -1;
//
//     while (graph->vertices > 0) {
//         int maxColor = -1;
//
//         // Choose a vertex p with a maximum color C(p) from set R
//         for (int i = 0; i < graph->vertices; ++i) {
//             if (colors[i].color > maxColor) {
//                 maxColor = colors[i].color;
//                 maxVertex = i;
//             }
//         }
//
//         // Remove the selected vertex p from set R
//         removeRowColumn(graph, maxVertex, maxVertex);
//         printGraph(graph);
//
//         if (*Q + colors[maxVertex].color > *Qmax) {
//             *Q = *Q + 1 ;
//             // Check if R ? ?(p) ? �
//             int hasNeighborsInR = 0;
//             for (int i = 0; i < graph->vertices; ++i) {
//                 if (graph->adjacencyMatrix[maxVertex][i]) {
//                     hasNeighborsInR = 1;
//                     break;
//                 }
//             }
//
//             if (hasNeighborsInR) {
//                 // Obtain a vertex-coloring C' of G(R ? ?(p))
//                 VertexColorPair *newColors = greedyVertexColoring(graph);
//                 for (int i = 0; i < graph->vertices; ++i) {
//                     printf("Vertex %d: Color %d\n", colors[i].vertex + 1, colors[i].color);
//                 }
//                 // Recursive call with C'
//                 maxClique(graph, newColors, &Q, &Qmax);
//
//                 // Free the memory for C'
//                 free(newColors);
//             } else if (*Q > *Qmax) {
//                 *Qmax = *Q;
//                 *Q = *Q -1;
//
//             }
//             else {
//                 return;
//             }
//         }
//     }
// }
