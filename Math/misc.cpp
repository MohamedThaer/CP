$$\sum_{i<j} a_i a_j = \frac{\left( \sum_{i=1}^{n} a_i \right)^2 - \sum_{i=1}^{n} a_i^2}{2}$$


// sum of numbers divisible by d in range
int calc(int l, int r, int d) {
  --l;
  ll sum = 1ll * d * (r / d) * (r / d + 1) / 2LL;
  sum -= 1ll * d * (l / d) * (l / d + 1) / 2LL;
  return sum;
}

// count number of y such that y%n = x (0 -> l)
int f(int l, int n, int x) {
 int cnt = (l / n) + (l % n >= x);
 return cnt;
}
// \sum{k=1}^{n}{floor(n/k)}
// count of numbers such that n/i = k -> n/k - n/(k+1)
ll floor_sum(ll n) {
  ll sum = 0;
  for (ll i = 1, last; i <= n; i = last + 1) {
    last = n / (n / i);
    sum += (n / i) * (last - i + 1);
    // n / x yields the same value for i <= x <= last.
  }
  return sum;
}

// sum of divisor of number in range [1 ... n]
ll divisorSum(ll num){
    ll sum = 0;
    for (ll i = 1; i*i <= num; i++) {
        ll t1 = i * (num / i - i + 1);
        ll t2 = (((num / i) * (num / i + 1)) / 2) - ((i * (i + 1)) / 2);
        sum += t1 + t2;
    } return sum;
}
// ax + by = gcd -> x = x. + b / gcd * T, y = y. - a / gcd * T
int extendedEuclidean(int a, int b, int &x, int &y) {
    if (not b) {
        x = 1, y = 0;
        return a;
    }

    long long i, j;
    long long g = extendedEuclidean(b, a % b, i, j);
    x = j, y = i - a / b * j;
    return g;
}
// CRT 
#define ll long long
#define M first
#define R second
typedef pair<ll, ll> mod_eq;

void nxt_r(ll &r0, ll &r1, ll &r) {
  int r2 = r0 - r * r1;
  r0 = r1, r1 = r2;
}

// r0 = a , r1 = b
// return gcd = a * x0 + b * y0
ll egcd(ll r0, ll r1, ll &x0, ll &y0) {
  ll x1 = y0 = 0, y1 = x0 = 1;
  while (r1) {
    ll r = r0 / r1;
    nxt_r(r0, r1, r);
    nxt_r(x0, x1, r);
    nxt_r(y0, y1, r);
  }
  return r0;
}

// c = x * a + y * b
// x` = x - (b / g) * k , y` = y + (a / g) * k - for any k
bool solveLDE(ll a, ll b, ll c, ll &x, ll &y, ll &g) {
  g = egcd(a, b, x, y);
  ll m = c / g;
  x *= m, y *= m;
  return m * g == c;
}

bool mod_inv(ll a, ll mod, ll &x) {
  ll y, g = egcd(a, mod, x, y);
  if (g != 1) {
    return false; // a and mod are not co-prime
  }
  x %= mod;
  if (x < 0)x += mod;
  return true;
}

bool CRT(const mod_eq &e1, const mod_eq &e2, mod_eq &res) {
  ll q1, q2, g;
  if (!solveLDE(e1.M, -e2.M, e2.R - e1.R, q1, q2, g)) {
    return false;
  }
  q2 %= e1.M / g;
  ll lcm = abs(e1.M / g * e2.M);
////  ll x = ((__int128) e2.M * (__int128) q2 + (__int128) e2.R) % lcm; ??
  ll x = e2.M * q2 + e2.R;
  x %= lcm;
  if (x < 0)x += lcm;
  res = {lcm, x};
  return true;
}

bool CRT(const vector<mod_eq> &eq, mod_eq &ret) {
  ret = eq[0];
  for (int i = 1; i < eq.size(); ++i) {
    if (!CRT(eq[i], ret, ret)) {
      return false;
    }
  }
  return true;
}

// compute f(n, k) = C(n, 0) + C(n, 1) + ... + C(n, k)
// use the identity: f(n + 1, k) = 2 * f(n, k) - C(n, k)
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int q; cin >> q;
  for(int i = 1; i <= q; i++) {
    int n, k; cin >> n >> k;
    Q[i] = {n, k, i};
  }
  sort(Q + 1, Q + q + 1);
  int n = 1, k = 0;
  mint sum = 1, i2 = (mod + 1) / 2;
  for (int i = 1; i <= q; i++) {
    int L = Q[i].l, R = Q[i].r;
    while (n < L) {
      sum = sum * 2 - C.ncr(n, k);
      n++;
    }
    while (n > L) {
      --n;
      sum += C.ncr(n, k);
      sum *= i2;
    }
    while (k < R) {
      ++k;
      sum += C.ncr(n, k);
    }
    while (k > R) {
      sum -= C.ncr(n, k);
      k--;
    }
    ans[Q[i].id] = sum;
  }
  for (int i = 1; i <= q; i++) {
    cout << ans[i] << '\n';
  }
  return 0;
}

int sumOfDigitsFrom1ToN(int n)
{
    // base case: if n<10 return sum of
    // first n natural numbers
    if (n<10)
        return n*(n+1)/2;

    // d = number of digits minus one in n. For 328, d is 2
    int d = log10(n);

    // computing sum of digits from 1 to 10^d-1,
    // d=1 a[0]=0;
    // d=2 a[1]=sum of digit from 1 to 9 = 45
    // d=3 a[2]=sum of digit from 1 to 99 = a[1]*10 + 45*10^1 = 900
    // d=4 a[3]=sum of digit from 1 to 999 = a[2]*10 + 45*10^2 = 13500
    int *a = new int[d+1];
    a[0] = 0, a[1] = 45;
    for (int i=2; i<=d; i++)
        a[i] = a[i-1]*10 + 45*ceil(pow(10,i-1));

    // computing 10^d
    int p = ceil(pow(10, d));

    // Most significant digit (msd) of n,
    // For 328, msd is 3 which can be obtained using 328/100
    int msd = n/p;

    // EXPLANATION FOR FIRST and SECOND TERMS IN BELOW LINE OF CODE
    // First two terms compute sum of digits from 1 to 299
    // (sum of digits in range 1-99 stored in a[d]) +
    // (sum of digits in range 100-199, can be calculated as 1*100 + a[d]
    // (sum of digits in range 200-299, can be calculated as 2*100 + a[d]
    //  The above sum can be written as 3*a[d] + (1+2)*100

    // EXPLANATION FOR THIRD AND FOURTH TERMS IN BELOW LINE OF CODE
    // The last two terms compute sum of digits in number from 300 to 328
    // The third term adds 3*29 to sum as digit 3 occurs in all numbers
    //                from 300 to 328
    // The fourth term recursively calls for 28
    return msd*a[d] + (msd*(msd-1)/2)*p +
           msd*(1+n%p) + sumOfDigitsFrom1ToN(n%p);
}
