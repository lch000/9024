// Graph ADT interface ... COMP9024 25T1
#include <stdbool.h>

typedef struct GraphRep *Graph;

// vertices are ints
typedef int Vertex;

// edges are pairs of vertices (end-points)
typedef struct Edge {
   Vertex v;
   Vertex w;
} Edge;

Graph newGraph(int);
int   numOfVertices(Graph);
void  insertEdge(Graph, Edge);
void  removeEdge(Graph, Edge);
bool  adjacent(Graph, Vertex, Vertex);
void  showGraph(Graph);
void  freeGraph(Graph);

Edge  randomEdge(Graph g);           // return a random element from the array of edges of g
bool  incident(Edge e1, Edge e2);    // check if e1 and e2 have an endpoint in common
Graph deleteEdges(Graph g, Edge e);  // delete all edges from g that have at least one endpoint in common with e
Graph copyGraph(Graph g);            // return a newly created graph that is an exact copy of g
bool  graphIsEmpty(Graph g);         // check if graph g has no edges