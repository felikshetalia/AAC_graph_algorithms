// AAC_graph_algorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define _CRT_SECURE_NO_WARNINGS


typedef struct Graph {
    int size;
    int **AdjacencyMtx;
}Graph;

Graph init_graph(char* filename) {
    // init size
    Graph graph;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        exit(0);
    }
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        //printf("%s", line);
        //init size
        if (line[1] == '\n') {
            graph.size = atoi(line);
            continue;
        }
    }

    //graph.AdjacencyMtx = (int**)malloc(graph.size * sizeof(int*));
    //if (!graph.AdjacencyMtx) {
    //    perror("malloc");
    //    fclose(file);
    //    exit(0);
    //}
    //for (int i = 0; i < graph.size; i++) {
    //    graph.AdjacencyMtx[i] = (int*)malloc(graph.size * sizeof(int));
    //    if (!graph.AdjacencyMtx[i]) {
    //        perror("malloc");
    //        // Free previously allocated memory
    //        for (int j = 0; j < i; j++) {
    //            free(graph.AdjacencyMtx[j]);
    //        }
    //        free(graph.AdjacencyMtx);
    //        fclose(file);
    //        exit(0);
    //    }
    //}

    /*for (int i = 0; i < graph.size; i++) {
        for (int j = 0; j < graph.size; j++) {
            graph.AdjacencyMtx[i][j] = 0;
        }
    }*/


    fclose(file);
    printf("%d", graph.size);

    /*for (int i = 0; i < graph->size; i++) {
        free(graph.AdjacencyMtx[i]);
    }
    free(graph.AdjacencyMtx);*/

    return graph;
}



int main(int argc, char* argv[])
{
    //const char* fileName = "C:/Users/cansu/OneDrive/Masaüstü/AAC/AAC_graph_algorithms/example graphs/graph1.txt"; //any dir
    //FILE* file = fopen(fileName, "r");
    //if (!file) {
    //    perror("fopen");
    //    return -1;
    //}
    //char line[500];
    //int size;
    //while (fgets(line, sizeof(line), file)) {
    //    printf("%s", line);
    //    if (line[1] == '\n') {
    //        size = line[0] - '0';
    //        continue;
    //    }
    //}
    //fclose(file);

    Graph graph = init_graph("../example graphs/graph1.txt");


    return 0;
}

