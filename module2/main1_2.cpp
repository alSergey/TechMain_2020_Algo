/* Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы
 * с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
 * Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
 * Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов
 * в случае, когда коэффициент заполнения таблицы достигает 3/4.
 * Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества
 * и проверки принадлежности данной строки множеству.
 * Для разрешения коллизий используйте двойное хеширование. */

#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>

const size_t DEFAULT_SIZE = 8;
const size_t RESIZE_SIZE = 2;
const std::string EMPTY_SLOT = "NIL";
const std::string DELETE_SLOT = "DEL";

class StringHasher {
 public:
  size_t operator()(const std::string &str, unsigned int prime = 42) const {
    size_t hash = 0;
    for (char i : str) {
      hash = hash * prime + i;
    }

    return hash;
  }
};

class HashTable {
 public:
  explicit HashTable(size_t initialSize = DEFAULT_SIZE)
      : size(0),
        table(initialSize, EMPTY_SLOT) {
  }

  bool Add(const std::string &key) {
    if (4 * size >= 3 * table.size()) {
      grow();
    }

    size_t index = 0;
    size_t pos = prob(key, index, table.size());
    size_t firstDel = table.size();

    while (table[pos] != EMPTY_SLOT && index < table.size()) {
      if (table[pos] == key) {
        return false;
      }

      if (table[pos] == DELETE_SLOT && firstDel == table.size()) {
        firstDel = pos;
      }

      pos = prob(key, ++index, table.size());
    }

    table[firstDel == table.size() ? pos : firstDel] = key;
    ++size;

    return true;
  }

  bool Has(const std::string &key) {
    size_t index = 0;
    size_t pos = prob(key, index, table.size());

    while (table[pos] != EMPTY_SLOT && index < table.size()) {
      if (table[pos] == key) {
        return true;
      }

      pos = prob(key, ++index, table.size());
    }

    return false;
  }

  bool Delete(const std::string &key) {
    size_t index = 0;
    size_t pos = prob(key, index, table.size());

    while (table[pos] != EMPTY_SLOT && index < table.size()) {
      if (table[pos] == key) {
        table[pos] = DELETE_SLOT;
        --size;
        return true;
      }

      pos = prob(key, ++index, table.size());
    }

    return false;
  }

 private:
  void grow() {
    std::vector<std::string> tmpTable(table.size() * RESIZE_SIZE, EMPTY_SLOT);
    for (const auto &key: table) {
      if (key == EMPTY_SLOT || key == DELETE_SLOT) {
        continue;
      }

      size_t index = 0;
      size_t pos = prob(key, index, tmpTable.size());
      while (tmpTable[pos] != EMPTY_SLOT) {
        pos = prob(key, ++index, tmpTable.size());
      }

      tmpTable[pos] = key;
    }

    table = std::move(tmpTable);
  }

  size_t prob(const std::string &key, size_t i, size_t m) {
    return (hasher(key) + i * (2 * hasher(key, 15) + 1)) % m;
  }

  StringHasher hasher;
  size_t size;
  std::vector<std::string> table;
};

