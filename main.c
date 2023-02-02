#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define MAX_EDGES 500
#define INT_MAX 32000

int level[MAX_VERTICES];

struct Edge {
  int from;
  int to;
  int capacity;
  int flow;
};

struct Graph {
  int vertices;
  int edges;
  int source;
  int sink;
  struct Edge edge[MAX_EDGES];
};

void initGraph(struct Graph *g, int vertices, int source, int sink) {
  int i;
  g->vertices = vertices;
  g->edges = 0;
  g->source = source;
  g->sink = sink;
  for (i = 0; i < MAX_EDGES; i++) {
    g->edge[i].flow = 0;
  }
}

void addEdge(struct Graph *g, int from, int to, int capacity) {
  int i = g->edges;
  g->edge[i].from = from;
  g->edge[i].to = to;
  g->edge[i].capacity = capacity;
  g->edges++;
}

void removeEdge(struct Graph *g, int edgeIndex) {
  if (edgeIndex < 0 || edgeIndex >= g->edges) {
    printf("Error: Invalid edge index %d\n", edgeIndex);
    return;
  }
  int flow = g->edge[edgeIndex].flow;
  int from = g->edge[edgeIndex].from;
  int to = g->edge[edgeIndex].to;
  int i;
  for (i = edgeIndex; i < g->edges - 1; i++) {
    g->edge[i] = g->edge[i + 1];
  }
  g->edges--;
  // Adjust the flow
  for (i = 0; i < g->edges; i++) {
    if (g->edge[i].from == to && g->edge[i].to == from) {
      g->edge[i].flow -= flow;
      break;
    }
  }
}

void removeVertex(struct Graph *g, int vertex) {
  if (vertex < 0 || vertex >= g->vertices) {
    printf("Error: Invalid vertex %d\n", vertex);
    return;
  }
  int i;
  for (i = 0; i < g->edges; i++) {
    if (g->edge[i].from == vertex || g->edge[i].to == vertex) {
      removeEdge(g, i);
      i--;
    }
  }
}

void changeCapacity(struct Graph *g, int edgeIndex, int newCapacity) {
  if (edgeIndex < 0 || edgeIndex >= g->edges) {
    printf("Error: Invalid edge index %d\n", edgeIndex);
    return;
  }
  if (newCapacity < 0) {
    printf("Error: Invalid capacity %d\n", newCapacity);
    return;
  }
  int flow = g->edge[edgeIndex].flow;
  int capacity = g->edge[edgeIndex].capacity;
  int from = g->edge[edgeIndex].from;
  int to = g->edge[edgeIndex].to;
  g->edge[edgeIndex].capacity = newCapacity;
  // Adjust the flow
  if (flow > newCapacity) {
    g->edge[edgeIndex].flow = newCapacity;
    for (int i = 0; i < g->edges; i++) {
      if (g->edge[i].from == to && g->edge[i].to == from) {
        g->edge[i].flow -= (flow - newCapacity);
        break;
      }
    }
  }
}

int checkEdgeExists(struct Graph *g, int from, int to) {
  for (int i = 0; i < g->edges; i++) {
    if (g->edge[i].from == from && g->edge[i].to == to) {
      return i;
    }
  }
  return -1;
}

void changeSource(struct Graph *g, int newSource) {
  if (newSource < 0 || newSource >= g->vertices) {
    printf("Error: Invalid new source %d\n", newSource);
    return;
  }
  g->source = newSource;
}

void changeSink(struct Graph *g, int newSink) {
  if (newSink < 0 || newSink >= g->vertices) {
    printf("Error: Invalid new sink %d\n", newSink);
    return;
  }
  g->sink = newSink;
}

int min(int a, int b) { return a < b ? a : b; }

bool BFS(struct Graph *g) {
  int i, j;
  for (i = 0; i < g->vertices; i++) {
    level[i] = -1;
  }
  level[g->source] = 0;
  int q[g->vertices], qh = 0, qt = 0;
  q[qt++] = g->source;
  while (qh != qt) {
    int u = q[qh++];
    for (i = 0; i < g->edges; i++) {
      struct Edge e = g->edge[i];
      if (e.capacity > 0 && e.from == u && level[e.to] < 0) {
        level[e.to] = level[u] + 1;
        q[qt++] = e.to;
      }
    }
  }
  return level[g->sink] >= 0;
}

int sendFlow(struct Graph *g, int u, int flow) {
  if (u == g->sink) {
    return flow;
  }
  int i;
  for (i = 0; i < g->edges; i++) {
    struct Edge e = g->edge[i];
    if (e.capacity > 0 && e.from == u && level[e.to] == level[u] + 1) {
      int t = sendFlow(g, e.to, min(flow, e.capacity));
      if (t > 0) {
        e.capacity -= t;
        g->edge[i ^ 1].capacity += t;
        return t;
      }
    }
  }
  return 0;
}

int Dinic(struct Graph *g) {
  int result = 0;
  while (BFS(g)) {
    //printf(".");
    int flow = 0;
    while ((flow = sendFlow(g, g->source, INT_MAX)) != 0) {
      //printf(".");
      result += flow;
    }
  }
  return result;
}

void printGraph(struct Graph g) {
  printf("\n[%d]: (%d) ===> (%d)\n", g.vertices, g.source, g.sink);
  int i;
  for (i = 0; i < g.edges; i++) {
    printf("%d: %d ---> %d [%d]\n", i, g.edge[i].from, g.edge[i].to, g.edge[i].capacity);
  }
}

int main() {
  struct Graph g;
  // int v, s, t;
  // scanf("%d %d %d", &v, &s, &t);
  initGraph(&g, 5, 1, 5);

  addEdge(&g, 1, 2, 10);
  addEdge(&g, 1, 4, 9);
  // removeEdge(&g, 0);
  addEdge(&g, 2, 4, 5);
  addEdge(&g, 3, 5, 3);
  
  printGraph(g);

  printf("\n\n\n");

  //removeVertex(&g, 4);
  changeCapacity(&g, 0, 5);
  changeSource(&g, 3);
  //changeSink(&g, 5);
  //printf("%d %d \n\n", checkEdgeExists(&g, 3, 5), checkEdgeExists(&g, 3, 4));
  printGraph(g);

  printf("\n%d\n", Dinic(&g));
  return 0;
}
