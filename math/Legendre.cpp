
// highest x such that prime^x divides n!
int64_t Legendre(int64_t n, int64_t prime) {
    int64_t res = 0;
    while (n) {
        n /= prime;
        res += n;
    }
    return res;
}
