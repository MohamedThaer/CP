
class factorizer {
private:
    void sieve(int N) { // O(N)
        assert(N >= 1);
        spf.resize(N + 1, 0);
        spfs.resize(N + 1, 0);

        for (int i = 2; i <= N; i++) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (auto p : primes) {
                if (p > spf[i] || i * p > N) {
                    break;
                }
                spf[i * p] = p;
            }

            spfs[i] = spf[i];
            if (spf[i / spf[i]] == spf[i]) {
                spfs[i] *= spfs[i / spf[i]];
            }
        }
    }

    template <class T>
    int mulmod(T a, T b, int mod) {
        return (int64_t)(a % mod) * (b % mod) % mod;
    }

    template <class T>
    int64_t mulmod(T a, T b, int64_t mod) {
        return (__int128_t)(a % mod) * (b % mod) % mod;
    }

    template <class T>
    __int128_t mulmod(T a, T b, __int128_t mod) { // incorrect
        return ((__int128_t)a * b - (__int128_t)((__float128)a / mod * b) * mod + mod) % mod;
    }

    template <class T>
    vector<T> merge(vector<T> a, vector<T> b) {
        return a.insert(a.end(), b.begin(), b.end()), a;
    }

public:
    factorizer(int N = 1) { sieve(N); }

    vector<int> spf = {-1, -1}, spfs = {-1, -1}, primes;

    static vector<int> sieve_primes(const int N, const int Q = 17, const int L = 1 << 15) {
        static const int rs[] = {1, 7, 11, 13, 17, 19, 23, 29};
        struct P {
            P(int p) : p(p) {}
            int p;
            int pos[8];
        };
        auto approx_prime_count = [](const int N) -> int {
            return N > 60184 ? N / (log(N) - 1.1) : max(1., N / (log(N) - 1.11)) + 1;
        };
        const int v = sqrt(N), vv = sqrt(v);
        vector<bool> isp(v + 1, true);
        for (int i = 2; i <= vv; i++) {
            if (isp[i]) {
                for (int j = i * i; j <= v; j += i)
                    isp[j] = false;
            }
        }
        const int rsize = approx_prime_count(N + 30);
        vector<int> primes = {2, 3, 5};
        int psize = 3;
        primes.resize(rsize);
        vector<P> sprimes;
        size_t pbeg = 0;
        int prod = 1;
        for (int p = 7; p <= v; p++) {
            if (!isp[p]) continue;
            if (p <= Q) prod *= p, ++pbeg, primes[psize++] = p;
            auto pp = P(p);
            for (int t = 0; t < 8; t++) {
                int j = (p <= Q) ? p : p * p;
                while (j % 30 != rs[t])
                    j += p << 1;
                pp.pos[t] = j / 30;
            }
            sprimes.push_back(pp);
        }
        vector<unsigned char> pre(prod, 0xFF);
        for (size_t pi = 0; pi < pbeg; pi++) {
            auto pp = sprimes[pi];
            const int p = pp.p;
            for (int t = 0; t < 8; t++) {
                const unsigned char m = ~(1 << t);
                for (int i = pp.pos[t]; i < prod; i += p)
                    pre[i] &= m;
            }
        }
        const int block_size = (L + prod - 1) / prod * prod;
        vector<unsigned char> block(block_size);
        unsigned char *pblock = block.data();
        const int M = (N + 29) / 30;
        for (int beg = 0; beg < M; beg += block_size, pblock -= block_size) {
            int end = min(M, beg + block_size);
            for (int i = beg; i < end; i += prod) {
                copy(pre.begin(), pre.end(), pblock + i);
            }
            if (beg == 0) pblock[0] &= 0xFE;
            for (size_t pi = pbeg; pi < sprimes.size(); pi++) {
                auto &pp = sprimes[pi];
                const int p = pp.p;
                for (int t = 0; t < 8; t++) {
                    int i = pp.pos[t];
                    const unsigned char m = ~(1 << t);
                    for (; i < end; i += p)
                        pblock[i] &= m;
                    pp.pos[t] = i;
                }
            }
            for (int i = beg; i < end; i++) {
                for (int m = pblock[i]; m > 0; m &= m - 1) {
                    primes[psize++] = i * 30 + rs[__builtin_ctz(m)];
                }
            }
        }
        assert(psize <= rsize);
        while (psize > 0 && primes[psize - 1] > N)
            --psize;
        primes.resize(psize);
        return primes;
    }

    static vector<bool> segmented_sieve(int64_t l, int64_t r) { // O(r^(1/2) + (r-l+1)*log2(r-l+1))
        vector<bool> primes_seg(r - l + 1, 1);
        if (l <= 1 && 1 <= r) {
            primes_seg[1 - l] = 0;
        }
        auto primes = sieve_primes(sqrt(r));
        for (auto it : primes) {
            int64_t start = l / it * it + (l % it ? it : 0);
            for (int64_t i = start; i <= r; i += it) {
                if (i != it) {
                    primes_seg[i - l] = 0;
                }
            }
        }
        return primes_seg;
    }

    template <class T>
    vector<pair<T, int>> get_freq(vector<T> a) {
        sort(a.begin(), a.end());
        vector<pair<T, int>> ret;
        for (auto it : a) {
            if (ret.empty() || ret.back().first != it) {
                ret.push_back({it, 1});
            }
            else {
                ret.back().second++;
            }
        }
        return ret;
    }

    template <class T>
    bool IsPrime(T n) { // O(7*log2(n))
        if (n < spf.size()) {
            return n == spf[n];
        }

        int s = 0;
        T d = n - 1;
        while ((d & 1) == 0) {
            d >>= 1, s++;
        }

        auto witness = [&](T a) {
            T cur = 1, p = d;
            while (p) {
                if (p & 1) {
                    cur = mulmod(cur, a, n);
                }
                a = mulmod(a, a, n);
                p >>= 1;
            }
            if (cur == 1) {
                return false;
            }
            for (int r = 0; r < s; r++) {
                if (cur == n - 1) {
                    return false;
                }
                cur = mulmod(cur, cur, n);
            }
            return true;
        };

        const static vector<int64_t> bases[2]{
            {2, 7, 61},
            {2, 325, 9375, 28178, 450775, 9780504, 1795265022},
        };

        for (auto a : bases[n > INT_MAX]) {
            if (a % n && witness(a)) {
                return false;
            }
        }
        return true;
    }

    template <class T>
    vector<T> Factorize(T n) { // O(n^(1/4))
        if (n < spf.size()) {
            vector<T> ret;
            while (n > 1) {
                ret.push_back(spf[n]);
                n /= spf[n];
            }
            return ret;
        }
        if (IsPrime(n)) return {n};
        // Pollard Rho
        static mt19937_64 rng(random_device{}());
        T x = 0, c = rng() % (n - 1) + 1;
        for (int p = 1;; p <<= 1) {
            T sum = 1, saved = x;
            for (int i = 1; i <= p; i++) {
                x = (mulmod(x, x, n) + c) % n;
                sum = mulmod(sum, saved - x + n, n);
            }
            T g = __gcd(sum, n);
            if (g != 1) return merge(Factorize(g), Factorize(n / g));
        }
    }

    template <class T>
    vector<T> divisors(T n) { // O(n^(1/3))
        vector<T> divisors = {1};
        for (auto p : get_freq(Factorize(n))) {
            int sz = divisors.size();
            for (int i = 0; i < sz; i++) {
                T cur = divisors[i];
                for (int j = 0; j < p.second; j++) {
                    cur *= p.first;
                    divisors.push_back(cur);
                }
            }
        }
        sort(divisors.begin(), divisors.end());
        return divisors;
    }

    template <class T>
    T phi(T n) { // O(n^(1/4))
        if (n < spfs.size()) {
            static vector<int> dp;
            if (dp.empty()) dp.resize(spfs.size(), -327), dp[1] = 1;
            if (dp[n] != -327) return dp[n];
            return dp[n] = phi(n / spfs[n]) * (spfs[n] - spfs[n] / spf[n]);
        }

        T ret = 1;
        for (auto [fac, frq] : get_freq(Factorize(n))) {
            T mul = 1;
            while (frq--) {
                mul *= fac;
            }
            ret *= (mul - mul / fac);
        }
        return ret;
    }

    template <class T>
    T mobius(T n) { // O(n^(1/4))
        if (n < spfs.size()) {
            static vector<int> dp;
            if (dp.empty()) dp.resize(spfs.size(), -327), dp[1] = 1;
            if (dp[n] != -327) return dp[n];
            return dp[n] = mobius(n / spfs[n]) * (spf[n] == spfs[n] ? -1 : 0);
        }

        T ret = 1;
        for (auto [fac, frq] : get_freq(Factorize(n))) {
            if (frq > 1) return 0;
            ret *= -1;
        }
        return ret;
    }
} F(1E6);
