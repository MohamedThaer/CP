
class FFT {
private:
    using cd = complex<double>;
    static int reverse(int num, int lg_n) {
        int res = 0;
        for (int i = 0; i < lg_n; i++) {
            if (num & (1 << i))
                res |= 1 << (lg_n - 1 - i);
        }
        return res;
    }
    static void fft(vector<cd> &a, bool invert) {
        int n = a.size(), lg_n = 0;
        while ((1 << lg_n) < n)
            lg_n++;

        for (int i = 0; i < n; i++) {
            int rev = reverse(i, lg_n);
            if (i < rev)
                swap(a[i], a[rev]);
        }

        double PI = acos(-1);
        for (int len = 2; len <= n; len <<= 1) {
            double ang = 2 * PI / len * (invert ? -1 : 1);
            cd wlen(cos(ang), sin(ang));
            for (int i = 0; i < n; i += len) {
                cd w(1);
                for (int j = 0; j < len / 2; j++) {
                    cd u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }

        if (invert) {
            for (auto &x : a) {
                x /= n;
            }
        }
    }

public:
    template <class A, class B>
    static vector<int64_t> multiply(vector<A> a, vector<B> b) {
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < a.size() + b.size())
            n <<= 1;
        fa.resize(n), fb.resize(n);

        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; i++) {
            fa[i] *= fb[i];
        }
        fft(fa, true);

        vector<int64_t> result(n);
        for (int i = 0; i < n; i++) {
            result[i] = round(fa[i].real());
        }
        return result;
    }
};

template <class T>
class polynomial {
private:
public:
    vector<T> v;
    int shift = 0;

    polynomial(int N) : v(N) {}
    polynomial(int l, int r) : v(r - l + 1), shift(-l) {}
    polynomial(vector<T> v) : v(v) {}
    polynomial(vector<T> v, int shift) : v(v), shift(shift) {}

    int size() {
        return v.size();
    }

    T &operator[](int n) {
        return v[n + shift];
    }
};

template <class A, class B>
auto operator*(polynomial<A> a, polynomial<B> b) {
    return polynomial<int64_t>(FFT::multiply(a.v, b.v), a.shift + b.shift);
}
