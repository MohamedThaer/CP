template <class M> // int, double, int64_t, mint
class matrix : public vector<vector<M>> {
public:
    matrix() : vector<vector<M>>() {}
    matrix(const vector<vector<M>> &v) : vector<vector<M>>(v) {}
    matrix(int n, int m) : vector<vector<M>>(n, vector<M>(m)) {}
    matrix(initializer_list<initializer_list<M>> init) : vector<vector<M>>(init.begin(), init.end()) {}

    matrix(int n) : matrix(n, n) {
        for (int i = 0; i < n; ++i) {
            (*this)[i][i] = 1;
        }
    }

    matrix<M> operator*(const auto &x) {
        int n = this->size(), m = x[0].size(), k = (*this)[0].size();
        matrix<M> ret(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int o = 0; o < k; o++) {
                    ret[i][j] += (*this)[i][o] * x[o][j];
                }
            }
        }
        return ret;
    }

    matrix<M> power(int64_t p) {
        matrix<M> ret(this->size()), base(*this);
        while (p) {
            if (p & 1) {
                ret = ret * base;
            }
            base = base * base;
            p >>= 1;
        }
        return ret;
    }
};
