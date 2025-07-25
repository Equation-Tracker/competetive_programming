/*==============================================*\
|  Author   : Equation Tracker                    |
|  Handle   : Equation_Tracker                    |
|  Mission  : Breach the Limits ⚡                 |
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
#define loop(x) for (ll i = 0; i < x; i++)
struct custom_hash {
  static uint64_t splitmix64(uint64_t x);
  size_t operator()(uint64_t x) const;
};
void totalSolution();
void caseSolution();
template <typename _Tp, typename Cm_fn = std::less<_Tp>>
using ordered_set = __gnu_pbds::tree<
  _Tp, __gnu_pbds::null_type, Cm_fn, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update
>;
template <typename _Tp, typename _vTp, typename Cm_fn = std::less<_Tp>>
using ordered_map = __gnu_pbds::tree<
  _Tp, _vTp, Cm_fn, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update
>;
template <typename _Tp>
class Fenwick_Tree {
private:
  std::vector<_Tp> Tree;
public:
  void init() {
    for (ll i = 1; i <= (ll)Tree.size(); i++) {
      ll parent = i + (i & -i);
      if (parent <= (ll)Tree.size()) {
        Tree[parent - 1] += Tree[i - 1];
      }
    }
  }
  _Tp sum(ll l, ll r) {
    _Tp sum = 0;
    while (r > 0) {
      sum += Tree[r - 1];
      r -= (r & -r);
    }
    l--;
    while (l > 0) {
      sum -= Tree[l - 1];
      l -= (l & -l);
    }
    return sum;
  }
  void update(ll index, _Tp difference) {
    while (index <= (ll)Tree.size()) {
      Tree[index - 1] += difference;
      index += (index & -index);
    }
  }
  Fenwick_Tree(std::vector<_Tp> &array) {
    Tree = array;
    init();
  }
};
template <typename _Tp>
class Segment_Tree {
private:
  std::vector<_Tp> Tree, arr, lazy;
  bool one_based;
  void lazyUpdate(ll node, ll l, ll r) {
    if (lazy[node] != 0) {
      Tree[node] += (r - l + 1) * lazy[node];
      if (l != r) {
        lazy[2 * node] += lazy[node];
        lazy[2 * node + 1] += lazy[node];
      }
      lazy[node] = 0;
    }
  }
  void build(ll node, ll l, ll r) {
    if (l == r) {
      Tree[node] = arr[l];
      return;
    }
    ll mid = (l + r) / 2;
    build(2 * node, l, mid);
    build(2 * node + 1, mid + 1, r);
    Tree[node] = Tree[2 * node] + Tree[2 * node + 1];
  }
  _Tp queryUtil(ll node, ll l, ll r, ll start, ll end) {
    lazyUpdate(node, l, r);
    if (end < l || r < start) return 0ll;
    if (start <= l && r <= end) return Tree[node];
    ll mid = (l + r) / 2;
    return queryUtil(2 * node, l, mid, start, end) + queryUtil(2 * node + 1, mid + 1, r, start, end);
  }
  void updateUtil(ll node, ll index, ll value, ll l, ll r) {
    lazyUpdate(node, l, r);
    if (l == r) {
      arr[index] += value;
      Tree[node] += value;
      return;
    }
    ll mid = (l + r) / 2;
    if (index <= mid)
      updateUtil(2 * node, index, value, l, mid);
    else
      updateUtil(2 * node + 1, index, value, mid + 1, r);
    Tree[node] = Tree[2 * node] + Tree[2 * node + 1];
  }
  void rangeUpdateUtil(ll node, ll l, ll r, ll value, ll start, ll end) {
    lazyUpdate(node, l, r);
    if (r < start || end < l) return;
    if (start <= l && r <= end) {
      Tree[node] += value * (r - l + 1);
      if (l != r) {
        lazy[2 * node] += value;
        lazy[2 * node + 1] += value;
      }
      return;
    }
    ll mid = (l + r) / 2;
    rangeUpdateUtil(2 * node, l, mid, value, start, end);
    rangeUpdateUtil(2 * node + 1, mid + 1, r, value, start, end);
    Tree[node] = Tree[2 * node] + Tree[2 * node + 1];
  }
public:
  _Tp query(ll start, ll end) {
    if (one_based) return queryUtil(1, 1, arr.size() - 1, start, end);
    return queryUtil(1, 0, arr.size() - 1, start, end);
  }
  void update(ll index, ll value) {
    if (one_based) updateUtil(1, index, value, 1, arr.size() - 1);
    else updateUtil(1, index, value, 0, arr.size() - 1);
  }
  void rangeUpdate(ll start, ll end, ll value) {
    if (one_based) rangeUpdateUtil(1, 1, arr.size() - 1, value, start, end);
    else rangeUpdateUtil(1, 0, arr.size() - 1, value, start, end);
  }
  Segment_Tree(std::vector<_Tp> &a, bool is_one_based = false) {
    one_based = is_one_based;
    if (one_based) {
      arr.resize(a.size() + 1);
      for (ll i = 1; i < arr.size(); i++) arr[i] = a[i - 1];
    } else arr = a;
    Tree.resize(4 * arr.size());
    lazy.resize(4 * arr.size());
    if (one_based) build(1, 1, arr.size() - 1);
    else build(1, 0, arr.size() - 1);
  }
};
vector<ll> primes{};
void genPrimes(ll lim = 2.53e6);
int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);

  vector<ll> a = {1, 2, 3, 4, 5}; // 0-based
  Segment_Tree<ll> st0(a, false); // 0-based indexing

  cout << st0.query(0, 4) << "\n";    // 1+2+3+4+5 = 15
  st0.update(2, 10);                  // a[2] += 10 → 13
  cout << st0.query(1, 3) << "\n";    // 2+13+4 = 19
  st0.rangeUpdate(1, 4, 5);           // a[1..4] += 5
  cout << st0.query(0, 4) << "\n";    // 1+7+13+9+10 = 40

  vector<ll> b = {10, 20, 30, 40, 50}; // 1-based
  Segment_Tree<ll> st1(b, true);      // 1-based indexing

  cout << st1.query(1, 5) << "\n";     // 10+20+30+40+50 = 150
  st1.update(3, -10);                  // b[3] -= 10 → 20
  cout << st1.query(2, 4) << "\n";     // 20+20+40 = 80
  st1.rangeUpdate(2, 5, 2);            // b[2..5] += 2
  cout << st1.query(1, 5) << "\n";     // 10+22+22+42+52 = 148

  return 0;
}

void caseSolution() {
  ll n = 0;
  cin >> n;
}
void totalSolution() {
  ll T;
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
void genPrimes(ll lim) {
  vector<bool> visited(lim, 0);
  for (ll p = 2; p < lim; p++) {
    if (visited[p]) continue;
    for (ll i = p * p; i < lim; i += p) visited[i] = 1;
    primes.push_back(p);
  }
}
