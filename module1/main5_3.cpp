/* На числовой прямой окрасили N отрезков.
 * Известны координаты левого и правого концов каждого отрезка (Li и Ri).
 * Найти длину окрашенной части числовой прямой. */

#include <iostream>
#include <sstream>
#include <cassert>

struct Data {
  explicit Data(int x = 0, int count = 0)
      : x(x),
        count(count) {
  }

  int x;
  int count;
};

bool sortComparator(const Data &l, const Data &r) {
  return l.x > r.x;
}

template<typename T, typename Comparator = std::greater<T>>
void merge(T array[], int l, int m, int r, Comparator cmp = Comparator()) {
  int i1 = l;
  int i2 = m;

  T *result = new T[r - l];
  int n = 0;

  while ((i1 < m) && (i2 < r)) {
    if (cmp(array[i1], array[i2])) {
      result[n] = array[i2++];
    } else {
      result[n] = array[i1++];
    }

    n++;
  }

  if (i1 == m) {
    for (int i = i2; i < r; ++i) {
      result[n++] = array[i];
    }
  } else {
    for (int i = i1; i < m; ++i) {
      result[n++] = array[i];
    }
  }

  for (int i = 0; i < r - l; ++i) {
    array[l + i] = result[i];
  }

  delete[] result;
}

template<typename T, typename Comparator = std::greater<T>>
void mergeSort(T array[], int n, Comparator cmp = Comparator()) {
  int step = 1;
  while (step < n) {
    int l = 0;

    while (l < n && l + step < n) {
      int m = l + step;
      int r = l + step * 2 > n ? n : m + step;

      merge(array, l, m, r, cmp);
      l += step * 2;
    }

    step *= 2;
  }
}

void run(std::istream &is, std::ostream &os) {
  int n = 0;
  is >> n;
  n *= 2;
  auto array = new Data[n];
  for (int i = 0; i < n; ++i) {
    is >> array[i].x;
    array[i].count = +1;

    is >> array[++i].x;
    array[i].count = -1;
  }

  mergeSort(array, n, sortComparator);

  int count = 0;
  int j = 0;
  int h = 0;
  for (int i = array[0].x; i <= array[n - 1].x; ++i) {
    if (h > 0) {
      count++;
    }

    while (array[j].x == i) {
      h += array[j].count;

      j++;
    }
  }

  os << count;

  delete[] array;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "1 4" << std::endl;
    input << "7 8" << std::endl;
    input << "2 5" << std::endl;

    run(input, output);

    assert(output.str() == "5");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "4" << std::endl;
    input << "1 7" << std::endl;
    input << "5 9" << std::endl;
    input << "3 7" << std::endl;
    input << "0 7" << std::endl;

    run(input, output);

    assert(output.str() == "9");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "4" << std::endl;
    input << "1 9" << std::endl;
    input << "12 15" << std::endl;
    input << "11 13" << std::endl;
    input << "0 5" << std::endl;

    run(input, output);

    assert(output.str() == "13");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "3" << std::endl;
    input << "1 50" << std::endl;
    input << "7 8" << std::endl;
    input << "2 5" << std::endl;

    run(input, output);

    assert(output.str() == "49");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "5" << std::endl;
    input << "1 2" << std::endl;
    input << "3 4" << std::endl;
    input << "5 6" << std::endl;
    input << "7 8" << std::endl;
    input << "9 10" << std::endl;

    run(input, output);

    assert(output.str() == "5");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "6" << std::endl;
    input << "5 6" << std::endl;
    input << "9 16" << std::endl;
    input << "0 3" << std::endl;
    input << "8 15" << std::endl;
    input << "1 7" << std::endl;
    input << "2 3" << std::endl;

    run(input, output);

    assert(output.str() == "15");
  }
}

int main() {
//  test();
  run(std::cin, std::cout);

  return 0;
}