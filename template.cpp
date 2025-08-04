/*==============================================*\
|  Author   : Equation Tracker                    |
|  Handle   : Equation_Tracker                    |
|  Mission  : Breach the Limits                   |
|  Realm    : Competitive Programming             |
|  Status   : [ ACTIVE ]                          |
\*==============================================*/

#if defined(__GNUC__) && !defined(DEBUG)
  #pragma GCC optimize("Ofast", "O3")
#endif
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <chrono>
using namespace std;
using namespace __gnu_pbds;
typedef long long ll;
#define whole(v) v.begin(), v.end()
#define arr_out(v) for (auto x: v) cout << x << " "; cout << "\n";
#define loop(x) for (long long i = 0; i < x; i++)
struct custom_hash {
  static uint64_t splitmix64(uint64_t x);
  size_t operator()(uint64_t x) const;
};
void totalSolution();
void caseSolution();
long long nCr(long long, long long);
template <typename _Tp, typename Cm_fn = std::less<_Tp>>
using ordered_set = __gnu_pbds::tree<_Tp, __gnu_pbds::null_type, Cm_fn, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;
template <typename _Tp, typename _vTp, typename Cm_fn = std::less<_Tp>>
using ordered_map = __gnu_pbds::tree<_Tp, _vTp, Cm_fn, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;
template <typename _Tp>
class Graph {
private:
  std::map<_Tp, std::set<_Tp>> g;
  std::set<std::pair<_Tp, _Tp>> edges;
  void dfsUtil(const _Tp& node, std::set<_Tp>& visited, const std::optional<_Tp>& parent) {
    if (visited.find(node) != visited.end()) return;
    visited.insert(node);
    // <== Do something
    for (const auto& n : g[node]) {
      if (parent && n == *parent) continue;
      dfsUtil(n, visited, node);
    }
  }
  bool dfsCycleDirected(const _Tp& node, std::set<_Tp>& visited, std::set<_Tp>& recStack) {
    if (recStack.find(node) != recStack.end()) return true;
    if (visited.find(node) != visited.end()) return false;
    visited.insert(node);
    recStack.insert(node);
    for (const auto& n : g[node]) {
      if (dfsCycleDirected(n, visited, recStack)) return true;
    }
    recStack.erase(node);
    return false;
  }
  bool dfsCycleUndirected(const _Tp& node, std::set<_Tp>& visited, const std::optional<_Tp>& parent) {
    if (visited.find(node) != visited.end()) return false;
    visited.insert(node);
    for (const auto& n : g[node]) {
      if (parent && n == *parent) continue;
      if (visited.find(n) != visited.end()) return true;
      if (dfsCycleUndirected(n, visited, node)) return true;
    }
    return false;
  }
public:
  void addEdge(const _Tp& from, const _Tp& to, bool directed = true) {
    g[from].insert(to);
    if (!directed) g[to].insert(from);
    edges.insert({from, to});
  }
  bool hasNode(const _Tp& u) const { return g.find(u) != g.end(); }
  bool hasEdge(const _Tp& u, const _Tp& v) const { auto it = g.find(u); return it != g.end() && it->second.count(v); }
  std::vector<_Tp> neighbors(const _Tp& u) const { if (!hasNode(u)) return {}; return std::vector<_Tp>(g.at(u).begin(), g.at(u).end()); }
  long long size() const { return static_cast<long long>(g.size()); }
  long long edgeCount() const { return static_cast<long long>(edges.size()); }
  long long degree(const _Tp& u) const { if (!hasNode(u)) return 0; return static_cast<long long>(g.at(u).size()); }
  void removeEdge(const _Tp& u, const _Tp& v, bool directed = true) {
    if (hasNode(u)) g[u].erase(v);
    if (!directed && hasNode(v)) g[v].erase(u);
    edges.erase({u, v});
  }
  void removeNode(const _Tp& u) {
    if (!hasNode(u)) return;
    g.erase(u);
    std::vector<std::pair<_Tp, _Tp>> toErase;
    for (auto it = edges.begin(); it != edges.end(); ++it) {
      if (it->first == u || it->second == u) toErase.push_back(*it);
    }
    for (auto& p : toErase) edges.erase(p);
    for (auto& [node, ns] : g) ns.erase(u);
  }
  void clear() { g.clear(); edges.clear(); }
  bool isConnected(const _Tp& root) const {
    if (!hasNode(root)) return false;
    std::set<_Tp> visited;
    const_cast<Graph*>(this)->dfsUtil(root, visited, std::nullopt);
    return visited.size() == g.size();
  }
  std::set<_Tp> connectedComponents() const {
    std::set<_Tp> seen;
    std::set<_Tp> all;
    for (const auto& [k, _v] : g) all.insert(k);
    for (const auto& node : all) {
      if (seen.find(node) != seen.end()) continue;
      std::queue<_Tp> q;
      q.push(node);
      while (!q.empty()) {
        auto cur = q.front(); q.pop();
        if (seen.find(cur) != seen.end()) continue;
        seen.insert(cur);
        for (const auto& n : g.at(cur)) if (seen.find(n) == seen.end()) q.push(n);
      }
    }
    return seen;
  }
  bool hasPath(const _Tp& src, const _Tp& dest) const {
    if (!hasNode(src) || !hasNode(dest)) return false;
    std::queue<_Tp> q; std::set<_Tp> vis;
    q.push(src); vis.insert(src);
    while (!q.empty()) {
      auto u = q.front(); q.pop();
      if (u == dest) return true;
      for (const auto& n : g.at(u)) if (vis.find(n) == vis.end()) { vis.insert(n); q.push(n); }
    }
    return false;
  }
  std::vector<_Tp> shortestPathUnweighted(const _Tp& src, const _Tp& dest) const {
    if (!hasNode(src) || !hasNode(dest)) return {};
    std::queue<_Tp> q; std::map<_Tp, _Tp> parent; std::set<_Tp> vis;
    q.push(src); vis.insert(src); parent[src] = src;
    while (!q.empty()) {
      auto u = q.front(); q.pop();
      if (u == dest) break;
      for (const auto& n : g.at(u)) {
        if (vis.find(n) != vis.end()) continue;
        vis.insert(n); parent[n] = u; q.push(n);
      }
    }
    if (vis.find(dest) == vis.end()) return {};
    std::vector<_Tp> path;
    for (_Tp cur = dest;; cur = parent[cur]) {
      path.push_back(cur);
      if (cur == parent[cur]) break;
    }
    std::reverse(path.begin(), path.end());
    return path;
  }
  bool hasCycle(bool directed) {
    std::set<_Tp> visited;
    if (directed) {
      std::set<_Tp> rec;
      for (const auto& [node, _] : g) if (dfsCycleDirected(node, const_cast<std::set<_Tp>&>(visited), const_cast<std::set<_Tp>&>(rec))) return true;
      return false;
    } else {
      std::set<_Tp> vis2;
      for (const auto& [node, _] : g) if (vis2.find(node) == vis2.end() && dfsCycleUndirected(node, vis2, std::nullopt)) return true;
      return false;
    }
  }
  std::vector<_Tp> topologicalSort() const {
    std::map<_Tp, int> indeg;
    for (const auto& [u, _] : g) indeg[u] = 0;
    for (const auto& [u, ns] : g) for (const auto& v : ns) indeg[v]++;
    std::queue<_Tp> q;
    for (const auto& [k, v] : indeg) if (v == 0) q.push(k);
    std::vector<_Tp> res;
    while (!q.empty()) {
      auto u = q.front(); q.pop(); res.push_back(u);
      for (const auto& v : g.at(u)) {
        indeg[v]--;
        if (indeg[v] == 0) q.push(v);
      }
    }
    if (res.size() != g.size()) return {};
    return res;
  }
  bool isBipartite(const _Tp& start) const {
    if (!hasNode(start)) return false;
    std::map<_Tp, int> color;
    std::queue<_Tp> q;
    q.push(start); color[start] = 0;
    while (!q.empty()) {
      auto u = q.front(); q.pop();
      for (const auto& v : g.at(u)) {
        if (!color.count(v)) { color[v] = color[u] ^ 1; q.push(v); }
        else if (color[v] == color[u]) return false;
      }
    }
    return true;
  }
  void DFS(const _Tp& root) {
    if (!hasNode(root)) return;
    std::set<_Tp> visited;
    dfsUtil(root, visited, std::nullopt);
  }
  void BFS(const _Tp& root) {
    if (!hasNode(root)) return;
    std::set<_Tp> visited;
    std::queue<_Tp> q;
    visited.insert(root);
    q.push(root);
    while (!q.empty()) {
      auto node = q.front(); q.pop();
      std::cout << node << "\n"; // <== Do something
      for (const auto& n : g[node]) {
        if (visited.find(n) != visited.end()) continue;
        visited.insert(n);
        q.push(n);
      }
    }
  }
};
template <typename _Tp>
class Segment_Tree {
private:
  std::vector<_Tp> Tree, arr, lazy;
  bool one_based;
  void lazyUpdate(long long node, long long l, long long r) {
    if (lazy[node] != 0) {
      Tree[node] += (r - l + 1) * lazy[node]; // <== CHANGE FOR max: Tree[node] += lazy[node];
      if (l != r) {
        lazy[2 * node] += lazy[node];
        lazy[2 * node + 1] += lazy[node];
      }
      lazy[node] = 0;
    }
  }
  void build(long long node, long long l, long long r) {
    if (l == r) {
      Tree[node] = arr[l];
      return;
    }
    long long mid = (l + r) / 2;
    build(2 * node, l, mid);
    build(2 * node + 1, mid + 1, r);
    Tree[node] = Tree[2 * node] + Tree[2 * node + 1]; // <== CHANGE FOR max: max(Tree[2 * node], Tree[2 * node + 1])
  }
  _Tp queryUtil(long long node, long long l, long long r, long long start, long long end) {
    lazyUpdate(node, l, r);
    if (end < l || r < start) return 0; // <== CHANGE FOR max: LLONG_MIN
    if (start <= l && r <= end) return Tree[node];
    long long mid = (l + r) / 2;
    _Tp left = queryUtil(2 * node, l, mid, start, end);
    _Tp right = queryUtil(2 * node + 1, mid + 1, r, start, end);
    return left + right; // <== CHANGE FOR max: max(left, right)
  }
  void updateUtil(long long node, long long index, long long value, long long l, long long r) {
    lazyUpdate(node, l, r);
    if (l == r) {
      arr[index] += value;
      Tree[node] += value;
      return;
    }
    long long mid = (l + r) / 2;
    if (index <= mid) updateUtil(2 * node, index, value, l, mid);
    else updateUtil(2 * node + 1, index, value, mid + 1, r);
    Tree[node] = Tree[2 * node] + Tree[2 * node + 1]; // <== CHANGE FOR max: max(Tree[2 * node], Tree[2 * node + 1])
  }
  void rangeUpdateUtil(long long node, long long l, long long r, long long value, long long start, long long end) {
    lazyUpdate(node, l, r);
    if (r < start || end < l) return;
    if (start <= l && r <= end) {
      Tree[node] += value * (r - l + 1); // <== CHANGE FOR max: Tree[node] += value
      if (l != r) {
        lazy[2 * node] += value;
        lazy[2 * node + 1] += value;
      }
      return;
    }
    long long mid = (l + r) / 2;
    rangeUpdateUtil(2 * node, l, mid, value, start, end);
    rangeUpdateUtil(2 * node + 1, mid + 1, r, value, start, end);
    Tree[node] = Tree[2 * node] + Tree[2 * node + 1]; // <== CHANGE FOR max: max(Tree[2 * node], Tree[2 * node + 1])
  }
public:
  _Tp query(long long start, long long end) {
    if (one_based) return queryUtil(1, 1, arr.size() - 1, start, end);
    return queryUtil(1, 0, arr.size() - 1, start, end);
  }
  void update(long long index, long long value) {
    if (one_based) updateUtil(1, index, value, 1, arr.size() - 1);
    else updateUtil(1, index, value, 0, arr.size() - 1);
  }
  void rangeUpdate(long long start, long long end, long long value) {
    if (one_based) rangeUpdateUtil(1, 1, arr.size() - 1, value, start, end);
    else rangeUpdateUtil(1, 0, arr.size() - 1, value, start, end);
  }
  Segment_Tree(std::vector<_Tp> &a, bool is_one_based = false) {
    one_based = is_one_based;
    arr = is_one_based ? vector<_Tp>(a.size() + 1) : a;
    if (is_one_based) for (long long i = 1; i < arr.size(); i++) arr[i] = a[i - 1];
    Tree.resize(4 * arr.size(), 0); // <== CHANGE FOR max: LLONG_MIN
    lazy.resize(4 * arr.size());
    if (one_based) build(1, 1, arr.size() - 1);
    else build(1, 0, arr.size() - 1);
  }
};
template <typename _Tp>
class Fenwick_Tree {
private:
  std::vector<_Tp> Tree;
public:
  void init() {
    for (long long i = 1; i <= this->Tree.size(); i++) {
      long long parent = i + (i & -i);
      if (parent <= this->Tree.size()) {
        this->Tree[parent - 1] += this->Tree[i - 1];
      }
    }
  }
  _Tp sum(long long l, long long r) {
    _Tp sum = 0;
    while (r > 0) {
      sum += this->Tree[r - 1];
      r -= (r & -r);
    }
    l--;
    while (l > 0) {
      sum -= this->Tree[l - 1];
      l -= (l & -l);
    }
    return sum;
  }
  void update(long long index, _Tp difference) {
    while (index <= this->Tree.size()) {
      this->Tree[index - 1] += difference;
      index += (index & -index);
    }
  }
  Fenwick_Tree(const std::vector<_Tp> &array) {
    this->Tree = array;
    this->init();
  }
};
vector<long long> primes{};
void genPrimes(long long lim = 2.53e6);
int main(int argc, char const *argv[]) {
  #if !defined(DEBUG)
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
  #endif
  totalSolution();
  return 0;
}
void caseSolution() {
  long long n = 0, m = 0, k = 0, l = 0, sum = 0, t1 = 0, t2 = 0;
  string s = "", t = "";
  cin >> n;

}
void totalSolution() {
  long long T;
  cin >> T;
  while (T-- > 0) caseSolution();
}
uint64_t custom_hash::splitmix64(uint64_t x) {
  x += 0x9e3779b97f4a7c15;
  x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
  x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
  return x ^ (x >> 31);
}
size_t custom_hash::operator()(uint64_t x) const {
  static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
  return splitmix64(x + FIXED_RANDOM);
}
void genPrimes(long long lim) {
  vector<bool> visited(lim, 0);
  for (long long p = 2; p < lim; p++) {
    if (visited[p]) continue;
    for (long long i = p * p; i < lim; i += p) visited[i] = 1;
    primes.push_back(p);
  }
}
long long nCr(long long n, long long r) {
  if (r > n) return 0;
  if (r > n - r) r = n - r;
  unsigned long long result = 1;
  for (int i = 0; i < r; ++i) {
    result *= (n - i);
    result /= (i + 1);
  }
  return result;
}
