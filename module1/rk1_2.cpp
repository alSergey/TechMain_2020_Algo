/* Дан отсортированный по возрастанию массив попарно различных целых чисел A[0..n-1].
 * На вход также подается некоторое целое число k. Необходимо вернуть
 * индекс элемента k в массиве A, если он там присутствует, либо вернуть позицию, куда
 * его следовало бы вставить, чтобы массив остался отсортированным. */

#include <iostream>
#include <cassert>
#include <sstream>

int binarySearch(const int *array, int left, int right, int element) {
  while (left < right) {
    int mid = (left + right) / 2;

    if (array[mid] < element) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }

  return left;
}

void run(std::istream &is, std::ostream &os) {
  int n = 0;
  is >> n;
  int *array = new int[n];

  for (int i = 0; i < n; ++i) {
    is >> array[i];
  }

  int element = 0;
  is >> element;

  os << binarySearch(array, 0, n, element) << std::endl;
  delete[] array;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "4" << std::endl;
    input << "1 3 5 6" << std::endl;
    input << "5" << std::endl;

    run(input, output);

    assert(output.str() == "2\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "4" << std::endl;
    input << "1 3 5 6" << std::endl;
    input << "2" << std::endl;

    run(input, output);

    assert(output.str() == "1\n");
  }
}

int main() {
//  test();
  run(std::cin, std::cout);
  return 0;
}
