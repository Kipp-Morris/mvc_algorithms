#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <set>
#include <string>

#include "graph.hpp"

Graph *create_graph_from_metis_io(std::string filename);
Graph *get_subgraph(Graph *G, std::set<int> *V_prime);
std::set<std::pair<int, int> > *maximal_matching(Graph *G);

#endif
