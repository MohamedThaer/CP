int mobius(int64_t n) {
    int ret = 1;
    for (int i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            n /= i;
            if (n % i == 0) {
                return 0;
            }
            ret = -ret;
        }
    }
    if (n > 1) {
        ret = -ret;
    }
    return ret;
}
