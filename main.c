#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  if (from <= g->vertices && to <= g->vertices) {
    int i = g->edges;
    g->edge[i].from = from;
    g->edge[i].to = to;
    g->edge[i].capacity = capacity;
    g->edges++;
  } else {
    printf("Error: Invalid vertices. \n");
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
  if(g.edges > 0) printf("------------------\n");
  for (i = 0; i < g.edges; i++) {
    printf("%d: %d ---> %d [%d]\n", i, g.edge[i].from, g.edge[i].to,
           g.edge[i].capacity);
  }
  printf("\n");
}

int main() {
  struct Graph g;
  int vertices, source, sink;

  printf("\n|V(G)| = ");
  scanf("%d", &vertices);
  printf("Source: ");
  scanf("%d", &source);
  printf("Sink: ");
  scanf("%d", &sink);
  printf("\n");
  initGraph(&g, vertices, source, sink);

  while (true) {
    int from, to, capacity, id;
    char command[20];

    printf("~/$  ");
    scanf("%s", command);

    if (strcmp(command, "edge") == 0) {
      scanf("%d %d %d", &from, &to, &capacity);
      addEdge(&g, from, to, capacity);
      if (checkEdgeExists(&g, from, to) != -1)
        printf("\nNew edge: %d: %d ---> %d [%d] \n\n",
               checkEdgeExists(&g, from, to), from, to, capacity);
    } else if (strcmp(command, "rme") == 0) {
      scanf("%d", &id);
      removeEdge(&g, id);
      if (checkEdgeExists(&g, from, to) != -1)
        printf("\nRemoved: %d: %d ---> %d [%d] \n\n", id, getFrom(&g, id),
               getTo(&g, id), getCap(&g, id));
    } else if (strcmp(command, "rmv") == 0) {
      scanf("%d", &id);
      if (id > 0 && id <= vertices && id != g.sink && id != g.source)
        printf("\nRemoved: Node %d \n\n", id);
      removeVertex(&g, id);
    } else if (strcmp(command, "cap") == 0) {
      scanf("%d %d", &id, &capacity);
      changeCapacity(&g, id, capacity);
      if (checkEdgeExists(&g, from, to) != -1)
        printf("\nEdge %d has now the capacity of %d \n\n", id, capacity);
    } else if (strcmp(command, "src") == 0) {
      scanf("%d", &id);
      changeSource(&g, id);
      if (id > 0 && id <= vertices)
        printf("\nNew source: %d\n\n", id);
    } else if (strcmp(command, "sink") == 0) {
      scanf("%d", &id);
      changeSink(&g, id);
      if (id > 0 && id <= vertices)
        printf("\nNew sink: %d\n\n", id);
    } else if (strcmp(command, "check") == 0) {
      scanf("%d %d", &from, &to);
      int index = checkEdgeExists(&g, from, to);
      if (index >= 0) {
        printf("\nEdge exists with index %d:\n", index);
        printf("%d: %d ---> %d [%d] \n\n", index, from, to, getCap(&g, index));
      } else {
        printf("\nEdge does not exist \n\n");
      }
    } else if (strcmp(command, "print") == 0) {
      printGraph(g);
    } else if(strcmp(command, "flow") == 0) {
      printf("\nMax flow: %d \n\n", maxFlow(&g));
    } else if(strcmp(command, "help") == 0) {
      printf("\nCommands:\n");
      printf("----------------\n");
      printf("\tedge [f] [t] [c] - add an edge from f to t of capacity c\n");
      printf("\trme [i] - remove edge with an index i\n");
      printf("\tcap [i] - change capacity of an edge i\n");
      printf("\trmv [n] - remove vertex no. n\n");
      printf("\tsrc [n] - change source to the vertex no. n\n");
      printf("\tsink [n] - change sink to the vertex no. n\n");

      printf("\n\tprint - prints graph\n\t\t important to note: \n\t\t\t index: from ---> to [capacity] \n\t\t\t [vertices]: source ===> sink\n");
      
      printf("\n\thelp - show this screen\n");
      printf("\texit - leave application\n");
    } else if (strcmp(command, "exit") == 0) {
      break;
    } else {
      printf("\nError: Invalid command\n\n");
    }
  }
  return 0;
}
