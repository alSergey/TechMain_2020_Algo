#include <string>

struct Person {
  std::string FirstName;
  std::string LastName;
  int ChildCount;
};

struct ComparatorPerson {
  bool operator()(const Person &l, const Person &r) {
    if (l.ChildCount < r.ChildCount) {
      return true;
    }
    if (l.ChildCount > r.ChildCount) {
      return false;
    }

    if (l.LastName < r.LastName) {
      return true;
    }
    if (l.LastName > r.LastName) {
      return false;
    }

    if (l.FirstName < r.FirstName) {
      return true;
    }
    if (l.FirstName > r.FirstName) {
      return false;
    }

    return true;
  }
};

int main() {

  return 0;
}