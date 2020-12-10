#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "graph.hpp"
#include "graph_utils.hpp"

using namespace std;
using namespace std::chrono;

// The best-known approximation algorithm for the MVC problem; gives
// a 2-approximation by using the vertices that make up a maximal
// matching on the graph.
set<int> *approximation_algorithm(Graph *G, string out_filename, high_resolution_clock::time_point start_time, double *sol_time_ptr) {
  ofstream trace_file ("results/traces/" + out_filename + ".trace");
  
  set<int> *vertex_cover = new set<int>;
  set<pair<int, int> > *mm = maximal_matching(G);

  for (set<pair<int, int> >::iterator it = mm->begin(); it != mm->end(); it++) {
    vertex_cover->insert(it->first);
    vertex_cover->insert(it->second);
  }

  duration<double> sol_time = high_resolution_clock::now() - start_time;
  (*sol_time_ptr) = sol_time.count();

  cout << "Found solution with quality " << vertex_cover->size() << " in " << sol_time.count() << " seconds" << "\n";
  
  trace_file << sol_time.count() << "," << vertex_cover->size() << "\n";
  trace_file.close();

  return vertex_cover;
}
