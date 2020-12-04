#include <iostream>
#include <cassert>
#include <sstream>
#include <stack>

template<typename T>
struct Node {
  explicit Node(const T &key)
      : key(key),
        height(1),
        count(1),
        left(nullptr),
        right(nullptr) {
  }
  T key;
  size_t height;
  size_t count;
  Node *left;
  Node *right;
};

template<typename T, typename Comparator = std::less<T>>
class AvlTree {
 public:
  AvlTree()
      : root(nullptr) {
  }

  ~AvlTree() {
    destroyTree(root);
  }

  void Add(const T &key) {
    root = addInternal(root, key);
  }

  bool Has(const T &key) {
    auto current = root;
    while (current) {
      if (cmp(key, current->key)) {
        current = current->left;
      } else if (cmp(current->key, key)) {
        current = current->right;
      } else {
        return true;
      }
    }

    return false;
  }

  void PostOrder() {
    if (!root) {
      return;
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
      std::cout << node->key << " ";
    }
  }

  void Delete(const T &key) {
    root = deleteInternal(root, key);
  }

 private:
  size_t getNodeCount(Node<T> *node) {
    return node ? node->count : 0;
  }

  void destroyTree(Node<T> *node) {
    if (node) {
      destroyTree(node->left);
      destroyTree(node->right);
      delete[] node;
    }
  }

  Node<T> *deleteInternal(Node<T> *node, const T &key) {
    if (!node) {
      return nullptr;
    }

    if (cmp(node->key, key))
      node->right = deleteInternal(node->right, key);
    else if (cmp(key, node->key))
      node->left = deleteInternal(node->left, key);
    else {
      auto left = node->left;
      auto right = node->right;
      delete node;

      if (!right) {
        return left;
      }

      if (!left) {
        return right;
      }

      return findAndRemove(left, right);
    }

    return doBalance(node);
  }

  Node<T> *addInternal(Node<T> *node, const T &key) {
    if (!node) {
      return new Node<T>(key);
    }

    if (cmp(key, node->key)) {
      node->left = addInternal(node->left, key);
    } else {
      node->right = addInternal(node->right, key);
    }

    return doBalance(node);
  }

  Node<T> *findAndRemove(Node<T> *left, Node<T> *right) {
    Node<T> *tmp;

    tmp = findMin(right);
    tmp->right = removeMin(right);
    tmp->left = left;

    return doBalance(tmp);
  }

  Node<T> *findMin(Node<T> *node) {
    while (node->left) {
      node = node->left;
    }

    return node;
  }


  Node<T> *removeMin(Node<T> *node) {
    if (!node->left) {
      return node->right;
    }
    node->left = removeMin(node->left);

    return doBalance(node);
  }

  size_t getHeight(Node<T> *node) {
    return node ? node->height : 0;
  }

  size_t getCount(Node<T> *node) {
    return node ? node->count : 0;
  }

  void fixHeight(Node<T> *node) {
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
  }

  void fixCount(Node<T> *node) {
    node->count = getCount(node->left) + getCount(node->right) + 1;
  }

  int getBalance(Node<T> *node) {
    return getHeight(node->right) - getHeight(node->left);
  }

  Node<T> *doBalance(Node<T> *node) {
    fixHeight(node);
    fixCount(node);

    switch (getBalance(node)) {
      case 2: {
        if (getBalance(node->right) < 0) {
          node->right = rotateRight(node->right);
        }

        return rotateLeft(node);
      }

      case -2: {
        if (getBalance(node->left) > 0) {
          node->left = rotateLeft(node->left);
        }

        return rotateRight(node);
      }

      default: {
        return node;
      }
    }
  }

  Node<T> *rotateLeft(Node<T> *node) {
    Node<T> *tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    fixHeight(node);
    fixHeight(tmp);
    fixCount(node);
    fixCount(tmp);
    return tmp;
  }

  Node<T> *rotateRight(Node<T> *node) {
    Node<T> *tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    fixHeight(node);
    fixHeight(tmp);
    fixCount(node);
    fixCount(tmp);
    return tmp;
  }

 private:
  Node<T> *root;
  Comparator cmp;
};

void run(std::istream &is, std::ostream &os) {
  int n;
  is >> n;
  AvlTree<int> avlTree;

  for (int i = 0; i < n; ++i) {
    int key = 0;
    is >> key;

    if (key > 0) {
      avlTree.Add(key);
    } else {
      avlTree.Delete(std::abs(key));
    }
  }

  avlTree.PostOrder();
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "10" << std::endl;
    input << "+8" << std::endl;
    input << "+1" << std::endl;
    input << "+5" << std::endl;
    input << "+2" << std::endl;
    input << "+4" << std::endl;
    input << "-5" << std::endl;
    input << "+0" << std::endl;
    input << "+5" << std::endl;
    input << "-0" << std::endl;
    input << "+3" << std::endl;

    run(input, output);

    assert(output.str() == "40\n40\n10\n4\n50\n");
  }
}

int main() {
  test();
//  run(std::cin, std::cout);

  return 0;
}