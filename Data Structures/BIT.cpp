struct BIT {
    vector<int64_t> bit;

    BIT(int n) {
        bit.assign(n + 1, 0);
    }

    void add(int i, int64_t val) {
        ++i;
        while (i < (int)bit.size()) {
            bit[i] += val;
            i += (i & -i);
        }
    }

    int64_t get(int i) {
        ++i;
        int64_t ret = 0;
        while (i) {
            ret += bit[i];
            i -= (i & -i);
        }
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

        for (int i = __lg((int)bit.size() - 1); i >= 0; --i) {
            if (pos + (1 << i) < (int)bit.size() &&
                sum + bit[pos + (1 << i)] < v) {
                sum += bit[pos + (1 << i)];
                pos += (1 << i);
                }
        }

        return pos;
    }
};
