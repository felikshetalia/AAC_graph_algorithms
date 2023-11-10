// AAC_graph_algorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS



int main(int argc, char* argv[])
{
    const char* fileName = "C:/Users/cansu/OneDrive/Masaüstü/AAC/AAC_graph_algorithms/example graphs/graph1.txt"; //any dir
    FILE* file = fopen(fileName, "r");
    if (!file) {
        perror("fopen");
        return -1;
    }
    char line[500];
    int size;
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
        if (line[1] == '\n') {
            size = line[0] - '0';
            continue;
        }
    }
    fclose(file);

    return 0;
}

