#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "branch_and_bound.hpp"
#include "graph.hpp"
#include "graph_utils.hpp"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {  
  string input_file;
  string alg = "BnB";
  string cutoff_time_str = "600";
  duration<double> cutoff_time = duration<double>(600);
  
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-inst") == 0) {
      input_file = argv[i + 1];
      i++;
    }

    if (strcmp(argv[i], "-alg") == 0) {
      alg = argv[i + 1];
      i++;
    }

    if (strcmp(argv[i], "-time") == 0) {
      cutoff_time_str = argv[i + 1];
      cutoff_time = (duration<double>) stoi(argv[i + 1]);
      i++;
    }
  }  

  if ((input_file != "") && (alg == "BnB")) {
    high_resolution_clock::time_point start_time = high_resolution_clock::now();
    Graph *G = create_graph_from_metis_io(input_file);
    double *sol_time = new double;

    size_t pos = 0;
    string token;
    string s = input_file;
    string delimiter = "/";
    while ((pos = s.find(delimiter)) != string::npos) {
      token = s.substr(0, pos);
      s.erase(0, pos + delimiter.length());
    }
    
    string out_filename = s.substr(0, s.size() - 6) + "_BnB_" + cutoff_time_str;

    set<int> *vc = branch_and_bound(G, out_filename, start_time, cutoff_time, cutoff_time_str, sol_time);
    
    ofstream sol_file ("results/solutions/" + out_filename + ".sol");
    sol_file << (*sol_time) << "," << vc->size() << "\n";
    sol_file.close();

    delete sol_time;
    delete vc;
    delete G;
  }    
  
  return 0;
}
