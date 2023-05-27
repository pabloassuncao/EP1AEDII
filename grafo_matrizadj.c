#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int Weight; // Weight pode assumir outro tipo de acordo com a necessidade

typedef struct matrixGraph {
	int nodes; // É o número de vértices
	int links; // É o número de arestas
	Weight** nodeMat; // Caso não tenha peso na aresta será um bool
} MatrixGraph;

void deleteMatrixGraph (MatrixGraph *g) {
	if(!g) return;
	for(int i = 0; i < g->nodes; i++) {
		free(g->nodeMat[i]);
	}
	free(g->nodeMat);
	free(g);
}

MatrixGraph* createMatrixGraph(int nodes) {
	if(nodes <= 0) return NULL;
	MatrixGraph *g = (MatrixGraph*) malloc(sizeof(MatrixGraph));
	if(!g) return NULL;
	g->nodes = nodes;
	g->links = 0;
	g->nodeMat = (Weight**) malloc(nodes * sizeof(Weight*));
	if(!g->nodeMat) {
		free(g);
		return NULL;
	}
	for(int i = 0; i < nodes; i++) {
		g->nodeMat[i] = (Weight*) calloc(nodes, sizeof(Weight));
		if(!g->nodeMat[i]) {
			for(int j = 0; j < i; j++) {
				free(g->nodeMat[j]);
			}
			free(g->nodeMat);
			free(g);
			return NULL;
		}
	}
	return (g);
}

void printMatrixGraph(MatrixGraph *g) {
	if(!g) return;
	for(int i = 0; i < g->nodes; i++) {
		for(int j = 0; j < g->nodes; j++) {
			printf("%d ", g->nodeMat[i][j]);
		}
		printf("\n");
	}
}