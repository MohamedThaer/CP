class Power_tower {
private:
    static int64_t phi(int64_t n) { // n * (1 - 1/p)
        int64_t ret = n;
        for (int64_t i = 2; i <= n / i; i++) {
            if (n % i == 0) {
                while (n % i == 0)
                    n /= i;
                ret -= ret / i;
            }
        }
        if (n > 1)
            ret -= ret / n;
        return ret;
    }

    static int64_t MOD(int64_t x, int64_t m) {
        if (x < m) return x;
        return x % m + m;
    }

    static int64_t power(int64_t n, int64_t k, int64_t mod) {
        int64_t ans = MOD(1, mod);
        while (k) {
            if (k & 1) ans = MOD(ans * n, mod);
            n = MOD(n * n, mod);
            k >>= 1;
        }
        return ans;
    }

    // if x >= log2(m), then a^x = a^(MOD(x, phi(m))) % m
    static int64_t solve(const auto &a, int64_t l, int64_t r, int64_t m) {
        if (l == r) return MOD(a[l], m);
        if (m == 1) return 1;
        return power(a[l], solve(a, l + 1, r, phi(m)), m);
    }

public:
    static int64_t get(const auto &a, int64_t l, int64_t r, int64_t m) {
        return solve(a, l, r, m) % m;
    }

    static int64_t get(const auto &a, int64_t m) {
        return solve(a, 0, a.size() - 1, m) % m;
    }
};
