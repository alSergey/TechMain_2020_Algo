/* Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
 * то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
 * Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.
 * Выведите элементы в порядке post-order (снизу вверх). */

#include <iostream>
#include <cassert>
#include <sstream>
#include <stack>

template<typename T>
struct Node {
  explicit Node(T val)
      : key(val),
        left(nullptr),
        right(nullptr) {
  }

  T key;
  Node *left;
  Node *right;
};

template<typename T, typename Comparator = std::less<T>>
class BinaryTree {
 public:
  explicit BinaryTree(std::ostream &os)
      : root(nullptr),
        os(os) {
  }

  ~BinaryTree() {
    PostOrder(false);
  }

  void Add(T &val) {
    if (!root) {
      root = new Node<T>(val);
      return;
    }

    auto current = root;
    Node<T> *last = nullptr;
    while (current) {
      last = current;
      current = cmp(val, current->key) ? current->left : current->right;
    }

    if (cmp(val, last->key)) {
      last->left = new Node<T>(val);
    } else {
      last->right = new Node<T>(val);
    }
  }

  void PostOrder(bool print = true) {
    if (!root) {
      return;
    }

    std::stack<Node<T> *> s1;
    s1.push(root);

    while (!s1.empty()) {
      auto node = s1.top();
      s1.pop();
      tree.push(node);

      if (node->left)
        s1.push(node->left);
      if (node->right)
        s1.push(node->right);
    }

    if (print) {
      printTree();
    } else {
      delTree();
    }
  }

 private:
  void printTree() {
    while (!tree.empty()) {
      auto node = tree.top();
      tree.pop();
      os << node->key << " ";
    }
  }

  void delTree() {
    while (!tree.empty()) {
      auto node = tree.top();
      tree.pop();
      delete[] node;
    }
  }

  Comparator cmp;
  Node<T> *root;
  std::ostream &os;
  std::stack<Node<T> *> tree;
};

void run(std::istream &is, std::ostream &os) {
  size_t n;
  is >> n;
  BinaryTree<int> binaryTree(os);
  for (size_t i = 0; i < n; ++i) {
    int val;
    is >> val;
    binaryTree.Add(val);
  }

  binaryTree.PostOrder();
  os << std::endl;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "2 1 3" << std::endl;

    run(input, output);

    assert(output.str() == "1 3 2 \n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "1 2 3" << std::endl;

    run(input, output);

    assert(output.str() == "3 2 1 \n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "3 1 2" << std::endl;

    run(input, output);

    assert(output.str() == "2 1 3 \n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "10" << std::endl;
    input << "7 2 10 8 5 3 6 4 1 9" << std::endl;

    run(input, output);

    assert(output.str() == "1 4 3 6 5 2 9 8 10 7 \n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "10" << std::endl;
    input << "7 10 2 6 1 9 4 5 8 3" << std::endl;

    run(input, output);

    assert(output.str() == "1 3 5 4 6 2 8 9 10 7 \n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "10" << std::endl;
    input << "4 3 9 5 6 8 7 2 10 1" << std::endl;

    run(input, output);

    assert(output.str() == "1 2 3 7 8 6 5 10 9 4 \n");
  }
}

int main() {
  test();
//  run(std::cin, std::cout);

  return 0;
}