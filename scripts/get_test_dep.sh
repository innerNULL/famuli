# file: get_test_dep.sh
# date: 2021-04-06


set -x

TEST_RESOURCE_ROOT="./_test"
# https://dl.fbaipublicfiles.com/fasttext/supervised-models/sogou_news.ftz
FASTTEXT_URI="https://dl.fbaipublicfiles.com/fasttext/supervised-models"
FASTTEXT_MODEL="sogou_news.ftz"


function preprocess() { 
    mkdir -p ${TEST_RESOURCE_ROOT}
    cd ${TEST_RESOURCE_ROOT}
}


function get_fasttext_dep() {
    wget ${FASTTEXT_URI}/${FASTTEXT_MODEL}
    unzip ${FASTTEXT_MODEL}
    rm ./*.zip
}


function main() {
    preprocess
    get_fasttext_dep
}


main
