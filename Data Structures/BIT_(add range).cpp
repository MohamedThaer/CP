struct BIT_Range {
    int n;
    vector<int64_t> bit1, bit2;

    BIT_Range(int _n) {
        n = _n;
        bit1.assign(n + 1, 0);
        bit2.assign(n + 1, 0);
    }

    void add(vector<int64_t> &bit, int idx, int64_t val) {
        for (; idx <= n; idx += idx & -idx)
            bit[idx] += val;
    }

    int64_t get(vector<int64_t> &bit, int idx) {
        int64_t ret = 0;
        for (; idx > 0; idx -= idx & -idx)
            ret += bit[idx];
        return ret;
    }
    
    void range_add(int l, int r, int64_t val) {
        add(bit1, l, val);
        add(bit1, r + 1, -val);

        add(bit2, l, val * (l - 1));
        add(bit2, r + 1, -val * r);
    }
    
    int64_t prefix_query(int idx) {
        return get(bit1, idx) * idx - get(bit2, idx);
    }
    
    int64_t query(int l, int r) {
        return prefix_query(r) - prefix_query(l - 1);
    }
};
