template <int base = (1LL << 30)>
class bigint {
public:
    int sign = 1;
    vector<int> num;

    bigint(int64_t x = 0) {
        sign = 1;
        if (x < 0) {
            x = -x;
            sign = -1;
        }

        if (!x) num = {0};
        while (x) {
            num.push_back(x % base);
            x /= base;
        }
    }

    // vector constructor
    bigint(int n, int64_t x) : sign(1), num(n, x) {}

    int size() const { return num.size(); }

    const int operator[](int idx) const { return (idx < num.size() ? num[idx] : 0); }

    bigint trim() {
        while (num.size() > 1 && !num.back()) {
            num.pop_back();
        }
        return *this;
    }

    bool is_zero() {
        trim();
        return num.size() == 1 && num[0] == 0;
    }

    bigint complement() {
        bigint ret(size(), 0);
        for (int i = 0; i < size(); ++i) {
            ret.num[i] = (base - 1) - num[i];
        }
        return ret;
    }

    string to_string(int new_base) {
        if (is_zero()) return "0";

        if (base == new_base) {
            string ret;
            if (sign == -1) ret += '-';
            for (int i = num.size() - 1; ~i; --i) {
                ret += (num[i] < 10 ? num[i] + '0' : num[i] - 10 + 'A');
            }
            return ret;
        }
        else {
            bigint cur = *this;
            string ret;
            while (!cur.is_zero()) {
                int x = (cur % new_base)[0];
                ret += (x < 10 ? x + '0' : x - 10 + 'A');
                cur = cur / new_base;
            }
            if (cur.sign == -1) ret += '-';
            return string(ret.rbegin(), ret.rend());
        }
    }

    static bigint to_bigint(string s, int base_in) {
        int sign = (s[0] == '-' ? -1 : 1);
        if (s[0] == '-') {
            s.erase(s.begin());
        }

        bigint ret = 0;
        if (base == base_in) {
            ret.num.clear();
            for (int i = s.size() - 1; ~i; --i) {
                ret.num.push_back(isdigit(s[i]) ? s[i] - '0' : toupper(s[i]) - 'A' + 10);
            }
        }
        else {
            for (auto c : s) {
                int d = (isdigit(c) ? c - '0' : toupper(c) - 'A' + 10);
                ret = ret * base_in + d;
            }
        }
        ret.sign = sign;
        return ret;
    }

    bigint operator-() {
        bigint ret = *this;
        ret.sign = -ret.sign;
        return ret;
    };

    friend bigint operator+(bigint a, bigint b) {
        a.trim(), b.trim();
        if (b.sign == -1) return a - -b;
        if (a.sign == -1) return b - -a;

        bigint ret(0, 0);

        int64_t carry = 0;
        for (int i = 0; i < max(a.size(), b.size()); ++i) {
            int64_t res = a[i] + b[i] + carry;
            ret.num.push_back(res % base);
            carry = res / base;
        }
        if (carry) {
            ret.num.push_back(carry);
        }
        return ret.trim();
    }

    friend bigint operator-(bigint a, bigint b) {
        a.trim(), b.trim();
        if (b.sign == -1) return a + -b;
        if (a.sign == -1) return -(-a + b);
        if (a < b) return -(b - a);

        while (b.size() < a.size()) {
            b.num.push_back(0);
        }
        a = a + b.complement() + 1;
        if (a.size() == b.size()) {
            return (-(a.complement() + 1)).trim();
        }
        a.num.pop_back();
        return a.trim();
    }

    friend bigint operator*(bigint a, bigint b) {
        bigint ret;
        for (int i = 0; i < b.size(); ++i) {
            __int128_t carry = 0;
            bigint res(i, 0);
            for (int j = 0; j < a.size(); ++j) {
                __int128_t x = (__int128_t)a[j] * b[i] + carry;
                res.num.push_back(x % base);
                carry = x / base;
            }
            if (carry) {
                res.num.push_back(carry);
            }
            ret = ret + res;
        }
        ret.sign = a.sign * b.sign;
        return ret.trim();
    }

    friend bigint operator/(bigint a, bigint b) {
        bigint quotient, current;

        for (int i = a.size() - 1; ~i; --i) {
            current = current * base + a[i];
            current.trim();

            int64_t low = 0, high = base - 1, count = 0;
            while (low <= high) {
                int64_t mid = (low + high) / 2;
                if ((current - b * mid).sign != -1) {
                    count = mid;
                    low = mid + 1;
                }
                else {
                    high = mid - 1;
                }
            }

            current = current - b * count;
            quotient = quotient * base + count;
        }
        quotient.sign = a.sign * b.sign;
        return quotient.trim();
    }

    friend bigint operator%(bigint a, bigint b) {
        return a - a / b * b;
    }

    friend istream &operator>>(istream &in, bigint &x) {
        string s;
        in >> s;
        x = bigint::to_bigint(s, 10);
        return in;
    }

    friend ostream &operator<<(ostream &out, bigint x) { return out << x.to_string(10); }

    friend bool operator<(bigint a, bigint b) {
        a.trim(), b.trim();
        if (a.sign != b.sign) return a.sign < b.sign;
        if (a.sign == -1) return -a > -b;
        if (a.size() != b.size()) return a.size() < b.size();
        return vector(a.num.rbegin(), a.num.rend()) < vector(b.num.rbegin(), b.num.rend());
    }

    friend bool operator>=(bigint a, bigint b) {
        return !(a < b);
    }

    friend bool operator>(bigint a, bigint b) {
        return b < a;
    }

    friend bool operator<=(bigint a, bigint b) {
        return !(a > b);
    }

    friend bool operator==(bigint a, bigint b) {
        a.trim(), b.trim();
        return a.sign == b.sign && a.num == b.num;
    }

    friend bool operator!=(bigint a, bigint b) {
        return !(a == b);
    }
};
