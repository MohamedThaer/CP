
using i128 = __int128_t;
istream &operator>>(istream &in, i128 &x) {
    string s;
    int sign = 1;
    cin >> s, x = 0;
    for (auto c : s) {
        if (c == '-') {
            sign = -1;
        }
        else {
            x = x * 10 + sign * (c - '0');
        }
    }
    return in;
}

ostream &operator<<(ostream &out, i128 x) {
    if (x < 0) {
        out << '-';
        x = -x;
    }

    if (x / 10) out << x / 10;
    return out << (int)(x % 10);
}
