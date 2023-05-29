#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grafo_matrizadj.h"
// #include "grafo_listaadj.h"

typedef struct {
  int center1;
  int center2;
  int weight;
} Consult;

typedef struct {
  Consult* consultsArray;
  int consultsSize;
} Consults;

typedef struct sNode {
  int node;
  int ant;
  int weight;
  bool open;
} SearchNode;

typedef struct {
  SearchNode* searchArray;
  int source;
  int size;
} SearchResult;

void getIndexFromBiggerMin(SearchNode* array, int size, int* index) {
  int bigger = -1;
  for(int i = 0; i < size; i++) {
    if(array[i].open && array[i].weight > bigger) {
      bigger = array[i].weight;
      *index = i;
    }
  }
}

void printConsults(Consults *consults) {
  for(int i = 0; i < consults->consultsSize; i++) {
    printf("Consult %d: C1-%d, C2-%d, Height-%.1f\n", i+1, consults->consultsArray[i].center1, consults->consultsArray[i].center2, ((double)consults->consultsArray[i].weight)/100);
  }
}

void printSearchResults(SearchResult* results) {
  for(int i = 0; i <= results->size; i++) {
    printf("Center %d: Ant - %d, Weight - %.1f\n", results->searchArray[i].node, results->searchArray[i].ant, ((double)results->searchArray[i].weight)/100);
  }
}

void treatSearchResults(SearchResult* resToTreat) {
  for(int i = 0; i <= resToTreat->size; i++) {
    if(resToTreat->searchArray[i].weight >= 450) resToTreat->searchArray[i].weight = 450;
    else if(resToTreat->searchArray[i].weight >= 400) resToTreat->searchArray[i].weight = 400;
    else if(resToTreat->searchArray[i].weight >= 350) resToTreat->searchArray[i].weight = 350;
    else if(resToTreat->searchArray[i].weight >= 300) resToTreat->searchArray[i].weight = 300;
    else if(resToTreat->searchArray[i].weight >= 250) resToTreat->searchArray[i].weight = 250;
    else resToTreat->searchArray[i].weight = -1;
  }
}

void deleteSearchResults(SearchResult** results, int size) {
  for(int i = 0; i < size; i++) {
    free(results[i]->searchArray);
    free(results[i]);
  }
  free(results);
}

Graph* readFile(char* path, Consults *consults) {
  // Open the file
  printf("Opening file...\n");
  printf("Path: %s\n", path);

  FILE* file = fopen(path, "r");

  // Check if the file is open
  if (file == NULL) {
    printf("Error opening file\n");
    return NULL;
  }

  printf("File opened\n\n");
  printf("Reading file...\n");

  // Read the first line
  int centers;
  int links;
  int consultsSize;
  fscanf(file, "%d %d %d\n", &centers, &links, &consultsSize);

  consults->consultsArray = (Consult*) malloc(sizeof(Consult) * consultsSize);
  consults->consultsSize = consultsSize;

  // printf("centers: %d\n", centers);
  // printf("links: %d\n", links);
  printf("consultsSize: %d\n", consultsSize);

  Graph* g = createGraph(centers);
  
  // Create the graph
  for(int i = 0; i < links; i++) {
    int center1, center2;
    float weight;

    fscanf(file, "%d %d %f", &center1, &center2, &weight);

    if((center1 != 0 && !center1) || (center2 != 0 && !center2) || !weight) {
      printf("Error reading graph links\n");
      return NULL;
    }

    if(center1 > centers || center2 > centers) {
      printf("Centro inválido\n");
      return NULL;
    }

    if(weight < 0 || weight > 5.5) {
      printf("Altura inválida\n");
      return NULL;
    }

    printf("Adicionando link %d-%d com peso %.1f\n", center1, center2, weight);

    addLink(g, center1, center2, (int)(weight * 100));
    addLink(g, center2, center1, (int)(weight * 100));

    printf("Link adicionado\n");
  }

  // Create the consults
  for(int i = 0; i < consults->consultsSize; i++) {
    int center1, center2;

    fscanf(file, "%d %d", &center1, &center2);

    if((center1 != 0 && !center1) || (center2 != 0 && !center2)) {
      printf("Error reading consults\n");
      return NULL;
    }

    if(center1 > centers || center2 > centers || center1 < 0 || center2 < 0) {
      printf("invalid Center, greater than number of centers or lower than 0\n");
      return NULL;
    }

    printf("Adicionando consulta %d: C1-%d, C2-%d\n", i+1, center1, center2);

    consults->consultsArray[i].center1 = center1;
    consults->consultsArray[i].center2 = center2;

    printf("Consulta adicionada\n");
  }

  printf("File read\n");
  fclose(file);
  return g;
}

