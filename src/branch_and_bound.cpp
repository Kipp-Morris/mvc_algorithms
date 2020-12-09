#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <queue>

#include "graph.hpp"
#include "graph_utils.hpp"

using namespace std;
using namespace std::chrono;

// Represents a search node in the priority queue maintained by the
// branch and bound algorithm.
struct FrontierNode {
  int lower_bound;
  int node_num;
  map<int, bool> *c;
};

// A comparator for ordering FrontierNodes.
struct FrontierNodeComparator {
  bool operator()(struct FrontierNode* node1, struct FrontierNode* node2) {
    return (node1->lower_bound < node2->lower_bound)
      && (node1->node_num < node2->node_num);
  }
};

// A comparator for ordering a graph's vertices by their degrees
// in descending order.
struct VertexComparator {
  bool operator()(pair<int, int> v1, pair<int, int> v2) {
    return (v1.second > v2.second);
  }
} vertex_comparator;

// A helper function that takes in a map representing a partial
// vertex cover and returns a set containing just the vertices
// that are in the vertex set.
set<int> * get_vertex_cover_set(map<int, bool> *vc_map) {
  set<int> *vc = new set<int>;

  for (map<int, bool>::iterator it = vc_map->begin(); it != vc_map->end(); it++) {
    if (it->second) {
      vc->insert(it->first);
    }
  }

  return vc;
}

set<int> * branch_and_bound(Graph *G, string out_filename, high_resolution_clock::time_point start_time, duration<double> cutoff_time, string cutoff_time_str, double *sol_time_ptr) {  
  ofstream trace_file (out_filename + ".trace");

  // Create a list of vertices sorted in order of decreasing degree
  // so the algorithm can consider the vertices in that order
  vector<pair<int, int> > vertex_list;
  
  for (map<int, int>::iterator it = G->degrees->begin(); it != G->degrees->end(); it++) {
    vertex_list.push_back(make_pair(it->first, it->second));
  }

  sort(vertex_list.begin(), vertex_list.end(), vertex_comparator);

  // Initialize the global upper bound and best solution
  int upper_bound = G->V->size();
  set<int> *c_best = new set<int>;
  int frontier_node_count = 0;

  // Initialize the search frontier with a search node containing
  // an empty vertex cover
  priority_queue<struct FrontierNode*, vector<struct FrontierNode*>, FrontierNodeComparator> frontier;
  
  struct FrontierNode *first_node = new struct FrontierNode;
  first_node->lower_bound = 0;
  first_node->node_num = frontier_node_count++;
  first_node->c = new map<int, bool>;
  
  frontier.push(first_node);

  // Search until we have checked/pruned all possibilities
  // or time is up
  while ((!frontier.empty()) && ((high_resolution_clock::now() - start_time) < cutoff_time)) {
    // Pop the next node from the priority queue
    struct FrontierNode *curr = frontier.top();
    frontier.pop();
    int lower_bound = curr->lower_bound;
    map<int, bool> *c = curr->c;
    set<int> *vc_set = get_vertex_cover_set(c);   

    // Create a subgraph; this subgraph is needed so that we can
    // construct a lower bound for new search nodes that we
    // add to the frontier using the size of a maximal matching
    // on the remaining uncovered subgraph
    set<int> subgraph_v;
    subgraph_v.insert(G->V->begin(), G->V->end());    
    
    if (!vc_set->empty()) {
      for (set<int>::iterator it = vc_set->begin(); it != vc_set->end(); it++) {
	subgraph_v.erase((*it));
      }
    }
      
    Graph *g = get_subgraph(G, &subgraph_v);

    // Check to see if the vertex cover is a solution
    bool g_is_empty = g->E->size() == 0;

    // If we found a solution and it's better than what
    // we currently have, update c_best and upper_bound
    if (g_is_empty && (vc_set->size() < upper_bound)) {      
      set<int> *temp = c_best;
      c_best = vc_set;
      vc_set = temp;
      upper_bound = c_best->size();
      duration<double> sol_time = high_resolution_clock::now() - start_time;
      (*sol_time_ptr) = sol_time.count();

      cout << "Found solution with quality " << upper_bound << " in " << sol_time.count() << " seconds" << "\n";
      trace_file << sol_time.count() << "," << c_best->size() << "\n";
    }

    // Else if we have a partial solution and it still looks promising
    else if ((!g_is_empty) && (c->size() < G->V->size()) && (lower_bound < upper_bound)) {
      // Get the next vertex to consider
      int v = vertex_list[c->size()].first;      

      // Add the case where we do not include v in the vertex cover
      // to the search frontier
      map<int, bool> *c_prime_dont_keep_v = new map<int, bool>;
      c_prime_dont_keep_v->insert(c->begin(), c->end());
      c_prime_dont_keep_v->insert(make_pair(v, false));

      // The size of a maximal matching on the uncovered subgraph
      // plus the size of the partial solution gives a lower bound
      set<int> *temp_vc_set = get_vertex_cover_set(c_prime_dont_keep_v);
      int c_prime_size = temp_vc_set->size();
      delete temp_vc_set;
      set<pair<int, int> > *mm = maximal_matching(g);
      int max_match_size = mm->size();
      delete mm;
      int new_lower_bound_dont_keep_v = c_prime_size + max_match_size;

      // Add the case where we include v in the vertex cover
      // to the search frontier
      map<int, bool> *c_prime_keep_v = new map<int, bool>;
      c_prime_keep_v->insert(c->begin(), c->end());
      c_prime_keep_v->insert(make_pair(v, true));

      set<int> subgraph_vertex_set;
      subgraph_vertex_set.insert(g->V->begin(), g->V->end());
      subgraph_vertex_set.erase(v);
      
      struct Graph *new_g = get_subgraph(g, &subgraph_vertex_set);      

      temp_vc_set = get_vertex_cover_set(c_prime_keep_v);
      c_prime_size = temp_vc_set->size();
      delete temp_vc_set;
      mm = maximal_matching(new_g);
      max_match_size = mm->size();
      delete mm;
      int new_lower_bound_keep_v = c_prime_size + max_match_size;

      delete new_g;

      if (new_lower_bound_keep_v < upper_bound) {
	struct FrontierNode *new_node = new struct FrontierNode;
	new_node->lower_bound = new_lower_bound_keep_v;
	new_node->node_num = frontier_node_count++;
	new_node->c = c_prime_keep_v;	
	
	frontier.push(new_node);	
      } else {
	delete c_prime_keep_v;
      }

      if (new_lower_bound_dont_keep_v < upper_bound) {    
	struct FrontierNode *new_node = new struct FrontierNode;
	new_node->lower_bound = new_lower_bound_dont_keep_v;
	new_node->node_num = frontier_node_count++;
	new_node->c = c_prime_dont_keep_v;

	frontier.push(new_node);
      } else {
	delete c_prime_dont_keep_v;
      }
    }

    delete curr;
    delete g;
    delete c;
    delete vc_set;
  }

  trace_file.close();
  return c_best;
}

