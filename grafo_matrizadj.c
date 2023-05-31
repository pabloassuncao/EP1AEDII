#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grafo_matrizadj.h"

void deleteGraph (Graph *g) {
	if(!g) return;
	for(int i = 0; i <= g->nodes; i++) {
		free(g->nodeMat[i]);
	}
	free(g->nodeMat);
	free(g);
}

Graph* createGraph(int nodes) {
	if(nodes <= 0) return NULL;
	Graph *g = (Graph*) malloc(sizeof(Graph));
	if(!g) return NULL;
	g->nodes = nodes;
	g->links = 0;
	g->nodeMat = (Weight**) malloc((nodes+1) * sizeof(Weight*));
	if(!g->nodeMat) {
		free(g);
		return NULL;
	}
	for(int i = 0; i <= nodes; i++) {
		g->nodeMat[i] = (Weight*) calloc((nodes+1), sizeof(Weight));
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

void printGraph(Graph *g) {
	if(!g) return;
	for(int i = 0; i <= g->nodes; i++) {
		for(int j = 0; j <= g->nodes; j++) {
			printf("%d ", g->nodeMat[i][j]);
		}
		printf("\n");
	}
}

bool addLink(Graph *g, int nodeI, int nodeE, Weight weight){
	if(!g) return false;
	if((nodeI < 0) || (nodeI > g->nodes)) return false;
	if((nodeE < 0) || (nodeE > g->nodes)) return false;
	if(g->nodeMat[nodeI][nodeE] != 0) return false;
	g->nodeMat[nodeI][nodeE] = weight;
	g->nodeMat[nodeE][nodeI] = weight;
	g->links += 2;
	return (true);
};

bool deleteLink(Graph *g, int nodeI, int nodeE){
	if(!g) return false;
	if((nodeI < 0) || (nodeI >= g->nodes)) return false;
	if((nodeE < 0) || (nodeE >= g->nodes)) return false;
	if(g->nodeMat[nodeI][nodeE] == 0) return false;
	g->nodeMat[nodeI][nodeE] = 0;
	g->links--;
	return (true);
};

void getLinkDestAndWeight(Graph *g, int nodeI, int LinkIndex, int *dest, Weight *weight){
	if(!g) return;
	if((nodeI < 0) || (nodeI > g->nodes)) return;
	if(g->nodeMat[nodeI][LinkIndex] == 0) {
		*dest = 0;
		*weight = 0;
		return;
	} else {
		*dest = LinkIndex;
		*weight = g->nodeMat[nodeI][LinkIndex];
		return;
	}
};

void getLinkWeight(Graph *g, int nodeI, int nodeE, Weight *weight){
	if(!g) return;
	if((nodeI < 0) || (nodeI > g->nodes)) return;
	if((nodeE < 0) || (nodeE > g->nodes)) return;
	if(g->nodeMat[nodeI][nodeE] == 0) {
		*weight = 0;
		return;
	} else {
		*weight = g->nodeMat[nodeI][nodeE];
		return;
	}
};

int getNumberOfEdges(Graph *g, int nodeI){
	if(!g) return -1;
  return (g->nodes+1);
};