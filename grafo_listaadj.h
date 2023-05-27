#include <stdio.h>
#include <stdlib.h>
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
} ListGraph;

ListGraph *createListGraph (int nodes);

Link *createEdge(int node, int weight);

bool addLink(ListGraph *g, int nodeI, int nodeE, Weight weight);

bool deleteLink(ListGraph *g, int nodeI, int nodeE);

void printListGraph(ListGraph *g);

void deleteListGraph(ListGraph *g);