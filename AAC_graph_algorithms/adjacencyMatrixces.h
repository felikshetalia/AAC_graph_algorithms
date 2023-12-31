
typedef struct VertexColorPair {
    int vertex;
    int color;
} VertexColorPair;

typedef struct Graph {
    int vertices;
    int** adjacencyMatrix;
    int* rowIDs;
} Graph;

typedef struct VertexDegreePair {
    int vertex;
    int degree;
} VertexDegreePair;
