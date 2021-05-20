/// file: main.cpp
/// date: 2021-03-12


#include <cstdlib>
#include <sstream>
#include <utility>
#include <fasttext/fasttext.h>
#include <fasttext/model.h>

#include "famuli/model/model.h"
#include "famuli/model/fasttext.h"




int main() {
  const std::string MODEL_URL = "https://dl.fbaipublicfiles.com/fasttext/supervised-models";
  const std::string MODEL_NAME = "sogou_news.ftz";

  std::string model_uri = MODEL_URL + "/" + MODEL_NAME;
  std::string model_path = "./" + MODEL_NAME;
  system(("rm -rf " + model_path + " && wget " + model_uri).c_str());

  famuli::ModelBase model = famuli::ModelBase("./fake");
  famuli::Fasttext fasttext(model_path);
  fasttext.init();
  std::vector< std::pair<float, std::string> > predictions;
  fasttext.infer(&predictions, 
      "上 海 老 相 机 制 造 博 物 馆 即 将 开 门 迎 客");
  famuli::Fasttext::print_outputs(predictions);
  return 0;
}
