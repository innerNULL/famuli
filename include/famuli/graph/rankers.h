/// file: rankers.h
/// date: 2021-06-03


#ifndef GRAPH_GRAPH_RANKERS_H_
#define GRAPH_GRAPH_RANKERS_H_


#include <iostream>
#include <fstream>
#include <memory>
#include <Eigen/Dense> 
#include <absl/strings/str_replace.h>
#include <absl/strings/str_split.h>
#include <spdlog/spdlog.h>
#include "./types.h"
#include "./utils.h"
#include "./graph_funcs.h"
#include "./ranker_cores.h"
#include "./ranker_core_funcs.h"


namespace famuli {


class BaseRanker {
 public:
  virtual void run() {};
};


class PageRankRanker : public BaseRanker {
 public:
  PageRankRanker();
  PageRankRanker(std::shared_ptr<Graph> graph_ptr);
  PageRankRanker(std::shared_ptr<Graph> graph_ptr, const float damping_factor);

  void run() override;

 protected:
  std::shared_ptr<PageRankCore> core_ptr;
};


PageRankRanker::PageRankRanker(std::shared_ptr<Graph> graph_ptr) {
  spdlog::info("Initializing PageRankRanker.");
  this->core_ptr = PageRank::new_ranker_ptr(graph_ptr);
}


PageRankRanker::PageRankRanker(
    std::shared_ptr<Graph> graph_ptr, const float damping_factor) {
  this->core_ptr = PageRank::new_ranker_ptr(graph_ptr, damping_factor);
}


void PageRankRanker::run() {
  Eigen::MatrixXd adjacency_mat_ = this->core_ptr->graph_ptr->adjacency_mat;
  Eigen::MatrixXd adjacency_col_norm = adjacency_mat_.colwise().sum();
  Eigen::MatrixXd edge_weights = adjacency_mat_;

  for (int32_t i = 0; i < adjacency_col_norm.cols(); ++i) {
    adjacency_col_norm(i) = (adjacency_col_norm(i) == 0) ? 0.0 : 1 / adjacency_col_norm(i);
    edge_weights.col(i) =  edge_weights.col(i) * adjacency_col_norm(i);
  }
  //std::cout << "\n" << adjacency_mat_ << std::endl;
  //std::cout << "\n" << adjacency_col_norm << std::endl;
  //std::cout << "\n" << edge_weights << std::endl;

  std::set<int64_t> nonisolated_vertexes = graph_get_nonisolated_vertexe_ids(this->core_ptr->graph_ptr);

  for (int32_t iter_num = 0; iter_num < 10; ++iter_num) 
  while (true) {
    PageRank::one_step_iteration(this->core_ptr, &edge_weights);
    spdlog::info("Current vertexes weights: ");
    std::cout << this->core_ptr->vertex_scores.transpose() << std::endl;
    std::cout << this->core_ptr->last_vertex_scores.transpose() << std::endl;
    graph_print_vertexes(this->core_ptr->graph_ptr);

    if ( PageRank::if_convergence(this->core_ptr, 0.0, &nonisolated_vertexes) ) {
      spdlog::info("Mining finished.");
      break; 
    } 
  }
  //printf("%i\n", nonisolated_vertexes.size());
}




} // namespace famuli


#endif
