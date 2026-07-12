struct BIT {
    int n;
    vector<int64_t> bit;

    BIT(int _n) {
        n = _n;
        bit.assign(n + 1, 0);
    }

    void add(int idx, int64_t val) {
        for (; idx <= n; idx += idx & -idx)
            bit[idx] += val;
    }

    int64_t get(int idx) {
        int64_t ret = 0;
        for (; idx > 0; idx -= idx & -idx)
            ret += bit[idx];
        return ret;
    }

    void update(int i, int64_t val) {
        add(i, val - query(i, i));
    }

    int64_t query(int l, int r) {
        return get(r) - get(l - 1);
    }

    // first position i where prefix[i] >= v
    // requires prefix sums to be monotonic
    int bit_search(int64_t v) {
        int64_t sum = 0;
        int pos = 0;

        for (int i = __lg((int) bit.size() - 1); i >= 0; --i) {
            if (pos + (1 << i) < (int) bit.size() &&
                sum + bit[pos + (1 << i)] < v) {
                sum += bit[pos + (1 << i)];
                pos += (1 << i);
            }
        }

        return pos;
    }
};
