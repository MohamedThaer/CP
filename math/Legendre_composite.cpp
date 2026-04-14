
// highest x such that prime^x divides n!
int64_t Legendre(int64_t n, int64_t prime) {
    int64_t res = 0;
    while (n) {
        n /= prime;
        res += n;
    }
    return res;
}

// highest x such that k^x divides n!
int64_t Legendre_composite(int64_t n, int64_t k) {
    int64_t ans = LLONG_MAX;

    for (int64_t i = 2; i * i <= k; i++) {
        if (k % i == 0) {
            int cnt = 0;
            while (k % i == 0) {
                k /= i;
                cnt++;
            }
            ans = min(ans, Legendre(n, i) / cnt);
        }
    }
    if (k > 1) ans = min(ans, Legendre(n, k));
    return ans;
}
