int64_t mulmod(int64_t a, int64_t b, int64_t mod) {
    int64_t res = 0;
    while (b) {
        a %= mod;
        if (b & 1) {
            res = (res + a) % mod;
        }
        a <<= 1;
        b >>= 1;
    }
    return res;
}
