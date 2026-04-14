

class Mobius {
private:
    static vector<int> mobius(int N) {
        vector<int> _primes, mob(N + 1, 1);
        vector<bool> _is_prime(N + 1, 1);
        _is_prime[0] = _is_prime[1] = 0;
        for (int64_t i = 1; i <= N; i++) {
            if (_is_prime[i]) {
                mob[i] = -1;
                _primes.push_back(i);
            }
            for (auto it : _primes) {
                if (i * it > N) {
                    break;
                }
                mob[i * it] = !!(i % it) * -mob[i];
                _is_prime[i * it] = 0;
                if (i % it == 0) {
                    break;
                }
            }
        }
        return mob;
    }

    static vector<int64_t> divisors(int64_t n) {
        vector<int64_t> ret;
        for (int64_t i = 1; i <= n / i; i++) {
            if (n % i == 0) {
                ret.push_back(i);
                if (i * i != n) {
                    ret.push_back(n / i);
                }
            }
        }
        return ret;
    }

public:
    static int64_t mob(int n) {
        static vector<int> m;
        if (m.size() <= n) {
            m = mobius(2 * n);
        }
        return m[n];
    }

    static int64_t coprime_pairs(const vector<int> &a, const vector<int> &b) {
        int N = max(*max_element(a.begin(), a.end()), *max_element(b.begin(), b.end()));
        vector<int> frqa(N + 1), frqb(N + 1), diva(N + 1), divb(N + 1);
        for (auto &it : a) {
            frqa[it]++;
        }
        for (auto &it : b) {
            frqb[it]++;
        }

        for (int i = 1; i <= N; ++i) {
            for (int j = i; j <= N; j += i) {
                diva[i] += frqa[j];
                divb[i] += frqb[j];
            }
        }

        int64_t ans = 0;
        for (int i = 1; i <= N; ++i) {
            ans += mob(i) * diva[i] * divb[i];
        }
        return ans;
    }

    static int64_t coprime_power(const vector<int> &a, int64_t p) {
        int N = *max_element(a.begin(), a.end());
        vector<int> frq(N + 1), div(N + 1);
        for (auto &it : a) {
            frq[it]++;
        }

        for (int i = 1; i <= N; ++i) {
            for (int j = i; j <= N; j += i) {
                div[i] += frq[j];
            }
        }

        int64_t ans = 0;
        for (int i = 1; i <= N; ++i) {
            ans += mob(i) * powl(div[i], p);
        }
        return ans;
    }

    static int64_t coprime_pairs(int l1, int r1, int l2, int r2) {
        int N = max(r1, r2);
        int64_t ans = 0;
        for (int i = 1; i <= N; ++i) {
            ans += mob(i) * (r1 / i - (l1 - 1) / i) * (r2 / i - (l2 - 1) / i);
        }
        return ans;
    }

    static int64_t coprime_pairs(const vector<int> &a) { // i < j
        return (coprime_pairs(a, a) - count(a.begin(), a.end(), 1)) / 2;
    }

    // how many numbers [from 1 to n] whose gcd with x equals y
    static int64_t x_gcd_1_n_equal_y(int x, int64_t n, int y) {
        if (x % y) return 0;
        x /= y, n /= y, y = 1;

        int64_t cnt = 0;
        for (auto d : divisors(x)) {
            cnt += mob(d) * n / d;
        }
        return cnt;
    }

    template <class T>
    static vector<T> zeta_multiples(vector<T> f) {
        int n = (int)f.size() - 1;
        for (int i = 1; i <= n; i++) {
            for (int j = 2 * i; j <= n; j += i) {
                f[i] += f[j];
            }
        }
        return f;
    }

    template <class T>
    static vector<T> mobius_multiples(vector<T> f) {
        int n = (int)f.size() - 1;
        for (int i = n; i >= 1; i--) {
            for (int j = 2 * i; j <= n; j += i) {
                f[i] -= f[j];
            }
        }
        return f;
    }

    template <class T>
    static vector<T> inverse_zeta_multiples(vector<T> f) {
        return mobius_multiples(f);
    }

    template <class T>
    static vector<T> inverse_mobius_multiples(vector<T> f) {
        return zeta_multiples(f);
    }

    template <class T>
    static vector<T> zeta_divisors(vector<T> f) {
        int n = (int)f.size() - 1;
        for (int i = n; i >= 1; i--) {
            for (int j = 2 * i; j <= n; j += i) {
                f[j] += f[i];
            }
        }
        return f;
    }

    template <class T>
    static vector<T> mobius_divisors(vector<T> f) {
        int n = (int)f.size() - 1;
        for (int i = 1; i <= n; i++) {
            for (int j = 2 * i; j <= n; j += i) {
                f[j] -= f[i];
            }
        }
        return f;
    }

    template <class T>
    static vector<T> inverse_zeta_divisors(vector<T> &f) {
        return mobius_divisors(f);
    }

    template <class T>
    static vector<T> inverse_mobius_divisors(vector<T> &f) {
        return zeta_divisors(f);
    }

    static vector<Z> gcd_subsets(vector<int64_t> f) {
        int n = f.size();

        f = zeta_multiples(f);
        vector<Z> frq(n);
        for (int i = 1; i < n; ++i) {
            frq[i] = Z(2).power(f[i]) - 1;
        }
        frq = inverse_zeta_multiples(frq);
        return frq;
    }
};
