#include <stdbool.h>

typedef int Weight; // Weight pode assumir outro tipo de acordo com a necessidade

typedef struct Graph{
	int nodes; // É o número de vértices
	int links; // É o número de arestas
	Weight** nodeMat; // Caso não tenha peso na aresta será um bool
} Graph;

Graph* createGraph(int nodes);

void deleteGraph (Graph *g);

bool addLink(Graph *g, int nodeI, int nodeE, Weight weight);

bool deleteLink(Graph *g, int nodeI, int nodeE);

void getLinkDestAndWeight(Graph *g, int nodeI, int LinkIndex, int *dest, Weight *weight);

int getNumberOfEdges(Graph *g, int nodeI);

void printGraph(Graph *g);