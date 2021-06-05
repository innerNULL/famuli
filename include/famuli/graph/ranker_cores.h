/// file: ranker_cores.h
/// date: 2021-06-03


#ifndef GRAPH_RANKER_CORES_H_
#define GRAPH_RANKER_CORES_H_


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


namespace famuli {


typedef struct PageRankCore {
  std::shared_ptr<Graph> graph_ptr;
  float damping_factor = 0.85; // Ref to `d` in textrank paper
  Eigen::MatrixXd vertex_scores;
  Eigen::MatrixXd last_vertex_scores;
} PageRankParam;


} // namespace famuli


#endif
