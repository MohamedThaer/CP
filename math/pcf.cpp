// Mohamed_Waheed
#include <bits/stdc++.h>
using namespace std;

template <int MAXN = 20000010, int PHI_N = 100000, int PHI_K = 100>
class PCF {
private:
    // MAXN: initial sieve
    // PHI_N: highest integer to chech its divisablility
    // PHI_K: consider first k primes

    factorizer F;
    vector<int> pref = vector<int>(MAXN + 1); // pref[i] --> number of primes <= i
    int dp[PHI_N][PHI_K];                     // precal of get(n,k)

    // returns the number of integers less or equal n which are
    // not divisible by any of the first k primes
    // recurrence --> get(n, k) = get(n, k-1) - get(n / p_k , k-1)
    // for sum of primes get(n, k) = get(n, k-1) - p_k * get(n / p_k , k-1)
    int64_t get(int64_t n, int k) {
        if (n < PHI_N && k < PHI_K) return dp[n][k];
        if (k == 1) return ((++n) >> 1);
        if (F.primes[k - 1] >= n) return 1;
        return get(n, k - 1) - get(n / F.primes[k - 1], k - 1);
    }

    // complexity: n^(2/3).(log n^(1/3))
    int64_t Legendre(int64_t n) {
        if (n < MAXN) return pref[n];
        int lim = sqrt(n) + 1;
        int k = upper_bound(F.primes.begin(), F.primes.end(), lim) - F.primes.begin();
        return get(n, k) + (k - 1);
    }

    // runs under 0.2s for n = 1e12
    int64_t Lehmer(int64_t n) {
        if (n < MAXN) return pref[n];
        int64_t w, res = 0;
        int b = sqrt(n), c = Lehmer(cbrt(n)), a = Lehmer(sqrt(b));
        b = Lehmer(b);
        res = get(n, a) + ((1LL * (b + a - 2) * (b - a + 1)) >> 1);
        for (int i = a; i < b; i++) {
            w = n / F.primes[i];
            int lim = Lehmer(sqrt(w));
            res -= Lehmer(w);
            if (i <= c) {
                for (int j = i; j < lim; j++) {
                    res += j;
                    res -= Lehmer(w / F.primes[j]);
                }
            }
        }
        return res;
    }

public:
    PCF() {
        F = factorizer(MAXN - 1);
        for (int i = 1; i < MAXN; i++) {
            pref[i] = pref[i - 1] + F.IsPrime(i);
        }

        // precalculation of phi upto size (PHI_N,PHI_K)
        for (int n = 0; n < PHI_N; n++) {
            dp[n][0] = n;
        }
        for (int k = 1; k < PHI_K; k++) {
            for (int n = 0; n < PHI_N; n++) {
                dp[n][k] = dp[n][k - 1] - dp[n / F.primes[k - 1]][k - 1];
            }
        }
    }

    int64_t prime_cnt(int64_t n) {
        return Lehmer(n);
    }

    int64_t prime_cnt(int64_t l, int64_t r) {
        return Lehmer(r) - Lehmer(l - 1);
    }
};

void Main(...) {
}
/*









*/
signed main() {
    cin.tie(0)->sync_with_stdio(0);
    int T = 1;
    // cin >> T;
    for (int i = 1; i <= T; i++) {
        Main(i), cout << '\n';
    }
}