void run(std::istream &is, std::ostream &os) {
  HashTable hashTable;

  char op;
  std::string key;
  while (is >> op >> key) {
    switch (op) {
      case '?': {
        os << (hashTable.Has(key) ? "OK" : "FAIL") << std::endl;
        break;
      }

      case '+': {
        os << (hashTable.Add(key) ? "OK" : "FAIL") << std::endl;
        break;
      }

      case '-': {
        os << (hashTable.Delete(key) ? "OK" : "FAIL") << std::endl;
        break;
      }

      default: {
        os << "FAIL" << std::endl;
        break;
      }
    }
  }
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "+ hello" << std::endl;
    input << "+ bye" << std::endl;
    input << "? bye" << std::endl;
    input << "+ bye" << std::endl;
    input << "- bye" << std::endl;
    input << "? bye" << std::endl;
    input << "? hello" << std::endl;

    run(input, output);

    assert(output.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "+ hello" << std::endl;
    input << "+ bye" << std::endl;
    input << "+ bye" << std::endl;
    input << "+ bye" << std::endl;
    input << "- bye" << std::endl;
    input << "- bye" << std::endl;
    input << "- hello" << std::endl;

    run(input, output);

    assert(output.str() == "OK\nOK\nFAIL\nFAIL\nOK\nFAIL\nOK\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "+ hello" << std::endl;
    input << "+ bye" << std::endl;
    input << "+ bye" << std::endl;
    input << "+ bye" << std::endl;
    input << "+ bye" << std::endl;
    input << "? bye" << std::endl;
    input << "? hello" << std::endl;

    run(input, output);

    assert(output.str() == "OK\nOK\nFAIL\nFAIL\nFAIL\nOK\nOK\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "- hello" << std::endl;
    input << "- bye" << std::endl;
    input << "+ bye" << std::endl;
    input << "+ hello" << std::endl;
    input << "- bye" << std::endl;
    input << "? bye" << std::endl;
    input << "? hello" << std::endl;

    run(input, output);

    assert(output.str() == "FAIL\nFAIL\nOK\nOK\nOK\nFAIL\nOK\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "- hello" << std::endl;
    input << "- bye" << std::endl;
    input << "+ bye" << std::endl;
    input << "+ hello" << std::endl;
    input << "- bye" << std::endl;
    input << "? bye" << std::endl;
    input << "? hello" << std::endl;

    run(input, output);

    assert(output.str() == "FAIL\nFAIL\nOK\nOK\nOK\nFAIL\nOK\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "- hello" << std::endl;
    input << "- hello" << std::endl;
    input << "- hello" << std::endl;
    input << "- hello" << std::endl;
    input << "- hello" << std::endl;
    input << "- hello" << std::endl;
    input << "- hello" << std::endl;

    run(input, output);

    assert(output.str() == "FAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "+ hello" << std::endl;
    input << "+ hello" << std::endl;
    input << "+ hello" << std::endl;
    input << "+ hello" << std::endl;
    input << "+ hello" << std::endl;
    input << "+ hello" << std::endl;
    input << "+ hello" << std::endl;

    run(input, output);

    assert(output.str() == "OK\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "+ hello" << std::endl;
    input << "- hello" << std::endl;
    input << "+ hello" << std::endl;
    input << "- hello" << std::endl;
    input << "+ hello" << std::endl;
    input << "- hello" << std::endl;
    input << "? hello" << std::endl;

    run(input, output);

    assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nFAIL\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "+ hello" << std::endl;
    input << "? hello" << std::endl;
    input << "+ hello" << std::endl;
    input << "? hello" << std::endl;
    input << "+ hello" << std::endl;
    input << "? hello" << std::endl;
    input << "? hello" << std::endl;

    run(input, output);

    assert(output.str() == "OK\nOK\nFAIL\nOK\nFAIL\nOK\nOK\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "+ hello" << std::endl;
    input << "+ hell" << std::endl;
    input << "+ hel" << std::endl;
    input << "+ he" << std::endl;
    input << "+ h" << std::endl;
    input << "+ bye" << std::endl;
    input << "+ no" << std::endl;
    input << "? no" << std::endl;
    input << "? he" << std::endl;
    input << "? good" << std::endl;
    input << "? hel" << std::endl;
    input << "- he" << std::endl;
    input << "- no" << std::endl;
    input << "- good" << std::endl;

    run(input, output);

    assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\nOK\nOK\nOK\nFAIL\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "+ hello" << std::endl;
    input << "- hell" << std::endl;
    input << "? hel" << std::endl;
    input << "+ he" << std::endl;
    input << "- h" << std::endl;
    input << "? bye" << std::endl;
    input << "? no" << std::endl;
    input << "- no" << std::endl;
    input << "- he" << std::endl;
    input << "+ good" << std::endl;
    input << "- hel" << std::endl;
    input << "+ he" << std::endl;
    input << "? no" << std::endl;
    input << "? good" << std::endl;

    run(input, output);

    assert(output.str() == "OK\nFAIL\nFAIL\nOK\nFAIL\nFAIL\nFAIL\nFAIL\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "+ hello" << std::endl;
    input << "+ hell" << std::endl;
    input << "+ hel" << std::endl;
    input << "+ he" << std::endl;
    input << "+ h" << std::endl;
    input << "+ bye" << std::endl;
    input << "+ no" << std::endl;
    input << "- no" << std::endl;
    input << "- he" << std::endl;
    input << "- good" << std::endl;
    input << "- hel" << std::endl;
    input << "- he" << std::endl;
    input << "- no" << std::endl;
    input << "- good" << std::endl;

    run(input, output);

    assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\nOK\nFAIL\nFAIL\nFAIL\n");
  }
  {
    std::stringstream input;
    std::stringstream output;

    input << "+ a" << std::endl;
    input << "+ b" << std::endl;
    input << "+ c" << std::endl;
    input << "+ d" << std::endl;
    input << "+ e" << std::endl;
    input << "+ f" << std::endl;
    input << "+ g" << std::endl;
    input << "+ h" << std::endl;
    input << "+ n" << std::endl;
    input << "+ q" << std::endl;
    input << "+ w" << std::endl;
    input << "+ p" << std::endl;
    input << "+ k" << std::endl;
    input << "+ r" << std::endl;
    input << "- r" << std::endl;
    input << "+ r" << std::endl;
    input << "- r" << std::endl;
    input << "+ r" << std::endl;
    input << "- r" << std::endl;
    input << "+ r" << std::endl;
    input << "- r" << std::endl;
    input << "+ r" << std::endl;
    input << "- r" << std::endl;
    input << "+ r" << std::endl;
    input << "- r" << std::endl;
    input << "? r" << std::endl;
    input << "- r" << std::endl;
    input << "+ r" << std::endl;
    input << "? r" << std::endl;
    input << "+ r" << std::endl;
    input << "- r" << std::endl;
    input << "? r" << std::endl;
    input << "- r" << std::endl;
    input << "? r" << std::endl;
    input << "- r" << std::endl;
    input << "+ r" << std::endl;
    input << "? r" << std::endl;
    input << "- r" << std::endl;
    input << "? r" << std::endl;
    input << "? a" << std::endl;
    input << "? b" << std::endl;
    input << "? c" << std::endl;
    input << "? d" << std::endl;
    input << "? e" << std::endl;
    input << "? f" << std::endl;
    input << "? g" << std::endl;
    input << "? h" << std::endl;
    input << "? n" << std::endl;
    input << "? q" << std::endl;
    input << "? w" << std::endl;
    input << "? p" << std::endl;
    input << "? k" << std::endl;
    input << "- a" << std::endl;
    input << "- b" << std::endl;
    input << "- c" << std::endl;
    input << "- d" << std::endl;
    input << "- e" << std::endl;
    input << "- f" << std::endl;
    input << "- g" << std::endl;
    input << "- h" << std::endl;
    input << "- n" << std::endl;
    input << "- q" << std::endl;
    input << "- w" << std::endl;
    input << "- p" << std::endl;
    input << "- k" << std::endl;

    run(input, output);

    assert(output.str()
               == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\nFAIL\nOK\nOK\nFAIL\nOK\nFAIL\nFAIL\nFAIL\nFAIL\nOK\nOK\nOK\nFAIL\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\n");
  }
}

int main() {
  test();
//  run(std::cin, std::cout);

  return 0;
}
