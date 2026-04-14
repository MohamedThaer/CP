
// 1-based
class GCD_Convolution {
private:
    template <class T>
    static void _gcd_zeta(vector<T> &v) {
        int n = v.size() - 1;
        vector<bool> sieve(n + 1, true);
        for (int64_t i = 2; i <= n; i++) {
            if (sieve[i]) {
                for (int64_t j = n / i; j >= 1; j--)
                    v[j] += v[i * j];
                for (int64_t j = i * i; j <= n; j += i)
                    sieve[j] = false;
            }
        }
    }

    template <class T>
    static void _gcd_mobius(vector<T> &v) {
        int n = v.size() - 1;
        vector<bool> sieve(n + 1, true);
        for (int64_t i = 2; i <= n; i++) {
            if (sieve[i]) {
                for (int64_t j = 1; j <= n / i; j++)
                    v[j] -= v[i * j];
                for (int64_t j = i * i; j <= n; j += i)
                    sieve[j] = false;
            }
        }
    }

public:
    template <class T>
    static vector<T> multiply(vector<T> A, vector<T> B) {
        int n = max(A.size(), B.size());
        A.resize(n), B.resize(n);
        _gcd_zeta(A);
        _gcd_zeta(B);
        for (int i = 0; i < n; i++)
            A[i] *= B[i];
        _gcd_mobius(A);
        return A;
    }
};
