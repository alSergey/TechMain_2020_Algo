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
  explicit BinaryTree()
      : root(nullptr) {
  }

  void Add(const T &key) {
    if (!root) {
      root = new Node<T>(key);
      return;
    }

    auto current = root;
    Node<T> *last = nullptr;
    while (current) {
      last = current;
      current = cmp(key, current->key) ? current->left : current->right;
    }

    if (cmp(key, last->key)) {
      last->left = new Node<T>(key);
    } else {
      last->right = new Node<T>(key);
    }
  }

  bool Comp() {
    if (!root) {
      return false;
    }

    std::stack<Node<T> *> s1, s2;
    s1.push(root);

    while (!s1.empty()) {
      auto node = s1.top();
      s1.pop();
      s2.push(node);

      if (node->left)
        s1.push(node->left);
      if (node->right)
        s1.push(node->right);
    }

    while (!s2.empty()) {
      auto node = s2.top();
      s2.pop();
      if (node->key != root->key) {
        return false;
      }
    }

    return true;
  }

 private:
  Comparator cmp;
  Node<T> *root;
};

void run(std::istream &is, std::ostream &os) {
  size_t n;
  is >> n;
  BinaryTree<int> binaryTree;
  for (size_t i = 0; i < n; ++i) {
    int val;
    is >> val;
    binaryTree.Add(val);
  }

  if (binaryTree.Comp()) {
    os << "COMP" << std::endl;
  } else {
    os << "NECOMP" << std::endl;
  }
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "3 3 3" << std::endl;

    run(input, output);

    assert(output.str() == "COMP\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "3 2 3" << std::endl;

    run(input, output);

    assert(output.str() == "NECOMP\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "1 3 3" << std::endl;

    run(input, output);

    assert(output.str() == "NECOMP\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "1" << std::endl;
    input << "7" << std::endl;

    run(input, output);

    assert(output.str() == "COMP\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "10" << std::endl;
    input << "7 7 7 7 7 7 7 7 7 7" << std::endl;

    run(input, output);

    assert(output.str() == "COMP\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "10" << std::endl;
    input << "7 7 7 7 7 7 7 2 7 7" << std::endl;

    run(input, output);

    assert(output.str() == "NECOMP\n");
  }
}

int main() {
  test();
//  run(std::cin, std::cout);

  return 0;
}