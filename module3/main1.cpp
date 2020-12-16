#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <queue>
#include <deque>
#include <unordered_set>

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

class MatrixGraph : public IGraph {
 public:
  explicit MatrixGraph(int size)
      : adjacencyMatrix(size, std::vector<int>(size, 0)) {
  }
  ~MatrixGraph() override = default;

  explicit MatrixGraph(const IGraph &graph)
      : adjacencyMatrix(graph.VerticesCount(), std::vector<int>(graph.VerticesCount(), 0)) {

    for (int i = 0; i < graph.VerticesCount(); i++) {
      for (auto &j : graph.GetNextVertices(i)) {
        adjacencyMatrix[i][j] = 1;
      }
    }
  }

  void AddEdge(int from, int to, bool isUnDirected = false) override {
    assert(from >= 0 && from < adjacencyMatrix.size());
    assert(to >= 0 && to < adjacencyMatrix.size());

    adjacencyMatrix[from][to] = 1;

    if (isUnDirected) {
      AddEdge(to, from);
    }
  }

  int VerticesCount() const override {
    return (int) adjacencyMatrix.size();
  }

  std::vector<int> GetNextVertices(int vertex) const override {
    assert(vertex >= 0 && vertex < adjacencyMatrix.size());

    std::vector<int> nextVertices;
    for (int i = 0; i < adjacencyMatrix.size(); ++i) {
      if (adjacencyMatrix[vertex][i] != 0) {
        nextVertices.push_back(i);
      }
    }

    return nextVertices;
  }

  std::vector<int> GetPrevVertices(int vertex) const override {
    assert(vertex >= 0 && vertex < adjacencyMatrix.size());

    std::vector<int> prevVertices;
    for (int i = 0; i < adjacencyMatrix.size(); ++i) {
      if (adjacencyMatrix[i][vertex] == 1) {
        prevVertices.push_back(i);
      }
    }

    return prevVertices;
  }

 private:
  std::vector<std::vector<int>> adjacencyMatrix;
};

class SetGraph : public IGraph {
 public:
  explicit SetGraph(int size)
      : adjacencySet(size) {
  }
  ~SetGraph() override = default;

  explicit SetGraph(const IGraph &graph)
      : adjacencySet(graph.VerticesCount()) {

    for (int i = 0; i < graph.VerticesCount(); i++) {
      for (auto &j : graph.GetNextVertices(i)) {
        adjacencySet[i].insert(j);
      }
    }
  }

  void AddEdge(int from, int to, bool isUnDirected = false) override {
    assert(from >= 0 && from < adjacencySet.size());
    assert(to >= 0 && to < adjacencySet.size());

    adjacencySet[from].insert(to);

    if (isUnDirected) {
      AddEdge(to, from);
    }
  }

  int VerticesCount() const override {
    return (int) adjacencySet.size();
  }

  std::vector<int> GetNextVertices(int vertex) const override {
    assert(vertex >= 0 && vertex < adjacencySet.size());

    std::vector<int> nextVertices;
    for (auto &v: adjacencySet[vertex]) {
      nextVertices.push_back(v);
    }

    return nextVertices;
  }

  std::vector<int> GetPrevVertices(int vertex) const override {
    assert(vertex >= 0 && vertex < adjacencySet.size());

    std::vector<int> prevVertices;
    for (int i = 0; i < adjacencySet.size(); ++i) {
      if (adjacencySet[i].find(vertex) != adjacencySet[i].end()) {
        prevVertices.push_back(i);
      }
    }

    return prevVertices;
  }

 private:
  std::vector<std::unordered_set<int>> adjacencySet;
};

class ArcGraph : public IGraph {
 public:
  explicit ArcGraph(int size)
      : size(size) {
  }
  ~ArcGraph() override = default;

  explicit ArcGraph(const IGraph &graph)
      : size(graph.VerticesCount()) {

    for (int i = 0; i < graph.VerticesCount(); i++) {
      for (auto &j : graph.GetNextVertices(i)) {
        adjacencyPair.emplace_back(i, j);
      }
    }
  }

  void AddEdge(int from, int to, bool isUnDirected = false) override {
    assert(from >= 0 && from < size);
    assert(to >= 0 && to < size);

    adjacencyPair.emplace_back(from, to);

    if (isUnDirected) {
      AddEdge(to, from);
    }
  }

