
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

/*
 * x = r_1 (mod m_1)
 * x = r_2 (mod m_2)
 * x = r_n (mod m_n)
 *
 * CRT(r, m) return min nonnegative integer fulfilling all equations
 * or return -1 if there is no such integer
 *
 * be careful about overflow
 * mods are positive and don't have to be prime or coprime
 * time complexity (n log(Mod))
 *
 */
template <class T>
__int128_t CRT(vector<T> rems, vector<T> mods) {
    __int128_t rem = rems[0], mod = mods[0];
    for (int i = 1; i < mods.size(); i++) {
        // solve:  rem + mod * k  ≡ rems[i]  (mod mods[i])
        int64_t x, y;
        int64_t g = extended_euclid(mod, mods[i], x, y);
        if ((rems[i] - rem) % g != 0) return -1; // no solution

        int64_t mm = mods[i] / g;
        int64_t k = (((rems[i] - rem) / g % mm) * (x % mm) % mm + mm) % mm;

        rem = rem + mod * k;
        mod = mod / g * mods[i]; // lcm
        rem %= mod;

        if (mod > ULLONG_MAX) {
            // overflow might happen
            return -1;
        }
    }
    return (rem + mod) % mod;
}
