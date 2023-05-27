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

ListGraph *createListGraph (int nodes) {
  printf("Criando grafo com %d vértices\n", nodes);
	ListGraph *g = (ListGraph *)malloc(sizeof(ListGraph)); //aloca espaço para estrtura grafo
	g->nodes = nodes; //atualizo o numero de vertice
	g->links = 0;  //atualizo o numero de vertice
	g->nodeList = (Node *)calloc((nodes), sizeof(Node)); //aloca espaço para o vetor de lista de adjacencia
	return(g);
}

Link *createEdge(int node, int weight){ 
	Link *temp = (Link *)malloc(sizeof(Link)); //aloca espaço para um nó
	temp->dest = node; //vertice alvo da adjacencia
	temp->weight = weight ; //peso da aresta
	temp->next = NULL; 
	return(temp); //retorno endereço da adjacencia
}

bool addLink(ListGraph *g, int nodeI, int nodeE, Weight weight) {
	if(!g) return (false);  //validações se o grafo existe
	if((nodeE < 0) || (nodeE >= g->nodes)) return(false); //validações se os valores não são neg
	if((nodeI < 0) || (nodeI >= g->nodes)) return(false); //ou maiores que o numero de vértice do grafo
	
	Link *newLink= createEdge(nodeE, weight); //crio adjacencia com o vértice final e o peso
	//coloco a adjacencia na lista do vértice inicial
	newLink->next = g->nodeList[nodeI].head; //o campo prox da adjacencia vai receber a cabeça da lista
	g->nodeList[nodeI].head = newLink; // e a cabeça da lista passa a ser o novo elemento
	g->nodeList[nodeI].edges++; // atualizo o numero de aresta no grafo
	g->links++; // atualizo o numero de aresta no grafo
	return (true);
}

bool deleteLink(ListGraph *g, int nodeI, int nodeE) {
	if(!g) return (false);  //validações se o grafo existe
	if((nodeE < 0) || (nodeE >= g->nodes)) return(false); //validações se os valores não são neg
	if((nodeI < 0) || (nodeI >= g->nodes)) return(false); //ou maiores que o numero de vértice do grafo
	
	Link *ant;
	Link *temp = g->nodeList[nodeI].head;
	while(!!temp && temp->dest !=  nodeE) {
		ant = temp;
		temp = temp->next;
	}
	ant->next = temp->next;
	free(temp);
	g->nodeList[nodeI].edges--;
	g->links--;
  return (true);
}

void printListGraph(ListGraph *g) {
  if(!g) return;
  for(int i = 0; i < g->nodes; i++) {
    printf("%d -> [", i+1);
    Link *temp = g->nodeList[i].head;
    while(!!temp->next) {
      printf("%d, ", temp->dest+1);
      temp = temp->next;
    }
    printf("%d]\n", temp->dest+1);
  }
}

void deleteListGraph(ListGraph *g) {
	if(!g) return;
	for(int i = 0; i < g->nodes; i++) {
		Link *temp = g->nodeList[i].head;
		while(!!temp->next) {
			Link *aux = temp;
			temp = temp->next;
			free(aux);
		}
	}
	free(g->nodeList);
	free(g);
}