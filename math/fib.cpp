#define matrix vector<vector<int64_t>>
matrix mul(const matrix &a, const matrix &b, int64_t mod = 1e9 + 7) {
    matrix ret(a.size(), vector<ll>(b[0].size(), 0));
    for (ll i = 0; i < a.size(); i++) {
        for (ll j = 0; j < b[0].size(); j++) {
            for (ll o = 0; o < b.size(); o++) {
                ret[i][j] += (a[i][o] * b[o][j]);
                ret[i][j] %= mod;
            }
        }
    }
    return ret;
}

matrix power(matrix n, int64_t p, int64_t mod = 1e9 + 7) {
    matrix ret = {{1, 0}, {0, 1}};
    while (p) {
        if (p & 1) {
            ret = mul(ret, n, mod);
        }
        n = mul(n, n, mod);
        p >>= 1;
    }
    return ret;
}

int64_t fib(int64_t ind, int64_t mod = 1e9 + 7) {
    return mul(power({{1, 1}, {1, 0}}, ind, mod), {{1}, {0}}, mod)[1][0];
}
