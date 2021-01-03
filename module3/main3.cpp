/* Требуется отыскать самый выгодный маршрут между городами.
 * Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
 * Формат входных данных.
 * Первая строка содержит число N – количество городов.
 * Вторая строка содержит число M - количество дорог.
 * Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
 * Последняя строка содержит маршрут (откуда и куда нужно доехать).
 * Формат выходных данных.
 * Вывести длину самого выгодного маршрута. */


#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <cassert>

struct Vertex {
  Vertex(int vertex, int weight)
      : vertex(vertex),
        weight(weight) {
  }

  bool operator<(const Vertex &rhs) const {
    if (weight == rhs.weight) {
      return vertex < rhs.vertex;;
    }

    return weight < rhs.weight;
  }

  int vertex;
  int weight;
};

class ListGraph {
 public:
  explicit ListGraph(int size)
      : adjacencyLists(size) {
  }
  ~ListGraph() = default;

  void AddEdge(int from, int to, int weight) {
    adjacencyLists[from].emplace_back(to, weight);
    adjacencyLists[to].emplace_back(from, weight);
  }

  int VerticesCount() const {
    return (int) adjacencyLists.size();
  }

  std::vector<Vertex> GetNextVertices(int vertex) const {
    return adjacencyLists[vertex];
  }

 private:
  std::vector<std::vector<Vertex>> adjacencyLists;
};

bool Relax(int u, int v, int w, std::vector<int> &pi, std::vector<int> &d) {
  if (d[v] > d[u] + w) {
    d[v] = d[u] + w;
    pi[v] = u;
    return true;
  }

  return false;
}

int Dijkstra(const ListGraph &graph, int from, int to) {
  std::vector<int> pi(graph.VerticesCount(), -1);
  std::vector<int> d(graph.VerticesCount(), std::numeric_limits<int>::max());
  d[from] = 0;

  std::set<Vertex> q;
  q.insert(Vertex(from, 0));

  while (!q.empty()) {
    auto u = *q.begin();
    q.erase(q.begin());

    for (auto &v: graph.GetNextVertices(u.vertex)) {
      if (d[v.vertex] == std::numeric_limits<int>::max()) {
        d[v.vertex] = d[u.vertex] + v.weight;
        pi[v.vertex] = u.vertex;
        q.insert(Vertex(v.vertex, d[v.vertex]));
      } else {
        auto oldWeight = d[v.vertex];
        if (Relax(u.vertex, v.vertex, v.weight, pi, d)) {
          q.erase(q.find(Vertex(v.vertex, oldWeight)));
          q.insert(Vertex(v.vertex, d[v.vertex]));
        }
      }
    }
  }

  return d[to];
}

void run(std::istream &is, std::ostream &os) {
  int count;
  is >> count;
  ListGraph graph(count);

  int n;
  is >> n;

  for (int i = 0; i < n; ++i) {
    int from, to, weight;
    is >> from >> to >> weight;
    graph.AddEdge(from, to, weight);
  }

  int from, to;
  is >> from >> to;
  os << Dijkstra(graph, from, to) << '\n';
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;

    input << "6" << std::endl;
    input << "9" << std::endl;

    input << "0 3 1" << std::endl;
    input << "0 4 2" << std::endl;
    input << "1 2 7" << std::endl;
    input << "1 3 2" << std::endl;
    input << "1 4 3" << std::endl;
    input << "1 5 3" << std::endl;
    input << "2 5 3" << std::endl;
    input << "3 4 4" << std::endl;
    input << "3 5 6" << std::endl;

    input << "0 2" << std::endl;

    run(input, output);

    assert(output.str() == "9\n");
  }
}

int main() {
//  test();
  run(std::cin, std::cout);

  return 0;
}
