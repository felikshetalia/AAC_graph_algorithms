#include <stdio.h>
#include <stdlib.h>

typedef struct VertexColorPair {
    int vertex;
    int color;
} VertexColorPair;

typedef struct Graph {
    int vertices;
    int** adjacencyMatrix;
} Graph;

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

    fclose(file);

    // Check if the matrix is symmetric
    if (!isSymmetric(graph->adjacencyMatrix, vertices)) {
        fprintf(stderr, "Error: The adjacency matrix is not symmetric or contains 1 on diagonal. Not a valid graph.\n");
        freeGraph(graph);
        exit(EXIT_FAILURE);
    }
    return graph;
}

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



void removeRowColumn(Graph* graph, int targetRow, int targetCol) {
    // Decrease the number of vertices
    graph->vertices--;

    // Remove the selected row
    free(graph->adjacencyMatrix[targetRow]);

    // Move data up for the remaining rows
    for (int row = targetRow; row < graph->vertices; ++row) {
        graph->adjacencyMatrix[row] = graph->adjacencyMatrix[row + 1];
    }

    // Resize each remaining row to have the correct number of columns
    for (int row = 0; row < graph->vertices; ++row) {
        graph->adjacencyMatrix[row] = realloc(graph->adjacencyMatrix[row], sizeof(int) * graph->vertices);
    }

    // Move data to the left for the remaining columns

    for (int row = 0; row < graph->vertices; ++row) {
        for (int col = targetCol; col < graph->vertices; ++col) {
            graph->adjacencyMatrix[row][col] = graph->adjacencyMatrix[row][col + 1];
        }
    }
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

void maxClique(Graph* graph, VertexColorPair* colors, int* Q, int* Qmax) {
    int maxVertex = -1;

    while (graph->vertices > 0) {
        int maxColor = -1;

        // Choose a vertex p with a maximum color C(p) from set R
        for (int i = 0; i < graph->vertices; ++i) {
            if (colors[i].color > maxColor) {
                maxColor = colors[i].color;
                maxVertex = i;
            }
        }

        // Remove the selected vertex p from set R
        removeRowColumn(graph, maxVertex, maxVertex);
        printGraph(graph);

        if (*Q + colors[maxVertex].color > *Qmax) {
            *Q = *Q + 1 ;
            // Check if R ? ?(p) ? Ø
            int hasNeighborsInR = 0;
            for (int i = 0; i < graph->vertices; ++i) {
                if (graph->adjacencyMatrix[maxVertex][i]) {
                    hasNeighborsInR = 1;
                    break;
                }
            }

            if (hasNeighborsInR) {
                // Obtain a vertex-coloring C' of G(R ? ?(p))
                VertexColorPair *newColors = greedyVertexColoring(graph);
                for (int i = 0; i < graph->vertices; ++i) {
                    printf("Vertex %d: Color %d\n", colors[i].vertex + 1, colors[i].color);
                }
                // Recursive call with C'
                maxClique(graph, newColors, &Q, &Qmax);

                // Free the memory for C'
                free(newColors);
            } else if (*Q > *Qmax) {
                *Qmax = *Q;
                *Q = *Q -1;

            }
            else {
                return;
            }
        }
    }
}

int main() {
    const char* filename = "../example graphs/graph1.txt";

    Graph* graph = createGraphFromFile(filename);
    VertexColorPair *colors = greedyVertexColoring(graph);
    int Q =0;
    int Qmax = 0;
    // Print the obtained vertex colors
    printGraph(graph);
    printf("Vertex Colors:\n");
    for (int i = 0; i < graph->vertices; ++i) {
        printf("Vertex %d: Color %d\n", colors[i].vertex+1, colors[i].color);
    }

    // Call maxClique function
    maxClique(graph, colors, &Q, &Qmax);
    printf("Number of Cliques %d",Qmax);

    free(colors);
    freeGraph(graph);
    return 0;
}