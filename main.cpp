#include "bst-hof.hpp"
#include "maybe-hof.hpp"
#include "simple-hof.hpp"
#include "string-hof.hpp"
#include "vector-hof.hpp"
#include <functional>

int main(int argc, const char *argv[]) {
  simple_hof_test();
  strings_hof_test();
  maybe_hof_test();
  vector_hof_test();
  bst_hof_test();
}
