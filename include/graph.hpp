#ifndef GRAPH_H
#define GRAPH_H

#include<map>
#include<set>

class Graph {
 public:
  std::set<int> *V;
  std::set<std::pair<int, int> > *E;
  std::map<int, int> *degrees;
  
  void print_vertex_set();
  void print_edge_set();
};

#endif
