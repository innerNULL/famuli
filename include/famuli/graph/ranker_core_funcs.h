/// file: ranker_core_funcs.h
/// date: 2021-06-03


#ifndef GRAPH_RANKER_CORE_FUNCS_H_
#define GRAPH_RANKER_CORE_FUNCS_H_


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


namespace famuli {

namespace PageRank {


std::shared_ptr<PageRankCore> new_ranker_ptr();
std::shared_ptr<PageRankCore> new_ranker_ptr(std::shared_ptr<Graph> graph_ptr);
std::shared_ptr<PageRankCore> new_ranker_ptr(std::shared_ptr<Graph> graph_ptr, const float damping_factor);


std::shared_ptr<PageRankCore> new_ranker_ptr() {
  std::shared_ptr<PageRankCore> ranker_ptr(new PageRankCore);
  return ranker_ptr;
}


std::shared_ptr<PageRankCore> new_ranker_ptr(std::shared_ptr<Graph> graph_ptr) {
  std::shared_ptr<PageRankCore> ranker_ptr = new_ranker_ptr();
  ranker_ptr->graph_ptr = graph_ptr;
  return ranker_ptr;
}


std::shared_ptr<PageRankCore> new_ranker_ptr(
    std::shared_ptr<Graph> graph_ptr, const float damping_factor) {
  std::shared_ptr<PageRankCore> ranker_ptr = new_ranker_ptr(graph_ptr);
  
  ranker_ptr->damping_factor = damping_factor;
  ranker_ptr->vertex_scores = 
      Eigen::MatrixXd::Constant(ranker_ptr->graph_ptr->meta.vertexes_count, 1, 1.0);

  for (const std::pair<int64_t, Vertex>& vertex_ : ranker_ptr->graph_ptr->vertexes) {
    (ranker_ptr->vertex_scores)(vertex_.second.internal_id) = vertex_.second.score;
  }
  //std::cout << ranker_ptr->vertex_scores << std::endl;
  return ranker_ptr;
}


void one_step_iteration(
    std::shared_ptr<PageRankCore> core_ptr, const Eigen::MatrixXd* edge_weights) {
  //std::cout << "\n" << core_ptr->vertex_scores << std::endl;
  //std::cout << "\ndbg0:\n" << edge_weights->transpose() << std::endl;
  //std::cout << "\ndbg1:\n" << core_ptr->vertex_scores << std::endl;
  core_ptr->last_vertex_scores = core_ptr->vertex_scores;
  core_ptr->vertex_scores = 
      Eigen::MatrixXd::Constant(core_ptr->vertex_scores.size(), 1, 1 - core_ptr->damping_factor) 
      + core_ptr->damping_factor * edge_weights->transpose() * core_ptr->vertex_scores;

  for (int32_t i = 0; i < core_ptr->vertex_scores.size(); ++i) {
    int64_t curr_id = core_ptr->graph_ptr->meta.internal_id2id[i];
    core_ptr->graph_ptr->vertexes[curr_id].score = core_ptr->vertex_scores(i);
  }
}


/// TODO@202106041820
bool if_convergence(std::shared_ptr<PageRankCore> core_ptr, 
    const float diff_threshold=0.0, std::set<int64_t>* target_internal_ids=NULL) {

  if (core_ptr->last_vertex_scores.size() == 0) { return false; }

  Eigen::MatrixXd diff_abs = (core_ptr->vertex_scores - core_ptr->last_vertex_scores).cwiseAbs();
  float last_abs_sum = 0.0;
  float diff_abs_sum = 0.0;

  std::set<int64_t>* target_internal_ids_ = NULL;
  if (target_internal_ids == NULL) {
    for (auto& kv : core_ptr->graph_ptr->vertexes) {
      target_internal_ids_->insert(kv.second.internal_id);
    }
  } else {
    target_internal_ids_ = target_internal_ids;
  }

  /// TODO@202106051653: This can try openmp to improve performance.
  for (std::set<int64_t>::iterator iter = target_internal_ids_->begin();
      iter != target_internal_ids_->end(); ++iter) {
    int64_t internal_id = core_ptr->graph_ptr->vertexes[*iter].internal_id;
    //printf("dbg: %i\(%i\): %f -> %f\n", *iter, internal_id, 
    //    core_ptr->last_vertex_scores(internal_id), core_ptr->vertex_scores(internal_id));

    float last_score_ = core_ptr->last_vertex_scores(internal_id);
    float curr_score_ = core_ptr->vertex_scores(internal_id);
    float last_score_abs_ = last_score_ > 0 ? last_score_ : -1.0 * last_score_;
    float curr_diff_abs_ = 
        curr_score_ > last_score_ ? (curr_score_ - last_score_) : (last_score_ - curr_score_);

    last_abs_sum += last_score_abs_;
    diff_abs_sum += curr_diff_abs_;
    //printf("dbg:%i, %i,  %f, %f, %f, %f\n", 
    //    *iter, internal_id, last_score_, curr_score_, last_score_abs_, curr_diff_abs_);
  }

  //printf("dbg: %f, %f\n", last_abs_sum, diff_abs_sum);
  if (last_abs_sum != 0.0) {
    if (diff_abs_sum / last_abs_sum <= diff_threshold) {
      //printf("satisfy convergence condition.\n");
      return true;
    }
  }
  return false;
}


} // namespace PageRankCore

} // namespace famuli


#endif
