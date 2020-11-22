/* Порядковые статистики. Дано число N и N строк.
 * Каждая строка содержит команду добавления или удаления
 * натуральных чисел, а также запрос на получение k-ой порядковой статистики.
 * Команда добавления числа A задается положительным числом A,
 * команда удаления числа A задается отрицательным числом “-A”.
 * Запрос на получение k-ой порядковой статистики задается числом k.
 * Требования: скорость выполнения запроса - O(log n).
 * Решение должно поддерживать передачу функции сравнения снаружи. */

#include <iostream>
#include <cassert>
#include <sstream>

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

  T kthStatistic(size_t kth) {
    auto current = root;
    while (true) {
      if (cmp(kth, getNodeCount(current->left))) {
        current = current->left;
      } else if (cmp(getNodeCount(current->left), kth)) {
        kth -= getNodeCount(current->left) + 1;
        current = current->right;
      } else {
        break;
      }

//      if (kth < getNodeCount(current->left)) {
//        current = current->left;
//      } else if (kth == getNodeCount(current->left)) {
//        break;
//      } else {
//        kth -= getNodeCount(current->left) + 1;
//        current = current->right;
//      }
    }

    return current->key;
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

    if (left->height > right->height) {
      tmp = findMax(left);
      tmp->left = removeMax(left);
      tmp->right = right;
    } else {
      tmp = findMin(right);
      tmp->right = removeMin(right);
      tmp->left = left;
    }

    return doBalance(tmp);
  }

  Node<T> *findMin(Node<T> *node) {
    while (node->left) {
      node = node->left;
    }

    return node;
  }

  Node<T> *findMax(Node<T> *node) {
    while (node->right) {
      node = node->right;
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

  Node<T> *removeMax(Node<T> *node) {
    if (!node->right) {
      return node->left;
    }
    node->right = removeMax(node->right);

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
    size_t kth = 0;
    is >> key >> kth;

    if (key > 0) {
      avlTree.Add(key);
    } else {
      avlTree.Delete(std::abs(key));
    }

    os << avlTree.kthStatistic(kth) << std::endl;
  }
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "5" << std::endl;
    input << "40 0" << std::endl;
    input << "10 1" << std::endl;
    input << "4 1" << std::endl;
    input << "-10 0" << std::endl;
    input << "50 2" << std::endl;

    run(input, output);

    assert(output.str() == "40\n40\n10\n4\n50\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "1 0" << std::endl;
    input << "2 0" << std::endl;
    input << "-1 0" << std::endl;

    run(input, output);

    assert(output.str() == "1\n1\n2\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "5" << std::endl;
    input << "1 0" << std::endl;
    input << "2 1" << std::endl;
    input << "3 2" << std::endl;
    input << "4 3" << std::endl;
    input << "5 4" << std::endl;

    run(input, output);

    assert(output.str() == "1\n2\n3\n4\n5\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "5" << std::endl;
    input << "5 0" << std::endl;
    input << "4 1" << std::endl;
    input << "3 2" << std::endl;
    input << "2 3" << std::endl;
    input << "1 0" << std::endl;

    run(input, output);

    assert(output.str() == "5\n5\n5\n5\n1\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "5" << std::endl;
    input << "5 0" << std::endl;
    input << "4 1" << std::endl;
    input << "3 2" << std::endl;
    input << "2 3" << std::endl;
    input << "1 0" << std::endl;

    run(input, output);

    assert(output.str() == "5\n5\n5\n5\n1\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "10" << std::endl;
    input << "5 0" << std::endl;
    input << "2 1" << std::endl;
    input << "-2 0" << std::endl;
    input << "2 1" << std::endl;
    input << "-5 0" << std::endl;
    input << "7 1" << std::endl;
    input << "3 2" << std::endl;
    input << "8 3" << std::endl;
    input << "1 4" << std::endl;
    input << "-2 0" << std::endl;

    run(input, output);

    assert(output.str() == "5\n5\n5\n5\n2\n7\n7\n8\n8\n1\n");
  }
}

int main() {
//  test();
  run(std::cin, std::cout);

  return 0;
}