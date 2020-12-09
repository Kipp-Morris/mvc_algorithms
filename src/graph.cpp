#include <iostream>
#include <map>
#include <set>

using namespace std;

// Represents a graph. Encompasses a vertex set, and edge set, and a mapping
// from vertices to their degrees.
class Graph {
  set<int> *V;
  set<pair<int, int> > *E;
  map<int, int> *degrees;

  ~Graph() {
    delete V;
    delete E;
    delete degrees;
  }

  void print_vertex_set() {
    set<int>::iterator it = V->begin();
  
    while (it != V->end()) {
      cout << (*it) << "\n";      
      it++;
    }
  }
  
  void print_edge_set() {
    set<pair<int, int> >::iterator it = E->begin();
    
    while (it != E->end()) {
      cout << it->first << ", " << it->second << "\n";
      it++;
    }
  }
};

