class NTT {
private:
    template <int mod, int primitive_root>
    static void ntt(vector<int> &A, bool invert) {
        using Z = mint<mod>;
        vector<Z> a(A.begin(), A.end());
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1)
                j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }

        for (int len = 2; len <= n; len <<= 1) {
            Z wlen = Z(primitive_root).power((mod - 1) / len);
            if (invert) wlen = 1 / wlen;
            for (int i = 0; i < n; i += len) {
                Z w = 1;
                for (int j = 0; j < len / 2; j++) {
                    Z u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }
        if (invert) {
            Z inv_n = 1 / (Z)n;
            for (auto &x : a) {
                x *= inv_n;
            }
        }
        for (int i = 0; i < n; ++i) {
            A[i] = a[i].value;
        }
    }

    template <int mod, int primitive_root>
    static vector<int> multiply(vector<int> a, vector<int> b) {
        int n = 1 << (1 + __lg(a.size() + b.size() - 1));
        a.resize(n), b.resize(n);
        ntt<mod, primitive_root>(a, false);
        ntt<mod, primitive_root>(b, false);
        for (int i = 0; i < n; i++) {
            a[i] = 1LL * a[i] * b[i] % mod;
        }
        ntt<mod, primitive_root>(a, true);
        return a;
    }

    constexpr static int m[3] = {167772161, 469762049, 2013265921};        // mods
    constexpr static int roots[3] = {3, 3, 31};                            // primitime roots for mods
    constexpr static int64_t coeff[3] = {1, 167772161, 78812994116517889}; // coeff[i] = mul(m[0], ..., m[i - 1])
    constexpr static int c_inv[3] = {1, 104391568, 1066314758};            // inv(coeff[i], mod m[i])

    template <class T>
    static vector<int> MOD(const vector<T> &a, int mod) {
        vector<int> ret(a.size());
        for (int i = 0; i < a.size(); ++i) {
            ret[i] = (a[i] % mod + mod) % mod;
        }
        return ret;
    }

public:
    template <class A, class B>
    static vector<int> multiply(const vector<A> &a, const vector<B> &b, int mod) {
        return MOD(multiply(MOD(a, mod), MOD(b, mod)), mod);
    }

    template <class A, class B>
    static vector<__int128_t> multiply(const vector<A> &a, const vector<B> &b) {
        vector<A> r[3]{
            multiply<m[0], roots[0]>(MOD(a, m[0]), MOD(b, m[0])),
            multiply<m[1], roots[1]>(MOD(a, m[1]), MOD(b, m[1])),
            multiply<m[2], roots[2]>(MOD(a, m[2]), MOD(b, m[2])),
        };

        vector<__int128_t> result(r[0].size());
        for (int i = 0; i < r[0].size(); i++) {
            __int128_t val = 0;
            for (int j = 0; j < 3; j++) {
                val += ((r[j][i] - val) % m[j] * c_inv[j] % m[j] + m[j]) % m[j] * coeff[j];
            }
            result[i] = val;
        }
        return result;
    }
};
