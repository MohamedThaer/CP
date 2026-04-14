class fenwick_2d {
private:
    vector<vector<int64_t>> fenw;

public:
    fenwick_2d(int n, int m) {
        fenw.assign(n + 1, vector<int64_t>(m + 1, 0));
    }

    void add(int x, int y, int64_t val) {
        ++x, ++y;
        for (int i = x; i < fenw.size(); i += (i & -i)) {
            for (int j = y; j < fenw[0].size(); j += (j & -j)) {
                fenw[i][j] += val;
            }
        }
    }

    int64_t get(int x, int y) {
        ++x, ++y;
        int64_t ret = 0;
        for (int i = x; i; i -= (i & -i)) {
            for (int j = y; j; j -= (j & -j)) {
                ret += fenw[i][j];
            }
        }
        return ret;
    }

    void update(int x, int y, int val) {
        add(x, y, val - query(x, y, x, y));
    }

    int64_t query(int x1, int y1, int x2, int y2) {
        return get(x2, y2) - get(x1 - 1, y2) - get(x2, y1 - 1) + get(x1 - 1, y1 - 1);
    }
};
