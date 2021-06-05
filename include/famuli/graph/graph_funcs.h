/// file: graph_funcs.h
/// date: 2021-06-02


#ifndef GRAPH_GRAPH_FUNCS_H_
#define GRAPH_GRAPH_FUNCS_H_


#include <iostream>
#include <fstream>
#include <memory>
#include <Eigen/Dense> 
#include <absl/strings/str_replace.h>
#include <absl/strings/str_split.h>
#include <spdlog/spdlog.h>
#include "./types.h"
#include "./utils.h"
#include "./vertex_funcs.h"


namespace famuli {


std::shared_ptr<Graph> new_graph() {
  std::shared_ptr<Graph> graph(new Graph);
  return graph;
}


std::shared_ptr<Graph> new_graph(
    const std::string& graph_file_path, const std::string& mode="default") {
  std::shared_ptr<Graph> graph(new Graph);
  
  std::ifstream graph_file(graph_file_path);
  std::string line;
  std::string stage;

  while (std::getline(graph_file, line)) {
    if (line == "meta") {
      stage = "parsing_meta";
    } else if (line == "vertexes") {
      stage = "parsing_vertex";
    } else if (line == "edges") {
      stage = "parsing_edge";
    } else {
      graph_parsing_line(graph, line, stage);         
    } 
  }
  graph_build_post_proc(graph);
  
  return graph;
}


void graph_build_post_proc(std::shared_ptr<Graph> obj) {
  graph_meta_filling(obj);
  graph_adjacency_mat_build(obj);
  graph_print_edges(obj);
}


void graph_parsing_meta_line(
    std::shared_ptr<Graph> obj, const std::string& line) {
  if (line != "meta") {
    std::string line_ = empty_char_clean(line);
    std::vector<std::string> kv = absl::StrSplit(line_, ":");
   
    if (kv.size() == 2) {
      GraphMeta* obj_meta = &(obj->meta);
      obj_meta->if_undirected = (kv[1] == "1");
    }
  }
}


void graph_parsing_vertex_line(
    std::shared_ptr<Graph> obj, const std::string& line) {
  if (line != "vertexes") {
    std::string cleaned_line = empty_char_clean(line); 
    if (cleaned_line.size() > 0) {
      std::unordered_map<int64_t, Vertex>* vertexes_ptr = &(obj->vertexes);
      int64_t id_ = std::stoi(line);
      int64_t internal_id_ = graph_allocate_internal_id(obj);

      std::shared_ptr<Vertex> vertex_ptr_ = new_vertex_ptr(line, internal_id_);
      (*vertexes_ptr)[id_] = *vertex_ptr_;
      obj->meta.internal_id2id[internal_id_] = id_;
    }
  }
}


void graph_parsing_edge_line(
    std::shared_ptr<Graph> obj, const std::string& line) {
  if (line != "edges") {
    std::string cleaned_line = empty_char_clean(line);
    std::vector<std::string> kv = absl::StrSplit(cleaned_line, ",");

    if (cleaned_line.size() > 0 and kv.size() >= 2) {
      std::unordered_map< int64_t, std::vector<int64_t> >* edges_ptr_ = &(obj->edges);
      for (int32_t i = 0; i < kv.size() - 1; ++i) {
        if (kv[i] == kv[i + 1]) continue;
        (*edges_ptr_)[std::stoi(kv[i])].emplace_back( std::stoi(kv[i + 1]) );
      }
    }
  }
}


void graph_parsing_line(std::shared_ptr<Graph> obj, 
    const std::string& line, const std::string& stage) {
  if (stage == "parsing_meta") { graph_parsing_meta_line(obj, line); }
  if (stage == "parsing_vertex") { graph_parsing_vertex_line(obj, line); }
  if (stage == "parsing_edge") { graph_parsing_edge_line(obj, line); }
}


void graph_adjacency_mat_build(std::shared_ptr<Graph> obj) {
  std::unordered_map<int64_t, Vertex>* vertexes_ptr_ = &(obj->vertexes);
  std::unordered_map< int64_t, std::vector<int64_t> >* edges_ptr_ = &(obj->edges);
  Eigen::MatrixXd* adjacency_mat_ = &(obj->adjacency_mat);
  
  *adjacency_mat_ = Eigen::MatrixXd::Constant(
      obj->meta.vertexes_count, obj->meta.vertexes_count, 0);

  for (std::unordered_map< int64_t, std::vector<int64_t> >::iterator iter = edges_ptr_->begin();
      iter != edges_ptr_->end(); ++iter) {
    int64_t node1 = iter->first;
    int64_t node1_internal_ = (*vertexes_ptr_)[node1].internal_id;

    for (int64_t node2 : iter->second) {
      int64_t node2_internal_ = (*vertexes_ptr_)[node2].internal_id;
      (*adjacency_mat_)(node1_internal_, node2_internal_) += 1;
      if (obj->meta.if_undirected) {
        (*adjacency_mat_)(node2_internal_, node1_internal_) += 1; 
      }
    }
  }
}


void graph_meta_filling(std::shared_ptr<Graph> obj) {
  obj->meta.vertexes_count = obj->vertexes.size();
  if (obj->meta.if_undirected) {
    obj->meta.edges_count = obj->edges.size() * 2;
  } else {
    obj->meta.edges_count = obj->edges.size();
  }
}


void graph_print_edges(std::shared_ptr<Graph> obj) {
  std::unordered_map< int64_t, std::vector<int64_t> >* edges_ptr_ = &(obj->edges);
  std::unordered_map<int64_t, Vertex>* vertexes_ptr_ = &(obj->vertexes);
  const std::string log_template = "edge: node {0}(internal {1}) -> node {2}(internal {3})";

  spdlog::info("Printing graph edges: ");
  for (std::unordered_map< int64_t, std::vector<int64_t> >::iterator iter = edges_ptr_->begin();
      iter != edges_ptr_->end(); ++iter) {
    int64_t node1 = iter->first;
    int64_t node1_internal_ = (*vertexes_ptr_)[node1].internal_id;
    for (int64_t node2 : iter->second) {
        int64_t node2_internal_ = (*vertexes_ptr_)[node2].internal_id;
      spdlog::info(log_template, node1, node1_internal_, node2, node2_internal_);
      if (obj->meta.if_undirected) {
        spdlog::info(log_template, node2, node2_internal_, node1, node1_internal_);
      }
    }
  }
  spdlog::info("Printing graph Adjacency Matrix: ");
  std::cout << obj->adjacency_mat << std::endl;
}


int64_t graph_allocate_internal_id(std::shared_ptr<Graph> obj) {
  obj->meta.vertexes_count += 1;
  return obj->meta.vertexes_count - 1;
}


void graph_print_vertexes(std::shared_ptr<Graph> obj) {
  spdlog::info("Printing graph vertexes info: ");
  std::string log_temp = "id: {0}, internal_id: {1}, score: {2}";

  for (auto& vertex : obj->vertexes) {
    spdlog::info(log_temp, vertex.second.id, vertex.second.internal_id, vertex.second.score);
  }
}


std::set<int64_t> graph_get_nonisolated_vertexe_ids(std::shared_ptr<Graph> obj) {
  std::set<int64_t> result;
  for (auto& item : obj->edges) {
    result.insert(item.first);
    for (int64_t to_node : item.second) {
      result.insert(to_node);
    }
  }
  return result;
}


} // namespace famuli


#endif
