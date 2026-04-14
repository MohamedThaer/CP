
// 1-based
class LCM_Convolution {
private:
    /* Linear Sieve, O(n) */
    static vector<int> PrimeEnumerate(int n) {
        vector<int> P;
        vector<bool> B(n + 1, 1);
        for (int i = 2; i <= n; i++) {
            if (B[i]) P.push_back(i);
            for (int j : P) {
                if (i * j > n) break;
                B[i * j] = 0;
                if (i % j == 0) break;
            }
        }
        return P;
    }

    template <typename T>
    static void DivisorZetaTransform(vector<T> &v) {
        const int n = (int)v.size() - 1;
        for (int p : PrimeEnumerate(n)) {
            for (int i = 1; i * p <= n; i++)
                v[i * p] += v[i];
        }
    }

    template <typename T>
    static void DivisorMobiusTransform(vector<T> &v) {
        const int n = (int)v.size() - 1;
        for (int p : PrimeEnumerate(n)) {
            for (int i = n / p; i; i--)
                v[i * p] -= v[i];
        }
    }

public:
    template <typename T>
    static vector<T> multiply(vector<T> A, vector<T> B) {
        int n = max(A.size(), B.size());
        A.resize(n), B.resize(n);

        DivisorZetaTransform(A);
        DivisorZetaTransform(B);
        for (int i = 0; i < n; i++)
            A[i] *= B[i];
        DivisorMobiusTransform(A);
        return A;
    }
};
