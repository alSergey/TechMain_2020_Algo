/* Пара слов называется стековой анаграмой, если одно слово
 * можно получить из другого, проведя последовательность стековых
 * операций с его буквами (взять очередную букву исходного слова и поместить ее
 * в стек; взять букву из стека и добавить ее в конец выходного слова).
 * Для заданной пары слов требуется определить, можно ли выполнить последовательность
 * стековых операций, переводящую первое слово во второе. Все буквы в слове различные.
 * Длина анаграммы ≤ 10000. */

#include <iostream>
#include <sstream>
#include <cassert>
#include <string>

#define FIRST_ALLOC_SIZE 5
#define REALLOC_SIZE 2

template<typename T>
class Stack {
 public:
  Stack()
      : head(0), allocSize(FIRST_ALLOC_SIZE), buffer(new T[allocSize]) {
  }

  ~Stack() {
    delete[] buffer;
  }

  void Push(T value) {
    if (allocSize == head) {
      resize();
    }

    buffer[head++] = value;
  }

  T Pop() {
    if (head == 0) {
      return -1;
    }

    return buffer[--head];
  }

  bool isEmpty() const {
    return head == 0;
  }

  Stack(const Stack &other) = delete;
  Stack(Stack &&other) = delete;
  Stack &operator=(const Stack &other) = delete;
  Stack &operator=(Stack &&other) = delete;

 private:
  int head;
  int allocSize;
  T *buffer;

  void resize() {
    allocSize *= REALLOC_SIZE;
    auto *newBuffer = new T[allocSize];

    for (int i = 0; i < head; i++) {
      newBuffer[i] = buffer[i];
    }

    delete[] buffer;
    buffer = newBuffer;
  }
};

void run(std::istream &is, std::ostream &os) {
  std::string input;
  std::string output;

  is >> input;
  is >> output;

  Stack<char> stack;
  size_t i = 0;
  size_t j = 0;

  while (i < input.size()) {
    if (input[i] == output[j]) {
      i++;
      j++;
    } else {
      if (stack.isEmpty()) {
        stack.Push(input[i++]);
      } else {
        char c = stack.Pop();
        if (output[j] == c) {
          j++;
        } else {
          stack.Push(c);
          stack.Push(input[i++]);
        }
      }
    }
  }

  for (; j < output.size(); ++j) {
    if (output[j] != stack.Pop()) {
      os << "NO\n";
      return;
    }
  }

  os << "YES\n";
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "STOL SLOT" << std::endl;

    run(input, output);

    assert(output.str() == "YES\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "ABC CAB" << std::endl;

    run(input, output);

    assert(output.str() == "NO\n");
  }
}

int main() {
//  test();
  run(std::cin, std::cout);

  return 0;
}