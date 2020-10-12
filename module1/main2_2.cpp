#include <iostream>
#include <cassert>
#include <sstream>

/* Дан массив целых чисел А[0..n-1]. Известно, что на интервале [0, m]
 * значения массива строго возрастают, а на интервале [m, n-1] строго убывают.
 * Найти m за O(log m).
 * Требования:  Время работы O(log m). Внимание! В этой задаче сначала нужно
 * определить диапазон для бинарного поиска размером порядка m, а потом уже в нем делать бинарный поиск. */

int binarySearch(const int *array, int left, int right) {
  while (left < right) {
    int mid = (left + right) / 2;

    if (array[mid] > array[mid + 1]) {
      right = mid;
    } else {
      left = mid + 1;
    }
  }

  return left;
}

int exponentialSearch(const int *array, int n) {
  int i = 1;
  while (i < n && array[i - 1] < array[i]) {
    i *= 2;
  }

  return binarySearch(array, i / 2, std::min(i, n - 1));
}

void run(std::istream &is, std::ostream &os) {
  int count = 0;
  is >> count;
  int *array = new int[count];

  for (int i = 0; i < count; ++i) {
    is >> array[i];
  }

  os << exponentialSearch(array, count) << std::endl;
  delete[] array;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "10" << std::endl;
    input << "1 2 3 4 5 6 7 6 5 4" << std::endl;

    run(input, output);

    assert(output.str() == "6\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "8" << std::endl;
    input << "0 1 2 3 4 5 4 2" << std::endl;

    run(input, output);

    assert(output.str() == "5\n");
  }
}

int main() {
//  test();
  run(std::cin, std::cout);
  return 0;
}
