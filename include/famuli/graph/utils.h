/// file: utils.h
/// date: 2021-06-02


#ifndef GRAPH_UTILS_H_
#define GRAPH_UTILS_H_


#include <iostream>
#include <fstream>
#include <memory>
#include <absl/strings/str_replace.h>
#include <absl/strings/str_split.h> 
#include "./types.h"


namespace famuli {


auto empty_char_clean(const std::string& line) -> std::string {
  std::vector<std::pair<const absl::string_view, std::string> > str_sub;
  str_sub.push_back({" ", ""}); 
  str_sub.push_back({"\n", ""});
  str_sub.push_back({"\t", ""});
  return absl::StrReplaceAll(line, str_sub);
}


} // namespace famuli


#endif
