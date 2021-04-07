// file: fasttext.h      
// date: 2021-04-06


#ifndef MODEL_FASTTEXT_H_
#define MODEL_FASTTEXT_H_


#include <string>
#include <memory>
#include <sstream>
#include "spdlog/spdlog.h"
#include "fasttext.h"
#include "./model.h"


namespace famuli {


class Fasttext : public ModelBase {
 public:
  Fasttext(const std::string model_path) : ModelBase(model_path) {}
  
  /// The following `delete` limitation is necessary, since `fasttext::Model` 
  /// has these limitations, and which is an attribute of `fasttext::FastText`, 
  /// so if we don't add these destruction limitations, when the destruction 
  /// of `Fasttext` happens, the destruction of `Fasttext::model_::fasttext` 
  /// will be error.
  Fasttext(const Fasttext& model) = delete;
  Fasttext(Fasttext&& model) = delete;
  Fasttext& operator=(const Fasttext& other) = delete;
  Fasttext& operator=(Fasttext&& other) = delete;

  void init() {
    this->load();
  }

  void load() {
    spdlog::info("Starts loading fastText model: {}", this->model_path_);
    this->model_.loadModel(this->model_path_);
    spdlog::info("Finished loading fastText model: {}", this->model_path_);
  }

  void run(std::vector< std::pair<float, std::string> >* outputs, 
      const std::string& inputs) {
    spdlog::info("Starts inference");
    std::stringstream inputs_;
    inputs_ << inputs;
    this->model_.predictLine(inputs_, *outputs, 3, 0.01);
    spdlog::info("Finished inference"); 
  }

  static void print_outputs(std::vector< std::pair<float, std::string> >& outputs) {
    spdlog::info("Fastte4xt output: ");
    for (std::vector< std::pair<float, std::string> >::iterator iter = outputs.begin(); 
        iter < outputs.end(); ++iter) {
      spdlog::info("\tlabel: {0}, score: {1}", iter->second, iter->first);
    }
  }


 protected:
  fasttext::FastText model_;
};


} // namespace famuli



#endif
