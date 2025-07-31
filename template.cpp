/*==============================================*\
|  Author   : Equation Tracker                    |
|  Handle   : Equation_Tracker                    |
|  Mission  : Breach the Limits                   |
|  Realm    : Competitive Programming             |
|  Status   : [ ACTIVE ]                          |
\*==============================================*/

#pragma GCC optimize("Ofast", "O3")
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
vector<long long> primes{};
void genPrimes(long long lim = 2.53e6);
int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);
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
