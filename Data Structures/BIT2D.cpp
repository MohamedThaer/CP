struct BIT2D {
    int n, m;
    vector<vector<int64_t>> bit;

    BIT2D(int _n, int _m) {
        n = _n;
        m = _m;
        bit.assign(n + 1, vector<int64_t>(m + 1, 0));
    }

    void add(int x, int y, int64_t val) {
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                bit[i][j] += val;
            }
        }
    }

    int64_t get(int x, int y) {
        int64_t ret = 0;
        for (int i = x; i > 0; i -= i & -i) {
            for (int j = y; j > 0; j -= j & -j) {
                ret += bit[i][j];
            }
        }
        return ret;
    }

    void update(int x, int y, int64_t val) {
        add(x, y, val - query(x, y, x, y));
    }

    int64_t query(int x1, int y1, int x2, int y2) {
        return get(x2, y2)
             - get(x1 - 1, y2)
             - get(x2, y1 - 1)
             + get(x1 - 1, y1 - 1);
    }
};
