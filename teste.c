#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "grafo_matrizadj.h"
#include "grafo_listaadj.h"

typedef struct {
  int center1;
  int center2;
  int weight;
} Consult;

typedef struct {
  Consult* consultsArray;
  int consultsSize;
} Consults;

typedef struct Edge {
    int src;
    int dest;
    Weight weight;
} Edge;

typedef struct DisjointSet {
    int *parent;
    int *rank;
} DisjointSet;

void printConsults(Consults *consults) {
  for(int i = 0; i < consults->consultsSize; i++) {
    printf("Consult %d: C1-%d, C2-%d, Height-%.1f\n", (i+1), consults->consultsArray[i].center1, consults->consultsArray[i].center2, ((double)consults->consultsArray[i].weight)/100);
  }
}

void treatConsults(Consults* resToTreat) {
  for(int i = 0; i <= resToTreat->consultsSize; i++) {
    if(resToTreat->consultsArray[i].weight >= 450) resToTreat->consultsArray[i].weight = 450;
    else if(resToTreat->consultsArray[i].weight >= 400) resToTreat->consultsArray[i].weight = 400;
    else if(resToTreat->consultsArray[i].weight >= 350) resToTreat->consultsArray[i].weight = 350;
    else if(resToTreat->consultsArray[i].weight >= 300) resToTreat->consultsArray[i].weight = 300;
    else if(resToTreat->consultsArray[i].weight >= 250) resToTreat->consultsArray[i].weight = 250;
    else resToTreat->consultsArray[i].weight = -1;
  }
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

void writeResults(Consults *consults) {
  FILE *file = fopen("saida.txt", "w");

  for(int i = 0; i < consults->consultsSize; i++) {
    printf("Escrevendo consulta %d\n", i+1);
    fprintf(file, "%.1f\n", ((double)consults->consultsArray[i].weight)/100);
  }

  fclose(file);
}

int find(DisjointSet* ds, int i) {
    if (ds->parent[i] != i) {
        ds->parent[i] = find(ds, ds->parent[i]);
    }
    return ds->parent[i];
}

void unionSet(DisjointSet *set, int x, int y) {
    printf("Unioning %d and %d\n", x, y);
    int rootX = find(set, x);
    int rootY = find(set, y);
    printf("RootX: %d\n", rootX);
    printf("RootY: %d\n", rootY);
    if (rootX == rootY) {
        return;  // Already in the same set
    }

    if (set->rank[rootX] < set->rank[rootY]) {
        set->parent[rootX] = rootY;
    } else if (set->rank[rootX] > set->rank[rootY]) {
        set->parent[rootY] = rootX;
    } else {
        set->parent[rootY] = rootX;
        set->rank[rootX]++;
    }
}

int compareEdges(const void* a, const void* b) {
    Edge* edgeA = (Edge*)a;
    Edge* edgeB = (Edge*)b;
    return edgeB->weight - edgeA->weight;
}

Graph* kruskalMaxSpanningTree(Graph* graph) {
  int nodes = graph->nodes;
  Edge edges[graph->links];

  int edgeIndex = 0;

  printf("Criando lista de arestas\n");
  printf("nodes: %d\n", nodes);
  printf("links: %d\n", graph->links);

  for(int i = 0; i <= graph->nodes; i++) {
    int edgesQty = getNumberOfEdges(graph, i);
    int linkDest;
    int linkWeight;
    for(int j = 0; j < edgesQty; j++) {
      getLinkDestAndWeight(graph, i, j, &linkDest, &linkWeight);
      if(linkWeight > 0) {
        printf("Adicionando aresta %d-%d com peso %d\n", i, linkDest, linkWeight);
        edges[edgeIndex].src = i;
        edges[edgeIndex].dest = linkDest;
        edges[edgeIndex].weight = linkWeight;
        edgeIndex++;
      }
    }
    edgesQty = getNumberOfEdges(graph, i);
  }

  printf("edgeIndex: %d\n", edgeIndex);

  printf("Ordenando lista de arestas\n");
  qsort(edges, edgeIndex, sizeof(Edge), compareEdges);

  printf("Lista de arestas ordenada\n");
  for(int i = 0; i < edgeIndex; i++) {
    printf("Aresta %d-%d com peso %d\n", edges[i].src, edges[i].dest, edges[i].weight);
  }

  printf("Criando conjunto disjunto\n");
  
  DisjointSet* ds = (DisjointSet*)malloc(sizeof(DisjointSet));
  ds->parent = (int*)malloc(nodes+1 * sizeof(int));
  ds->rank = (int*)malloc(nodes+1 * sizeof(int));
  
  for (int i = 0; i <= nodes; i++) {
      ds->parent[i] = i;
      ds->rank[i] = 0;
  }
  
  printf("Criando grafo\n");

  Graph* maxSpanningTree = clearGraph(graph);
  
  int edgeCount = 0;
  edgeIndex = 0;
  
  while (edgeCount < nodes - 1) {
    Edge currentEdge = edges[edgeIndex++];
    int srcParent = find(ds, currentEdge.src);
    int destParent = find(ds, currentEdge.dest);
    
    if (srcParent != destParent) {
      addLink(maxSpanningTree, currentEdge.src, currentEdge.dest, currentEdge.weight);
      unionSet(ds, srcParent, destParent);
      edgeCount++;
    }
  }
  
  free(ds);
  
  return maxSpanningTree;
}

int findMinCostEdge(Graph* graph, int source, int destination) {
    bool* visited = (bool*)malloc(graph->nodes+1 * sizeof(bool));
    int* minCost = (int*)malloc(graph->nodes+1 * sizeof(int));
    int* parent = (int*)malloc(graph->nodes+1 * sizeof(int));

    // Inicializa os arrays visited, minCost e parent
    for (int i = 0; i <= graph->nodes; i++) {
        visited[i] = false;
        minCost[i] = INT_MAX;
        parent[i] = -1;
    }

    // Define o custo do vértice de origem como 0
    minCost[source] = 0;

    // Loop para encontrar o caminho de menor custo
    for (int count = 0; count <= graph->nodes - 1; count++) {
        int minCostVertex = -1;
        int minCostValue = INT_MAX;

        // Encontra o vértice não visitado com o menor custo
        for (int v = 0; v <= graph->nodes; v++) {
            if (!visited[v] && minCost[v] < minCostValue) {
                minCostVertex = v;
                minCostValue = minCost[v];
            }
        }

        // Marca o vértice encontrado como visitado
        visited[minCostVertex] = true;

        int linkWeight;

        // Atualiza os custos dos vértices adjacentes
        for (int v = 0; v <= graph->nodes; v++) {
            getLinkWeight(graph, minCostVertex, v, &linkWeight);
            if (!visited[v] && linkWeight != 0 && linkWeight < minCost[v]) {
                minCost[v] = linkWeight;
                parent[v] = minCostVertex;
            }
        }
    }

    // Encontra a aresta de menor custo entre os vértices source e destination
    int minCostEdge = INT_MAX;
    int currentVertex = destination;
    int linkWeight;

    while (parent[currentVertex] != -1) {
        int prevVertex = parent[currentVertex];
        getLinkWeight(graph, prevVertex, currentVertex, &linkWeight);
        if (linkWeight < minCostEdge) {
            minCostEdge = linkWeight;
        }
        currentVertex = prevVertex;
    }

    free(visited);
    free(minCost);
    free(parent);

    return minCostEdge;
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

  g = kruskalMaxSpanningTree(g);

  printf("Kruskal Graph:\n");
  printGraph(g);

  printf("Calculating consults...\n");
  for(int i = 0; i < consults->consultsSize; i++) {
    printf("Calculating consult %d\n", i+1);
    consults->consultsArray[i].weight = findMinCostEdge(g, consults->consultsArray[i].center1, consults->consultsArray[i].center2);
  }

  printf("Consults calculated\n");
  printConsults(consults);

  printf("Writing results...\n");

  writeResults(consults);

  deleteGraph(g);

  return 0;
}