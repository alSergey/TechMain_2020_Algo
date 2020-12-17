/* Дан невзвешенный неориентированный граф.
 * В графе может быть несколько кратчайших путей между какими-то вершинами.
 * Найдите количество различных кратчайших путей между заданными вершинами.
 * Требования: сложность O(V+E).
 * Формат ввода.
 * v: кол-во вершин (макс. 50000),
 * n: кол-во ребер (макс. 200000),
 * n пар реберных вершин, пара вершин u, w для запроса.
 * Формат вывода.
 * Количество кратчайших путей от u к w. */

#include <iostream>
#include <sstream>
#include <cassert>
#include <vector>
#include <queue>
#include <functional>

class IGraph {
 public:
  virtual ~IGraph() = default;

  virtual void AddEdge(int from, int to, bool isUnDirected) = 0;
  virtual int VerticesCount() const = 0;

  virtual std::vector<int> GetNextVertices(int vertex) const = 0;
  virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
 public:
  explicit ListGraph(int size)
      : adjacencyLists(size) {
  }
  ~ListGraph() override = default;

  explicit ListGraph(const IGraph &graph)
      : adjacencyLists(graph.VerticesCount()) {
    for (int i = 0; i < graph.VerticesCount(); i++) {
      adjacencyLists[i] = graph.GetNextVertices(i);
    }
  }

  void AddEdge(int from, int to, bool isUnDirected = false) override {
    assert(from >= 0 && from < adjacencyLists.size());
    assert(to >= 0 && to < adjacencyLists.size());

    adjacencyLists[from].push_back(to);

    if (isUnDirected) {
      AddEdge(to, from);
    }
  }

  int VerticesCount() const override {
    return (int) adjacencyLists.size();
  }

  std::vector<int> GetNextVertices(int vertex) const override {
    assert(vertex >= 0 && vertex < adjacencyLists.size());

    return adjacencyLists[vertex];
  }

  std::vector<int> GetPrevVertices(int vertex) const override {
    assert(vertex >= 0 && vertex < adjacencyLists.size());

    std::vector<int> prevVertices;
    for (int from = 0; from < adjacencyLists.size(); from++) {
      for (int to: adjacencyLists[from]) {
        if (to == vertex)
          prevVertices.push_back(from);
      }
    }

    return prevVertices;
  }

 private:
  std::vector<std::vector<int>> adjacencyLists;
};

int shortestPath(const IGraph &graph, int from, int to) {
  std::vector<int> r(graph.VerticesCount(), std::numeric_limits<int>::max());

  std::queue<int> queue;
  queue.push(from);
  r[from] = 0;

  std::vector<int> count(graph.VerticesCount(), 0);
  count[from] = 1;

  while (!queue.empty()) {
    int v = queue.front();
    queue.pop();

    for (auto w : graph.GetNextVertices(v)) {
      if (r[v] + 1 == r[w]) {
        count[w] += count[v];
      }

      if (r[w] > r[v] + 1) {
        r[w] = r[v] + 1;
        count[w] = count[v];
        queue.push(w);
      }
    }
  }

  return count[to];
}

void run(std::istream &is, std::ostream &os) {
  int v;
  is >> v;
  ListGraph graph(v);

  int n;
  is >> n;
  for (int i = 0; i < n; i++) {
    int vertex1, vertex2;
    is >> vertex1 >> vertex2;
    graph.AddEdge(vertex1, vertex2, true);
  }

  int from, to;
  is >> from >> to;
  os << shortestPath(graph, from, to) << '\n';
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "4" << std::endl;
    input << "5" << std::endl;

    input << "0 1" << std::endl;
    input << "0 2" << std::endl;
    input << "1 2" << std::endl;
    input << "1 3" << std::endl;
    input << "2 3" << std::endl;

    input << "0 3" << std::endl;

    run(input, output);

    assert(output.str() == "2\n");
  }
}

int main() {
  test();
//  run(std::cin, std::cout);

  return 0;
}