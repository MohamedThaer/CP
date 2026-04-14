template <class M>
struct fraction {
    M n = 0, d = 1;
    fraction() {}
    fraction(M x = 0) {
        n = x, d = 1;
    }

    M gcd(M a, M b) {
        if (b == 0) return a;
        return gcd(b, a % b);
    }

    fraction(M a, M b) {
        if (b < 0) a = -a, b = -b;
        n = a, d = b;
        return;
        M gc = gcd(a, b);
        if (gc == 0) {
            n = a, d = b;
            return;
        }
        n = a / gc, d = b / gc;
    }

    M val() {
        return n / d;
    }

    friend fraction operator+(fraction a, fraction b) { return fraction(a.n * b.d + b.n * a.d, a.d * b.d); }
    friend fraction operator-(fraction a, fraction b) { return fraction(a.n * b.d - b.n * a.d, a.d * b.d); }
    friend fraction operator*(fraction a, fraction b) { return fraction(a.n * b.n, a.d * b.d); }
    friend fraction operator/(fraction a, fraction b) { return fraction(a.n * b.d, a.d * b.n); }
    fraction operator+=(fraction x) { return *this = *this + x; }
    fraction operator-=(fraction x) { return *this = *this - x; }
    fraction operator*=(fraction x) { return *this = *this * x; }
    fraction operator/=(fraction x) { return *this = *this / x; }

    const bool operator<(fraction x) const { return n * x.d < x.n * d; }
    const bool operator>(fraction x) const { return x < *this; }
    const bool operator==(fraction x) const { return !(x < *this) && !(x > *this); }
    const bool operator<=(fraction x) const { return *this < x || *this == x; }
    const bool operator>=(fraction x) const { return *this > x || *this == x; }

    friend istream &operator>>(istream &in, fraction &v) {
        long long x;
        cin >> x, v.n = x, v.d = 1;
        return in;
    }

    friend ostream &operator<<(ostream &out, const fraction &v) {
        cout << (int64_t)v.n << " " << (int64_t)v.d << '\n';
        return out;
    }
};
