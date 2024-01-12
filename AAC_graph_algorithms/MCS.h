#include <stdio.h>
#include <string.h>

void init_graph(Graph *graph, int numVertices) {
    if (graph == NULL) {
        printf("Error: Graph is NULL.\n");
        exit(1);
    }

    graph->vertices = numVertices;

    //graph->adjacencyMatrix = allocateMatrix(graph, numVertices);
    graph->adjacencyMatrix = (int**)malloc(numVertices*sizeof(int*));
    for(int i = 0; i < numVertices; i++){
        graph->adjacencyMatrix[i] = (int*)malloc(numVertices*sizeof(int));
    }
    if (graph->adjacencyMatrix == NULL) {
        printf("Error: Unable to allocate adjacency matrix.\n");
        exit(1);
    }

    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            graph->adjacencyMatrix[i][j] = 0;
        }
    }
}


void addEdge(Graph *graph, int src, int dest) {
    graph->adjacencyMatrix[src][dest] = 1;
    graph->adjacencyMatrix[dest][src] = 1;
}

void modularProduct(const Graph *g1,const Graph *g2,Graph *g3){

    int numVertices = g1->vertices*g2->vertices;
    init_graph(g3, numVertices);
    for (int i = 0; i < g1->vertices; ++i) {
        g1->adjacencyMatrix[i] = (int*)malloc(g1->vertices * sizeof(int));
    }
    for(int i=0;i<g1->vertices;i++){
        for(int j=0;j<g2->vertices;j++){
            for(int x =0;x<g1->vertices;x++){
                for(int y=0;y< g2->vertices;y++){
                    if((g1->adjacencyMatrix[i][x]== 1 && g2->adjacencyMatrix[j][y] == 1 && i != x && j !=1) ||
                            (g1->adjacencyMatrix[i][y]==0 && g2->adjacencyMatrix[j][y]==0 && i!=x && j!=1)){
                        int v1 = i*g2->vertices+j;
                        int v2 = x*g2->vertices+y;
                        addEdge(g3,v1,v2);
                    }

                }
            }
        }
    }


}

void MaxCommonSubgraph(Graph* g1, Graph* g2){
    Graph* res = (Graph*)malloc(sizeof(Graph));
    if (res == NULL) {
        printf("Error allocating memory for the result graph.\n");
        // Handle memory allocation error, such as by exiting the program
        exit(1);
    }
    modularProduct(g1,g2,res);
    printf("Number of Vertices: %d\n", res->vertices);
    printAdjacencyMatrix(res->adjacencyMatrix, res->vertices);
    int Q, Qmax;
    //VertexColorPair *colors = greedyVertexColoring(res);

    //maxClique(res, colors, &Q, &Qmax);
    bruteMaxClique(res);

    //free(colors);
    free(res);
}