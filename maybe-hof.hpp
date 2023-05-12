#include <iostream>
#include <ostream>
#include <type_traits>

template <class T> struct Maybe {
  T value;
  bool has_value;

  Maybe(T value) {
    this->value = value;
    this->has_value = true;
  }

  Maybe() {
    this->value = T{};
    this->has_value = false;
  }

  template <class V, class F> Maybe<V> bind(F f) {
    static_assert(std::is_invocable_r<Maybe<V>, F, T>::value,
                  "f should be Maybe<V>(T)");

    if (this->has_value) {
      return f(this->value);
    } else {
      return Maybe<V>();
    }
  }

  template <class V, class F> Maybe<V> fmap(F f) {
    static_assert(std::is_invocable_r<V, F, T>::value, "f should be V(T)");

    return this->bind<V>([=](T value) { return Maybe(f(value)); });
  }
};

template <class T>
std::ostream &operator<<(std::ostream &out, const Maybe<T> &m) {
  if (m.has_value) {
    return out << "Just " << m.value;
  } else {
    return out << "None";
  }
}

inline Maybe<double> safe_inverse(double v) {
  if (v == 0) {
    return Maybe<double>();
  } else {
    return Maybe<double>(1 / v);
  }
}

inline void maybe_hof_test() {
  std::cout << "MAYBE_TEST\n";

  {
    Maybe<double> m(0);

    std::cout << m
                     // increment
                     .fmap<double>([](int v) { return v + 1; })
                     // invert
                     .bind<double>(safe_inverse);
    std::cout << "\n";
  }

  {
    Maybe<double> m(-1);

    std::cout << m
                     // increment
                     .fmap<double>([](int v) { return v + 1; })
                     // invert
                     .bind<double>(safe_inverse);
    std::cout << "\n";
  }

  std::cout << std::endl;
}
