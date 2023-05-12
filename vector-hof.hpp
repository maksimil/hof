#include <cstddef>
#include <iostream>
#include <type_traits>
#include <vector>

template <class T, class W>
void walk_vector(const std::vector<T> &data, W walk_fn) {
  static_assert(std::is_invocable_r<void, W, const T &>::value,
                "walk_fn should be void(const T&)");

  for (size_t i = 0; i < data.size(); i++) {
    walk_fn(data[i]);
  }
}

template <class V, class T, class F>
std::vector<V> fmap(const std::vector<T> &data_arr, F f) {
  static_assert(std::is_invocable_r<V, F, T>::value, "f should be V(T)");

  std::vector<V> out_arr;
  out_arr.reserve(data_arr.size());

  walk_vector(data_arr, [&out_arr, f](const T &v) { out_arr.push_back(f(v)); });

  return out_arr;
}

template <class V, class T, class F>
V foldl(V initial_value, const std::vector<T> &data_arr, F f) {
  static_assert(std::is_invocable_r<V, F, const V &, const T &>::value,
                "f should be V(V, const T&)");

  V acc = initial_value;

  walk_vector(data_arr, [&acc_ref = acc, f](const T &value) {
    acc_ref = f(acc_ref, value);
  });

  return acc;
}

inline void vector_hof_test() {
  std::cout << "VECTOR_TEST\n";

  std::vector<double> data = {0.2, 2, 4, 5, 2};

  std::vector<double> data_inverse =
      fmap<double>(data, [](const double &v) { return 1 / v; });

  for (size_t i = 0; i < data_inverse.size(); i++) {
    std::cout << data_inverse[i] << " ";
  }

  std::cout << "\n";

  double data_min = foldl(
      data_inverse[0], data_inverse,
      [](const double &acc, const double &v) { return std::min(acc, v); });

  std::cout << "MIN: " << data_min << "\n";

  double data_sum =
      foldl(0.0, data_inverse,
            [](const double &acc, const double &v) { return acc + v; });

  std::cout << "SUM: " << data_sum << "\n";

  std::cout << std::endl;
}
