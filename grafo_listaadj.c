#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grafo_listaadj.h"

Graph *createGraph (int nodes) {
  printf("Criando grafo com %d vértices\n", nodes);
	Graph *g = (Graph *)malloc(sizeof(Graph)); //aloca espaço para estrtura grafo
	g->nodes = nodes; //atualizo o numero de vertice
	g->links = 0;  //atualizo o numero de vertice
	g->nodeList = (Node *)malloc((nodes + 1) * sizeof(Node)); //aloca espaço para o vetor de lista de adjacencia
	for(int i = 0; i <= nodes; i++) { //percorro o vetor de lista de adjacencia
		g->nodeList[i].head = NULL; //inicializo a cabeça da lista como nulo
		g->nodeList[i].edges = 0; //inicializo o numero de aresta como 0
	}
	printf("Grafo com %d vértices criado\n", nodes);
	return(g);
}

Link *createEdge(int node, int weight){ 
	Link *temp = (Link *)malloc(sizeof(Link)); //aloca espaço para um nó
	temp->dest = node; //vertice alvo da adjacencia
	temp->weight = weight ; //peso da aresta
	temp->next = NULL; 
	return(temp); //retorno endereço da adjacencia
}

bool addLink(Graph *g, int nodeI, int nodeE, Weight weight) {
	if(!g) return (false);  //validações se o grafo existe
	if((nodeE < 0) || (nodeE > g->nodes)) return(false); //validações se os valores não são neg
	if((nodeI < 0) || (nodeI > g->nodes)) return(false); //ou maiores que o numero de vértice do grafo
	
	Link *newLink= createEdge(nodeE, weight); //crio adjacencia com o vértice final e o peso
	//coloco a adjacencia na lista do vértice inicial
	newLink->next = g->nodeList[nodeI].head; //o campo prox da adjacencia vai receber a cabeça da lista
	g->nodeList[nodeI].head = newLink; // e a cabeça da lista passa a ser o novo elemento
	g->nodeList[nodeI].edges++; // atualizo o numero de aresta no grafo
	g->links++; // atualizo o numero de aresta no grafo
	return (true);
}

bool deleteLink(Graph *g, int nodeI, int nodeE) {
	if(!g) return (false);  //validações se o grafo existe
	if((nodeE < 0) || (nodeE > g->nodes)) return(false); //validações se os valores não são neg
	if((nodeI < 0) || (nodeI > g->nodes)) return(false); //ou maiores que o numero de vértice do grafo
	
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

void printGraph(Graph *g) {
  if(!g) return;
	printf("Printando grafo com %d vértices e %d arestas\n", g->nodes, g->links);
	printf("Lista de adjacência:\n");
	
  for(int i = 0; i <= g->nodes; i++) {
    printf("%d -> [", i);
		if(g->nodeList[i].edges != 0) {
			Link *temp = g->nodeList[i].head;
			for(int j = 0; j < g->nodeList[i].edges; j++) {
				if(j == g->nodeList[i].edges - 1) printf("%d]\n", temp->dest);
				else printf("%d, ", temp->dest);
				temp = temp->next;
			}
		} else printf("]\n");
	}
}

void deleteGraph(Graph *g) {
	if(!g) return;
	for(int i = 0; i < g->nodes; i++) {
		if(g->nodeList[i].edges != 0) {
			Link *temp = g->nodeList[i].head;
			while(!!temp->next) {
				Link *aux = temp;
				temp = temp->next;
				free(aux);
			}
		}
	}
	free(g->nodeList);
	free(g);
}

void getLinkDestAndWeight(Graph *g, int nodeI, int LinkIndex, int *dest, Weight *weight){
	if(!g) return;
	if((nodeI < 0) || (nodeI > g->nodes)) return;
	if((LinkIndex < 0) || (LinkIndex > g->nodeList[nodeI].edges)) return;
	if(g->nodeList[nodeI].edges == 0) {
		*dest = -1;
		*weight = -1;
		return;
	};
	Link *temp = g->nodeList[nodeI].head;
	for(int i = 0; i < LinkIndex; i++) {
		temp = temp->next;
	}
	*dest = temp->dest;
	*weight = temp->weight;
};

int getNumberOfEdges(Graph *g, int nodeI){
	if(!g) return -1;
	if((nodeI < 0) || (nodeI > g->nodes)) return -1;
	return g->nodeList[nodeI].edges;
};