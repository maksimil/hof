#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

template <class SplitFn>
std::vector<std::string> split_at_fn(const std::string &str, SplitFn split_fn) {
  static_assert(std::is_invocable_r<bool, SplitFn, const char &>::value,
                "split_fn should be invocable as bool(char)");

  std::vector<std::string> res;

  if (str.empty()) {
    return res;
  }

  res.push_back(std::string());

  for (char c : str) {
    if (split_fn(c)) {
      res.push_back(std::string());
    } else {
      res[res.size() - 1].push_back(c);
    }
  }

  return res;
}

struct CaptureSplitFn {
  const char &split_char;

  bool operator()(const char &c) { return c == split_char; }
};

inline std::vector<std::string> split_at_char(std::string str,
                                              const char &split_char) {
  return split_at_fn(str, CaptureSplitFn{split_char});
}

inline std::vector<std::string> split_at_char_lambda(std::string str,
                                                     const char &split_char) {
  return split_at_fn(str, [&](const char &c) { return c == split_char; });
}

inline bool split_fn(const char &c) { return c == ' ' || c == '-'; }

inline void strings_hof_test() {
  std::cout << "STRINGS_TEST\n";

  std::string a = "ha-ha i-i";

  {
    std::vector<std::string> split = split_at_fn(a, split_fn);

    for (const std::string &s : split) {
      std::cout << "\"" << s << "\", ";
    }

    std::cout << "\n";
  }

  {
    std::vector<std::string> split = split_at_char(a, ' ');

    for (const std::string &s : split) {
      std::cout << "\"" << s << "\", ";
    }

    std::cout << "\n";
  }

  {
    std::vector<std::string> split = split_at_char_lambda(a, '-');

    for (const std::string &s : split) {
      std::cout << "\"" << s << "\", ";
    }

    std::cout << "\n";
  }

  std::cout << std::endl;
}
