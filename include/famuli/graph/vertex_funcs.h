/// file: vertex_funcs.h
/// date: 2021-06-02


#ifndef GRAPH_VERTEX_FUNCS_H_
#define GRAPH_VERTEX_FUNCS_H_


#include <iostream>
#include <fstream>
#include <memory>
#include "./types.h"
#include "./utils.h"


namespace famuli {


auto new_vertex_ptr(const int64_t& id) -> std::shared_ptr<Vertex> {
  std::shared_ptr<Vertex> vertex(new Vertex);
  vertex->id = id;
  return vertex;
} 


auto new_vertex_ptr(
    const int64_t& id, const int64_t internal_id) -> std::shared_ptr<Vertex> {
  std::shared_ptr<Vertex> vertex(new Vertex);
  vertex->id = id; 
  vertex->internal_id = internal_id;
  return vertex;
}


auto new_vertex_ptr(const std::string& id) -> std::shared_ptr<Vertex> {
  return new_vertex_ptr(std::stoi(id));
}


auto new_vertex_ptr(
    const std::string& id, const int64_t internal_id) -> std::shared_ptr<Vertex> {
  return new_vertex_ptr(std::stoi(id), internal_id);
}


auto new_bare_vertex_ptr(const int64_t& id) -> Vertex* {
  return new_vertex_ptr(id).get();    
}


} // namespace famuli


#endif
