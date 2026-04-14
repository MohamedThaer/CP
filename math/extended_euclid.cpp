int64_t extended_euclid(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    if (a < 0) {
        auto t = extended_euclid(-a, b, x, y);
        x = -x;
        return t;
    }
    if (b < 0) {
        auto t = extended_euclid(a, -b, x, y);
        y = -y;
        return t;
    }
    if (!b) {
        x = 1, y = 0;
        return a;
    }
    int64_t g = extended_euclid(b, a % b, y, x);
    y -= x * (a / b);
    return g;
}
