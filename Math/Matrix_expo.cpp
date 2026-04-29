template <class T>
struct Matrix {
    T D = 0;
    vector<vector<T>> a;
    Matrix(int n, int m) : a(n, vector<T>(m, D)) {}
    Matrix(int n) : a(n, vector<T>(n, D)) {
        for (int i = 0; i < n; i++) a[i][i] = 1;
    }

    Matrix operator*(const Matrix& other) const {
        int n = a.size(), m = other.a[0].size(), k = a[0].size();
        Matrix res(n, m);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int x = 0; x < k; x++) {
                    res.a[i][j] = res.a[i][j] + a[i][x] * other.a[x][j];
                }
            }
        }
        return res;
    }

    Matrix power(long long p) {
        Matrix res(a.size()),base = *this;
        while (p) {
            if (p & 1) res = res * base;
            base = base * base;
            p >>= 1;
        }
        return res;
    }
};
