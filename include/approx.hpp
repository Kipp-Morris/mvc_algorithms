#ifndef APPROX_H
#define APPROX_H

#include<chrono>
#include<set>
#include<string>

#include "graph.hpp"

std::set<int> *approximation_algorithm(Graph *G, std::string out_filename, std::chrono::high_resolution_clock::time_point start_time, double *sol_time_ptr);

#endif
