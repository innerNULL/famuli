/// file: graph_dev.cpp
/// date: 2021-06-02


#include <memory>
#include "famuli/graph/types.h" 
#include "famuli/graph/graph_funcs.h"
#include "famuli/graph/rankers.h"


int main(int argc, char** argv) {
  std::string graph_path = "../test_data/graph.txt";
  float damping_factor = 0.85;

  for (int32_t i = 1; i < argc; ++i) {
    if (i == 1) {
      printf("reset graph_path to: %s\n", argv[i]);
      graph_path = std::string(argv[i]);
    } 
    if (i == 2) {
      printf("reset damping_factor to: %s\n", argv[i]);
      damping_factor = std::stof(std::string(argv[i]));
    }
  }

  //printf("graph_path: %s\n", argv[i]);
  //printf("damping_factor: %s\n", argv[i]);

  std::shared_ptr<famuli::Graph> graph_ptr = famuli::new_graph(graph_path);
  std::shared_ptr<famuli::BaseRanker> pagerank_ptr(
      new famuli::PageRankRanker(graph_ptr, damping_factor));

  pagerank_ptr->run();
  return 0;
}
