/* Жадина.
 * Вовочка ест фрукты из бабушкиной корзины. В корзине лежат фрукты разной массы.
 * Вовочка может поднять не более K грамм. Каждый фрукт весит не более K грамм.
 * За раз он выбирает несколько самых тяжелых фруктов, которые может поднять одновременно,
 * откусывает от каждого половину и кладет огрызки обратно в корзину.
 * Если фрукт весит нечетное число грамм, он откусывает большую половину. Фрукт массы 1гр он съедает полностью.
 * Определить за сколько подходов Вовочка съест все фрукты в корзине.
 * Формат входных данных. Вначале вводится n - количество фруктов и n строк с массами фруктов. Затем K - "грузоподъемность".
 * Формат выходных данных. Неотрицательное число - количество подходов к корзине. */

#define FIRST_ALLOC_SIZE 5
#define REALLOC_SIZE 2

#include <iostream>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <cstring>

template<class T, typename Comparator = std::greater<T>>
class Heap {
 public:
  Heap()
      : bufferSize(0),
        allocSize(FIRST_ALLOC_SIZE),
        buffer(new T[allocSize]) {
  }

  explicit Heap(const T array[], int size)
      : bufferSize(size),
        allocSize(size * REALLOC_SIZE),
        buffer(new T[allocSize]) {
    std::memcpy(buffer, array, bufferSize * sizeof(T));
    build();
  }

  ~Heap() {
    delete[] buffer;
  }

  bool isEmpty() {
    return bufferSize == 0;
  }

  void insert(T val) {
    if (allocSize == bufferSize) {
      resize();
    }

    buffer[bufferSize] = val;
    siftUp(bufferSize);
    bufferSize++;
  }

  T getTop() {
    if (bufferSize == 0) {
      return -1;
    }

    return buffer[0];
  }

  T extractTop() {
    if (bufferSize == 0) {
      return -1;
    }

    T result = buffer[0];
    buffer[0] = buffer[bufferSize - 1];
    siftDown(0);
    bufferSize--;

    return result;
  }

  Heap(const Heap &other) = delete;
  Heap(Heap &&other) = delete;
  Heap &operator=(const Heap &other) = delete;
  Heap &operator=(Heap &&other) = delete;

 private:
  int bufferSize;
  int allocSize;
  T *buffer;
  Comparator cmp;

  void build() {
    for (int i = bufferSize / 2 - 1; i >= 0; --i) {
      siftDown(i);
    }
  }

  void resize() {
    allocSize *= REALLOC_SIZE;
    auto *newBuffer = new T[allocSize];

    std::memcpy(newBuffer, buffer, bufferSize * sizeof(T));
    delete[] buffer;
    buffer = newBuffer;
  }

  void siftDown(int index) {
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    if (leftChild < bufferSize && cmp(buffer[leftChild], buffer[index])) {
      std::swap(buffer[leftChild], buffer[index]);
      siftDown(leftChild);
    }

    if (rightChild < bufferSize && cmp(buffer[rightChild], buffer[index])) {
      std::swap(buffer[rightChild], buffer[index]);
      siftDown(rightChild);
    }
  }

  void siftUp(int index) {
    int parent = (index - 1) / 2;

    if (parent >= 0 && cmp(buffer[index], buffer[parent])) {
      std::swap(buffer[parent], buffer[index]);
      siftUp(parent);
    }
  }
};

void run(std::istream &is, std::ostream &os) {
  int n = 0;
  is >> n;

  Heap<int> heap;
  for (int i = 0; i < n; ++i) {
    int a;
    is >> a;
    heap.insert(a);
  }

  int k;
  is >> k;
  auto buffer = new int[k];

  int steps = 0;
  while (!heap.isEmpty()) {
    int i = 0;
    int weight = 0;
    while (weight + heap.getTop() <= k && heap.getTop() != -1) {
      buffer[i] = heap.extractTop();
      weight += buffer[i];
      i++;
    }

    for (int j = 0; j < i; ++j) {
      if (buffer[j] != 1) {
        heap.insert(buffer[j] / 2);
      }
    }

    steps++;
  }

  os << steps;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "1 2 2" << std::endl;
    input << "2" << std::endl;

    run(input, output);

    assert(output.str() == "4");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "4 3 5" << std::endl;
    input << "6" << std::endl;

    run(input, output);

    assert(output.str() == "5");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "7" << std::endl;
    input << "1 1 1 1 1 1 1" << std::endl;
    input << "3" << std::endl;

    run(input, output);

    assert(output.str() == "3");
  }
}

int main() {
//  test();
  run(std::cin, std::cout);

  return 0;
}
