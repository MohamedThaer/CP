namespace combinatorics {
    const int64_t mod = 1e9 + 7;
    using Z = mint<mod>;

    // int64_t mod = 1;
    // using Z = mint;

    vector<Z> _fac = {1, 1};
    vector<Z> _inv = {1, 1};
    vector<Z> _facinv = {1, 1};

    void init(int N) {
        for (int i = _fac.size(); i <= N; i++) {
            _fac.push_back(_fac[i - 1] * i);
            _inv.push_back(mod - mod / i * _inv[mod % i]);
            _facinv.push_back(_facinv[i - 1] * _inv[i]);
        }
    }

    // void reset(int mod) {
    //     mint::mod = mod;
    //     combinatorics::mod = mod;
    //     _fac = _facinv = _inv = {1, 1};
    // }

    Z fac(int n) {
        init(n);
        return _fac[n];
    }

    Z inv(int n) {
        init(n);
        return _inv[n];
    }

    Z facinv(int n) {
        init(n);
        return _facinv[n];
    }

    Z npr(int n, int r) {
        if (n < r) return 0;
        return fac(n) * facinv(n - r);
    }

    Z ncr(int n, int r) {
        return npr(n, r) * facinv(r);
    }

    // O(log(n) / log(mod))
    Z ncr_lucas(int64_t n, int64_t r) { // mod is prime, or if square-free use CRT
        if (r > n) return 0;
        if (n < mod) return ncr(n, r);
        return ncr_lucas(n / mod, r / mod) * ncr_lucas(n % mod, r % mod);
    }

    Z catalan(int n, int k = 0) {
        return ncr(2 * n - k, n) * (k + 1) * inv(n + 1);
    }

    /// Generalized Catalan:
    /// number of bracket sequences of length 2*n
    /// starting with a '(' and ending with b ')'
    Z catalan(int n, int a = 0, int b = 0) {
        if (n < 0 || a < 0 || b < 0 || a > n || b > n) return 0;
        int len = 2 * n - a - b;
        return ncr(len, n - a) - ncr(len, n + 1);
    }

    Z stars_and_bars(int star, int bar) { // **||**|*|||
        return ncr(star + bar, star);
    }

    Z permutation_with_repetition(const vector<int> &frq) {
        Z ret = fac(accumulate(frq.begin(), frq.end(), 0));
        for (auto it : frq) {
            ret *= facinv(it);
        }
        return ret;
    }

    Z derangement(int n) {
        static vector<Z> d = {1, 0};
        // d(i) = (i - 1) * (d(i - 1) + d(i - 2))
        while (d.size() <= n) {
            d.emplace_back();
            int i = d.size() - 1;
            d[i] = (i - 1) * (d[i - 1] + d[i - 2]);
        }
        return d[n];
    }

    Z count_necklaces(int beads, Z colors) { // for (divisors(beads)) total += phi(d) * pow(colors, beads / d)
        Z total = 0;
        for (int rot = 0; rot < beads; rot++) {
            total += colors.power(gcd(rot, beads));
        }
        return total / beads;
    }

    // count permutations of n items with k cycles
    Z stirling_kind1(int n, int k) {
        if (n == 0 && k == 0) return 1;
        if (k == 0) return 0;
        if (n < k) return 0;
        return (n - 1) * stirling_kind1(n - 1, k) + stirling_kind1(n - 1, k - 1);
    }

    // count ways to divide n distinct items into k non-empty groups
    Z stirling_kind2(int n, int k) {
        if (n == 0 && k == 0) return 1;
        if (k == 0) return 0;
        if (n < k) return 0;
        return k * stirling_kind2(n - 1, k) + stirling_kind2(n - 1, k - 1);
    }
}
using namespace combinatorics;
