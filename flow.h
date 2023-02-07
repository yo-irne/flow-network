#ifndef FLOW_H_
#define FLOW_H_

#define MAX_VERTICES 100
#define MAX_EDGES 500
//#define INT_MAX 32000

extern int level[MAX_VERTICES];

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

void initGraph(struct Graph *g, int vertices, int source, int sink);
void addEdge(struct Graph *g, int from, int to, int capacity);
void removeEdge(struct Graph *g, int edgeIndex);
void addVertex(struct Graph *g, int vertex);
void removeVertex(struct Graph *g, int vertex);
void changeCapacity(struct Graph *g, int edgeIndex, int newCapacity);
int getFrom(struct Graph *g, int id);
int getTo(struct Graph *g, int id);
int getCap(struct Graph *g, int id);
int checkEdgeExists(struct Graph *g, int from, int to);
void changeSource(struct Graph *g, int newSource);
void changeSink(struct Graph *g, int newSink);
int min(int a, int b);
int bfs(struct Graph *g, int *parent);
int dfs(struct Graph *g, int u, int flow, int *parent);
int maxFlow(struct Graph *g);
void printGraph(struct Graph g);

#endif //FLOW_H_
