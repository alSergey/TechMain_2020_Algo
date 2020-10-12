/* Реализовать очередь с динамическим зацикленным буфером.
Требования: Очередь должна быть реализована в виде класса. */

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstring>

#define FIRST_ALLOC_SIZE 5
#define REALLOC_SIZE 2

template<typename T>
class Queue {
 public:
  Queue()
      : head(0), tail(0), bufferSize(0), allocSize(FIRST_ALLOC_SIZE), buffer(new T[allocSize]) {
  }

  ~Queue() {
    delete[] buffer;
  }

  void Push(T value) {
    if (allocSize == bufferSize) {
      resize();
    }

    if (bufferSize != 0) {
      tail = (tail + 1) % allocSize;
    }

    buffer[tail] = value;
    bufferSize++;
  }

  T Pop() {
    if (isEmpty()) {
      return -1;
    }

    T res = buffer[head];

    if (bufferSize != 1) {
      head = (head + 1) % allocSize;
    }

    --bufferSize;
    return res;
  }

  Queue(const Queue &other) = delete;
  Queue(Queue &&other) = delete;
  Queue &operator=(const Queue &other) = delete;
  Queue &operator=(Queue &&other) = delete;

 private:
  int head;
  int tail;
  int bufferSize;
  int allocSize;
  T *buffer;

  bool isEmpty() const {
    return bufferSize == 0;
  }

  void resize() {
    allocSize *= REALLOC_SIZE;
    auto *newBuffer = new T[allocSize];

    for (int i = 0; i < bufferSize; i++) {
      newBuffer[i] = buffer[(head + i) % bufferSize];
    }

    head = 0;
    tail = bufferSize - 1;
    delete[] buffer;
    buffer = newBuffer;
  }
};

void run(std::istream &is, std::ostream &os) {
  int n = 0;
  is >> n;
  Queue<int> queue;

  for (int i = 0; i < n; ++i) {
    int command, value;
    is >> command >> value;

    switch (command) {
      case 2: {
        if (queue.Pop() != value) {
          os << "NO" << std::endl;
          return;
        }
        break;
      }

      case 3: {
        queue.Push(value);
        break;
      }

      default: {
        os << "NO" << std::endl;
        return;
      }
    }
  }

  os << "YES" << std::endl;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "3 44" << std::endl;
    input << "3 50" << std::endl;
    input << "2 44" << std::endl;

    run(input, output);

    assert(output.str() == "YES\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "2" << std::endl;
    input << "2 -1" << std::endl;
    input << "3 10" << std::endl;

    run(input, output);

    assert(output.str() == "YES\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "2" << std::endl;
    input << "3 44" << std::endl;
    input << "2 66" << std::endl;

    run(input, output);

    assert(output.str() == "NO\n");
  }
}

int main() {
//  test();
  run(std::cin, std::cout);

  return 0;
}
