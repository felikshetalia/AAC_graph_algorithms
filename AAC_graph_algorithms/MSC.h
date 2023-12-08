#include <stdio.h>
#include <string.h>

void addEdge(Graph *graph, int src, int dest) {
    graph->adjacencyMatrix[src][dest] = 1;
    graph->adjacencyMatrix[dest][src] = 1;
}

void modularProduct(const Graph *g1,const Graph *g2,Graph *g3){

    int numVertices = g1->vertices*g2->vertices;
    g3 = (int**)malloc(g1->vertices * sizeof(int*));
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