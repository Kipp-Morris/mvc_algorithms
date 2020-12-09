#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "graph.h"

using namespace std;

// Given the path to a file containing a Metis IO description of
// a graph, creates and returns the graph.
Graph *create_graph_from_metis_io(string filename) {
  ifstream graph_file;
  graph_file.open(filename);
  Graph *G = new Graph;
  G->V = new set<int>;
  G->E = new set<pair<int, int> >;
  G->degrees = new map<int, int>;

  string line = "";
  int i = 1;
  getline(graph_file, line);
  while (getline(graph_file, line)) {
    int num = 0;
    int degree = 0;
    istringstream iss (line);

    while (iss >> num) {
      if (i < num) {
	G->E->insert(make_pair(i, num));
      } else {
	G->E->insert(make_pair(num, i));
      }

      G->V->insert(num);
      degree++;
    }

    G->V->insert(i);
    G->degrees->insert(make_pair(i, degree));
    i++;
  }

  graph_file.close();

  return G;
}

// Creates and returns a new Graph object G' = (V', E'), where
// V' = V_prime
// E' = {(u, v) for all u, v such that u in V' and v in V' and (u, v) in E}
Graph *get_subgraph(Graph *G, set<int> *V_prime) {
  struct Graph *subgraph = new struct Graph;
  subgraph->V = new set<int>;
  subgraph->E = new set<pair<int, int> >;
  
  set<pair<int, int> >::iterator edges = G->E->begin();
  
  while (edges != G->E->end()) {
    pair<int, int> e = (*edges);
      
    if ((V_prime->find(e.first) != V_prime->end()) && (V_prime->find(e.second) != V_prime->end())) {
      subgraph->V->insert(e.first);
      subgraph->V->insert(e.second);
      subgraph->E->insert(e);
    }
    
    edges++;
  }
  
  return subgraph;
}


// Returns a greedily calculated maximal matching on the input graph
// in O(|E|) time.
set<pair<int, int> > *maximal_matching(Graph *G) {
  set<pair<int, int> > *mm = new set<pair<int, int> >;
  set<int> mm_vertices;

  set<pair<int, int> >::iterator it = G->E->begin();

  while (it != G->E->end()) {
    if ((mm_vertices.find(it->first) == mm_vertices.end()) && (mm_vertices.find(it->second) == mm_vertices.end())) {
      mm->insert(*it);
      mm_vertices.insert(it->first);
      mm_vertices.insert(it->second);
    }
    
    it++;
  }

  return mm;
}

