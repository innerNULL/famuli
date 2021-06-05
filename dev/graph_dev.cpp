/// file: graph_dev.cpp
/// date: 2021-06-02


#include <memory>
#include "famuli/graph/types.h" 
#include "famuli/graph/graph_funcs.h"
#include "famuli/graph/rankers.h"


int main(int argc, char** argv) {
  std::shared_ptr<famuli::Graph> graph_ptr = famuli::new_graph("../test_data/graph.txt");
  std::shared_ptr<famuli::BaseRanker> pagerank_ptr(new famuli::PageRankRanker(graph_ptr, 0.85));
  pagerank_ptr->run();
  return 0;
}
