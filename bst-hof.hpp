#include <iomanip>
#include <iostream>
#include <list>
#include <numeric>
#include <optional>
#include <sstream>
#include <type_traits>
#include <vector>

template <class T> struct Node {
  T value;
  Node<T> *p_right;
  Node<T> *p_left;

  Node(T value) {
    this->value = value;
    this->p_right = nullptr;
    this->p_left = nullptr;
  }

  ~Node() {
    delete this->p_right;
    delete this->p_left;
  }
};

template <class T> std::vector<std::string> tree_levels(const Node<T> *root) {
  std::stringstream root_string;
  root_string << root->value;

  std::vector<std::string> root_levels;
  root_levels.push_back(root_string.str());

  auto left_child = root->p_left;
  auto right_child = root->p_right;

  std::vector<std::string> left_levels, right_levels;

  if (left_child) {
    left_levels = tree_levels(left_child);
  }

  if (right_child) {
    right_levels = tree_levels(right_child);
  }

  size_t left_width = std::accumulate(left_levels.begin(), left_levels.end(),
                                      root_levels.front().length(),
                                      [](size_t acc, const std::string &v) {
                                        return std::max(acc, v.length());
                                      });

  auto left_iter = left_levels.begin();
  auto right_iter = right_levels.begin();

  while (left_iter != left_levels.end()) {
    std::stringstream this_level;
    this_level << std::left << std::setw(left_width + 1) << *left_iter;

    if (right_iter != right_levels.end()) {
      this_level << *right_iter << " ";
      right_iter++;
    }

    left_iter++;

    root_levels.push_back(this_level.str());
  }

  while (right_iter != right_levels.end()) {
    std::stringstream this_level;
    this_level << std::left << std::setw(left_width + 1) << " ";
    this_level << *right_iter << " ";

    right_iter++;
    root_levels.push_back(this_level.str());
  }

  return root_levels;
}

template <class T>
std::ostream &pretty_print_node(std::ostream &out, const Node<T> *node) {
  auto levels = tree_levels(node);
  for (auto level : levels) {
    out << level << std::endl;
  }
  return out;
}

template <class T, class Keyer>
using KeyType = typename std::invoke_result_t<Keyer, const T &>;

template <class T, class Keyer>
Node<T> *search_node(Node<T> *node, const KeyType<T, Keyer> &key) {
  if (node) {
    KeyType<T, Keyer> node_key = Keyer{}(node->value);

    if (node_key == key) {
      return node;
    } else if (node_key <= key) {
      return search(node->p_right, key);
    } else {
      return search(node->p_left, key);
    }
  } else {
    return nullptr;
  }
}

template <class T, class Keyer> void add_node(Node<T> *node, T value) {
  KeyType<T, Keyer> node_key = Keyer{}(node->value);
  KeyType<T, Keyer> value_key = Keyer{}(value);

  if (node_key <= value_key) {
    if (node->p_right) {
      add_node<T, Keyer>(node->p_right, value);
    } else {
      node->p_right = new Node<T>(value);
    }
  } else {
    if (node->p_left) {
      add_node<T, Keyer>(node->p_left, value);
    } else {
      node->p_left = new Node<T>(value);
    }
  }
}

template <class T, class WalkFn> void walk_node(Node<T> *node, WalkFn walk_fn) {
  static_assert(std::is_invocable_r<void, WalkFn, const T &>::value,
                "walk_fn should be void(const T&)");

  if (node) {
    walk_node(node->p_left, walk_fn);
    walk_fn(node->value);
    walk_node(node->p_right, walk_fn);
  }
}

template <class T> struct DefaultKeyer {
  T operator()(const T &value) { return value; }
};

template <class T, class Keyer = DefaultKeyer<T>> struct BSTTree {
  Node<T> *root;

  BSTTree() {
    static_assert(std::is_invocable<Keyer, const T &>::value,
                  "Keyer should be KeyType(const T&)");
    static_assert(std::is_default_constructible<Keyer>::value,
                  "Keyer should be default constructible");
    this->root = nullptr;
  }

  ~BSTTree() { delete this->root; }

  std::optional<T> search(const KeyType<T, Keyer> &key) {
    Node<T> *node = search_node<T, Keyer>(this->root, key);

    if (node) {
      return node->value;
    } else {
      return std::nullopt;
    }
  }

  void add(T value) {
    if (this->root) {
      add_node<T, Keyer>(this->root, value);
    } else {
      this->root = new Node<T>(value);
    }
  }

  template <class WalkFn> void walk(WalkFn walk_fn) {
    walk_node(this->root, walk_fn);
  }
};

struct NamedInt {
  int uid;
  std::string name;
};

struct NamedIntKeyer {
  int operator()(const NamedInt &value) { return value.uid; }
};

inline std::ostream &operator<<(std::ostream &out, const NamedInt &value) {
  return out << value.uid << ":" << value.name;
}

inline void bst_hof_test() {
  std::cout << "BST_TEST\n";

  BSTTree<NamedInt, NamedIntKeyer> tree;

  tree.add({0, "A"});
  tree.add({-1, "B"});
  tree.add({1, "C"});
  tree.add({5, "D"});
  tree.add({2, "E"});
  tree.add({-3, "F"});
  tree.add({6, "G"});

  pretty_print_node(std::cout, tree.root);

  std::cout << "\n";

  tree.walk([](const NamedInt &value) { std::cout << value << "\n"; });
}
