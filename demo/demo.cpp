/// file: main.cpp
/// date: 2021-03-12


#include "fasttext.h"
#include "model.h"

#include "famuli/model/model.h"
#include "famuli/model/fasttext.h"




int main() {
  famuli::ModelBase model = famuli::ModelBase("./fake");
  famuli::Fasttext fasttext("../_test/sogou_news.ftz");
  fasttext.init();
  std::vector< std::pair<float, std::string> > predictions;
  fasttext.run(&predictions, 
      "上 海 老 相 机 制 造 博 物 馆 即 将 开 门 迎 客");
  return 0;
}
