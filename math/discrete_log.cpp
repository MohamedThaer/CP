// Returns minimum x for which a ^ x % m = b % m. a is positive. O(sqrt(m))
int discrete_log(int a, int b, int m) {
    a %= m, b %= m;
    int k = 1, add = 0, g;
    while ((g = gcd(a, m)) > 1) {
        if (b == k) return add;
        if (b % g) return -1;
        b /= g, m /= g, ++add;
        k = (1LL * k * a / g) % m;
    }

    int n = sqrt(m) + 1, an = 1;
    for (int i = 0; i < n; ++i) {
        an = (1LL * an * a) % m;
    }

    unordered_map<int, int> vals;
    for (int q = 0, cur = b; q <= n; ++q) {
        vals[cur] = q;
        cur = (1LL * cur * a) % m;
    }

    for (int p = 1, cur = k; p <= n; ++p) {
        cur = (1LL * cur * an) % m;
        if (vals.count(cur)) {
            return n * p - vals[cur] + add;
        }
    }
    return -1;
}
