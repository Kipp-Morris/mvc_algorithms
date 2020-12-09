#ifndef BRANCH_AND_BOUND_H
#define BRANCH_AND_BOUND_H

#include <chrono>
#include <set>

#include "graph.h"

std::set<int> *branch_and_bound(Graph *G, std::string out_filename, std::chrono::high_resolution_clock::time_point start_time, std::chrono::duration<double> cutoff_time, std::string cutoff_time_str, double *sol_time_ptr);

#endif
