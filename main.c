#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "flow.h"

int main() {
  struct Graph g;
  int vertices, source, sink;

  while (true) {
    int from, to, capacity, id;
    char command[20];

    printf("~/$  ");
    scanf("%s", command);

    if (strcmp(command, "init") == 0 || strcmp(command, "clear") == 0) {
      printf("\n|V(G)| = ");
      scanf("%d", &vertices);
      printf("Source: ");
      scanf("%d", &source);
      printf("Sink: ");
      scanf("%d", &sink);
      printf("\n");
      if(vertices <= 0 || source <= 0 || sink <= 0 || source > vertices || sink > vertices) {
        printf("Error: Invalid vertices. \n");
        continue;
      }
      initGraph(&g, vertices, source, sink);
    } else if (strcmp(command, "edge") == 0) {
      int added = 0;
      scanf("%d %d %d", &from, &to, &capacity);
      if(!(from <= vertices && from > 0 && to <= vertices && to > 0)) {
        printf("Error: Invalid vertices. \n");
        continue;
      }
      if (checkEdgeExists(&g, from, to) == -1) {
        addEdge(&g, from, to, capacity);
        added = 1;
      }
      if (checkEdgeExists(&g, from, to) != -1 && added == 1)
        printf("\nNew edge: %d: %d ---> %d [%d] \n\n",
               checkEdgeExists(&g, from, to), from, to, capacity);
      else {
        printf("\nError: Edge already exists or you have passed wrong input. \n\n");
      }
    } else if (strcmp(command, "rme") == 0) {
      scanf("%d", &id);
      removeEdge(&g, id);
      if (checkEdgeExists(&g, from, to) != -1)
        printf("\nRemoved: %d: %d ---> %d [%d] \n\n", id, getFrom(&g, id),
               getTo(&g, id), getCap(&g, id));
    } else if (strcmp(command, "cut") == 0) {
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
    } else if (strcmp(command, "flow") == 0) {
      if(vertices <= 0) {
        printf("Error: Invalid graph. \n");
        continue;
      }
      printf("\nMax flow: %d \n\n", maxFlow(&g));
    //} else if(strcmp(command, "node") == 0) {
      //continue;
    } else if (strcmp(command, "help") == 0) {
      printf("\nCommands:\n");
      printf("----------------\n");
      printf("\tedge [f] [t] [c] - add an edge from f to t of capacity c\n");
      printf("\trme [i] - remove edge with an index i\n");
      printf("\tcap [i] - change capacity of an edge i\n");
      printf("\tcut [n] - remove all edges from/to the vertex no. n\n");
      printf("\tsrc [n] - change source to the vertex no. n\n");
      printf("\tsink [n] - change sink to the vertex no. n\n");
      printf("\tflow - run Edmonds-Karp algorithm to measure maximal flow in "
             "the network\n");

      printf(
          "\n\tprint - prints graph\n\t\t important to note: \n\t\t\t index: "
          "from ---> to [capacity] \n\t\t\t [vertices]: source ===> sink\n");

      printf("\n\tclear|init - restart everything and initialise new graph\n");
      printf("\thelp - show this screen\n");
      printf("\texit|quit - leave application\n\n");
    } else if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
      break;
    } else {
      printf("\nError: Invalid command\n\n");
    }
  }
  return 0;
}
