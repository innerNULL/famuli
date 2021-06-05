/// file: types.h
/// date: 2021-06-02


#ifndef GRAPH_TYPES_H_
#define GRAPH_TYPES_H_


#include <string>
#include <vector>
#include <unordered_map>
#include <Eigen/Dense>


namespace famuli {


/*
typedef struct Edge {
  int8_t if_undirected = 1;
  void* from;
  void* to;
} Edge;
*/


typedef struct Vertex {
  int64_t id;
  int64_t internal_id;
  float score = 1.0;
} Vertex;


typedef struct GraphMeta {
  int8_t if_undirected = 1;
  int32_t edges_count = 0;
  int32_t vertexes_count = 0;
  std::unordered_map<int64_t, int64_t> internal_id2id;
} GraphMeta;


typedef struct Graph {
  std::unordered_map< int64_t, std::vector<int64_t> > edges;
  std::unordered_map<int64_t, Vertex> vertexes;
  Eigen::MatrixXd adjacency_mat;
  GraphMeta meta;
} Graph;


/// Graph member function
void graph_parsing_line(std::shared_ptr<Graph> obj, const std::string& line, const std::string& stage);
void graph_parsing_line(Graph* obj, const std::string& line, const std::string& stage);
void graph_adjacency_mat_build(std::shared_ptr<Graph> obj);
void graph_print_edges(std::shared_ptr<Graph> obj);
void graph_meta_filling(std::shared_ptr<Graph> obj);
int64_t graph_allocate_internal_id(std::shared_ptr<Graph> obj);
void graph_build_post_proc(std::shared_ptr<Graph> obj);

} // namespace famuli


#endif
