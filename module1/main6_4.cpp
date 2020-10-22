/* Реализуйте стратегию выбора опорного элемента “случайный элемент”.
 * Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу. */

#include <iostream>
#include <sstream>
#include <cassert>

template<typename Comparator>
int partition(int array[], int l, int r, Comparator cmp = Comparator()) {
  int i = r;
  int pivot = (r + l) / 2;
  std::swap(array[l], array[pivot]);
  pivot = l;

  for (int j = r; j > l; --j) {
    if (cmp(array[j], array[pivot])) {
      std::swap(array[j], array[i]);
      --i;
    }
  }

  std::swap(array[i], array[pivot]);

  return i;
}

template<typename Comparator = std::greater_equal<int>>
int kth_statistic(int array[], int l, int r, int k, Comparator cmp = Comparator()) {
  int pivot = partition(array, l, r, cmp);

  while (pivot != k) {
    if (pivot > k) {
      r = pivot - 1;
    } else {
      l = pivot + 1;
    }

    pivot = partition(array, l, r, cmp);
  }

  return array[pivot];
}

void run(std::istream &is, std::ostream &os) {
  int n, k;
  is >> n >> k;
  auto array = new int[n];
  for (int i = 0; i < n; ++i) {
    is >> array[i];
  }

  os << kth_statistic(array, 0, n - 1, k);

  delete[] array;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "10 4" << std::endl;
    input << "1 2 3 4 5 6 7 8 9 10" << std::endl;

    run(input, output);

    assert(output.str() == "5");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "10 0" << std::endl;
    input << "3 6 5 7 2 9 8 10 4 1" << std::endl;

    run(input, output);

    assert(output.str() == "1");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "10 9" << std::endl;
    input << "0 0 0 0 0 0 0 0 0 1" << std::endl;

    run(input, output);

    assert(output.str() == "1");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "10 4" << std::endl;
    input << "1 2 4 4 4 4 4 8 9 10" << std::endl;

    run(input, output);

    assert(output.str() == "4");
  }
}

int main() {
//  test();
  run(std::cin, std::cout);

  return 0;
}
