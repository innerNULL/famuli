// file: model.h
// date: 2021-04-06


#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_


#include <string>
#include "spdlog/spdlog.h"


namespace famuli {


class ModelBase {
 public:
  ModelBase(const std::string& model_path) {
    assert(model_path.size() > 0);
    spdlog::info("Init stage, model path: {}", model_path);
    this->model_path_ = model_path;
  }

  virtual void init() {}
  virtual void load() {}
  virtual void infer() {}
  static void print_outputs() {}

 protected:
  std::string model_path_;
};


} // namespace famuli



#endif
