/********   Header File ********/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <stack>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <bitset>

#include <memory>
#include <functional>
#include <algorithm>
#include <numeric>
#include <utility>
#include <random>
#include <limits>

using namespace std;

// typedefs
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef pair<int, int> II;
typedef vector<string> VS;

// containers
//------------------------------------------
#define ALL(a)  (a).begin(),(a).end()
#define RALL(a) (a).rbegin(),(a).rend()
#define SZ(a)   ((a).size())

#define EXIST(s, e)     ((s).find(e) != (s).end())
#define CEXIST(s, e)    (find(ALL(s), e) != (s).end())
#define SORT(c)         sort(ALL(c))
#define TR(c, i)        for(auto i = begin(c); i != end(c); i++)
#define FE(c, e)        for(auto &e : c)

// loops
//------------------------------------------
#define FOR(i, k, n)   for (decltype(n) i=k; i<(n); i++)
#define FORV(i, a)     FOR(i, 0, (int)SZ(a))
#define FORVV(j, i, a) FORV(j, a) FORV(i,(a)[j])

#define RFOR(i, j, n)  for (decltype(n) i=(n - 1); (i + 1) > j; i--)
// RFOR is also FOR(_i, 0, n) { auto i = n - _i - 1;

// parsing/formatting
//------------------------------------------
inline int toint(string &s) { int v; istringstream sin(s); sin >> v; return v; }
inline int toint(char *s)   { int v; istringstream sin(s); sin >> v; return v; }
template<class T> inline    string tostring(T t) { stringstream s; s << t; return s.str(); }
template <class U, class V> string tostring(pair<U, V> u, string del=",") { return "(" + tostring(u.first) + del + tostring(u.second) + ")"; }
template <class T>          string strjoin(const T &t, string del = " ") { string s; TR(t, it) s += (!s.empty() ? del : "")+tostring(*it); return s; }
template <class T>          string tostring(const vector<T>& v, string delim = " ")        { return "(" + strjoin(v, delim) + ")"; }
template <class T>          string tostring(const set<T>& h,    string delim = " ")        { return "{" + strjoin(h, delim) + "}"; }
template <class T>          string tostring(const unordered_set<T>& h, string delim = " ") { return "{" + strjoin(h, delim) + "}"; }
template <class U, class V> string tostring(const map<U, V>& m, string delim = " ")           { string s; TR(m, it) s += (!s.empty()?delim:"") + tostring(*it, "->"); return "{" + s + "}"; }
template <class U, class V> string tostring(const unordered_map<U, V>& m, string delim = " ") { string s; TR(m, it) s += (!s.empty()?delim:"") + tostring(*it, "->"); return "{" + s + "}"; }

// constant
//--------------------------------------------
const double EPS = 1e-10;
const double PI  = acos(-1.0);

// shortcuts
#define F first
#define S second
#define PB push_back
#define MP make_pair
// #define B begin()
// #define E end()

// ranges
#define WITHIN(i, n) ((i) >= 0 && (i) < (n))
#define SIGN(x)      (((x) < 0) ? -1 : 1)

// clear memory
#define CLR(a)       memset((a), 0, sizeof(a))

// fast stack
//typedef int _sn21;  // stack node
#define STACK_CAP             2*BUFSIZ
#define DECLARE_STACK(_type, _sz) _type _bttm21[_sz]; auto _top21 = _bttm21;
#define SPUSH(item)           ((void) (*_top21++ = (item)))
#define SPOP(item)            ((void) (item = *(--_top21)))
#define STACK_NOT_EMPTY	      (_bttm21 < _top21)
#define STACK_SIZE            (_top21 - _bttm21)

// fast queue
// typedef int _qn21; // queue node
#define QUEUE_CAP           2*BUFSIZ
#define DECLARE_QUEUE(_type, _sz) _type _q21[(_sz)]; size_t _qm = (_sz); size_t _f21 = 0; size_t _b21 = 0;
#define QPUSH(item)         ((void) (_q21[(_b21++)%_qm] = (item)))
#define QPOP(item)          ((void) (item = _q21[(_f21++)%_qm]))
#define QFRONT              (_q21[_f21%_qm])
#define QBACK               (_q21[_b21%_qm])
#define QUEUE_NOT_EMPTY     (_f21 != _b21)
#define QUEUE_SIZE          ((_b21 - _f21 + _qm) % _qm)

// basic math
template<typename T> int dEqual(T x, double y) { return abs(x-y) < EPS;  }
template<typename T> int dLess (T x, double y) { return (x-y)    < -EPS; }
template<typename T> T gcd(T x, T y)  { return y ? gcd(y,x%y) : abs(x); }
template<typename T> T lcm(T x, T y)  { return x*y ? abs(x*y)/gcd(x,y) : 0; }
#define L1NORM(x, y) (abs(x) + abs(y))
#define L2NORM(x, y) ((x)*(x)+(y)*(y))

// bits
#define CHECK(S, j)  (S &   (1 << j))
#define SET(S, j)    (S |=  (1 << j))
#define UNSET(S, j)  (S &= ~(1 << j))
#define SETALL(S, j) (S =   (1 << j)-1)
#define TOGGLE(S, j) (S ^=  (1 << j))

//debug
#define dump(x)  cout << #x << " = " << (x) << endl
#define debug(x) cout << #x << " = " << (x) << " (L" << __LINE__ << ")" << " " << __FILE__ << endl

static const auto fastIO = []() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    return 0;
}();

/******** End of Header File ********/