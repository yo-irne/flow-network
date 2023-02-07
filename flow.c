#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "flow.h"

int level[MAX_VERTICES];

void initGraph(struct Graph *g, int vertices, int source, int sink) {
  if(vertices <= 0 || source <= 0 || sink <= 0 || source > vertices || sink > vertices) {
    printf("Error: Invalid vertices. \n");
    return;
  }
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
  if (from <= g->vertices && to <= g->vertices) {
    int i = g->edges;
    g->edge[i].from = from;
    g->edge[i].to = to;
    g->edge[i].capacity = capacity;
    g->edges++;
  } else {
    printf("Error: Invalid vertices. \n");
    return;
  }
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
  //g->vertices--;
}

void addVertex(struct Graph *g, int vertex) { g->vertices++; }

void removeVertex(struct Graph *g, int vertex) {
  if (vertex <= 0 || vertex > g->vertices) {
    printf("Error: Invalid vertex %d\n", vertex);
    return;
  }
  if (vertex == g->source) {
    printf("Warning: Change source in order to remove this vertex. \n");
    return;
  } else if (vertex == g->sink) {
    printf("Warning: Change sink in order to remove this vertex. \n");
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

int getFrom(struct Graph *g, int id) { return g->edge[id].from; }

int getTo(struct Graph *g, int id) { return g->edge[id].to; }

int getCap(struct Graph *g, int id) { return g->edge[id].capacity; }

int checkEdgeExists(struct Graph *g, int from, int to) {
  for (int i = 0; i < g->edges; i++) {
    if (g->edge[i].from == from && g->edge[i].to == to) {
      return i;
    }
  }
  return -1;
}

void changeSource(struct Graph *g, int newSource) {
  if (newSource <= 0 || newSource > g->vertices) {
    printf("Error: Invalid new source %d\n", newSource);
    return;
  }
  g->source = newSource;
}

void changeSink(struct Graph *g, int newSink) {
  if (newSink <= 0 || newSink > g->vertices) {
    printf("Error: Invalid new sink %d\n", newSink);
    return;
  }
  g->sink = newSink;
}

int min(int a, int b) { return a < b ? a : b; }

int bfs(struct Graph *g, int *parent) {
  memset(level, -1, sizeof(level));
  level[g->source] = 0;
  int u, v;
  int i, e;
  int queue[MAX_VERTICES];
  int head = 0;
  int tail = 0;
  queue[tail++] = g->source;
  while (head != tail) {
    u = queue[head++];
    for (i = 0; i < g->edges; i++) {
      e = g->edge[i].to;
      if (g->edge[i].from == u && level[e] == -1 &&
          g->edge[i].capacity - g->edge[i].flow > 0) {
        level[e] = level[u] + 1;
        queue[tail++] = e;
      }
    }
  }
  return level[g->sink] != -1;
}

int dfs(struct Graph *g, int u, int flow, int *parent) {
  if (u == g->sink)
    return flow;
  int v;
  int i, e;
  int delta;
  for (i = 0; i < g->edges; i++) {
    e = g->edge[i].to;
    if (g->edge[i].from == u && level[e] == level[u] + 1 &&
        g->edge[i].capacity - g->edge[i].flow > 0) {
      delta =
          dfs(g, e, fmin(flow, g->edge[i].capacity - g->edge[i].flow), parent);
      if (delta > 0) {
        g->edge[i].flow += delta;
        for (int j = 0; j < g->edges; j++) {
          if (g->edge[j].to == u && g->edge[j].from == e) {
            g->edge[j].flow -= delta;
            break;
          }
        }
        return delta;
      }
    }
  }
  return 0;
}

int maxFlow(struct Graph *g) {
  int parent[MAX_VERTICES];
  int flow = 0;
  int delta;
  while (bfs(g, parent)) {
    while ((delta = dfs(g, g->source, INT_MAX, parent)) > 0) {
      flow += delta;
    }
  }
  return flow;
}

void printGraph(struct Graph g) {
  printf("\n[%d]: (%d) ===> (%d)\n", g.vertices, g.source, g.sink);
  int i;
  if (g.edges > 0)
    printf("------------------\n");
  for (i = 0; i < g.edges; i++) {
    printf("%d: %d ---> %d [%d]\n", i, g.edge[i].from, g.edge[i].to,
           g.edge[i].capacity);
  }
  printf("\n");
}


