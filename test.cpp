#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cfenv>
#include <cfloat>
#include <chrono>
#include <cinttypes>
#include <climits>
#include <cmath>
#include <complex>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <streambuf>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
// #include<atcoder/dsu>

using namespace std;
// using namespace atcoder;

#define ll long long

template <typename T>
using V = vector<T>;
template <typename T>
using VV = vector<vector<T>>;
using vi = vector<int>;
using vl = vector<long long>;
using vd = V<double>;
using vs = V<string>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<long long>>;
using vvc = vector<vector<char>>;

template <typename T, typename U>
struct P : pair<T, U> {
    template <typename... Args>
    P(Args... args) : pair<T, U>(args...) {}

    using pair<T, U>::first;
    using pair<T, U>::second;

    P &operator+=(const P &r) {
        first += r.first;
        second += r.second;
        return *this;
    }
    P &operator-=(const P &r) {
        first -= r.first;
        second -= r.second;
        return *this;
    }
    P &operator*=(const P &r) {
        first *= r.first;
        second *= r.second;
        return *this;
    }
    template <typename S>
    P &operator*=(const S &r) {
        first *= r, second *= r;
        return *this;
    }
    P operator+(const P &r) const { return P(*this) += r; }
    P operator-(const P &r) const { return P(*this) -= r; }
    P operator*(const P &r) const { return P(*this) *= r; }
    template <typename S>
    P operator*(const S &r) const {
        return P(*this) *= r;
    }
    P operator-() const { return P{-first, -second}; }
};
template <typename T>
using minpq = priority_queue<T, vector<T>, greater<T>>;
using pl = P<long long, long long>;
using pi = P<int, int>;
using vp = V<pl>;

#define all(v) (v).begin(), (v).end()
#define rall(v) (v).rbegin(), v.rend()
#define each(i, v) for (auto i : v)
#define each2(x, y, v) for (auto [x, y] : v)
#define rep(i, N) for (long long i = 0; i < (long long)(N); i++)
#define repr(i, N) for (long long i = (long long)(N)-1; i >= 0; i--)
#define rep1(i, N) for (long long i = 1; i <= (long long)(N); i++)
#define repr1(i, N) for (long long i = (N); (long long)(i) > 0; i--)
#define reg(i, a, b) for (long long i = (a); i < (b); i++)
#define regr(i, a, b) for (long long i = (b)-1; i >= (a); i--)
// #define fi first
// #define se second

#define in(a) cin >> a
#define out2(a, b) cout << a << b
#define out(a) cout << a << "\n"
constexpr int INF = 0x3f3f3f3f;
constexpr long long LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr double EPS = 1e-8;
constexpr int MOD = 998244353;
// constexpr int MOD = 1000000007;
#define pb push_back
#define mod(a, b) (a % b + b) % b // 出力が正整数になるmod計算

/// constexpr static int bit_len_of_pocket = dec2bin(sum_stone_num).size();



// dec_numのbit数を数える
constexpr int calculate_bit_length(unsigned int dec_num) {
    int bits = 0;
    while (dec_num > 0) {
        dec_num >>= 1;
        ++bits;
    }
    return bits > 0 ? bits : 1;  // 最小値として1を保証
}

constexpr int sum_stone_num = 48;
constexpr int bit_len_of_pocket = calculate_bit_length(sum_stone_num);

int main() {
    bitset<bit_len_of_pocket> a(12);
    cout << a << endl;

    return 0;
}