  int VerticesCount() const override {
    return size;
  }

  std::vector<int> GetNextVertices(int vertex) const override {
    assert(vertex >= 0 && vertex < size);

    std::vector<int> nextVertices;
    for (auto &v: adjacencyPair) {
      if (v.first == vertex) {
        nextVertices.push_back(v.second);
      }
    }

    return nextVertices;
  }

  std::vector<int> GetPrevVertices(int vertex) const override {
    assert(vertex >= 0 && vertex < size);

    std::vector<int> prevVertices;
    for (auto &v: adjacencyPair) {
      if (v.second == vertex) {
        prevVertices.push_back(v.first);
      }
    }

    return prevVertices;
  }

 private:
  int size;
  std::vector<std::pair<int, int>> adjacencyPair;
};

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func) {
  std::queue<int> qu;
  qu.push(vertex);
  visited[vertex] = true;

  while (!qu.empty()) {
    int currentVertex = qu.front();
    qu.pop();

    func(currentVertex);

    for (int nextVertex: graph.GetNextVertices(currentVertex)) {
      if (!visited[nextVertex]) {
        visited[nextVertex] = true;
        qu.push(nextVertex);
      }
    }
  }
}

void mainBFS(const IGraph &graph, const std::function<void(int)> &func) {
  std::vector<bool> visited(graph.VerticesCount(), false);
  for (int i = 0; i < graph.VerticesCount(); i++) {
    if (!visited[i])
      BFS(graph, i, visited, func);
  }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func) {
  visited[vertex] = true;
  func(vertex);

  for (int nextVertex: graph.GetNextVertices(vertex)) {
    if (!visited[nextVertex]) {
      DFS(graph, nextVertex, visited, func);
    }
  }
}

void mainDFS(const IGraph &graph, const std::function<void(int)> &func) {
  std::vector<bool> visited(graph.VerticesCount(), false);
  for (int i = 0; i < graph.VerticesCount(); i++) {
    if (!visited[i])
      DFS(graph, i, visited, func);
  }
}

void topologicalSort(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted) {
  visited[vertex] = true;

  for (int nextVertex: graph.GetNextVertices(vertex)) {
    if (!visited[nextVertex]) {
      topologicalSort(graph, nextVertex, visited, sorted);
    }
  }

  sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph &graph) {
  std::deque<int> sorted;
  std::vector<bool> visited(graph.VerticesCount(), false);

  for (int i = 0; i < graph.VerticesCount(); i++) {
    if (!visited[i])
      topologicalSort(graph, i, visited, sorted);
  }

  return sorted;
}

int main() {
  ListGraph graph1(7);
  graph1.AddEdge(0, 1);
  graph1.AddEdge(0, 5);
  graph1.AddEdge(1, 2);
  graph1.AddEdge(1, 3);
  graph1.AddEdge(1, 5);
  graph1.AddEdge(1, 6);
  graph1.AddEdge(3, 2);
  graph1.AddEdge(3, 4);
  graph1.AddEdge(3, 6);
  graph1.AddEdge(5, 4);
  graph1.AddEdge(5, 6);
  graph1.AddEdge(6, 4);

  MatrixGraph graph2(graph1);
  SetGraph graph3(graph2);
  ArcGraph graph4(graph3);

  std::cout << std::setw(11) << "ListGraph" << " size: " << graph1.VerticesCount() << " elements: ";
  mainBFS(graph1, [](int vertex) { std::cout << vertex << " "; });
  std::cout << std::endl;

  std::cout << std::setw(11) << "MatrixGraph" << " size: " << graph2.VerticesCount() << " elements: ";
  mainBFS(graph2, [](int vertex) { std::cout << vertex << " "; });
  std::cout << std::endl;

  std::cout << std::setw(11) << "SetGraph" << " size: " << graph3.VerticesCount() << " elements: ";
  mainBFS(graph3, [](int vertex) { std::cout << vertex << " "; });
  std::cout << std::endl;

  std::cout << std::setw(11) << "ArcGraph" << " size: " << graph4.VerticesCount() << " elements: ";
  mainBFS(graph4, [](int vertex) { std::cout << vertex << " "; });
  std::cout << std::endl;

  return 0;
}