SearchResult* makeConsult(Graph *g, Consult consult) {
  SearchResult *searchResult = (SearchResult*) malloc(sizeof(SearchResult));
  searchResult->searchArray = (SearchNode*) malloc(sizeof(SearchNode) * (g->nodes+1));
  searchResult->source = consult.center1;
  searchResult->size = g->nodes;

  for(int i = 0; i <= g->nodes; i++) {
    searchResult->searchArray[i].node = i;
    searchResult->searchArray[i].ant = -1;
    searchResult->searchArray[i].weight = -1;
    searchResult->searchArray[i].open = true;
  }

  searchResult->searchArray[consult.center1].weight = INT_MAX;
  searchResult->searchArray[consult.center1].open = false;

  int i = consult.center1;
  int edges = getNumberOfEdges(g, i);

  printf("Consulting...\n");
  for(int z = 0; z <= g->nodes - 1; z++) {
    int linkDest;
    int linkWeight;
    for(int j = 0; j < edges; j++) {
      getLinkDestAndWeight(g, i, j, &linkDest, &linkWeight);
      int min = searchResult->searchArray[i].weight < linkWeight ? searchResult->searchArray[i].weight : linkWeight;
      if(searchResult->searchArray[linkDest].open 
        && searchResult->searchArray[i].node != linkDest 
        && min > searchResult->searchArray[linkDest].weight) {
        searchResult->searchArray[linkDest].weight = min;
        searchResult->searchArray[linkDest].ant = searchResult->searchArray[i].node;
      }
    }
    searchResult->searchArray[i].open = false;
    getIndexFromBiggerMin(searchResult->searchArray, g->nodes, &i);
    edges = getNumberOfEdges(g, i);
  }

  printf("Consulted\n");
  printf("Treating results...\n");
  treatSearchResults(searchResult);
  printf("Results treated\n");

  printSearchResults(searchResult);

  return searchResult;
}

void writeResults(SearchResult **searchResult, Consults *consults) {
  FILE *file = fopen("saida.txt", "w");

  for(int i = 0; i < consults->consultsSize; i++) {
    printf("Escrevendo consulta %d\n", i+1);
    fprintf(file, "%.1f\n", ((double)searchResult[i]->searchArray[consults->consultsArray[i].center2].weight)/100);
  }

  fclose(file);
}

int main(int argc, char* argv[]) {
  if(argc < 2) {
    printf("Insira o caminho do arquivo\n");
    return 1;
  }

  Consults *consults = (Consults*) malloc(sizeof(Consults));
  Graph *g = readFile(strdup(argv[1]), consults);

  printf("Graph:\n");
  printGraph(g);

  SearchResult **searchResult = (SearchResult**) malloc(sizeof(SearchResult*) * consults->consultsSize);

  for(int i = 0; i < consults->consultsSize; i++) {
    for(int j = 0; j < i; j++) {
      if(consults->consultsArray[i].center1 == consults->consultsArray[j].center1) {
        searchResult[i] = searchResult[j];
        break;
      }
    }
    searchResult[i] = makeConsult(g, consults->consultsArray[i]);
  }

  printf("Writing results...\n");

  writeResults(searchResult, consults);

  deleteGraph(g);

  deleteSearchResults(searchResult, consults->consultsSize);

  return 0;
}