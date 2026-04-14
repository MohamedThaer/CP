class primitive_generator {
protected:
    int64_t p, _phi;
    vector<int64_t> fact;

public:
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

    static int64_t power(int64_t n, int64_t p, int64_t mod) {
        n %= mod;
        int64_t ret = 1;
        while (p) {
            if (p & 1) {
                ret = ret * n % mod;
            }
            n = n * n % mod;
            p >>= 1;
        }
        return ret;
    }

    // p = 2, p = 4, p = (odd_prime^k), p = 2*(odd_prime^k)
    primitive_generator(int64_t p) : p(p), _phi(phi(p)) { // O(sqrt(p))
        int64_t n = _phi;
        for (int64_t i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                fact.push_back(i);
                while (n % i == 0) {
                    n /= i;
                }
            }
        }
        if (n > 1) fact.push_back(n);
    }

    bool is_primitive_root(int64_t x) { // O(log(p))
        for (auto f : fact) {
            if (power(x, _phi / f, p) == 1) {
                return false;
            }
        }
        return true;
    }

    vector<int64_t> all_primitive_roots() { // O(p*log(p))
        vector<int64_t> ret;
        for (int i = 1; i < p; ++i) {
            if (is_primitive_root(i)) {
                ret.push_back(i);
            }
        }
        return ret;
    }

    int64_t smallest_primitive_root() { // O(log^6(p))
        for (int i = 1; i < p; ++i) {
            if (is_primitive_root(i)) {
                return i;
            }
        }
        return -1;
    }
};
