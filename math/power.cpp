int64_t power(int64_t n, int64_t p, int64_t mod = 1e9 + 7) {
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
