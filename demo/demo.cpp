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
  return 0;
}
