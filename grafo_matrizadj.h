typedef int Weight; // Weight pode assumir outro tipo de acordo com a necessidade

typedef struct matrixGraph {
	int nodes; // É o número de vértices
	int links; // É o número de arestas
	Weight** nodeMat; // Caso não tenha peso na aresta será um bool
} MatrixGraph;

MatrixGraph* createMatrixGraph(int nodes);

void deleteMatrixGraph (MatrixGraph *g);

void printMatrixGraph(MatrixGraph *g);