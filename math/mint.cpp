
template <const int mod = 998244353>
struct mint {
    // static int mod; // for variable mod
    int64_t value;

    mint(int64_t x = 0) {
        value = x % mod;
        if (value < 0) value += mod;
    }

    mint power(int64_t p) {
        mint ret = 1, base = value;
        while (p) {
            if (p & 1) ret *= base;
            base *= base;
            p /= 2;
        }
        return ret;
    }

    mint inverse() {
        int64_t m = mod;
        int64_t a = value;
        int64_t u = 0, v = 1;
        while (a != 0) {
            int64_t t = m / a;
            m -= t * a;
            swap(a, m);
            u -= t * v;
            swap(u, v);
        }
        return u;
    }

    friend mint operator+(mint a, mint b) { return mint(a.value + b.value); }
    friend mint operator-(mint a, mint b) { return mint(a.value - b.value); }
    friend mint operator*(mint a, mint b) { return mint(a.value * b.value); }
    friend mint operator/(mint a, mint b) { return mint(a.value * b.power(mod - 2)); }
    friend bool operator==(mint a, mint b) { return a.value == b.value; }
    friend bool operator!=(mint a, mint b) { return a.value != b.value; }

    mint &operator+=(mint x) { return *this = *this + x; }
    mint &operator-=(mint x) { return *this = *this - x; }
    mint &operator*=(mint x) { return *this = *this * x; }
    mint &operator/=(mint x) { return *this = *this / x; }

    friend istream &operator>>(istream &in, mint &a) { return in >> a.value, a = a.value, in; }
    friend ostream &operator<<(ostream &out, mint a) { return out << a.value; }
};
// int mint::mod = 1; // for variable mod
using Z = mint<(int)1e9 + 7>;
