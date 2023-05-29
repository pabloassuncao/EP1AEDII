#include <stdbool.h>

typedef int Weight; // Weight pode assumir outro tipo de acordo com a necessidade

typedef struct link {
	int dest; // Vértice destino, onde a aresta aponta
  Weight weight; // Peso da aresta caso o grafo exija, caso contrário não é essencial
	struct link *next; // Ponteiro para o próximo vértice adjacente
} Link;

// Estrutura não obrigatória por ter outra opção para usar
typedef struct node {
	Link *head; // Nó que indica a lista ligada dos vértices adjacentes ao vértice em questão
	int edges; // Número de arestas que saem do vértice
} Node;

typedef struct listGraph {
	int nodes; // É o número de vértices, não obrigatório mas ajuda a não ter que contar caso precise
	int links; // É o número de arestas, não obrigatório mas ajuda a não ter que contar caso precise
	Node *nodeList;
	// Link **nodeList; Esse código poderia ser usado caso não quisesse declarar uma terceira estrutura Node na implementação
} Graph;

Graph *createGraph (int nodes);

Link *createEdge(int node, int weight);

bool addLink(Graph *g, int nodeI, int nodeE, Weight weight);

bool deleteLink(Graph *g, int nodeI, int nodeE);

void getLinkDestAndWeight(Graph *g, int nodeI, int LinkIndex, int *dest, Weight *weight);

int getNumberOfEdges(Graph *g, int nodeI);

void printGraph(Graph *g);

void deleteGraph(Graph *g);