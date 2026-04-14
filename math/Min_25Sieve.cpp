
/**
 * Min_25 Sieve for multiplicative functions
 *
 * Requirements:
 * - f(p) is a polynomial in p for prime p
 * - f(p^k) can be computed quickly
 *
 * Usage:
 * Min_25Sieve<sieve_size, max_degree> sieve;
 * mint result = sieve.solve(n, poly_coeffs, eval_func);
 *
 * poly_coeffs: coefficients of the polynomial f(p) for prime p
 * eval_func: function that computes f(p^k) given prime p, exponent k, and p^k
 */
template <int N, int D = 3> // N >= 2 * sqrt(n), D is how many coeffs in polynomial
class Min_25Sieve {
private:
    int primes[N], p;
    Z pref[D][N];
    Z poly[D];
    int r, id1[N], id2[N];
    int64_t n_val;
    int tot;
    Z g[D][N];
    int64_t a[N];

    void sieve(int n) {
        vector<bool> f(n + 1, false);
        p = 0;
        for (int i = 2; i <= n; i++) {
            if (!f[i]) {
                primes[++p] = i;
                Z cur = 1;
                for (int k = 0; k < D; k++) {
                    pref[k][p] = pref[k][p - 1] + cur;
                    cur *= i;
                }
            }
            for (int j = 1; j <= p && primes[j] * i <= n; j++) {
                f[i * primes[j]] = 1;
                if (i % primes[j] == 0) break;
            }
        }
    }

    int get_id(int64_t x) {
        return x <= r ? id1[x] : id2[n_val / x];
    }

    // Recursive function to compute sum
    Z solve(int64_t x, int j, const function<Z(int, int, int64_t)> &eval) {
        if (primes[j] >= x) return 0;
        int id = get_id(x);
        Z ans = 0;

        // Initialize with g(x) - g(primes[j])
        for (int k = 0; k < D; k++) {
            ans += poly[k] * g[k][id];
        }
        for (int k = 0; k < D; k++) {
            ans -= poly[k] * pref[k][j];
        }

        // Add contributions from numbers with smallest prime factor > primes[j]
        for (int i = j + 1; i <= p && primes[i] <= x / primes[i]; i++) {
            int64_t pw = primes[i];
            for (int e = 1; pw <= x; e++) {
                ans += eval(primes[i], e, pw) * (solve(x / pw, i, eval) + (e != 1));
                if (!(pw <= x / primes[i])) break;
                pw *= primes[i];
            }
        }
        return ans;
    }

public:
    /**
     * Computes sum_{i=1}^n f(i)
     *
     * @param n upper bound
     * @param poly_coeffs coefficients of polynomial f(p) for prime p
     * @param eval function that computes f(p^k) given (prime, exponent, prime^k)
     * @return sum_{i=1}^n f(i)
     */
    Z solve(int64_t n, const vector<Z> &poly_coeffs, const function<Z(int, int, int64_t)> &eval) {
        n_val = n;
        int deg = poly_coeffs.size();
        for (int i = 0; i < deg; i++) {
            poly[i] = poly_coeffs[i];
        }

        r = sqrt(n);
        while (1LL * r * r < n)
            ++r;
        while (1LL * r * r > n)
            --r;
        sieve(r);

        tot = 0;
        int64_t i = 1;
        while (i <= n) {
            int64_t x = n / i;
            int64_t j = n / x;
            a[++tot] = x;

            // Initialize g[k][tot] = sum of i^k for 2 <= i <= x
            Z z = x;
            for (int k = 0; k < deg; k++) {
                if (k == 0)
                    g[k][tot] = z - 1;
                else if (k == 1)
                    g[k][tot] = z * (z + 1) / 2 - 1;
                else if (k == 2)
                    g[k][tot] = z * (z + 1) * (2 * z + 1) / 6 - 1;
                // Add more polynomial basis functions as needed
            }

            if (x <= r)
                id1[x] = tot;
            else
                id2[n / x] = tot;
            i = j + 1;
        }

        // Sieve to compute g arrays
        for (int i = 1; i <= p; i++) {
            for (int j = 1; j <= tot && primes[i] <= a[j] / primes[i]; j++) {
                int id = get_id(a[j] / primes[i]);
                Z cur = 1;
                for (int k = 0; k < deg; k++) {
                    g[k][j] -= cur * (g[k][id] - pref[k][i - 1]);
                    cur *= primes[i];
                }
            }
        }

        return solve(n, 0, eval) + 1; // Add f(1)
    }

    // phi
    /*
        vector<Z> poly_coeffs = {-1, 1};
        auto eval = [&](int p, int k, int64_t pw) -> Z {
            if (k == 1) return p - 1;
            return Z(pw / p) * (p - 1);
        };
    */

    // sigma_1
    /*
        vector<Z> poly_coeffs = {1, 1};
        auto eval = [&](int p, int k, int64_t pw) -> Z {
            // σ₁(p^k) = 1 + p + p² + ... + p^k
            // = (p^(k+1) - 1) / (p - 1)
            return (Z(p).power(k + 1) - 1) / (p - 1);
        };
    */

    // sigma_0
    /*
        vector<Z> poly_coeffs = {2, 0};
        auto eval = [&](int p, int k, int64_t pw) -> Z {
            return k + 1;
        };
    */

    // mobius
    /*
        vector<Z> poly_coeffs = {-1, 0};
        auto eval = [&](int p, int k, int64_t pw) -> Z {
            return (k == 1) ? -1 : 0;
        };
    */

    // solve(n, poly_coeffs, eval)
};
