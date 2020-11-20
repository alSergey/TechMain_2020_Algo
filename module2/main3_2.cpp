/* Дано число N < 106 и последовательность пар целых чисел из [-231..231] длиной N.
 * Построить декартово дерево из N узлов, характеризующихся парами чисел {Xi, Yi}.
 * Каждая пара чисел {Xi, Yi} определяет ключ Xi и приоритет Yi в декартовом дереве.
 * Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного на лекции:
 * При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
 * Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x,
 * а во втором больше или равны x. Получившиеся два дерева сделайте дочерними для нового узла (x, y).
 * Новый узел вставьте на место узла P.
 * Построить также наивное дерево поиска по ключам Xi методом из задачи 2.
 * Вычислить количество узлов в самом широком слое декартового дерева и количество узлов
 * в самом широком слое наивного дерева поиска. Вывести их разницу.
 * Разница может быть отрицательна. */

#include <iostream>
#include <cassert>
#include <sstream>
#include <stack>
#include <queue>

template<typename T>
struct Node {
  explicit Node(T val, int priority = 0)
      : key(val),
        priority(priority),
        left(nullptr),
        right(nullptr) {
  }

  T key;
  int priority;
  Node *left;
  Node *right;
};

template<typename T>
void destructor(Node<T> *root) {
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
    delete[] node;
  }
}

template<typename T>
int levelOrder(Node<T> *root) {
  if (!root) {
    return 0;
  }

  std::queue<Node<T> *> queue;
  queue.push(root);

  int maxLeverSize = 0;
  int currentLevelSize = 1;

  while (!queue.empty()) {
    auto node = queue.front();
    queue.pop();

    if (node->left) {
      queue.push(node->left);
    }

    if (node->right) {
      queue.push(node->right);
    }

    if (--currentLevelSize == 0) {
      currentLevelSize = queue.size();
      if (currentLevelSize > maxLeverSize) {
        maxLeverSize = currentLevelSize;
      }
    }
  }

  return maxLeverSize;
}

template<typename T, typename Comparator = std::less<T>>
class Treap {
 public:
  explicit Treap()
      : root(nullptr) {
  }

  ~Treap() {
    destructor(root);
  }

  void Add(T &val, int priority) {
    if (!root) {
      root = new Node<T>(val, priority);
      return;
    }

    auto current = root;
    Node<T> *last = nullptr;
    while (current && cmp(priority, current->priority)) {
      last = current;
      current = cmp(val, current->key) ? current->left : current->right;
    }

    auto newNode = new Node<T>(val, priority);
    split(current, val, newNode->left, newNode->right);

    if (!last) {
      root = newNode;
      return;
    }

    if (cmp(val, last->key)) {
      last->left = newNode;
    } else {
      last->right = newNode;
    }
  }

  Node<T> *getRoot() {
    return root;
  }

 private:
  void split(Node<T> *currentNode, int key, Node<T> *&left, Node<T> *&right) {
    if (!currentNode) {
      left = nullptr;
      right = nullptr;

    } else if (currentNode->key <= key) {
      split(currentNode->right, key, currentNode->right, right);
      left = currentNode;

    } else {
      split(currentNode->left, key, left, currentNode->left);
      right = currentNode;
    }
  }

  Comparator cmp;
  Node<T> *root;
};

template<typename T, typename Comparator = std::less<T>>
class BinaryTree {
 public:
  explicit BinaryTree()
      : root(nullptr) {
  }

  ~BinaryTree() {
    destructor(root);
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

  Node<T> *getRoot() {
    return root;
  }

 private:
  Comparator cmp;
  Node<T> *root;
};

void run(std::istream &is, std::ostream &os) {
  size_t n;
  is >> n;
  BinaryTree<int> binaryTree;
  Treap<int> treap;
  for (size_t i = 0; i < n; ++i) {
    int val;
    int priority;
    is >> val >> priority;
    binaryTree.Add(val);
    treap.Add(val, priority);
  }

  os << levelOrder(treap.getRoot()) - levelOrder(binaryTree.getRoot()) << std::endl;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "10" << std::endl;
    input << "5 11" << std::endl;
    input << "18 8" << std::endl;
    input << "25 7" << std::endl;
    input << "50 12" << std::endl;
    input << "30 30" << std::endl;
    input << "15 15" << std::endl;
    input << "20 10" << std::endl;
    input << "22 5" << std::endl;
    input << "40 20" << std::endl;
    input << "45 9" << std::endl;

    run(input, output);

    assert(output.str() == "1\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "10" << std::endl;
    input << "38 19" << std::endl;
    input << "37 5" << std::endl;
    input << "47 15" << std::endl;
    input << "35 0" << std::endl;
    input << "12 3" << std::endl;
    input << "0 42" << std::endl;
    input << "31 37" << std::endl;
    input << "21 45" << std::endl;
    input << "30 26" << std::endl;
    input << "41 6" << std::endl;

    run(input, output);

    assert(output.str() == "1\n");
  }
}

int main() {
  test();
//  run(std::cin, std::cout);

  return 0;
}