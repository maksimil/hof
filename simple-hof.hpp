#include <iostream>
#include <type_traits>

template <class X, class Y, class F> Y apply(F f, X x) {
  static_assert(std::is_invocable_r<Y, F, X>::value, "f should be Y(X)");

  return f(x);
}

inline int inc(int a) { return a + 1; }

inline void simple_hof_test() {
  std::cout << "SIMPLE_TEST\n";

  std::cout << apply<int, int>(inc, 0) << "\n";

  std::cout << std::endl;
}
