#include <iostream>

/* Вычислить площадь выпуклого n-угольника, заданного координатами своих вершин.
 * Вначале вводится количество вершин, затем последовательно целочисленные координаты
 * всех вершин в порядке обхода против часовой стрелки. n < 1000, координаты < 10000.
 * Указание. Для вычисления площади n-угольника можно посчитать сумму ориентированных
 * площадей трапеций под каждой стороной многоугольника.
 * Требования: Координата должна быть реализована в виде структуры. */


struct Point {
  explicit Point(int x = 0, int y = 0)
      : x(x), y(y) {
  }
  int x;
  int y;
};

void run(std::istream &is, std::ostream &os) {
  int count;
  is >> count;
  auto *pointsArray = new Point[count];
  for (int i = 0; i < count; ++i) {
    is >> pointsArray[i].x >> pointsArray[i].y;
  }

  double square = 0;
  for (int i = 0; i < count; ++i) {
    int j = (i + 1) % count;
    square += (pointsArray[i].y + pointsArray[j].y) / 2.0 * (pointsArray[i].x - pointsArray[j].x);
  }

  os << std::abs(square);

  delete[] pointsArray;
}

int main() {
  run(std::cin, std::cout);

  return 0;
}
